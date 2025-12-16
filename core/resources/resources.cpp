#include "magic_enum/magic_enum.hpp"
#include <cassert>
#include <filesystem>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>

#include <paplease/compiler.h>
#include <paplease/ocr.h>
#include <paplease/resources.h>

static bool load_charset(std::unordered_map<u64, char> &out,
			 const std::filesystem::path &asset_root, typeface tf)
{
	std::filesystem::path typeface_path = asset_root / "typefaces";
	switch (tf) {
	case typeface::booth:
		typeface_path /= "BoothNumber";
		break;
	case typeface::bm_mini:
		typeface_path /= "BM Mini";
		break;
	case typeface::mini_kylie:
		typeface_path /= "MiniKylie";
		break;
	case typeface::t04b03:
		typeface_path /= "04b03";
		break;
	}

	std::vector<cv::String> files;
	cv::glob(typeface_path / "*.png", files, true);

	if (files.empty())
		return false; // No font files found

	for (const cv::String &path : files) {
		std::string_view name =
			std::filesystem::path(path).stem().c_str();

		char ch;
		if (name == "QUESTION_MARK")
			ch = '?';
		else if (name.length() == 1)
			ch = name[0];
		else
			continue; // Skip malformed files instead of throwing

		cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);
		if (img.empty())
			continue;

		u64 bits = encode_character_bits(img);
		out[bits] = ch;
	}

	return !out.empty();
}
resources_ctx make_resources(std::string_view root)
{
	resources_ctx ctx;
	ctx.asset_root = root;

	// Load all charsets at construction
	for (int i = 0; i < magic_enum::enum_count<typeface>(); i++) {
		load_charset(ctx.charsets[i], ctx.asset_root,
			     static_cast<typeface>(i));
	}
	return ctx;
}
