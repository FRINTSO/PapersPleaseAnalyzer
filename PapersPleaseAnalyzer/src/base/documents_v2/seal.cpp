#include "pch.h"
#include "base/documents_v2/seal.h"

#include <climits>

#include "base/common.h"
#include "base/image_process.h"

namespace paplease {
	namespace documents {
		namespace v2 {

			static inline cv::Mat PreprocessSealedDocument(const cv::Mat& mat)
			{
				cv::Mat imgHsv;
				cv::cvtColor(mat, imgHsv, cv::COLOR_BGR2HSV);

				constexpr int hueMin = 0;
				constexpr int hueMax = 179;
				constexpr int satMin = 0;
				constexpr int satMax = 110;
				constexpr int valMin = 0;
				constexpr int valMax = 255;
				cv::Mat lower{ hueMin, satMin, valMin };
				cv::Mat upper{ hueMax, satMax, valMax };

				cv::Mat mask;
				cv::inRange(imgHsv, lower, upper, mask);

				return mask;
			}

			static inline cv::Mat ExtractSeal(const cv::Mat& binaryDocument)
			{
				int leftMostPixel = INT_MAX;
				int rightMostBlackPixel = -1;
				int topMostPixel = -1;
				int bottomMostPixel = -1;
				for (int row = 0; row < binaryDocument.rows; row++)
				{
					bool rowContainsBlackPixel = false;
					for (int col = 0; col < binaryDocument.cols; col++)
					{
						if (binaryDocument.at<uchar>(row, col)) continue;
						rowContainsBlackPixel = true;

						if (col < leftMostPixel) leftMostPixel = col;
						if (col > rightMostBlackPixel) rightMostBlackPixel = col;
					}

					if (rowContainsBlackPixel)
					{
						if (topMostPixel == -1) topMostPixel = row;
						bottomMostPixel = row;
					}
					else if (topMostPixel != -1)
					{
						break;
					}
				}

				int regionWidth = rightMostBlackPixel - leftMostPixel + 1;
				int regionHeight = bottomMostPixel - topMostPixel + 1;

				return binaryDocument(cv::Rect(leftMostPixel, topMostPixel, regionWidth, regionHeight));
			}

			static std::vector<cv::Mat> LoadDocumentSealsByPath(const std::string& path)
			{
				std::vector<cv::Mat> seals{};
				for (const auto& entry : std::filesystem::directory_iterator(path))
				{
					if (entry.is_directory())
					{
						for (const auto& entry1 : std::filesystem::directory_iterator(entry.path()))
						{
							auto seal = cv::imread(entry1.path().generic_string(), cv::IMREAD_UNCHANGED);
#if DOWNSCALE_OPTIMIZATION
							seal = ScaleImage(seal, SCALE);
#endif
							seals.push_back(seal);
						}
						continue;
					}

					auto seal = cv::imread(entry.path().generic_string(), cv::IMREAD_UNCHANGED);
#if DOWNSCALE_OPTIMIZATION
					seal = ScaleImage(seal, SCALE);
#endif
					seals.push_back(seal);
				}
				return seals;
			}

			static const std::vector<cv::Mat>& LoadDocumentSeals(DocType documentType)
			{
				switch (documentType)
				{
					case DocType::DiplomaticAuthorization:
					{
						static const auto seals = LoadDocumentSealsByPath("C:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/seals/diplomatic_authorization");
						return seals;
					}
					case DocType::EntryPermit:
					{
						static const auto seals = LoadDocumentSealsByPath("C:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/seals/entry_permit");
						return seals;
					}
					case DocType::AccessPermit:
					{
						static const auto seals = LoadDocumentSealsByPath("C:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/seals/access_permit");
						return seals;
					}
					case DocType::WorkPass:
					{
						static const auto seals = LoadDocumentSealsByPath("C:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/seals/work_pass");
						return seals;
					}
					case DocType::GrantOfAsylum:
					{
						static const auto seals = LoadDocumentSealsByPath("C:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/seals/grant_of_asylum");
						return seals;
					}
				}

				return std::vector<cv::Mat>{};
			}

			static bool CompareSeals(const cv::Mat& seal1, const cv::Mat& seal2)
			{
				if (seal1.size != seal2.size) return false;
				return cv::countNonZero(seal2 - seal1) == 0;
			}

			static bool IsValidSeal(const cv::Mat& mat, DocType documentType)
			{
				const auto& seals = LoadDocumentSeals(documentType);
				for (const auto& seal : seals)
				{
					if (CompareSeals(mat, seal))
					{
						return true;
					}
				}
				return false;
			}

			bool IsDocumentValidlySealed(const cv::Mat& mat, DocType documentType)
			{
				auto binary = PreprocessSealedDocument(mat);
				auto seal = ExtractSeal(binary);
				return IsValidSeal(seal, documentType);
			}

		}  // namespace v2
	}  // namespace documents
}  // namespace paplease
