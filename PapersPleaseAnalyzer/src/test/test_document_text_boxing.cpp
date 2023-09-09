#include "pch.h"
#include "test/test_document_text_boxing.h"

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
#include "base/ocr/font.h"
#include "base/ocr/ocr.h"
#include "base/shape.h"

static void display_document_info_boxing(const cv::Mat& document, DocumentType documentType, const Rectangle& boundingBox, std::string&& partName) {
	auto passPart = ExtractDocumentField(document, boundingBox);
	auto& fontInfo = GetFontInfo(documentType);
	auto boxes = ImageToBoxes(passPart, fontInfo);

	for (const auto& rect : boxes) {
		cv::rectangle(passPart, cv::Rect(rect.x, rect.y, rect.width, rect.height), cv::Scalar(127));
	}

	cv::imshow(partName, DownScale(passPart, 0.25f));
}

void test_access_permit_text_boxing(const std::string& number) {
	GameView game = GetGameView(number);

	auto accessPermit = FindAccessPermit(game.inspection);
	auto binary = PreprocessAccessPermit(accessPermit);

	display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetCountryBox(), "Country");
	display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetDurationOfStayBox(), "Duration of Stay");
	display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetExpirationDateBox(), "Expiration Date");
	display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetHeightBox(), "Height");
	display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetNameBox(), "Name");
	display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetPassportNumberBox(), "Passport Number");
	display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetPhysicalAppearanceBox(), "Physical Appearance");
	display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetPurposeBox(), "Purpose");
	display_document_info_boxing(binary, DocumentType::AccessPermit, accessPermit.layoutProvider->GetWeightBox(), "Weight");
}

void test_booth_text_boxing(const std::string& number) {
	GameView game = GetGameView(number);

	auto binary = PreprocessBooth(game.booth.mat);

	display_document_info_boxing(binary, DocumentType::Booth, game.booth.layoutProvider->GetDateBox(), "Date");
	display_document_info_boxing(binary, DocumentType::Booth, game.booth.layoutProvider->GetCounterBox(), "Counter");
	display_document_info_boxing(binary, DocumentType::Booth, game.booth.layoutProvider->GetWeightBox(), "Weight");
}

void test_certificate_of_vaccination_text_boxing(const std::string& number) {
	GameView game = GetGameView(number);

	auto cov = FindCertificateOfVaccination(game.inspection);
	auto binary = PreprocessCertificateOfVaccination(cov);

	display_document_info_boxing(binary, DocumentType::CertificateOfVaccination, cov.layoutProvider->GetNameBox(), "Name");
	display_document_info_boxing(binary, DocumentType::CertificateOfVaccination, cov.layoutProvider->GetPassportNumberBox(), "Passport Number");
	display_document_info_boxing(binary, DocumentType::CertificateOfVaccination, cov.layoutProvider->GetVaccination1Box(), "Vaccination 1");
	display_document_info_boxing(binary, DocumentType::CertificateOfVaccination, cov.layoutProvider->GetVaccination2Box(), "Vaccination 2");
	display_document_info_boxing(binary, DocumentType::CertificateOfVaccination, cov.layoutProvider->GetVaccination3Box(), "Vaccination 3");
}

void test_diplomatic_authorization_text_boxing(const std::string& number) {
	GameView game = GetGameView(number);

	auto da = FindDiplomaticAuthorization(game.inspection);
	auto binary = PreprocessDiplomaticAuthorization(da);

	display_document_info_boxing(binary, DocumentType::DiplomaticAuthorization, da.layoutProvider->GetCountryListBox(), "Country List");
	display_document_info_boxing(binary, DocumentType::DiplomaticAuthorization, da.layoutProvider->GetIssuingCountryBox(), "Issuing Country");
	display_document_info_boxing(binary, DocumentType::DiplomaticAuthorization, da.layoutProvider->GetNameBox(), "Name");
	display_document_info_boxing(binary, DocumentType::DiplomaticAuthorization, da.layoutProvider->GetPassportNumberBox(), "Passport Number");
}

void test_entry_permit_text_boxing(const std::string& number) {
	GameView game = GetGameView(number);

	auto entryPermit = FindEntryPermit(game.inspection);
	auto binary = PreprocessEntryPermit(entryPermit);

	display_document_info_boxing(binary, DocumentType::EntryPermit, entryPermit.layoutProvider->GetDurationOfStayBox(), "Duration Of Stay");
	display_document_info_boxing(binary, DocumentType::EntryPermit, entryPermit.layoutProvider->GetExpirationDateBox(), "Expiration Date");
	display_document_info_boxing(binary, DocumentType::EntryPermit, entryPermit.layoutProvider->GetNameBox(), "Name");
	display_document_info_boxing(binary, DocumentType::EntryPermit, entryPermit.layoutProvider->GetPassportNumberBox(), "Passport Number");
	display_document_info_boxing(binary, DocumentType::EntryPermit, entryPermit.layoutProvider->GetPurposeBox(), "Purpose");
}

void test_entry_ticket_text_boxing(const std::string& number) {
	GameView game = GetGameView(number);

	auto entryTicket = FindEntryTicket(game.inspection);
	auto binary = PreprocessEntryTicket(entryTicket);

	display_document_info_boxing(binary, DocumentType::EntryTicket, entryTicket.layoutProvider->GetValidDateBox(), "Valid Date");
}

void test_grant_of_asylum_text_boxing(const std::string& number) {
	GameView game = GetGameView(number);

	auto grantOfAsylum = FindGrantOfAsylum(game.inspection);
	auto binary = PreprocessGrantOfAsylum(grantOfAsylum);

	display_document_info_boxing(binary, DocumentType::GrantOfAsylum, grantOfAsylum.layoutProvider->GetCountryBox(), "Country");
	display_document_info_boxing(binary, DocumentType::GrantOfAsylum, grantOfAsylum.layoutProvider->GetDateOfBirthBox(), "Date of Birth");
	display_document_info_boxing(binary, DocumentType::GrantOfAsylum, grantOfAsylum.layoutProvider->GetExpirationDateBox(), "Expiration Date");
	display_document_info_boxing(binary, DocumentType::GrantOfAsylum, grantOfAsylum.layoutProvider->GetHeightBox(), "Height");
	display_document_info_boxing(binary, DocumentType::GrantOfAsylum, grantOfAsylum.layoutProvider->GetNameBox(), "Name");
	display_document_info_boxing(binary, DocumentType::GrantOfAsylum, grantOfAsylum.layoutProvider->GetPassportNumberBox(), "Passport Number");
	display_document_info_boxing(binary, DocumentType::GrantOfAsylum, grantOfAsylum.layoutProvider->GetWeightBox(), "Weight");
}

void test_identity_card_text_boxing(const std::string& number) {
	GameView game = GetGameView(number);

	auto idCard = FindIdentityCard(game.inspection);
	auto binary = PreprocessIdentityCard(idCard);

	display_document_info_boxing(binary, DocumentType::IdentityCard, idCard.layoutProvider->GetDateOfBirthBox(), "Date of Birth");
	display_document_info_boxing(binary, DocumentType::IdentityCard, idCard.layoutProvider->GetDistrictBox(), "District");
	display_document_info_boxing(binary, DocumentType::IdentityCard, idCard.layoutProvider->GetHeightBox(), "Height");
	display_document_info_boxing(binary, DocumentType::IdentityCard, idCard.layoutProvider->GetNameBox(), "Name");
	display_document_info_boxing(binary, DocumentType::IdentityCard, idCard.layoutProvider->GetWeightBox(), "Weight");
}

void test_identity_supplement_text_boxing(const std::string& number) {
	GameView game = GetGameView(number);

	auto identitySupplement = FindIdentitySupplement(game.inspection);
	auto binary = PreprocessIdentitySupplement(identitySupplement);

	display_document_info_boxing(binary, DocumentType::IdentitySupplement, identitySupplement.layoutProvider->GetDescriptionBox(), "Description");
	display_document_info_boxing(binary, DocumentType::IdentitySupplement, identitySupplement.layoutProvider->GetExpirationDateBox(), "Expiration Date");
	display_document_info_boxing(binary, DocumentType::IdentitySupplement, identitySupplement.layoutProvider->GetHeightBox(), "Height");
	display_document_info_boxing(binary, DocumentType::IdentitySupplement, identitySupplement.layoutProvider->GetWeightBox(), "Weight");
}

void test_passport_text_boxing(const std::string& number) {
	GameView game = GetGameView(number);

	auto passport = FindPassport(game.inspection);
	auto binary = PreprocessPassport(passport);

	display_document_info_boxing(binary, DocumentType::Passport, passport.layoutProvider->GetDateOfBirthBox(), "DOB");
	display_document_info_boxing(binary, DocumentType::Passport, passport.layoutProvider->GetExpirationDateBox(), "EXP");
	display_document_info_boxing(binary, DocumentType::Passport, passport.layoutProvider->GetIssuingCityBox(), "ISS");
	display_document_info_boxing(binary, DocumentType::Passport, passport.layoutProvider->GetNameBox(), "Name");
	display_document_info_boxing(binary, DocumentType::Passport, passport.layoutProvider->GetPassportNumberBox(), "Passport Number");
	display_document_info_boxing(binary, DocumentType::Passport, passport.layoutProvider->GetSexBox(), "Sex");
}

void test_work_pass_text_boxing(const std::string& number) {
	GameView game = GetGameView(number);

	auto workPass = FindWorkPass(game.inspection);
	auto binary = PreprocessWorkPass(workPass);

	display_document_info_boxing(binary, DocumentType::WorkPass, workPass.layoutProvider->GetEndDateBox(), "End Date");
	display_document_info_boxing(binary, DocumentType::WorkPass, workPass.layoutProvider->GetFieldBox(), "Field");
	display_document_info_boxing(binary, DocumentType::WorkPass, workPass.layoutProvider->GetNameBox(), "Name");
}
