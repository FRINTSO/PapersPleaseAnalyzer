#include "pch.h"
#include "base/analysis/photo_analysis/photo_analyzer.h"

#include <array>

#include "base/documents/bounding_box_finder.inc"
#include "base/image_process.h"
#include "base/shape.h"

namespace paplease {
    namespace analysis {

		static cv::Mat CutoutHeadshot(const cv::Mat& headshot)
		{
			// Start at bottom and make a pivot in the middle
			// Widest is in the shoulders

			Rectangle box = documents::FindBoundingBox(headshot, [&](int row, int col)
			{
				return headshot.at<uchar>(row, col) == 0;
			});

			return cv::Mat(headshot, cv::Rect(box.x, box.y, box.width, box.height));
		}

        static cv::Mat ScaleImage(const cv::Mat& image, int canvasSize = 100)
        {
            int originalWidth = image.cols;
            int originalHeight = image.rows;

            cv::Mat resized;
            cv::resize(image, resized, cv::Size(canvasSize, canvasSize), 0, 0, cv::INTER_AREA);

            return resized;
        }

        struct HeadshotScore
        {

        };

        static void GridScoring(const cv::Mat& binaryHeadshot)
        {
            constexpr int gridSize = 10;
        }

        static void RowWeightScoring(const cv::Mat& binaryHeadshot)
        {

        }

        static void RowLengthScoring(const cv::Mat& binaryHeadshot)
        {

        }

        static std::array<float, 100> CalculateTopPixelRatios(const cv::Mat& binaryHeadshot)
        {
            auto image = ScaleImage(binaryHeadshot, 100);

            const int imageHeight = image.rows;  // Height of image
            const int dataPoints = image.cols;
            std::array<float, 100> topPixelPerColumn = {};

            for (int col = 0; col < image.cols; col++)
            {
                for (int row = 0; row < image.rows; row++)
                {
                    if (image.at<uchar>(row, col) == 0)
                    {
                        topPixelPerColumn[col] = (imageHeight - row) / (float)imageHeight;
                        break;
                    }
                }
            }

            // Smooth out data points
            for (size_t i = 2; i < dataPoints; i++)
            {
                float val1 = topPixelPerColumn[i - 2];
                float val2 = topPixelPerColumn[i - 1];
                float val3 = topPixelPerColumn[i];

                if ((val2 < val1 && val2 < val3) || (val2 > val1 && val2 > val3))
                {
                    // Average
                    // topPixelPerColumn[i - 1] = (val1 + val3) / 2.0f;

                    // To closest
                    topPixelPerColumn[i - 1] = std::abs(val2 - val1) < std::abs(val2 - val3) ? val1 : val3;
                }
            }

#ifdef DO_LOG
            std::cout << "Row Count: " << imageHeight << "\n";
            std::cout << "Values: ";
            for (size_t i = 0; i < dataPoints; i++)
            {
                std::cout << topPixelPerColumn[i] << ", ";
            }

            std::cout << "\n";
#endif
            return topPixelPerColumn;
        }
        
        static float TopPixelInColumnScoring(const cv::Mat& A, const cv::Mat& B)
        {
            auto scoreA = CalculateTopPixelRatios(A);
            auto scoreB = CalculateTopPixelRatios(B);

            float value = 0.0f;

            for (size_t i = 0; i < 100; i++)
            {
                value += std::abs(scoreA[i] - scoreB[i]);
            }
            return value;
        }

        static float CalculatePixelRatio(const cv::Mat& binaryHeadshot)
        {
            auto blackPixels = (binaryHeadshot.cols * binaryHeadshot.rows) - cv::countNonZero(binaryHeadshot);
            auto ratio = blackPixels / (float)(binaryHeadshot.cols * binaryHeadshot.rows);
#ifdef DO_LOG
            std::cout << "Black pixels: " << blackPixels << "\n";
            std::cout << "Total pixels: " << binaryHeadshot.cols * binaryHeadshot.rows << "\n";
            std::cout << "Black pixel ratio: " << ratio << "\n";
#endif
            return ratio;
        }

        HeadshotScore ScoreHeadshot(const cv::Mat& A, const cv::Mat& B)
        {
            auto errorScore = TopPixelInColumnScoring(A, B);
            std::cout << "Error score: " << errorScore << "\n";

            // Pixel ratio
            auto pixelRatioA = CalculatePixelRatio(A);
            auto pixelRatioB = CalculatePixelRatio(B);
            std::cout << "Pixel ratio A: " << pixelRatioA << "\nPixel ratio B: " << pixelRatioB << "\n";
            std::cout << "Pixel ratio difference: " << std::abs(pixelRatioA - pixelRatioB) / ((pixelRatioA + pixelRatioB) / 2.0f) * 100 << "%\n";

            return HeadshotScore();
        }

        void pasteToCanvas(const cv::Mat& src, cv::Mat& canvas, int x, int y)
        {
            // Define the target ROI on the canvas
            cv::Rect roi(x, y, src.cols, src.rows);

            // Ensure ROI fits inside canvas
            CV_Assert(x >= 0 && y >= 0 &&
                      x + src.cols <= canvas.cols &&
                      y + src.rows <= canvas.rows);

            // Copy src into the ROI of the canvas
            src.copyTo(canvas(roi));
        }

        void preprocess(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& img1Out, cv::Mat& img2Out)
        {
            size_t targetHeight = 100;

            img1Out = cv::Mat(targetHeight, targetHeight, img1.type(), cv::Scalar(255));
            img2Out = cv::Mat(targetHeight, targetHeight, img2.type(), cv::Scalar(255));

            double scale1 = static_cast<double>(targetHeight) / img1.rows;
            double scale2 = static_cast<double>(targetHeight) / img2.rows;

            int width1 = static_cast<int>(img1.cols * scale1);
            int width2 = static_cast<int>(img2.cols * scale2);

            cv::Mat resized1, resized2;
            cv::resize(img1, resized1, cv::Size(width1, targetHeight), 0, 0, cv::INTER_AREA);
            cv::resize(img2, resized2, cv::Size(width2, targetHeight), 0, 0, cv::INTER_AREA);

            pasteToCanvas(resized1, img1Out, (targetHeight - resized1.cols) / 2, 0);
            pasteToCanvas(resized2, img2Out, (targetHeight - resized2.cols) / 2, 0);

            cv::imshow("img1", img1);
            cv::imshow("img2", img2);
            cv::imshow("img1Out", img1Out);
            cv::imshow("img2Out", img2Out);
            cv::waitKey();
        }

        cv::Mat adjust_scale(const cv::Mat& face)
        {
            cv::Mat resized, resized2;
            cv::resize(face, resized, cv::Size(40, 40), 0, 0, cv::INTER_AREA); // Normalize size
            cv::resize(resized, resized2, cv::Size(400, 400), 0, 0, cv::INTER_AREA); // Normalize size
            return resized2;
        }

        double computeMSE(const cv::Mat& img1, const cv::Mat& img2)
        {
            cv::Mat diff;
            cv::absdiff(img1, img2, diff);
            diff.convertTo(diff, CV_32F);
            diff = diff.mul(diff);
            return cv::sum(diff)[0] / (img1.total());
        }

        double computeSSIM(const cv::Mat& img1, const cv::Mat& img2)
        {
            const double C1 = 6.5025, C2 = 58.5225;

            cv::Mat I1, I2;
            img1.convertTo(I1, CV_32F);
            img2.convertTo(I2, CV_32F);

            cv::Mat I1_2 = I1.mul(I1); // I1^2
            cv::Mat I2_2 = I2.mul(I2); // I2^2
            cv::Mat I1_I2 = I1.mul(I2); // I1 * I2

            cv::Mat mu1, mu2;
            cv::GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
            cv::GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);

            cv::Mat mu1_2 = mu1.mul(mu1);
            cv::Mat mu2_2 = mu2.mul(mu2);
            cv::Mat mu1_mu2 = mu1.mul(mu2);

            cv::Mat sigma1_2, sigma2_2, sigma12;

            cv::GaussianBlur(I1_2, sigma1_2, cv::Size(11, 11), 1.5);
            sigma1_2 -= mu1_2;

            cv::GaussianBlur(I2_2, sigma2_2, cv::Size(11, 11), 1.5);
            sigma2_2 -= mu2_2;

            cv::GaussianBlur(I1_I2, sigma12, cv::Size(11, 11), 1.5);
            sigma12 -= mu1_mu2;

            cv::Mat t1 = 2 * mu1_mu2 + C1;
            cv::Mat t2 = 2 * sigma12 + C2;
            cv::Mat t3 = t1.mul(t2);     // Numerator

            t1 = mu1_2 + mu2_2 + C1;
            t2 = sigma1_2 + sigma2_2 + C2;
            t1 = t1.mul(t2);             // Denominator

            cv::Mat ssim_map;
            cv::divide(t3, t1, ssim_map);
            return cv::mean(ssim_map)[0];
        }

        void Test(const cv::Mat& face1, const cv::Mat& face2)
        {
            cv::imshow("face1", face1);
            cv::imshow("face2", face2);
            cv::waitKey();
            cv::Mat img1, img2;
            preprocess(face1, face2, img1, img2);
            img1 = adjust_scale(img1);
            img2 = adjust_scale(img2);

            cv::imshow("img1", img1);
            cv::imshow("img2", img2);
            cv::waitKey();
            double mse = computeMSE(img1, img2);
            double ssim = computeSSIM(img1, img2);

            std::cout << "MSE: " << mse << "\n";
            std::cout << "SSIM: " << ssim << "\n";

            if (mse > 2000 || ssim < 0.7)
            {
                std::cout << "Faces do not match!\n";
            }
            else
            {
                std::cout << "Faces match.\n";
            }

            cv::destroyAllWindows();
        }

        bool IsSamePerson(const documents::data::Photo& headshot1, const documents::data::Photo& headshot2)
        {
            if (headshot1.m_mat.empty() || headshot2.m_mat.empty())
            {
                return false;
            }

            const auto ref1 = CutoutHeadshot(headshot1.m_mat);
            const auto ref2 = CutoutHeadshot(headshot2.m_mat);

            Test(ref1, ref2);

            auto score1 = ScoreHeadshot(ref1, ref2);

            //cv::imshow("Headshot 1", ref1);
            //cv::imshow("Headshot 2", ref2);

            //cv::waitKey();
            //cv::destroyAllWindows();
            return false;
        }

    }
}