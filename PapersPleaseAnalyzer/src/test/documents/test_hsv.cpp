#include "pch.h"
#include "test/documents/test_hsv.h"

namespace tests::documents {

	namespace utils
	{
		static void on_trackbar(int, void*)
		{

		}
	}

	void test_hsv(
		const cv::Mat& image,
		int hueMin,
		int hueMax,
		int satMin,
		int satMax,
		int valMin,
		int valMax
	) {
		cv::namedWindow("TrackBars");
		cv::resizeWindow("TrackBars", cv::Size(640, 240));
		cv::createTrackbar("Hue Min", "TrackBars", &hueMin, 179, utils::on_trackbar);
		cv::createTrackbar("Hue Max", "TrackBars", &hueMax, 179, utils::on_trackbar);
		cv::createTrackbar("Sat Min", "TrackBars", &satMin, 255, utils::on_trackbar);
		cv::createTrackbar("Sat Max", "TrackBars", &satMax, 255, utils::on_trackbar);
		cv::createTrackbar("Val Min", "TrackBars", &valMin, 255, utils::on_trackbar);
		cv::createTrackbar("Val Max", "TrackBars", &valMax, 255, utils::on_trackbar);

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
	}

	cv::Mat find_hsv(
		const cv::Mat& image,
		paplease::HSVConfig& hsvConfig
	)
	{
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
			cv::Mat imgHsv;
			cv::cvtColor(image, imgHsv, cv::COLOR_BGR2HSV);

			cv::Mat lower{ hsvConfig.hueMin, hsvConfig.satMin, hsvConfig.valMin };
			cv::Mat upper{ hsvConfig.hueMax, hsvConfig.satMax, hsvConfig.valMax };

			cv::Mat mask;
			cv::inRange(imgHsv, lower, upper, mask);

			cv::Mat result;
			cv::bitwise_and(image, image, result, mask);

			cv::imshow("Original", image);
			cv::imshow("HSV", imgHsv);
			cv::imshow("Mask", mask);
			cv::imshow("Result", result);
			if (cv::waitKeyEx(1) == 0x230000)
			{
				std::cout << "Hue Min: " << hsvConfig.hueMin << "\n"
					<< "Hue Max: " << hsvConfig.hueMax << "\n"
					<< "Sat Min: " << hsvConfig.satMin << "\n"
					<< "Sat Max: " << hsvConfig.satMax << "\n"
					<< "Val Min: " << hsvConfig.valMin << "\n"
					<< "Val Max: " << hsvConfig.valMax << "\n";
				cv::destroyAllWindows();
				return mask;
			}
			
		}
	}

}