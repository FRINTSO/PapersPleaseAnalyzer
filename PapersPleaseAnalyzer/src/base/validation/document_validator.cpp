#include "pch.h"
#include "base/validation/document_validator.h"

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


bool ValidateAccessPermitData(const AccessPermitData& data) {
	if (!data.hasValidSeal) {
		return false;
	}
	return false;
}

bool ValidateCertificateOfVaccinationData(const CertificateOfVaccinationData& data) {
	return false;
}

bool ValidateDiplomaticAuthorizationData(const DiplomaticAuthorizationData& data) {
	return false;
}

bool ValidateEntryPermitData(const EntryPermitData& data) {
	return false;
}

bool ValidateEntryTicketData(const EntryTicketData& data) {
	return false;
}

bool ValidateGrantOfAsylumData(const GrantOfAsylumData& data) {
	return false;
}

bool ValidateIdentityCardData(const IdentityCardData& data) {
	return false;
}

bool ValidateIdentitySupplementData(const IdentitySupplementData& data) {
	return false;
}



bool ValidateWorkPassData(const WorkPassData& data) {
	return false;
}