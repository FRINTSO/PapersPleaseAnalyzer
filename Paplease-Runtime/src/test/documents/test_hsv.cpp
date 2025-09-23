#include "pch.h"
#include "test/documents/test_hsv.h"

#include <vector>

#include "paplease/common/image_process.h"

namespace test::documents {

	namespace utils
	{
		static void on_trackbar(int, void*)
		{

		}
	}

	void test_hsv(
		const cv::Mat& image,
		paplease::HSVConfig* hsvConfig
	) {
		paplease::HSVConfig temp{};
		if (!hsvConfig)
		{
			hsvConfig = &temp;
		}
		cv::namedWindow("TrackBars");
		cv::resizeWindow("TrackBars", cv::Size(640, 240));
		cv::createTrackbar("Hue Min", "TrackBars", (int*)&hsvConfig->hueMin, 179, utils::on_trackbar);
		cv::createTrackbar("Hue Max", "TrackBars", (int*)&hsvConfig->hueMax, 179, utils::on_trackbar);
		cv::createTrackbar("Sat Min", "TrackBars", (int*)&hsvConfig->satMin, 255, utils::on_trackbar);
		cv::createTrackbar("Sat Max", "TrackBars", (int*)&hsvConfig->satMax, 255, utils::on_trackbar);
		cv::createTrackbar("Val Min", "TrackBars", (int*)&hsvConfig->valMin, 255, utils::on_trackbar);
		cv::createTrackbar("Val Max", "TrackBars", (int*)&hsvConfig->valMax, 255, utils::on_trackbar);

		while (true) {
			cv::Mat imgHsv;
			cv::cvtColor(image, imgHsv, cv::COLOR_BGR2HSV);

			int h_min = cv::getTrackbarPos("Hue Min", "TrackBars");
			int h_max = cv::getTrackbarPos("Hue Max", "TrackBars");
			int s_min = cv::getTrackbarPos("Sat Min", "TrackBars");
			int s_max = cv::getTrackbarPos("Sat Max", "TrackBars");
			int v_min = cv::getTrackbarPos("Val Min", "TrackBars");
			int v_max = cv::getTrackbarPos("Val Max", "TrackBars");
			cv::Mat lower{ h_min, s_min, v_min };
			cv::Mat upper{ h_max, s_max, v_max };

			cv::Mat mask;
			cv::inRange(imgHsv, lower, upper, mask);

			cv::Mat result;
			cv::bitwise_and(image, image, result, mask);

			cv::imshow("Original", image);
			cv::imshow("HSV", imgHsv);
			cv::imshow("Mask", mask);
			cv::imshow("Result", result);
			cv::waitKey(1);
		}

		hsvConfig = nullptr;
	}

	static bool initiate_hsv_header()
	{
		std::cout << "| Hue Min | Hue Max | Sat Min | Sat Max | Val Min | Val Max |\n";
		std::cout << "|---------|---------|---------|---------|---------|---------|\n";
		return true;
	}

	static bool initiate_hsl_header()
	{
		std::cout << "| Hue Min | Hue Max | Sat Min | Sat Max | Lit Min | Lit Max |\n";
		std::cout << "|---------|---------|---------|---------|---------|---------|\n";
		return true;
	}

	static std::string format_table_entry(int value, int entry_size = 9)
	{
		std::string str_value = std::to_string(value);
		int value_size = str_value.size();

		int total_space = entry_size - value_size;
		int left_space = total_space / 2;
		int right_space = total_space - left_space;

		std::string str{};
		str.append(left_space, ' ');
		str.append(str_value);
		str.append(right_space, ' ');
		return str;
	}

	cv::Mat find_hsv(
		const cv::Mat& image,
		paplease::HSVConfig& hsvConfig
	)
	{
		static bool _ = initiate_hsv_header();

		cv::namedWindow("TrackBars");
		cv::resizeWindow("TrackBars", cv::Size(640, 240));
		cv::createTrackbar("Hue Min", "TrackBars", &hsvConfig.hueMin, 179, utils::on_trackbar);
		cv::createTrackbar("Hue Max", "TrackBars", &hsvConfig.hueMax, 179, utils::on_trackbar);
		cv::createTrackbar("Sat Min", "TrackBars", &hsvConfig.satMin, 255, utils::on_trackbar);
		cv::createTrackbar("Sat Max", "TrackBars", &hsvConfig.satMax, 255, utils::on_trackbar);
		cv::createTrackbar("Val Min", "TrackBars", &hsvConfig.valMin, 255, utils::on_trackbar);
		cv::createTrackbar("Val Max", "TrackBars", &hsvConfig.valMax, 255, utils::on_trackbar);

		
		while (true)
		{
			cv::Mat gray;
			cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
			cv::imshow("Gray", gray);

			cv::Mat imgHsv;
			cv::cvtColor(image, imgHsv, cv::COLOR_BGR2HSV);

			cv::Mat lower{ hsvConfig.hueMin, hsvConfig.satMin, hsvConfig.valMin };
			cv::Mat upper{ hsvConfig.hueMax, hsvConfig.satMax, hsvConfig.valMax };

			cv::Mat mask;
			cv::inRange(imgHsv, lower, upper, mask);

			cv::Mat result;
			cv::bitwise_and(image, image, result, mask);

			// Split channels
			cv::Mat channels[3];
			cv::split(imgHsv, channels);
			// Hue
			cv::imshow("Hue channel", channels[0]);
			cv::Mat hueMask;
			cv::inRange(channels[0], cv::Mat{ hsvConfig.hueMin }, cv::Mat{ hsvConfig.hueMax }, hueMask);
			cv::Mat hueResult;
			cv::bitwise_and(channels[0], channels[0], hueResult, hueMask);
			cv::imshow("Hue mask", hueMask);

			// Saturation
			cv::imshow("Saturation channel", channels[1]);
			cv::Mat satMask;
			cv::inRange(channels[1], cv::Mat{ hsvConfig.satMin }, cv::Mat{ hsvConfig.satMax }, satMask);
			cv::Mat satResult;
			cv::bitwise_and(channels[1], channels[1], satResult, satMask);
			cv::imshow("Sat mask", satMask);

			// Light
			cv::imshow("Value channel", channels[2]);
			cv::Mat valMask;
			cv::inRange(channels[2], cv::Mat{ hsvConfig.valMin }, cv::Mat{ hsvConfig.valMax }, valMask);
			cv::Mat valResult;
			cv::bitwise_and(channels[2], channels[2], valResult, valMask);
			cv::imshow("Value mask", valMask);

			cv::imshow("Original", image);
			cv::imshow("HSV", imgHsv);
			cv::imshow("Mask", mask);
			cv::imshow("Result", result);
			cv::imshow("Gray", paplease::ToGrayscale(image));
			if (cv::waitKeyEx(1) == 0x230000)
			{
				std::cout << '|' << format_table_entry(hsvConfig.hueMin)
					<< '|' << format_table_entry(hsvConfig.hueMax)
					<< '|' << format_table_entry(hsvConfig.satMin)
					<< '|' << format_table_entry(hsvConfig.satMax)
					<< '|' << format_table_entry(hsvConfig.valMin)
					<< '|' << format_table_entry(hsvConfig.valMax)
					<< "|\n";
				cv::destroyAllWindows();
				return mask;
			}
			
		}
	}

	cv::Mat find_hsl(
		const cv::Mat& image,
		paplease::HSLConfig& hslConfig
	)
	{
		static bool _ = initiate_hsl_header();

		cv::namedWindow("TrackBars");
		cv::resizeWindow("TrackBars", cv::Size(640, 240));
		cv::createTrackbar("Hue Min", "TrackBars", &hslConfig.hueMin, 179, utils::on_trackbar);
		cv::createTrackbar("Hue Max", "TrackBars", &hslConfig.hueMax, 179, utils::on_trackbar);
		cv::createTrackbar("Sat Min", "TrackBars", &hslConfig.satMin, 255, utils::on_trackbar);
		cv::createTrackbar("Sat Max", "TrackBars", &hslConfig.satMax, 255, utils::on_trackbar);
		cv::createTrackbar("Lit Min", "TrackBars", &hslConfig.litMin, 255, utils::on_trackbar);
		cv::createTrackbar("Lit Max", "TrackBars", &hslConfig.litMax, 255, utils::on_trackbar);


		while (true)
		{
			cv::Mat imgHsl;
			cv::cvtColor(image, imgHsl, cv::COLOR_BGR2HLS);

			cv::Mat lower{ hslConfig.hueMin, hslConfig.satMin, hslConfig.litMin };
			cv::Mat upper{ hslConfig.hueMax, hslConfig.satMax, hslConfig.litMax };

			cv::Mat mask;
			cv::inRange(imgHsl, lower, upper, mask);

			cv::Mat result;
			cv::bitwise_and(image, image, result, mask);

			// Split channels
			cv::Mat channels[3];
			cv::split(imgHsl, channels);
			// Hue
			cv::imshow("Hue channel", channels[0]);
			cv::Mat hueMask;
			cv::inRange(channels[0], cv::Mat{hslConfig.hueMin}, cv::Mat{ hslConfig.hueMax }, hueMask);
			cv::Mat hueResult;
			cv::bitwise_and(channels[0], channels[0], hueResult, hueMask);
			cv::imshow("Hue mask", hueResult);

			// Saturation
			cv::imshow("Saturation channel", channels[1]);
			cv::Mat satMask;
			cv::inRange(channels[1], cv::Mat{ hslConfig.satMin }, cv::Mat{ hslConfig.satMax }, satMask);
			cv::Mat satResult;
			cv::bitwise_and(channels[1], channels[1], satResult, satMask);
			cv::imshow("Sat mask", satResult);

			// Light
			cv::imshow("Light channel", channels[2]);
			cv::Mat litMask;
			cv::inRange(channels[2], cv::Mat{ hslConfig.litMin }, cv::Mat{ hslConfig.litMax }, litMask);
			cv::Mat litResult;
			cv::bitwise_and(channels[2], channels[2], litResult, litMask);
			cv::imshow("Lit mask", litResult);

			cv::imshow("Original", image);
			cv::imshow("HSL", imgHsl);
			cv::imshow("Mask", mask);
			cv::imshow("Result", result);
			cv::imshow("Gray", paplease::ToGrayscale(image));
			if (cv::waitKeyEx(1) == 0x230000)
			{
				std::cout << '|' << format_table_entry(hslConfig.hueMin)
					<< '|' << format_table_entry(hslConfig.hueMax)
					<< '|' << format_table_entry(hslConfig.satMin)
					<< '|' << format_table_entry(hslConfig.satMax)
					<< '|' << format_table_entry(hslConfig.litMin)
					<< '|' << format_table_entry(hslConfig.litMax)
					<< "|\n";
				cv::destroyAllWindows();
				return mask;
			}

		}
	}

}