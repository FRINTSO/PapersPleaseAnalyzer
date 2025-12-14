/*
 * test_is_image_file.cpp - Test image file detection
 *
 * Since is_image_file is static in game_screen.cpp, we test the same logic here.
 */

#include <filesystem>
#include <string>
#include <algorithm>
#include <cctype>
#include "test.h"

/* Local copy of the logic from game_screen.cpp */
static bool is_image_file(const std::string &path)
{
	if (!std::filesystem::exists(path) ||
	    !std::filesystem::is_regular_file(path))
		return false;

	std::string ext = std::filesystem::path(path).extension();
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".tif" ||
	    ext == ".tiff" || ext == ".bmp" || ext == ".webp")
		return true;
	return false;
}

/* Test version that only checks extension logic (no filesystem) */
static bool is_image_extension(const std::string &path)
{
	std::string ext = std::filesystem::path(path).extension();
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".tif" ||
	    ext == ".tiff" || ext == ".bmp" || ext == ".webp")
		return true;
	return false;
}

int main()
{
	/* Test extension detection without filesystem dependency */

	/* Valid image extensions */
	TEST_ASSERT(is_image_extension("test.png"));
	TEST_ASSERT(is_image_extension("test.jpg"));
	TEST_ASSERT(is_image_extension("test.jpeg"));
	TEST_ASSERT(is_image_extension("test.bmp"));
	TEST_ASSERT(is_image_extension("test.tif"));
	TEST_ASSERT(is_image_extension("test.tiff"));
	TEST_ASSERT(is_image_extension("test.webp"));

	/* Case insensitivity */
	TEST_ASSERT(is_image_extension("test.PNG"));
	TEST_ASSERT(is_image_extension("test.JPG"));
	TEST_ASSERT(is_image_extension("test.JPEG"));
	TEST_ASSERT(is_image_extension("TEST.BMP"));

	/* Mixed case */
	TEST_ASSERT(is_image_extension("test.PnG"));
	TEST_ASSERT(is_image_extension("test.JpEg"));

	/* Invalid extensions */
	TEST_ASSERT(!is_image_extension("test.txt"));
	TEST_ASSERT(!is_image_extension("test.cpp"));
	TEST_ASSERT(!is_image_extension("test.pdf"));
	TEST_ASSERT(!is_image_extension("test.gif")); /* GIF not in the list */
	TEST_ASSERT(!is_image_extension("test.svg"));
	TEST_ASSERT(!is_image_extension("test"));     /* No extension */

	/* Path with directories */
	TEST_ASSERT(is_image_extension("/path/to/image.png"));
	TEST_ASSERT(is_image_extension("relative/path/image.jpg"));
	TEST_ASSERT(!is_image_extension("/path/to/document.pdf"));

	/* Edge cases */
	TEST_ASSERT(!is_image_extension(".png"));      /* Just extension, technically valid path */
	TEST_ASSERT(is_image_extension("a.png"));      /* Minimal valid */
	TEST_ASSERT(!is_image_extension(""));          /* Empty string */

	return 0;
}

