#include "opencv2/core/cvstd.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgcodecs.hpp"
#include "paplease/resources.h"
#include <cassert>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <limits.h>

#include <opencv2/imgproc.hpp>

#include <paplease/compiler.h>
#include <paplease/geometry.h>
#include <paplease/ocr.h>
#include <paplease/types.h>

struct font_info {
	typeface tf;
	int max_pixels_tall;

	int letter_spacing_horizontal;
	int whitespace_pixel_width;
	int letter_spacing_vertical;
	int newline_size;
};

static constexpr font_info BOOTH_FONT_INFO{ .tf = typeface::booth,
					    .max_pixels_tall = 12,
					    .letter_spacing_horizontal = 4,
					    .whitespace_pixel_width = 6,
					    .letter_spacing_vertical = 8,
					    .newline_size = 4 };

static constexpr font_info BM_MINI_FONT_INFO{ .tf = typeface::bm_mini,
					      .max_pixels_tall = 16,
					      .letter_spacing_horizontal = 2,
					      .whitespace_pixel_width = 4,
					      .letter_spacing_vertical = 4,
					      .newline_size = 8 };

static const font_info &font_info_for(typeface tf)
{
	switch (tf) {
	case typeface::booth:
		return BOOTH_FONT_INFO;
	case typeface::bm_mini:
		return BM_MINI_FONT_INFO;
	default:
		unreachable();
	}
}

/* The theory here is that in 'Papers, Please', every pixel is a 2x2 chunk.
 * So we can compress every 2x2 -> 1x1. Which makes processing more efficient,
 *  and in this case, allows us to fit more character pixels into an encoded integer, for easy comparison.
 **/
static cv::Mat downscale_image(const cv::Mat &image)
{
	cv::Mat result;
	cv::resize(image, result,
		   cv::Size((int)((float)image.cols * 0.5f),
			    (int)((float)image.rows * 0.5f)),
		   0, 0, cv::INTER_NEAREST);
	return result;
}

static u64 encode_character_bits(const cv::Mat &character)
{
	cv::Mat ch;
	if (character.rows / 2 >= 1 && character.cols / 2 >= 1)
		ch = downscale_image(character);
	else
		ch = character;

	ch = (ch & 1) ^ 1;

	// Character too large to encode - return 0 (invalid)
	if (ch.rows * ch.cols >= 64)
		return 0;

	u64 num = 0;
	for (int i = 0; i < ch.rows; i++) {
		for (int j = 0; j < ch.cols; j++) {
			u8 c = ch.at<u8>(i, j);
			num = (num << 1) | c;
		}
	}
	return num;
}

static std::array<std::unordered_map<u64, char>, 4> g_charsets{};

static const std::unordered_map<u64, char> &charset_for(typeface tf)
{
	auto &charset = g_charsets[static_cast<u64>(tf)];
	if (!charset.empty()) {
		return charset;
	}

	std::filesystem::path typeface_path = resources::typeface_path(tf);
	std::vector<cv::String> files{};
	cv::glob(typeface_path / "*.png", files, true);
	for (const cv::String &font_char_path : files) {
		char charset_char;

		std::string_view file_name =
			std::filesystem::path(font_char_path).stem().c_str();

		if (file_name == "QUESTION_MARK") {
			charset_char = '?';
		} else if (file_name.length() == 1) {
			charset_char = file_name[0];
		} else {
			throw;
		}

		cv::Mat character_image =
			cv::imread(font_char_path, cv::IMREAD_GRAYSCALE);
		u64 character_bits = encode_character_bits(character_image);

		charset[character_bits] = charset_char;
	}
	return charset;
}

static int find_top_line(const cv::Mat &mat)
{
	for (int row = 0; row < mat.rows; row++) {
		for (int col = 0; col < mat.cols; col++) {
			if (mat.at<uchar>(row, col))
				continue;
			return row;
		}
	}
	return -1;
}

static int find_next_top_line(const cv::Mat &mat, int previous_line,
			      int font_size)
{
	for (int row = previous_line + font_size; row < mat.rows; row++) {
		for (int col = 0; col < mat.cols; col++) {
			if (mat.at<uchar>(row, col))
				continue;
			return row;
		}
	}
	return -1;
}

std::vector<rectangle> extract_character_boxes_multiline(const cv::Mat &image,
							 const font_info &font)
{
	std::vector<rectangle> boxes{};
	constexpr int max_allowed_whitespace = 10;

	const int whitespace_size = font.whitespace_pixel_width;
	const int max_steps_from_last_character =
		whitespace_size * max_allowed_whitespace;

	int line_top = find_top_line(image);
	while (line_top != -1) {
		int last_box_x = INT_MAX;

		int left = INT_MAX;
		int top = INT_MAX;
		int right = 0;
		int bottom = 0;
		for (int x = 0; x < image.cols; x++) {
			// check last character x position, if it's greater than max, cut scanner short
			if (x - last_box_x > max_steps_from_last_character)
				break;

			bool column_has_black_pixel = false;
			const int bottom_of_Line = std::min(
				line_top + font.max_pixels_tall, image.rows);
			for (int y = line_top; y < bottom_of_Line; y++) {
				if (image.at<uchar>(y, x))
					continue;
				// We found a black pixel
				column_has_black_pixel = true;

				top = std::min(top, y);
				bottom = std::max(bottom, y);
			}

			if (column_has_black_pixel) {
				left = std::min(left, x);
				right = std::max(right, x);
			} else if (left != INT_MAX) {
				last_box_x = x;

				int width = right - left + 1;
				int height = bottom - top + 1;
				boxes.emplace_back(
					rectangle{ left, top, width, height });
				left = INT_MAX;
				top = INT_MAX;
				width = 0;
				bottom = 0;
			}
		}

		line_top = find_next_top_line(image, line_top,
					      font.max_pixels_tall);
	}

	return boxes;
}

std::vector<rectangle> extract_character_boxes(const cv::Mat &image,
					       const font_info &font)
{
	if (image.rows / font.max_pixels_tall > 1)
		return extract_character_boxes_multiline(image, font);

	std::vector<rectangle> boxes{};

	int last_box_x = INT_MAX;
	const int whitespace_size =
		font.whitespace_pixel_width; // depends on font
	constexpr int max_allowed_whitespace = 10;
	const int max_steps_from_last_character =
		whitespace_size * max_allowed_whitespace;

	int left = INT_MAX;
	int top = INT_MAX;
	int right = 0;
	int bottom = 0;
	for (int x = 0; x < image.cols; x++) {
		// check last character x position, if it's greater than max, cut scanner short
		if (x - last_box_x > max_steps_from_last_character)
			break;

		bool column_has_black_pixel = false;
		for (int y = 0; y < image.rows; y++) {
			if (image.at<uchar>(y, x))
				continue;
			// We found a black pixel
			column_has_black_pixel = true;

			top = std::min(top, y);
			bottom = std::max(bottom, y);
		}

		if (column_has_black_pixel) {
			left = std::min(left, x);
			right = std::max(right, x);
		} else if (left != INT_MAX) {
			last_box_x = x;

			int width = right - left + 1;
			int height = bottom - top + 1;
			boxes.emplace_back(
				rectangle{ left, top, width, height });
			left = INT_MAX;
			top = INT_MAX;
			width = 0;
			bottom = 0;
		}
	}

	return boxes;
}

bool extract_text_strict(std::string &out, const cv::Mat &binary_image,
			 typeface tf)
{
	const font_info &font = font_info_for(tf);
	std::vector<rectangle> boxes =
		extract_character_boxes(binary_image, font);

	const auto &charset = charset_for(tf);

	const rectangle *previous_rectangle = nullptr;
	char previous_char = '\0';

	std::string field{};
	for (const auto &box : boxes) {
		cv::Mat character = binary_image(box.to_cv());
		u64 encoded_char = encode_character_bits(character);

		if (encoded_char == 0 ||
		    charset.find(encoded_char) == charset.end()) {
			// TODO: Add error logging
			fprintf(stderr, "Invalid char");
			std::cin.get();
			return false;
		}

		char ch = charset.at(encoded_char);

		if (previous_rectangle) { // add spaces
			int horizontal_space_from_last_box =
				box.x - (previous_rectangle->x +
					 previous_rectangle->width);
			const int letter_space_size =
				font.letter_spacing_horizontal;
			const int whitespace_size = font.whitespace_pixel_width;

			int whitespaces = ((horizontal_space_from_last_box -
					    letter_space_size) /
					   whitespace_size);

			if (!(whitespaces >= 1 && ch == '1' &&
			      field.back() == '1')) {
				if (whitespaces > 0)
					field.append(whitespaces, ' ');
			}
		}

		if (previous_rectangle) { // add newlines
			int vertical_space_from_last_box =
				box.y - (previous_rectangle->y +
					 previous_rectangle->height);
			const int wrapSize = font.letter_spacing_vertical;
			const int newLineSize = font.newline_size;
			int newlines =
				((vertical_space_from_last_box - wrapSize) /
				 newLineSize);

			if (vertical_space_from_last_box - wrapSize >=
			    0) { // add extra space when wrapping
				field.push_back(' ');
			}

			if (newlines > 0)
				field.append(newlines, '\n');
		}

		previous_rectangle = &box;
		previous_char = ch;

		field.push_back(ch);
	}

	out = field;
	return true;
}
