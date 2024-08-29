#pragma once
#include <vector>

#include <opencv2/opencv.hpp>

#include "base/documents_v2/doc_appearance.h"
#include "base/documents_v2/doc_type.h"
#include "base/documents_v2/doc_layout.h"
#include "base/documents_v2/doc_data.h"
#include "base/game_view.h"

namespace Documents::V2
{

class Doc {
public:
	Doc(const cv::Mat& mat, DocAppearance appearance, DocType docType, PassportType passportType);

	cv::Mat PreprocessDocument() const;
	const DocLayout& GetLayout() const noexcept;
	DocData GetDocumentData() const;
	bool IsValid() const;

	const cv::Mat& RevealMat() const;
private:
	AppearanceType ToAppearanceType() const;
	DocData ExtractDocData() const;
private:
	cv::Mat m_mat;
	DocAppearance m_appearance; // should be const ref
	DocType m_docType;
	PassportType m_passportType;
	DocLayout m_docLayout; // should be const ref
};


Doc FindDocument(const GameView& gameView, DocType type);
std::vector<Doc> FindAllDocuments(const GameView& gameView);

} // namespace Documents::V2