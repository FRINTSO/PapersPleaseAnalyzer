#include "pch.h"
#include "base/analyzer/analyzer.h"

#include <opencv2/opencv.hpp>

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


Analysis Analyze(const GameView& gameView) {
	// Get Game state
	// Find what documents are present
	// Get all documents
	// Validate document against itself and game state (exp data, seal, etc)
	// Add Document in comparision que, and compare same fields of all documents against each other


	Documents::V1::AccessPermit accessPermit = Documents::V1::FindAccessPermit(gameView.inspection);
	Documents::V1::CertificateOfVaccination cov = Documents::V1::FindCertificateOfVaccination(gameView.inspection);
	Documents::V1::DiplomaticAuthorization da = Documents::V1::FindDiplomaticAuthorization(gameView.inspection);
	Documents::V1::EntryPermit entryPermit = Documents::V1::FindEntryPermit(gameView.inspection);
	Documents::V1::EntryTicket entryTicket = Documents::V1::FindEntryTicket(gameView.inspection);
	Documents::V1::GrantOfAsylum goa = Documents::V1::FindGrantOfAsylum(gameView.inspection);
	Documents::V1::IdentityCard idCard = Documents::V1::FindIdentityCard(gameView.inspection);
	Documents::V1::IdentitySupplement idSupplement = Documents::V1::FindIdentitySupplement(gameView.inspection);
	Documents::V1::Passport passport = Documents::V1::FindPassport(gameView.inspection);
	Documents::V1::WorkPass workPass = Documents::V1::FindWorkPass(gameView.inspection);


	
	/*
	
	*/


	cv::imshow("Inspection Area", gameView.inspection);
	cv::imshow("Booth Area", gameView.booth.mat);

	cv::waitKey();

	return { true };
}