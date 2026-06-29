#ifndef PAPLEASE_CORE_INSPECTOR_CHECKS_H
#define PAPLEASE_CORE_INSPECTOR_CHECKS_H
#include <vector>

#include <paplease/date.h>
#include <paplease/documents.h>
#include <paplease/inspector.h>

std::vector<problem> find_problems(const case_file& cf, const date_t& today);

#endif // PAPLEASE_CORE_INSPECTOR_CHECKS_H
