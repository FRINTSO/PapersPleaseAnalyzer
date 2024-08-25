#pragma once
#include <vector>

#include <opencv2/opencv.hpp>

#include "base/documents_v2/doc_appearance.h"
#include "base/documents_v2/doc_type.h"
#include "base/documents_v2/doc_layout.h"
#include "base/documents_v2/doc_data.h"

namespace Documents::V2
{

class Doc {
public:
	Doc(const cv::Mat& mat, DocAppearance appearance, DocType docType, PassportType passportType);

	cv::Mat PreprocessDocument() const;
	const DocLayout const& const GetLayout() const noexcept;
	DocData GetDocumentData() const;
	bool IsValid() const;

	const cv::Mat& RevealMat() const;
private:
	AppearanceType ToAppearanceType() const;
private:
	cv::Mat m_mat;
	DocAppearance m_appearance;
	DocType m_docType;
	PassportType m_passportType;
	DocLayout m_docLayout;
};


Doc FindDocument(const cv::Mat& inspection, DocType type);
std::vector<Doc> FindAllDocuments(const cv::Mat& inspection);

} // namespace Documents::V2