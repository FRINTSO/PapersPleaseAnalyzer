#ifndef PAPLEASE_OCR_CHARSET_TABLE_H
#define PAPLEASE_OCR_CHARSET_TABLE_H

#include <paplease/types.h>

struct charset_entry {
	char ch;
	u64 encoding;
};

struct charset_table {
	const charset_entry *entries;
	u32 count;
};

extern const charset_table charset_t04b03;
extern const charset_table charset_bm_mini;
extern const charset_table charset_mini_kylie;
extern const charset_table charset_booth;

#endif // PAPLEASE_OCR_CHARSET_TABLE_H
