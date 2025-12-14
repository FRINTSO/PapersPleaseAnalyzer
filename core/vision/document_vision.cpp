#include "opencv2/core.hpp"
#include "paplease/colorspace.h"
#include <paplease/compiler.h>
#include <paplease/documents.h>
#include <paplease/geometry.h>
#include <paplease/game_screen.h>
#include <paplease/vision.h>
#include <vector>

static cv::Mat preprocess_for_scanning(const cv::Mat &area, ui_section section)
{
	if (section == ui_section::booth) {
		TODO("Implement preprocessing for booth document scanning.");
	}

	static hsl_range config{ 0, 179, 45, 255, 0, 255 };
	auto mask = bgr_to_hsl_mask(area, config);
	cv::Mat result;
	cv::bitwise_and(area, area, result, mask);

	// HIGHLIGHT DISCREPANCIES
	static hsl_range highlightDiscrepanciesActiveConfig{ 0,	  0,   112,
							     112, 157, 157 };
	auto highlightMask =
		bgr_to_hsl_mask(area, highlightDiscrepanciesActiveConfig);

	static hsl_range majorDocumentLocatorConfig{ 0, 179, 135, 255, 0, 255 };
	return result;
}

static constexpr rgb_color passport_colors[] = {
	{ /* antegria RGB */ get_appearance(doc_variant::passport_antegria)
		  .border_colors[0] },
	{ /* arstotzka RGB */ get_appearance(doc_variant::passport_arstotzka)
		  .border_colors[0] },
	{ /* impor RGB */ get_appearance(doc_variant::passport_impor)
		  .border_colors[0] },
	{ /* kolechia RGB */ get_appearance(doc_variant::passport_kolechia)
		  .border_colors[0] },
	{ /* obristan RGB */ get_appearance(doc_variant::passport_obristan)
		  .border_colors[0] },
	{ /* republia RGB */ get_appearance(doc_variant::passport_republia)
		  .border_colors[0] },
	{ /* united_federation RGB */ get_appearance(
		  doc_variant::passport_united_federation)
		  .border_colors[0] },
};

static constexpr int passport_color_count =
	sizeof(passport_colors) / sizeof(passport_colors[0]);

static bool detect_passport_country(country &out, const cv::Mat &processed)
{
	for (int row = 0; row < processed.rows; row++) {
		const bgr_color *pixel = processed.ptr<bgr_color>(row);

		for (int col = 0; col < processed.cols; col++) {
			u32 pixel_val = pixel[col].to_u32();

			for (int i = 0; i < passport_color_count; i++) {
				if (pixel_val == passport_colors[i].to_u32()) {
					out = static_cast<country>(i);
					return true;
				}
			}
		}
	}
	return false;
}

static bool matches_border_color(u32 pixel_bgr, const doc_appearance &app)
{
	for (u8 i = 0; i < app.color_count; i++) {
		if (pixel_bgr == app.border_colors[i].to_u32())
			return true;
	}
	return false;
}

static bool is_border_color_at(const cv::Mat &image, int x, int y,
			       const doc_appearance &app)
{
	u32 pixel = image.ptr<bgr_color>(y, x)->to_u32();
	return matches_border_color(pixel, app);
}

rectangle find_doc_bounding_box(const cv::Mat &image, const doc_appearance &app)
{
	int left = INT_MAX, top = INT_MAX, right = 0, bottom = 0;
	bool found_first = false;

	int max_y = image.rows;
	int min_x = 0, max_x = image.cols;

	for (int y = 0; y < max_y; y++) {
		const bgr_color *row = image.ptr<bgr_color>(y);

		int row_min_x = INT_MAX, row_max_x = 0;
		bool row_matched = false;

		for (int x = min_x; x < max_x; x++) {
			if (!matches_border_color(row[x].to_u32(), app))
				continue;

			// Cheeky early-out: first match, check opposite corner
			if (!found_first) {
				found_first = true;
				int br_x = x + app.width - 1;
				int br_y = y + app.height - 1;
				if (br_x < image.cols && br_y < image.rows) {
					if (is_border_color_at(image, br_x,
							       br_y, app))
						return { x, y, (int)app.width,
							 (int)app.height };
				}
			}

			row_matched = true;
			row_min_x = std::min(row_min_x, x);
			row_max_x = std::max(row_max_x, x);
		}

		if (row_matched) {
			if (y < top) {
				top = y;
				max_y = std::min(image.rows,
						 top + (int)app.height);
			}
			bottom = std::max(bottom, y);

			if (row_min_x < left) {
				left = row_min_x;
				max_x = std::min(image.cols,
						 left + (int)app.width);
			}
			if (row_max_x > right) {
				right = row_max_x;
				min_x = std::max(0, right - (int)app.width + 1);
			}
		}
	}

	if (left == INT_MAX)
		return {}; // nothing found

	return { left, top, right - left + 1, bottom - top + 1 };
}

// Check if corner pixel traces inward along border
static bool is_valid_corner(const cv::Mat &image, const rectangle &bbox,
			    bool is_top, bool is_left,
			    const doc_appearance &app)
{
	int x_corner = bbox.x + (bbox.width - 1) * !is_left;
	int y_corner = bbox.y + (bbox.height - 1) * !is_top;

	int step = 0;
	int dx = is_left ? 1 : -1;
	int dy = is_top ? 1 : -1;

	// Walk diagonally inward while on border color
	while (step < bbox.width && step < bbox.height) {
		if (!is_border_color_at(image, x_corner + step * dx,
					y_corner + step * dy, app))
			break;
		step++;
	}

	if (step == 0)
		return false;

	// Check perpendicular edges exist
	return is_border_color_at(image, x_corner + step * dx, y_corner, app) &&
	       is_border_color_at(image, x_corner, y_corner + step * dy, app);
}

bool validate_doc_bounding_box(const cv::Mat &image, const rectangle &bbox,
			       const doc_appearance &app)
{
	// Reject if too large
	if (bbox.width > app.width || bbox.height > app.height)
		return false;

	// Accept if exact size
	if (bbox.width == app.width && bbox.height == app.height)
		return true;

	// Accept if any corner is valid (partial visibility)
	if (is_valid_corner(image, bbox, true, true, app))
		return true; // top-left
	if (is_valid_corner(image, bbox, true, false, app))
		return true; // top-right
	if (is_valid_corner(image, bbox, false, true, app))
		return true; // bottom-left
	if (is_valid_corner(image, bbox, false, false, app))
		return true; // bottom-right

	return false;
}

bool find_document(doc &out, doc_type type, ui_section section,
		   const game_screen &screen)
{
	// 1. Get section
	cv::Mat area = slice_section(screen, section);

	// 2. Preprocess
	cv::Mat processed = preprocess_for_scanning(area, section);

	// 3. Handle passport special case
	doc_variant variant;
	if (type == doc_type::passport) {
		country c;
		if (!detect_passport_country(c, processed))
			return false;

		variant = variant_from_country(c);
	} else {
		variant = variant_from_type(type); // simple mapping
	}

	// 4. Get appearance
	const doc_appearance &appearance = get_appearance(variant);

	// 5. Find bounding box
	rectangle bbox = find_doc_bounding_box(processed, appearance);
	if (bbox.empty())
		return false;

	// 6. Extract and return
	out.pixels = area(bbox.to_cv());
	out.variant = variant;
	return true;
}

std::vector<doc> find_all_documents(const game_screen &screen, ui_section section)
{
	cv::Mat area = slice_section(screen, section);
	cv::Mat processed = preprocess_for_scanning(area, section);

	std::vector<doc> documents{};
	constexpr u64 max_count =
		static_cast<u64>(doc_variant::passport_united_federation);
	for (int i = 0; i <= max_count; i++) {
		doc_variant variant = static_cast<doc_variant>(i);
		const doc_appearance &appearance = get_appearance(variant);

		// 5. Find bounding box
		rectangle bbox = find_doc_bounding_box(processed, appearance);
		if (bbox.empty())
			continue;

		// 6. Extract and return
		documents.emplace_back(
			doc{ .pixels = area(bbox.to_cv()), .variant = variant });
	}

	return documents;
}
