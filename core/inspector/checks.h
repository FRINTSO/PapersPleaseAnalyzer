#ifndef _PAPLEASE_CORE_INSPECTOR_CHECKS_H
#define _PAPLEASE_CORE_INSPECTOR_CHECKS_H
#include "paplease/date.h"
#include <set>
#include <vector>

#include "paplease/documents.h"
#include "paplease/inspector.h"

std::vector<problem> find_problems(const case_file& cf, const date_t& today);

#endif // _PAPLEASE_CORE_INSPECTOR_CHECKS_H
