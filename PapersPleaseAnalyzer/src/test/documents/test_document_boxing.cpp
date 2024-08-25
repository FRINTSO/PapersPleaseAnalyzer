#include "pch.h"
#include "test/documents/test_document_boxing.h"

#include "base/booth.h"
#include "base/documents/access_permit.h"
#include "base/documents/certificate_of_vaccination.h"
#include "base/documents/diplomatic_authorization.h"
#include "base/documents/document.h"
#include "base/documents/entry_permit.h"
#include "base/documents/entry_ticket.h"
#include "base/documents/grant_of_asylum.h"
#include "base/documents/identity_card.h"
#include "base/documents/identity_supplement.h"
#include "base/documents/passport.h"
#include "base/documents/work_pass.h"
#include "base/game_view.h"
#include "base/image_process.h"
#include "base/ocr/font.h"
#include "base/ocr/ocr.h"
#include "base/shape.h"

#include "base/documents_v2/doc_class.h"
#include "base/documents_v2/doc_layout.h"

namespace Documents {

	namespace V1::Test {

		namespace Utils {
		
			static void display_document_info_boxing(const cv::Mat& document, DocumentType documentType, const Rectangle& boundingBox, std::string&& partName)
			{
				auto passPart = ExtractDocumentField(document, boundingBox);
				auto& fontInfo = GetFontInfo(documentType);
				auto boxes = ImageToBoxes(passPart, fontInfo);

				for (const auto& rect : boxes)
				{
					cv::rectangle(passPart, cv::Rect(rect.x, rect.y, rect.width, rect.height), cv::Scalar(127));
				}

				cv::imshow(partName, ScaleImage(passPart, 0.25f));
			}
		}

		void test_access_permit_text_boxing(const std::string& number)
		{
			GameView game = GetGameView(number);

			auto accessPermit = FindAccessPermit(game.inspection);
			auto binary = PreprocessAccessPermit(accessPermit);

			Utils::display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetCountryBox(), "Country");
			Utils::display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetDurationOfStayBox(), "Duration of Stay");
			Utils::display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetExpirationDateBox(), "Expiration Date");
			Utils::display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetHeightBox(), "Height");
			Utils::display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetNameBox(), "Name");
			Utils::display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetPassportNumberBox(), "Passport Number");
			Utils::display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetPhysicalAppearanceBox(), "Physical Appearance");
			Utils::display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetPurposeBox(), "Purpose");
			Utils::display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetWeightBox(), "Weight");
		}

		void test_booth_text_boxing(const std::string& number)
		{
			GameView game = GetGameView(number);

			auto binary = PreprocessBooth(game.booth.mat);

			Utils::display_document_info_boxing(binary, DocumentType::Booth, game.booth.layoutProvider->GetDateBox(), "Date");
			Utils::display_document_info_boxing(binary, DocumentType::Booth, game.booth.layoutProvider->GetCounterBox(), "Counter");
			Utils::display_document_info_boxing(binary, DocumentType::Booth, game.booth.layoutProvider->GetWeightBox(), "Weight");
		}

		void test_certificate_of_vaccination_text_boxing(const std::string& number)
		{
			GameView game = GetGameView(number);

			auto cov = FindCertificateOfVaccination(game.inspection);
			auto binary = PreprocessCertificateOfVaccination(cov);

			Utils::display_document_info_boxing(binary, DocumentType::CertificateOfVaccination, cov.layoutProvider->GetNameBox(), "Name");
			Utils::display_document_info_boxing(binary, DocumentType::CertificateOfVaccination, cov.layoutProvider->GetPassportNumberBox(), "Passport Number");
			Utils::display_document_info_boxing(binary, DocumentType::CertificateOfVaccination, cov.layoutProvider->GetVaccination1Box(), "Vaccination 1");
			Utils::display_document_info_boxing(binary, DocumentType::CertificateOfVaccination, cov.layoutProvider->GetVaccination2Box(), "Vaccination 2");
			Utils::display_document_info_boxing(binary, DocumentType::CertificateOfVaccination, cov.layoutProvider->GetVaccination3Box(), "Vaccination 3");
		}

		void test_diplomatic_authorization_text_boxing(const std::string& number)
		{
			GameView game = GetGameView(number);

			auto da = FindDiplomaticAuthorization(game.inspection);
			auto binary = PreprocessDiplomaticAuthorization(da);

			Utils::display_document_info_boxing(binary, DocumentType::DiplomaticAuthorization, da.layoutProvider->GetCountryListBox(), "Country List");
			Utils::display_document_info_boxing(binary, DocumentType::DiplomaticAuthorization, da.layoutProvider->GetIssuingCountryBox(), "Issuing Country");
			Utils::display_document_info_boxing(binary, DocumentType::DiplomaticAuthorization, da.layoutProvider->GetNameBox(), "Name");
			Utils::display_document_info_boxing(binary, DocumentType::DiplomaticAuthorization, da.layoutProvider->GetPassportNumberBox(), "Passport Number");
		}

		void test_entry_permit_text_boxing(const std::string& number)
		{
			GameView game = GetGameView(number);

			auto entryPermit = FindEntryPermit(game.inspection);
			auto binary = PreprocessEntryPermit(entryPermit);

			Utils::display_document_info_boxing(binary, DocumentType::EntryPermit, entryPermit.layoutProvider->GetDurationOfStayBox(), "Duration Of Stay");
			Utils::display_document_info_boxing(binary, DocumentType::EntryPermit, entryPermit.layoutProvider->GetExpirationDateBox(), "Expiration Date");
			Utils::display_document_info_boxing(binary, DocumentType::EntryPermit, entryPermit.layoutProvider->GetNameBox(), "Name");
			Utils::display_document_info_boxing(binary, DocumentType::EntryPermit, entryPermit.layoutProvider->GetPassportNumberBox(), "Passport Number");
			Utils::display_document_info_boxing(binary, DocumentType::EntryPermit, entryPermit.layoutProvider->GetPurposeBox(), "Purpose");
		}

		void test_entry_ticket_text_boxing(const std::string& number)
		{
			GameView game = GetGameView(number);

			auto entryTicket = FindEntryTicket(game.inspection);
			auto binary = PreprocessEntryTicket(entryTicket);

			Utils::display_document_info_boxing(binary, DocumentType::EntryTicket, entryTicket.layoutProvider->GetValidDateBox(), "Valid Date");
		}

		void test_grant_of_asylum_text_boxing(const std::string& number)
		{
			GameView game = GetGameView(number);

			auto grantOfAsylum = FindGrantOfAsylum(game.inspection);
			auto binary = PreprocessGrantOfAsylum(grantOfAsylum);

			Utils::display_document_info_boxing(binary, DocumentType::GrantOfAsylum, grantOfAsylum.layoutProvider->GetCountryBox(), "Country");
			Utils::display_document_info_boxing(binary, DocumentType::GrantOfAsylum, grantOfAsylum.layoutProvider->GetDateOfBirthBox(), "Date of Birth");
			Utils::display_document_info_boxing(binary, DocumentType::GrantOfAsylum, grantOfAsylum.layoutProvider->GetExpirationDateBox(), "Expiration Date");
			Utils::display_document_info_boxing(binary, DocumentType::GrantOfAsylum, grantOfAsylum.layoutProvider->GetHeightBox(), "Height");
			Utils::display_document_info_boxing(binary, DocumentType::GrantOfAsylum, grantOfAsylum.layoutProvider->GetNameBox(), "Name");
			Utils::display_document_info_boxing(binary, DocumentType::GrantOfAsylum, grantOfAsylum.layoutProvider->GetPassportNumberBox(), "Passport Number");
			Utils::display_document_info_boxing(binary, DocumentType::GrantOfAsylum, grantOfAsylum.layoutProvider->GetWeightBox(), "Weight");
		}

		void test_identity_card_text_boxing(const std::string& number)
		{
			GameView game = GetGameView(number);

			auto idCard = FindIdentityCard(game.inspection);
			auto binary = PreprocessIdentityCard(idCard);

			Utils::display_document_info_boxing(binary, DocumentType::IdentityCard, idCard.layoutProvider->GetDateOfBirthBox(), "Date of Birth");
			Utils::display_document_info_boxing(binary, DocumentType::IdentityCard, idCard.layoutProvider->GetDistrictBox(), "District");
			Utils::display_document_info_boxing(binary, DocumentType::IdentityCard, idCard.layoutProvider->GetHeightBox(), "Height");
			Utils::display_document_info_boxing(binary, DocumentType::IdentityCard, idCard.layoutProvider->GetNameBox(), "Name");
			Utils::display_document_info_boxing(binary, DocumentType::IdentityCard, idCard.layoutProvider->GetWeightBox(), "Weight");
		}

		void test_identity_supplement_text_boxing(const std::string& number)
		{
			GameView game = GetGameView(number);

			auto identitySupplement = FindIdentitySupplement(game.inspection);
			auto binary = PreprocessIdentitySupplement(identitySupplement);

			Utils::display_document_info_boxing(binary, DocumentType::IdentitySupplement, identitySupplement.layoutProvider->GetDescriptionBox(), "Description");
			Utils::display_document_info_boxing(binary, DocumentType::IdentitySupplement, identitySupplement.layoutProvider->GetExpirationDateBox(), "Expiration Date");
			Utils::display_document_info_boxing(binary, DocumentType::IdentitySupplement, identitySupplement.layoutProvider->GetHeightBox(), "Height");
			Utils::display_document_info_boxing(binary, DocumentType::IdentitySupplement, identitySupplement.layoutProvider->GetWeightBox(), "Weight");
		}

		void test_passport_text_boxing(const std::string& number)
		{
			GameView game = GetGameView(number);

			auto passport = FindPassport(game.inspection);
			auto binary = PreprocessPassport(passport);

			Utils::display_document_info_boxing(binary, DocumentType::Passport, passport.layoutProvider->GetDateOfBirthBox(), "DOB");
			Utils::display_document_info_boxing(binary, DocumentType::Passport, passport.layoutProvider->GetExpirationDateBox(), "EXP");
			Utils::display_document_info_boxing(binary, DocumentType::Passport, passport.layoutProvider->GetIssuingCityBox(), "ISS");
			Utils::display_document_info_boxing(binary, DocumentType::Passport, passport.layoutProvider->GetNameBox(), "Name");
			Utils::display_document_info_boxing(binary, DocumentType::Passport, passport.layoutProvider->GetPassportNumberBox(), "Passport Number");
			Utils::display_document_info_boxing(binary, DocumentType::Passport, passport.layoutProvider->GetSexBox(), "Sex");
		}

		void test_work_pass_text_boxing(const std::string& number)
		{
			GameView game = GetGameView(number);

			auto workPass = FindWorkPass(game.inspection);
			auto binary = PreprocessWorkPass(workPass);

			Utils::display_document_info_boxing(binary, DocumentType::WorkPass, workPass.layoutProvider->GetEndDateBox(), "End Date");
			Utils::display_document_info_boxing(binary, DocumentType::WorkPass, workPass.layoutProvider->GetFieldBox(), "Field");
			Utils::display_document_info_boxing(binary, DocumentType::WorkPass, workPass.layoutProvider->GetNameBox(), "Name");
		}

		

	}

	namespace V2::Test {

		namespace Utils {

			static void display_text_field_character_boxing(const cv::Mat& document, DocType documentType, const Rectangle& boundingBox, std::string&& partName)
			{
				auto passPart = ExtractDocumentField(document, boundingBox);

				auto& fontInfo = GetFontInfo(documentType);
				auto boxes = ImageToBoxes(passPart, fontInfo);

				for (const auto& rect : boxes)
				{
					cv::rectangle(document, cv::Rect(rect.x + boundingBox.x , rect.y + boundingBox.y, rect.width, rect.height), cv::Scalar(127));
				}
			}

			static void draw_rectangle(cv::Mat& image, const Rectangle& box)
			{
				cv::rectangle(image, cv::Rect(box.x, box.y, box.width, box.height), cv::Scalar(127));
			}
		}

		void test_document_character_boxing(const std::string& number, DocType docType)
		{
			//
			// Draw boxes where text is found
			//

			// Setup
			GameView game = GetGameView(number);
			auto doc = V2::FindDocument(game.inspection, docType);
			auto binary = doc.PreprocessDocument();

			// Test
			char name = 'a';
			auto layouts = doc.GetLayout().GetAllLayouts();
			auto layoutCount = doc.GetLayout().GetLayoutCount();
			for (size_t i = 0; i < layoutCount; i++)
			{
				if (layouts[i].GetType() == DataFieldType::TextField)
				{
					Utils::display_text_field_character_boxing(binary, docType, layouts[i].GetBox(), "" + name);
					name += 1;
				}
			}

			cv::imshow("Document", ScaleImage(binary, 2.0f));
			cv::waitKey();
		}


		void test_document_field_boxing(const std::string& number, DocType docType)
		{
			// Setup
			GameView game = GetGameView(number);
			auto doc = V2::FindDocument(game.inspection, docType);

			if (!doc.IsValid())
			{
				std::cerr << "Document is not valid, test failed on setup!\n";
				return;
			}

			auto binary = doc.PreprocessDocument();
			const auto& layout = doc.GetLayout();

			// Test

			for (size_t i = 0; i < layout.GetLayoutCount(); i++)
			{
				Utils::draw_rectangle(binary, layout.GetAllLayouts()[i].GetBox());
			}

			cv::imshow("Document", ScaleImage(binary, 2.0f));
			cv::waitKey();
		}

		void test_text_field_character_boxing(const std::string& number, DocType docType, DataFieldCategory dataField)
		{
			// Setup
			GameView game = GetGameView(number);
			auto doc = V2::FindDocument(game.inspection, docType);

			if (!doc.IsValid())
			{
				std::cerr << "Document is not valid, test failed on setup!\n";
				return;
			}

			auto binary = doc.PreprocessDocument();
			const auto& layout = doc.GetLayout();

			auto dataLayout = layout.GetLayout(dataField);

			// Test
			Utils::display_text_field_character_boxing(binary, docType, dataLayout.GetBox(), "TextField");

			auto docPart = ExtractDocumentField(binary, dataLayout.GetBox());

			cv::imshow("Document Field", ScaleImage(docPart, 2.0f));
			cv::waitKey();
		}
	}
}