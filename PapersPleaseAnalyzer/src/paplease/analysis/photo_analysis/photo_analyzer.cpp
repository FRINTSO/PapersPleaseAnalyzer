#include "pch.h"
#include "paplease/analysis/photo_analysis/photo_analyzer.h"

#include <array>

#include "paplease/common/shape.h"

#include "paplease/documents/bounding_box_finder.inc"

namespace paplease {
    namespace analysis {

		static cv::Mat CutoutHeadshot(const cv::Mat& headshot)
		{
			// Start at bottom and make a pivot in the middle
			// Widest is in the shoulders

			Rectangle box = documents::FindBoundingBox(headshot, [&](int row, int col)
			{
				return headshot.at<uchar>(row, col) == 0;
			}).value();

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

        static HeadshotScore ScoreHeadshot(const cv::Mat& A, const cv::Mat& B)
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

        static void pasteToCanvas(const cv::Mat& src, cv::Mat& canvas, int x, int y)
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

        static void preprocess(const cv::Mat& img, cv::Mat& imgOut)
        {
            constexpr int targetSize = 100;
            constexpr int interpolation = cv::INTER_NEAREST;
            imgOut = cv::Mat(targetSize, targetSize, img.type(), cv::Scalar(255));

            const int width = img.cols;
            const int height = img.rows;
            
            if (height >= width)
            {
                double scale = static_cast<double>(targetSize) / height;

                int newWidth = static_cast<int>(width * scale);

                cv::Mat resized;
                cv::resize(img, resized, cv::Size(newWidth, targetSize), 0, 0, interpolation);

                pasteToCanvas(resized, imgOut, (targetSize - resized.cols) / 2, 0);
            }
            else
            {
                double scale = static_cast<double>(targetSize) / width;

                int newHeight = static_cast<int>(height * scale);

                cv::Mat resized;
                cv::resize(img, resized, cv::Size(targetSize, newHeight), 0, 0, interpolation);

                //pasteToCanvas(resized, imgOut, 0, (targetSize - resized.rows) / 2);
                pasteToCanvas(resized, imgOut, 0, targetSize - resized.rows);
            }
        }

        static cv::Mat adjust_scale(const cv::Mat& face)
        {
            constexpr int interpolation = cv::INTER_AREA;

            cv::Mat resized, resized2;
            cv::resize(face, resized, cv::Size(40, 40), 0, 0, interpolation); // Normalize size
            cv::resize(resized, resized2, cv::Size(400, 400), 0, 0, interpolation); // Normalize size
            return resized2;
        }

        static double computeMSE(const cv::Mat& img1, const cv::Mat& img2)
        {
            cv::Mat diff;
            cv::absdiff(img1, img2, diff);
            diff.convertTo(diff, CV_32F);
            diff = diff.mul(diff);
            return cv::sum(diff)[0] / (img1.total());
        }

        static double computeSSIM(const cv::Mat& img1, const cv::Mat& img2)
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

        static void calculate_humoments(const cv::Mat& img, bool isBinary = true)
        {
            cv::Moments moments = cv::moments(img, isBinary);

            double huMoments[7];
            cv::HuMoments(moments, huMoments);

            // Log scale hu moments 
            for (int i = 0; i < 7; i++)
            {
                std::cout << "h[" << i << "] = " << huMoments[i] << "\n";
                huMoments[i] = -1 * copysign(1.0, huMoments[i]) * log10(abs(huMoments[i]));
            }

            std::cout << "\n";
            // Log scale hu moments 
            for (int i = 0; i < 7; i++)
            {
                std::cout << "H[" << i << "] = " << huMoments[i] << "\n";
            }

            std::cout << "--------------------\n\n\n";

        }

        static void Test(const cv::Mat& face1, const cv::Mat& face2)
        {
            std::cout << "Test(images are scaled):\n";
            //cv::imshow("face1", face1);
            //cv::imshow("face2", face2);
            //cv::waitKey();
            cv::Mat img1, img2;
            preprocess(face1, img1);
            preprocess(face2, img2);
            img1 = adjust_scale(img1);
            img2 = adjust_scale(img2);

            double d1 = cv::matchShapes(img1, img2, cv::CONTOURS_MATCH_I1, 0);
            double d2 = cv::matchShapes(img1, img2, cv::CONTOURS_MATCH_I2, 0);
            double d3 = cv::matchShapes(img1, img2, cv::CONTOURS_MATCH_I3, 0);

            std::cout << "d1=" << d1 << "\n";
            std::cout << "d2=" << d2 << "\n";
            std::cout << "d3=" << d3 << "\n";

            calculate_humoments(img1, false);
            calculate_humoments(img2, false);
            

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

            cv::imshow("img1", img1);
            cv::imshow("img2", img2);
            cv::waitKey();
            cv::destroyAllWindows();
        }

        

        static void TestHuMoments(const cv::Mat& face1, const cv::Mat& face2)
        {
            std::cout << "TestHuMoments(images are not scaled):\n";
            double d1 = cv::matchShapes(face1, face2, cv::CONTOURS_MATCH_I1, 0);
            double d2 = cv::matchShapes(face1, face2, cv::CONTOURS_MATCH_I2, 0);
            double d3 = cv::matchShapes(face1, face2, cv::CONTOURS_MATCH_I3, 0);

            std::cout << "d1=" << d1 << "\n";
            std::cout << "d2=" << d2 << "\n";
            std::cout << "d3=" << d3 << "\n";
            calculate_humoments(face1);
            calculate_humoments(face2);
        }

        bool IsSamePerson(const documents::data::Photo& headshot1, const documents::data::Photo& headshot2)
        {
            if (headshot1.m_mat.empty() || headshot2.m_mat.empty())
            {
                return false;
            }

            const auto ref1 = CutoutHeadshot(headshot1.m_mat);
            const auto ref2 = CutoutHeadshot(headshot2.m_mat);

            TestHuMoments(ref1, ref2);
            Test(ref1, ref2);

            auto score1 = ScoreHeadshot(ref1, ref2);

            //cv::imshow("Headshot 1", ref1);
            //cv::imshow("Headshot 2", ref2);

            //cv::waitKey();
            //cv::destroyAllWindows();
            std::cout << "====================\n\n";
            return false;
        }

    }
}