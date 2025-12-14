/*
 * test_encode_character_bits.cpp - Test character bit encoding for OCR
 *
 * Tests the algorithm that converts a binary character image into a u64.
 * 
 * ENCODING RULES (from ocr.cpp):
 * 1. If image is >= 2x2, it gets downscaled by 0.5x with INTER_NEAREST
 * 2. Each pixel: (value & 1) ^ 1  ->  0 becomes 1, 255 becomes 0
 * 3. Pixels read row-by-row, left-to-right, shifted into u64
 */

#include <cstdio>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <paplease/types.h>
#include "test.h"

/*
 * Local copy of the encoding logic from ocr.cpp for testing.
 */
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

	u64 num = 0;
	for (int i = 0; i < ch.rows; i++) {
		for (int j = 0; j < ch.cols; j++) {
			u8 c = ch.at<u8>(i, j);
			num = (num << 1) | c;
		}
	}
	return num;
}

/* Helper to print binary representation */
static void print_binary(const char* name, u64 val, int bits) {
	fprintf(stderr, "  %s = 0b", name);
	for (int i = bits - 1; i >= 0; i--) {
		fprintf(stderr, "%d", (int)((val >> i) & 1));
	}
	fprintf(stderr, " (%llu)\n", (unsigned long long)val);
}

int main()
{
	fprintf(stderr, "=== Encoding Rules ===\n");
	fprintf(stderr, "  Black pixel (0)   -> bit 1\n");
	fprintf(stderr, "  White pixel (255) -> bit 0\n");
	fprintf(stderr, "  Reading order: row-by-row, left-to-right\n\n");

	/*
	 * Test 1: 1x1 image (no downscale possible)
	 * Single black pixel should encode to 1
	 */
	fprintf(stderr, "Test 1: 1x1 black pixel\n");
	cv::Mat single_black(1, 1, CV_8UC1, cv::Scalar(0));
	u64 enc1 = encode_character_bits(single_black);
	print_binary("result", enc1, 1);
	TEST_ASSERT_EQ(enc1, 1u);

	/*
	 * Test 2: 1x1 white pixel
	 * Single white pixel should encode to 0
	 */
	fprintf(stderr, "Test 2: 1x1 white pixel\n");
	cv::Mat single_white(1, 1, CV_8UC1, cv::Scalar(255));
	u64 enc2 = encode_character_bits(single_white);
	print_binary("result", enc2, 1);
	TEST_ASSERT_EQ(enc2, 0u);

	/*
	 * Test 3: 1x4 row (no downscale - only 1 row)
	 * Pattern: [black, white, white, black] = [0, 255, 255, 0]
	 * Expected bits: 1, 0, 0, 1 = 0b1001 = 9
	 */
	fprintf(stderr, "Test 3: 1x4 row [B W W B]\n");
	cv::Mat row(1, 4, CV_8UC1);
	row.at<u8>(0, 0) = 0;
	row.at<u8>(0, 1) = 255;
	row.at<u8>(0, 2) = 255;
	row.at<u8>(0, 3) = 0;
	u64 enc3 = encode_character_bits(row);
	print_binary("result", enc3, 4);
	TEST_ASSERT_EQ(enc3, 0b1001u);

	/*
	 * Test 4: 4x1 column (no downscale - only 1 col)
	 * Pattern: [black, white, black, white] = top to bottom
	 * Expected bits: 1, 0, 1, 0 = 0b1010 = 10
	 */
	fprintf(stderr, "Test 4: 4x1 column [B W B W]\n");
	cv::Mat col(4, 1, CV_8UC1);
	col.at<u8>(0, 0) = 0;
	col.at<u8>(1, 0) = 255;
	col.at<u8>(2, 0) = 0;
	col.at<u8>(3, 0) = 255;
	u64 enc4 = encode_character_bits(col);
	print_binary("result", enc4, 4);
	TEST_ASSERT_EQ(enc4, 0b1010u);

	/*
	 * Test 5: 2x2 -> downscales to 1x1
	 * All black 2x2 -> 1x1 black -> 1
	 */
	fprintf(stderr, "Test 5: 2x2 all black (downscales to 1x1)\n");
	cv::Mat black_2x2(2, 2, CV_8UC1, cv::Scalar(0));
	u64 enc5 = encode_character_bits(black_2x2);
	print_binary("result", enc5, 1);
	TEST_ASSERT_EQ(enc5, 1u);

	/*
	 * Test 6: 4x4 -> downscales to 2x2
	 * Create 4x4 with known pattern, verify 2x2 result
	 *
	 * Input 4x4 (each 2x2 block same color):
	 *   BB WW      B = black (0)
	 *   BB WW      W = white (255)
	 *   WW BB
	 *   WW BB
	 *
	 * After downscale to 2x2:
	 *   B W
	 *   W B
	 *
	 * Encoding: row0=[1,0], row1=[0,1] -> 1,0,0,1 = 0b1001 = 9
	 */
	fprintf(stderr, "Test 6: 4x4 checkerboard (downscales to 2x2)\n");
	cv::Mat checker(4, 4, CV_8UC1);
	// Top-left 2x2: black
	checker.at<u8>(0, 0) = 0; checker.at<u8>(0, 1) = 0;
	checker.at<u8>(1, 0) = 0; checker.at<u8>(1, 1) = 0;
	// Top-right 2x2: white
	checker.at<u8>(0, 2) = 255; checker.at<u8>(0, 3) = 255;
	checker.at<u8>(1, 2) = 255; checker.at<u8>(1, 3) = 255;
	// Bottom-left 2x2: white
	checker.at<u8>(2, 0) = 255; checker.at<u8>(2, 1) = 255;
	checker.at<u8>(3, 0) = 255; checker.at<u8>(3, 1) = 255;
	// Bottom-right 2x2: black
	checker.at<u8>(2, 2) = 0; checker.at<u8>(2, 3) = 0;
	checker.at<u8>(3, 2) = 0; checker.at<u8>(3, 3) = 0;

	u64 enc6 = encode_character_bits(checker);
	print_binary("result", enc6, 4);
	TEST_ASSERT_EQ(enc6, 0b1001u);

	/*
	 * Test 7: Encoding is deterministic
	 */
	fprintf(stderr, "Test 7: Determinism check\n");
	u64 enc7a = encode_character_bits(checker);
	u64 enc7b = encode_character_bits(checker);
	TEST_ASSERT_EQ(enc7a, enc7b);
	fprintf(stderr, "  Two runs produced same result: %llu\n", (unsigned long long)enc7a);

	fprintf(stderr, "\nAll encoding tests passed!\n");
	return 0;
}
