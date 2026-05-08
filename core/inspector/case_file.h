#ifndef _PAPLEASE_CORE_INSPECTOR_CASE_FILE_H
#define _PAPLEASE_CORE_INSPECTOR_CASE_FILE_H
#include <set>

#include <paplease/date.h>
#include <paplease/documents.h>
#include <paplease/inspector.h>
#include <paplease/observation.h>

std::set<doc_type> update_case(case_file &cf, const observation &obs);

std::string get_purpose(const case_file &cf);
std::string get_district(const case_file &cf);

#endif // _PAPLEASE_CORE_INSPECTOR_CASE_FILE_H
