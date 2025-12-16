#pragma once

// ============================================
// Debug flags - uncomment to enable per-module
// Or pass -DDEBUG_VISION etc. to compiler
// ============================================
// #define DEBUG_VISION
// #define DEBUG_OCR
// #define DEBUG_INSPECTOR
// #define DEBUG_DOCUMENTS

// Debug output macros
#include <cstdio>

#ifdef DEBUG_VISION
	#define DBG_VISION(...) fprintf(stderr, "[VISION] " __VA_ARGS__)
#else
	#define DBG_VISION(...) ((void)0)
#endif

#ifdef DEBUG_OCR
	#define DBG_OCR(...) fprintf(stderr, "[OCR] " __VA_ARGS__)
#else
	#define DBG_OCR(...) ((void)0)
#endif

#ifdef DEBUG_INSPECTOR
	#define DBG_INSPECTOR(...) fprintf(stderr, "[INSPECTOR] " __VA_ARGS__)
#else
	#define DBG_INSPECTOR(...) ((void)0)
#endif

#ifdef DEBUG_DOCUMENTS
	#define DBG_DOCUMENTS(...) fprintf(stderr, "[DOCUMENTS] " __VA_ARGS__)
#else
	#define DBG_DOCUMENTS(...) ((void)0)
#endif

// ============================================
// Debug utilities for OCR and document parsing
// ============================================

#include <string>
#include <vector>

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <paplease/geometry.h>
#include <paplease/ocr.h>
#include <paplease/resources.h>


// Debug: extract text from a box, print result with coordinates
// Returns the extracted text (empty on failure)
inline std::string debug_ocr(const cv::Mat &img, const rectangle &box,
			     typeface tf, const resources_ctx &ctx,
			     const char *label = nullptr)
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
	bool ok = extract_text_strict(text, region, tf, ctx);
	
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

