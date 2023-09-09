#include "pch.h"
#include "test/test_document_data_extraction.h"

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

void test_access_permit_data_extraction(const std::string& number) {
	GameView game = GetGameView(number);

	AccessPermit accessPermit = FindAccessPermit(game.inspection);
	AccessPermitData accessPermitData = GetAccessPermitData(accessPermit);

	std::cout << accessPermitData.name << "\n";
	std::cout << accessPermitData.country << "\n";
	std::cout << accessPermitData.passportNumber << "\n";
	std::cout << accessPermitData.purpose << "\n";
	std::cout << accessPermitData.durationOfStay << "\n";
	std::cout << accessPermitData.height << "\n";
	std::cout << accessPermitData.weight << "\n";
	std::cout << accessPermitData.physicalAppearance << "\n";
	std::cout << accessPermitData.expirationDate << "\n";
	std::cout << accessPermitData.hasValidSeal << "\n";
}
void test_booth_data_extraction(const std::string& number) {
	GameView game = GetGameView(number);
	BoothData boothData = GetBoothData(game.booth);

	std::cout << boothData.date << "\n";
	std::cout << boothData.counter << "\n";
	std::cout << boothData.weight << "\n";
}
void test_certificate_of_vaccination_data_extraction(const std::string& number) {
	GameView game = GetGameView(number);

	CertificateOfVaccination cov = FindCertificateOfVaccination(game.inspection);
	CertificateOfVaccinationData covData = GetCertificateOfVaccinationData(cov);

	std::cout << covData.name << "\n";
	std::cout << covData.passportNumber << "\n";
	std::cout << covData.vaccination1 << "\n";
	std::cout << covData.vaccination2 << "\n";
	std::cout << covData.vaccination3 << "\n";
}
void test_diplomatic_authorization_data_extraction(const std::string& number) {
	GameView game = GetGameView(number);

	DiplomaticAuthorization da = FindDiplomaticAuthorization(game.inspection);
	DiplomaticAuthorizationData daData = GetDiplomaticAuthorizationData(da);

	std::cout << daData.name << "\n";
	std::cout << daData.passportNumber << "\n";
	std::cout << daData.issuingCountry << "\n";
	std::cout << daData.countryList << "\n";
	std::cout << daData.hasValidSeal << "\n";
}
void test_entry_permit_data_extraction(const std::string& number) {
	GameView game = GetGameView(number);

	EntryPermit entryPermit = FindEntryPermit(game.inspection);
	EntryPermitData entryPermitData = GetEntryPermitData(entryPermit);

	std::cout << entryPermitData.name << "\n";
	std::cout << entryPermitData.passportNumber << "\n";
	std::cout << entryPermitData.purpose << "\n";
	std::cout << entryPermitData.duration << "\n";
	std::cout << entryPermitData.expirationDate << "\n";
	std::cout << entryPermitData.hasValidSeal << "\n";
}
void test_entry_ticket_data_extraction(const std::string& number) {
	GameView game = GetGameView(number);

	EntryTicket entryTicket = FindEntryTicket(game.inspection);
	EntryTicketData entryTicketData = GetEntryTicketData(entryTicket);

	std::cout << entryTicketData.validDate << "\n";
}
void test_grant_of_asylum_data_extraction(const std::string& number) {
	GameView game = GetGameView(number);

	GrantOfAsylum goa = FindGrantOfAsylum(game.inspection);
	GrantOfAsylumData goaData = GetGrantOfAsylumData(goa);

	std::cout << goaData.name << "\n";
	std::cout << goaData.country << "\n";
	std::cout << goaData.passportNumber << "\n";
	std::cout << goaData.dateOfBirth << "\n";
	std::cout << goaData.height << "\n";
	std::cout << goaData.weight << "\n";
	std::cout << goaData.expirationDate << "\n";
	std::cout << goaData.hasValidSeal << "\n";
}
void test_identity_card_data_extraction(const std::string& number) {
	GameView game = GetGameView(number);

	IdentityCard idCard = FindIdentityCard(game.inspection);
	IdentityCardData idCardData = GetIdentityCardData(idCard);

	std::cout << idCardData.district << "\n";
	std::cout << idCardData.name << "\n";
	std::cout << idCardData.dateOfBirth << "\n";
	std::cout << idCardData.height << "\n";
	std::cout << idCardData.weight << "\n";
}
void test_identity_supplement_data_extraction(const std::string& number) {
	GameView game = GetGameView(number);

	IdentitySupplement idSupplement = FindIdentitySupplement(game.inspection);
	IdentitySupplementData idSupplementData = GetIdentitySupplementData(idSupplement);

	std::cout << idSupplementData.height << "\n";
	std::cout << idSupplementData.weight << "\n";
	std::cout << idSupplementData.description << "\n";
	std::cout << idSupplementData.expirationDate << "\n";
}
void test_passport_data_extraction(const std::string& number) {
	GameView game = GetGameView(number);

	Passport passport = FindPassport(game.inspection);
	PassportData passportData = GetPassportData(passport);

	std::cout << passportData.name << "\n";
	std::cout << passportData.dateOfBirth << "\n";
	std::cout << passportData.sex << "\n";
	std::cout << passportData.issuingCity << "\n";
	std::cout << passportData.expirationDate << "\n";
	std::cout << passportData.passportNumber << "\n";
	std::cout << (int)passportData.passportType << "\n";
}
void test_work_pass_data_extraction(const std::string& number) {
	GameView game = GetGameView(number);

	WorkPass workPass = FindWorkPass(game.inspection);
	WorkPassData workPassData = GetWorkPassData(workPass);

	std::cout << workPassData.name << "\n";
	std::cout << workPassData.field << "\n";
	std::cout << workPassData.endDate << "\n";
	std::cout << workPassData.hasValidSeal << "\n";
}