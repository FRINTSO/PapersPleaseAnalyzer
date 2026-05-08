#include <climits>
#include <ctime>
#include <vector>

#include <magic_enum/magic_enum.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <paplease/colorspace.h>
#include <paplease/compiler.h>
#include <paplease/debug.h>
#include <paplease/documents.h>
#include <paplease/game_screen.h>
#include <paplease/geometry.h>
#include <paplease/vision.h>

static cv::Mat preprocess_for_scanning(const cv::Mat &area, ui_section section)
{
	if (section == ui_section::booth) {
		TODO("Implement preprocessing for booth document scanning.");
	}

	constexpr hsl_range config{ 0, 179, 45, 255, 0, 255 };
	auto mask = bgr_to_hsl_mask(area, config);
	cv::Mat result;
	cv::bitwise_and(area, area, result, mask);
	return result;
}

static constexpr rgb_color passport_colors[] = {
	{ /* antegria RGB */ get_passport_appearance(country::antegria)
		  .border_colors[0] },
	{ /* arstotzka RGB */ get_passport_appearance(country::arstotzka)
		  .border_colors[0] },
	{ /* impor RGB */ get_passport_appearance(country::impor)
		  .border_colors[0] },
	{ /* kolechia RGB */ get_passport_appearance(country::kolechia)
		  .border_colors[0] },
	{ /* obristan RGB */ get_passport_appearance(country::obristan)
		  .border_colors[0] },
	{ /* republia RGB */ get_passport_appearance(country::republia)
		  .border_colors[0] },
	{ /* united_federation RGB */ get_passport_appearance(
		  country::united_federation)
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

static rectangle find_doc_bounding_box(const cv::Mat &image,
				       const doc_appearance &app)
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

static bool is_valid_doc(const cv::Mat &image, const rectangle &bbox,
			 const doc_appearance &appearance)
{
	if (bbox.empty())
		return false;

	if (bbox.width != appearance.width || bbox.height != appearance.height)
		return false;

	return true;
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

	// 3. Get appearance (handle passport special case)
	const doc_appearance *appearance;
	country detected_country{};

	if (type == doc_type::passport) {
		if (!detect_passport_country(detected_country, processed))
			return false;
		appearance = &get_passport_appearance(detected_country);
	} else {
		appearance = &get_appearance(type);
	}

	// 4. Find bounding box
	rectangle bbox = find_doc_bounding_box(processed, *appearance);
	if (!is_valid_doc(processed, bbox, *appearance))
		return false;

	// 5. Extract and return
	out.pixels = area(bbox.to_cv());
	out.type = type;
	out.issuing_country = detected_country;
	return true;
}

std::vector<doc> scan_for_documents(const game_screen &screen, ui_section section)
{
	cv::Mat area = slice_section(screen, section);
	cv::Mat processed = preprocess_for_scanning(area, section);

	std::vector<doc> documents{};
	constexpr int non_passport_count = static_cast<int>(doc_type::passport);
	for (int i = 0; i < non_passport_count; i++) {
		doc_type type = static_cast<doc_type>(i);
		const doc_appearance &appearance = get_appearance(type);

		rectangle bbox = find_doc_bounding_box(processed, appearance);
		if (!is_valid_doc(processed, bbox, appearance))
			continue;

		documents.emplace_back(doc{
			.pixels = area(bbox.to_cv()),
			.type = type,
			.issuing_country = {} // Not applicable for non-passports
		});
	}

	// Scan passports (one per country)
	constexpr int country_count =
		static_cast<int>(country::united_federation) + 1;
	for (int i = 0; i < country_count; i++) {
		country c = static_cast<country>(i);
		const doc_appearance &appearance = get_passport_appearance(c);

		rectangle bbox = find_doc_bounding_box(processed, appearance);
		if (!is_valid_doc(processed, bbox, appearance))
			continue;

		documents.emplace_back(doc{ .pixels = area(bbox.to_cv()),
					    .type = doc_type::passport,
					    .issuing_country = c });
	};
	return documents;
}
