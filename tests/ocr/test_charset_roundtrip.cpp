/*
 * test_charset_roundtrip.cpp - Verify charset tables have no collisions
 *
 * Since gen_charset precomputes encodings at build time, this test
 * verifies the invariants that gen_charset enforces:
 *   1. Every encoding is non-zero (glyph wasn't too large)
 *   2. No two entries share the same encoding (no collisions)
 *
 * If gen_charset is working correctly, this should always pass.
 * It exists as a defense-in-depth check.
 */

#include <cstdio>
#include <unordered_map>

#include <paplease/types.h>

#include "ocr/charset_table.h"
#include "test.h"

static int check_table(const char *name, const charset_table &table)
{
	fprintf(stderr, "  %-12s: %u entries\n", name, table.count);
	int failures = 0;

	std::unordered_map<u64, char> seen;

	for (u32 i = 0; i < table.count; i++) {
		const auto &e = table.entries[i];

		if (e.encoding == 0) {
			fprintf(stderr, "    FAIL: entry %u ('%c') has zero encoding\n",
				i, e.ch);
			failures++;
			continue;
		}

		auto it = seen.find(e.encoding);
		if (it != seen.end()) {
			fprintf(stderr,
				"    FAIL: entry %u ('%c') collides with '%c' "
				"at encoding 0x%llx\n",
				i, e.ch, it->second,
				(unsigned long long)e.encoding);
			failures++;
			continue;
		}
		seen[e.encoding] = e.ch;
	}

	return failures;
}

int main()
{
	int total_failures = 0;

	struct {
		const char *name;
		const charset_table *table;
	} sets[] = {
		{ "t04b03",     &charset_t04b03     },
		{ "bm_mini",    &charset_bm_mini    },
		{ "mini_kylie", &charset_mini_kylie },
		{ "booth",      &charset_booth      },
	};

	for (const auto &s : sets)
		total_failures += check_table(s.name, *s.table);

	if (total_failures > 0) {
		fprintf(stderr, "\nFAIL: %d charset table failures\n",
			total_failures);
		return 1;
	}

	fprintf(stderr, "\nAll charset roundtrip tests passed!\n");
	return 0;
}
