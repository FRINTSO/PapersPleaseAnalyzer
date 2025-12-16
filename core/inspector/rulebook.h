#ifndef _PAPLEASE_CORE_INSPECTOR_RULEBOOK_H
#define _PAPLEASE_CORE_INSPECTOR_RULEBOOK_H
#include <paplease/documents.h>
#include <paplease/resources.h>

bool parse_rulebook(struct day_rules &out, const doc &rulebook,
		    const resources_ctx &ctx);

#endif // _PAPLEASE_CORE_INSPECTOR_RULEBOOK_H
