#pragma once
#include <optional>
#include <vector>

#include <opencv2/opencv.hpp>

#include "base/documents_v2/doc_appearance.h"
#include "base/documents_v2/doc_data.h"
#include "base/documents_v2/doc_layout.h"
#include "base/documents_v2/doc_type.h"
#include "base/game_view.h"


namespace Documents::V2 {

	class Doc
	{
	public:
		Doc(); // default initialization

		~Doc();
		Doc(const Doc& other);
		Doc& operator=(const Doc& other);

		DocData GetDocumentData() const;

		const DocType GetDocumentType() const;
		const PassportType GetPassportType() const;
		bool IsValid() const;

		friend std::optional<Doc> FindDocument(const GameView& gameView, DocType documentType);
	public: // Used for testing

		cv::Mat PreprocessDocument() const;
		const DocLayout& GetLayout() const noexcept;

	private:
		Doc(const cv::Mat& mat, const DocType documentType, const PassportType passportType);

		DocData ExtractDocData() const;

		AppearanceType GetAppearanceType() const noexcept;
		const DocAppearance& GetAppearance() const noexcept;
	private:
		cv::Mat m_mat;
		DocType m_documentType; // Needs to come before, since we need to be able to compute appearance type
		PassportType m_passportType;
	};

	std::optional<Doc> FindDocument(const GameView& gameView, DocType documentType);
	std::vector<Doc> FindAllDocuments(const GameView& gameView);

} // namespace Documents::V2