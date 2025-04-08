#pragma once
#include <optional>
#include <vector>

#include <opencv2/opencv.hpp>

#include "base/documents/doc_appearance.h"
#include "base/documents/doc_data.h"
#include "base/documents/doc_layout.h"
#include "base/documents/doc_type.h"
#include "base/game_view.h"

namespace paplease {
	namespace documents {

		class Doc
		{
		public: // Constructors
			Doc();
			~Doc();
			Doc(const Doc& other);
			Doc(Doc&& other) noexcept;
			Doc& operator=(const Doc& other);
			Doc& operator=(Doc&& other) noexcept;

		public: // Public Methods
			DocData GetDocumentData() const;

			const DocType GetDocumentType() const;
			const PassportType GetPassportType() const;
			bool IsValid() const;

			bool HasSeal() const;
			bool IsAuthentic() const;

			friend std::optional<Doc> FindDocument(const GameView& gameView, DocType documentType);

		public: // Methods used for testing
			cv::Mat PreprocessDocument() const;
			const DocLayout& GetLayout() const noexcept;

		private: // Private Constructors and Methods
			Doc(cv::Mat&& mat, const DocType documentType, const PassportType passportType);

			DocData ExtractDocData() const;

			AppearanceType GetAppearanceType() const noexcept;
			const DocAppearance& GetAppearance() const noexcept;

		private: // Private members
			cv::Mat m_mat;  // Idea is to save this in some MatStore, so the Doc class can be flyweight, with about 16 bytes compared to 104, plus image access should be done once, and should not remain after, since we will do lots of scanning
			DocType m_documentType;
			PassportType m_passportType;
		};

		std::optional<Doc> FindDocument(const GameView& gameView, DocType documentType);

	}  // namespace documents
}  // namespace paplease