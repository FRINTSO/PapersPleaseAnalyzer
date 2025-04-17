#include "pch.h"
#include "base/documents/seal.h"

#include <optional>
#include <string>

#include "base/image_process.h"

#include "base/documents/bounding_box_finder.inc"

namespace paplease {
	namespace documents {

		static inline std::optional<cv::Mat> ExtractSeal(const cv::Mat& binaryDocument)
		{
			auto boundingBoxOpt = FindBoundingBox(binaryDocument, [&](int row, int col)
			{
				return binaryDocument.at<uchar>(row, col) == 255;
			});
			if (!boundingBoxOpt)
			{
				return std::nullopt;
			}

			return binaryDocument(*boundingBoxOpt);
		}

		static std::vector<cv::Mat> LoadDocumentSealsByPath(const std::string_view& path)
		{
			std::vector<cv::Mat> seals;
			for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
			{
				if (entry.is_regular_file())
				{
					const auto& filePath = entry.path();
					auto image = LoadImageFile(filePath.generic_string());
					if (!image.empty())
						seals.emplace_back(std::move(image));
				}
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

			assert(false, "Tried to load a document seal of a document that never gets sealed.");
		}

		static bool CompareSeals(const cv::Mat& seal1, const cv::Mat& seal2)
		{
			if (seal1.size != seal2.size) return false;
			return cv::countNonZero(seal2 - seal1) == 0;
		}

		static bool IsValidSeal(const cv::Mat& mat, DocType documentType)
		{
			for (const auto& seal : LoadDocumentSeals(documentType))
			{
				if (CompareSeals(mat, seal))
				{
					return true;
				}
			}
			return false;
		}

#pragma region Exposed Functions

		bool IsDocumentValidlySealed(const cv::Mat& mat, DocType documentType)
		{
			if (mat.empty()) return false;

			static constinit HSVConfig config{ 0, 179, 128, 216, 182, 255 };
			auto binary = ToHSVBinary(mat, config);
			auto seal = ExtractSeal(binary);
			if (!seal)
			{
				LOG_ERR("Invalid seal of document type: {}", ToStringView(documentType));
				return false;
			}
			return IsValidSeal(seal.value(), documentType);
		}

#pragma endregion

	}  // namespace documents
}  // namespace paplease
