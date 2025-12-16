#ifndef INSPECTOR_PRINT_H
#define INSPECTOR_PRINT_H
#include <paplease/documents.h>

void print_passport(const passport_data &data);
void print_entry_permit(const entry_permit_data &data);
void print_entry_ticket(const entry_ticket_data &data);
void print_work_pass(const work_pass_data &data);
void print_access_permit(const access_permit_data &data);
void print_diplomatic_auth(const diplomatic_authorization_data &data);
void print_grant_of_asylum(const grant_of_asylum_data &data);
void print_identity_card(const identity_card_data &data);
void print_identity_supplement(const identity_supplement_data &data);
void print_vaccination_cert(const certificate_of_vaccination_data &data);

#endif // INSPECTOR_PRINT_H
