#include <cstdio>
#include <ctime>
#include <limits.h>
#include <unordered_map>
#include <vector>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <paplease/debug.h>
#include <paplease/geometry.h>
#include <paplease/ocr.h>
#include <paplease/resources.h>
#include <paplease/types.h>

#include "font_info.h"

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

u64 encode_character_bits(const cv::Mat &character)
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

#ifdef DEBUG_OCR
static bool validate_roi(const cv::Mat &image, const cv::Rect &roi,
			 const char *)
{
	return roi.x >= 0 && roi.y >= 0 && roi.x + roi.width <= image.cols &&
	       roi.y + roi.height <= image.rows;
}

static void debug_dump_character_boxes(
	const cv::Mat &image, const std::vector<rectangle> &boxes,
	const std::unordered_map<u64, char> &charset, const char *label)
{
	constexpr int SCALE = 4;

	cv::Mat scaled;
	cv::resize(image, scaled,
		   cv::Size(image.cols * SCALE, image.rows * SCALE), 0, 0,
		   cv::INTER_NEAREST);

	cv::Mat debug_img;
	cv::cvtColor(scaled, debug_img, cv::COLOR_GRAY2BGR);

	// First pass: draw all boxes
	for (const auto &box : boxes) {
		if (!validate_roi(image, box.to_cv(), "debug_boxes"))
			continue;

		cv::Mat character = image(box.to_cv());
		u64 encoded = encode_character_bits(character);

		auto it = charset.find(encoded);
		bool recognized = (it != charset.end() && encoded != 0);

		cv::Scalar color = recognized ? cv::Scalar(0, 200, 0) :
						cv::Scalar(0, 0, 255);

		cv::Rect scaled_box(box.x * SCALE, box.y * SCALE,
				    box.width * SCALE, box.height * SCALE);
		cv::rectangle(debug_img, scaled_box, color, 1);
	}

	// Text report to stderr
	DBG_OCR("\n=== OCR DEBUG: %s (%dx%d, %zu boxes) ===\n", label,
		image.cols, image.rows, boxes.size());
	DBG_OCR("%-4s %-6s %-10s %-12s %s\n", "#", "CHAR", "POS(x,y)",
		"SIZE(wxh)", "ENCODED");
	DBG_OCR("----------------------------------------------\n");

	int box_idx = 0;
	for (const auto &box : boxes) {
		if (!validate_roi(image, box.to_cv(), nullptr))
			continue;

		cv::Mat character = image(box.to_cv());
		u64 encoded = encode_character_bits(character);

		auto it = charset.find(encoded);
		char ch = (it != charset.end()) ? it->second : '?';
		bool recognized = (it != charset.end() && encoded != 0);

		DBG_OCR("%-4d %-6c (%3d,%3d)   %2dx%-2d        0x%llx%s\n",
			box_idx, ch, box.x, box.y, box.width, box.height,
			(unsigned long long)encoded,
			recognized ? "" : " <-- UNKNOWN");
		box_idx++;
	}
	DBG_OCR("==============================================\n\n");

	char filename[256];
	snprintf(filename, sizeof(filename), "/tmp/ocr_debug_%s_%ld.png", label,
		 (long)time(nullptr));
	cv::imwrite(filename, debug_img);
	DBG_OCR("OCR debug image: %s\n", filename);
}
#else
static inline void
debug_dump_character_boxes(const cv::Mat &, const std::vector<rectangle> &,
			   const std::unordered_map<u64, char> &, const char *)
{
}
#endif
static const std::unordered_map<u64, char> &
charset_for(const resources_ctx &ctx, typeface tf)
{
	return ctx.charsets[static_cast<size_t>(tf)];
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

static void scan_line_for_boxes(std::vector<rectangle> &boxes,
				const cv::Mat &image, int line_top,
				int line_bottom, const font_info &font)
{
	const int whitespace_size = font.whitespace_pixel_width;
	constexpr int max_allowed_whitespace = 10;
	const int max_steps_from_last_character =
		whitespace_size * max_allowed_whitespace;

	int last_box_x = INT_MAX;
	int left = INT_MAX, top = INT_MAX, right = 0, bottom = 0;

	for (int x = 0; x < image.cols; x++) {
		if (x - last_box_x > max_steps_from_last_character)
			break;

		bool column_has_black_pixel = false;
		for (int y = line_top; y < line_bottom; y++) {
			if (image.at<uchar>(y, x))
				continue;
			column_has_black_pixel = true;
			top = std::min(top, y);
			bottom = std::max(bottom, y);
		}

		if (column_has_black_pixel) {
			left = std::min(left, x);
			right = std::max(right, x);
		} else if (left != INT_MAX) {
			last_box_x = x;
			boxes.emplace_back(rectangle{ left, top,
						      right - left + 1,
						      bottom - top + 1 });
			left = INT_MAX;
			top = INT_MAX;
			right = 0;
			bottom = 0;
		}
	}
}

std::vector<rectangle> extract_character_boxes(const cv::Mat &image,
					       const font_info &font)
{
	std::vector<rectangle> boxes{};

	int line_top = find_top_line(image);
	while (line_top != -1) {
		int line_bottom =
			std::min(line_top + font.max_pixels_tall, image.rows);
		scan_line_for_boxes(boxes, image, line_top, line_bottom, font);
		line_top = find_next_top_line(image, line_top,
					      font.max_pixels_tall);
	}

	return boxes;
}

bool extract_text_strict(std::string &out, const cv::Mat &binary_image,
			 typeface tf, const resources_ctx &ctx)
{
	const font_info &font = font_info_for(tf);
	std::vector<rectangle> boxes =
		extract_character_boxes(binary_image, font);

	const auto &charset = charset_for(ctx, tf);
	if (charset.empty()) {
		DBG_OCR("OCR error: charset for requested typeface is empty\n");
		return false;
	}

	const rectangle *previous_rectangle = nullptr;

	std::string field{};
	for (const auto &box : boxes) {
		cv::Mat character = binary_image(box.to_cv());
		u64 encoded_char = encode_character_bits(character);

		if (encoded_char == 0 ||
		    charset.find(encoded_char) == charset.end()) {
			DBG_OCR("OCR error: unrecognized character at box (%d,%d) size %dx%d, encoded=0x%llx\n",
				box.x, box.y, box.width, box.height,
				(unsigned long long)encoded_char);

			debug_dump_character_boxes(binary_image, boxes, charset,
						   "text_field");
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

		field.push_back(ch);
	}

	out = field;
	return true;
}
