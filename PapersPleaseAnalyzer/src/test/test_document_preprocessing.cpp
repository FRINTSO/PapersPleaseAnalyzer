#include "pch.h"
#include "test/test_document_preprocessing.h"

#include "base/booth.h"
#include "base/documents/access_permit.h"
#include "base/documents/certificate_of_vaccination.h"
#include "base/documents/diplomatic_authorization.h"
#include "base/documents/entry_permit.h"
#include "base/documents/entry_ticket.h"
#include "base/documents/grant_of_asylum.h"
#include "base/documents/identity_card.h"
#include "base/documents/identity_supplement.h"
#include "base/documents/passport.h"
#include "base/documents/work_pass.h"
#include "base/game_view.h"

void test_access_permit_preprocessing(const std::string& number) {
	GameView game = GetGameView(number);

	auto doc = FindAccessPermit(game.inspection);
	auto binary = PreprocessAccessPermit(doc);
	cv::imshow("Binary Document", binary);
}

void test_booth_preprocessing(const std::string& number) {
	GameView game = GetGameView(number);

	auto binary = PreprocessBooth(game.booth);
	cv::imshow("Binary Document", binary);
}

void test_certificate_of_vaccination_preprocessing(const std::string& number) {
	GameView game = GetGameView(number);

	auto doc = FindCertificateOfVaccination(game.inspection);
	auto binary = PreprocessCertificateOfVaccination(doc);
	cv::imshow("Binary Document", binary);
}

void test_diplomatic_authorization_preprocessing(const std::string& number) {
	GameView game = GetGameView(number);

	auto doc = FindDiplomaticAuthorization(game.inspection);
	auto binary = PreprocessDiplomaticAuthorization(doc);
	cv::imshow("Binary Document", binary);
}

void test_entry_permit_preprocessing(const std::string& number) {
	GameView game = GetGameView(number);

	auto doc = FindEntryPermit(game.inspection);
	auto binary = PreprocessEntryPermit(doc);
	cv::imshow("Binary Document", binary);
}

void test_entry_ticket_preprocessing(const std::string& number) {
	GameView game = GetGameView(number);

	auto doc = FindEntryTicket(game.inspection);
	auto binary = PreprocessEntryTicket(doc);
	cv::imshow("Binary Document", binary);
}

void test_grant_of_asylum_preprocessing(const std::string& number) {
	GameView game = GetGameView(number);

	auto doc = FindGrantOfAsylum(game.inspection);
	auto binary = PreprocessGrantOfAsylum(doc);
	cv::imshow("Binary Document", binary);
}

void test_identity_card_preprocessing(const std::string& number) {
	GameView game = GetGameView(number);

	auto doc = FindIdentityCard(game.inspection);
	auto binary = PreprocessIdentityCard(doc);
	cv::imshow("Binary Document", binary);
}

void test_identity_supplement_preprocessing(const std::string& number) {
	GameView game = GetGameView(number);

	auto doc = FindIdentitySupplement(game.inspection);
	auto binary = PreprocessIdentitySupplement(doc);
	cv::imshow("Binary Document", binary);
}

void test_passport_preprocessing(const std::string& number) {
	GameView game = GetGameView(number);

	auto doc = FindPassport(game.inspection);
	auto binary = PreprocessPassport(doc);
	cv::imshow("Binary Document", binary);
}

void test_work_pass_preprocessing(const std::string& number) {
	GameView game = GetGameView(number);

	auto doc = FindWorkPass(game.inspection);
	auto binary = PreprocessWorkPass(doc);
	cv::imshow("Binary Document", binary);
}
