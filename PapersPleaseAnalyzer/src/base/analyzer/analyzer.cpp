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


	AccessPermit accessPermit = FindAccessPermit(gameView.inspection);
	CertificateOfVaccination cov = FindCertificateOfVaccination(gameView.inspection);
	DiplomaticAuthorization da = FindDiplomaticAuthorization(gameView.inspection);
	EntryPermit entryPermit = FindEntryPermit(gameView.inspection);
	EntryTicket entryTicket = FindEntryTicket(gameView.inspection);
	GrantOfAsylum goa = FindGrantOfAsylum(gameView.inspection);
	IdentityCard idCard = FindIdentityCard(gameView.inspection);
	IdentitySupplement idSupplement = FindIdentitySupplement(gameView.inspection);
	Passport passport = FindPassport(gameView.inspection);
	WorkPass workPass = FindWorkPass(gameView.inspection);


	
	/*
	
	*/


	cv::imshow("Inspection Area", gameView.inspection);
	cv::imshow("Booth Area", gameView.booth.mat);

	cv::waitKey();

	return { true };
}