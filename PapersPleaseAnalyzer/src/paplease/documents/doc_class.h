#pragma once
#include <opencv2/opencv.hpp>

#include "paplease/documents/doc_appearance.h"
#include "paplease/documents/doc_data.h"
#include "paplease/documents/doc_layout.h"
#include "paplease/documents/doc_type.h"

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
			const AppearanceType GetAppearanceType() const noexcept;
			bool IsValid() const;

			bool HasSeal() const;
			bool IsAuthentic() const;

			// friend std::optional<Doc> scannable::FindDocument(const GameView& gameView, DocType documentType);
			friend class DocView;

		public: // Methods used for testing
			cv::Mat PreprocessDocument() const;
			const DocLayout& GetLayout() const noexcept;

		private: // Private Constructors and Methods
			Doc(cv::Mat&& mat, const DocType documentType, const PassportType passportType);

			DocData ExtractDocData() const;
			
			const DocAppearance& GetAppearance() const noexcept;

		private: // Private members
			cv::Mat m_mat;  // Idea is to save this in some MatStore, so the Doc class can be flyweight, with about 16 bytes compared to 104, plus image access should be done once, and should not remain after, since we will do lots of scanning
			DocType m_documentType;
			PassportType m_passportType;
		};

	}  // namespace documents
}  // namespace paplease