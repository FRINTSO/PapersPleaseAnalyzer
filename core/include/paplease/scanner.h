#ifndef PAPLEASE_SCANNER_H
#define PAPLEASE_SCANNER_H

#include <paplease/documents.h>
#include <paplease/game_screen.h>

bool find_doc(doc &out, const game_screen &screen, ui_section section,
	      doc_type type);

#endif // PAPLEASE_SCANNER_H
