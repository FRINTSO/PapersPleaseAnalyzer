#ifndef PAPLEASE_PARSE_HELPERS_H
#define PAPLEASE_PARSE_HELPERS_H

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <paplease/documents.h>
#include <paplease/geometry.h>
#include <string>
#include <vector>
#include <cstdio>

inline cv::Mat preprocess_document(const cv::Mat &img, int threshValue = 127, int thresholdType = cv::THRESH_BINARY)
{
	cv::Mat grayscale;
	if (img.channels() == 1)
		grayscale = img;
	else
		cv::cvtColor(img, grayscale, cv::COLOR_BGR2GRAY);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, threshValue, 255, thresholdType);
	return thresh;
}

// === DEBUG TOOLS ===

// Debug: extract text from a box, print result with coordinates
// Returns the extracted text (empty on failure)
inline std::string debug_ocr(const cv::Mat &img, const rectangle &box, typeface tf, const char *label = nullptr)
{
	std::string text;
	
	// Print label or coordinates
	if (label)
		printf("[%s] ", label);
	printf("box(%d,%d %dx%d): ", box.x, box.y, box.width, box.height);
	
	// Check bounds
	if (box.x < 0 || box.y < 0 ||
	    box.x + box.width > img.cols ||
	    box.y + box.height > img.rows) {
		printf("OUT_OF_BOUNDS (img is %dx%d)\n", img.cols, img.rows);
		return "";
	}
	
	cv::Mat region = img(box.to_cv());
	bool ok = extract_text_strict(text, region, tf);
	
	if (!ok) {
		printf("FAILED\n");
	} else if (text.empty()) {
		printf("EMPTY\n");
	} else {
		printf("\"%s\"\n", text.c_str());
	}
	
	return text;
}

// Debug: save image with a single box drawn on it
inline void debug_save_box(const cv::Mat &img, const rectangle &box, const char *path)
{
	cv::Mat out = img.clone();
	if (out.channels() == 1)
		cv::cvtColor(out, out, cv::COLOR_GRAY2BGR);
	
	cv::rectangle(out,
		cv::Point(box.x, box.y),
		cv::Point(box.x + box.width, box.y + box.height),
		cv::Scalar(0, 0, 255), 1);
	
	cv::imwrite(path, out);
	printf("Saved: %s\n", path);
}

// Debug: save image with multiple labeled boxes
inline void debug_save_boxes(const cv::Mat &img, const std::vector<std::pair<const char*, rectangle>> &boxes, const char *path)
{
	cv::Mat out = img.clone();
	if (out.channels() == 1)
		cv::cvtColor(out, out, cv::COLOR_GRAY2BGR);
	
	for (const auto &[label, box] : boxes) {
		cv::rectangle(out,
			cv::Point(box.x, box.y),
			cv::Point(box.x + box.width, box.y + box.height),
			cv::Scalar(0, 0, 255), 1);
		
		if (label) {
			cv::putText(out, label,
				cv::Point(box.x, box.y - 3),
				cv::FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(0, 0, 255), 1);
		}
	}
	
	cv::imwrite(path, out);
	printf("Saved: %s\n", path);
}

// Debug: extract and print all boxes, save annotated image
inline void debug_ocr_all(const cv::Mat &img, const std::vector<std::pair<const char*, rectangle>> &boxes, 
                          typeface tf, const char *output_path)
{
	cv::Mat binary = preprocess_document(img);
	cv::Mat annotated = img.clone();
	if (annotated.channels() == 1)
		cv::cvtColor(annotated, annotated, cv::COLOR_GRAY2BGR);
	
	printf("\n=== OCR DEBUG (%dx%d) ===\n", img.cols, img.rows);
	
	for (const auto &[label, box] : boxes) {
		std::string text;
		bool ok = false;
		bool oob = (box.x < 0 || box.y < 0 ||
		            box.x + box.width > binary.cols ||
		            box.y + box.height > binary.rows);
		
		cv::Scalar color;
		if (oob) {
			color = cv::Scalar(255, 0, 255);  // Magenta
		} else {
			ok = extract_text_strict(text, binary(box.to_cv()), tf);
			color = ok ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255);  // Green or Red
		}
		
		cv::rectangle(annotated,
			cv::Point(box.x, box.y),
			cv::Point(box.x + box.width, box.y + box.height),
			color, 1);
		
		if (label) {
			cv::putText(annotated, label,
				cv::Point(box.x, box.y - 3),
				cv::FONT_HERSHEY_SIMPLEX, 0.35, color, 1);
		}
		
		// Console output
		printf("%-16s [%3d,%3d %3dx%3d] ", label ? label : "", box.x, box.y, box.width, box.height);
		if (oob) printf("OOB\n");
		else if (!ok) printf("FAIL\n");
		else if (text.empty()) printf("(empty)\n");
		else printf("\"%s\"\n", text.c_str());
	}
	
	cv::imwrite(output_path, annotated);
	printf("Saved: %s\n", output_path);
}

// Safely extract a region, returns empty Mat if out of bounds
inline cv::Mat safe_region(const cv::Mat &img, const rectangle &box)
{
	if (box.x < 0 || box.y < 0)
		return {};
	if (box.x + box.width > img.cols || box.y + box.height > img.rows)
		return {};
	return img(box.to_cv());
}

// Extract text from a region, returns false if region invalid or OCR fails
inline bool extract_field(std::string &out, const cv::Mat &img, const rectangle &box, typeface tf)
{
	cv::Mat region = safe_region(img, box);
	if (region.empty())
		return false;
	return extract_text_strict(out, region, tf);
}

inline bool parse_date(date_t &out, const std::string &text)
{
	int day, month, year;
	if (sscanf(text.c_str(), "%d.%d.%d", &day, &month, &year) != 3)
		return false;
	if (year < 100)
		year += 1900;
	out = {(u8)day, (u8)month, (u16)year};
	return true;
}

inline bool parse_height(u16 &out, const std::string &text)
{
	int val;
	if (sscanf(text.c_str(), "%dcm", &val) != 1)
		return false;
	out = (u16)val;
	return true;
}

inline bool parse_weight(u16 &out, const std::string &text)
{
	int val;
	if (sscanf(text.c_str(), "%dkg", &val) != 1)
		return false;
	out = (u16)val;
	return true;
}

#endif // PAPLEASE_PARSE_HELPERS_H

