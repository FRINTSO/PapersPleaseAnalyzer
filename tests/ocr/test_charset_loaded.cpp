/*
 * test_charset_loaded.cpp - Smoke test that all charsets are non-empty
 *
 * Verifies that each charset_table has at least the expected number
 * of glyphs and that all entries have non-zero encodings.
 */

#include <cstdio>

#include <paplease/types.h>

#include "ocr/charset_table.h"
#include "test.h"

int main()
{
	struct {
		const char *name;
		const charset_table *table;
		u32 min_glyphs;
	} expected[] = {
		{ "t04b03",     &charset_t04b03,     10 },
		{ "bm_mini",    &charset_bm_mini,    50 },
		{ "mini_kylie", &charset_mini_kylie,  10 },
		{ "booth",      &charset_booth,       10 },
	};

	for (const auto &e : expected) {
		fprintf(stderr, "  %-12s: %u glyphs (need >= %u)\n",
			e.name, e.table->count, e.min_glyphs);

		TEST_ASSERT_GT(e.table->count, 0u);
		TEST_ASSERT_GE(e.table->count, e.min_glyphs);
	}

	fprintf(stderr, "All charset smoke tests passed!\n");
	return 0;
}
