#include "paplease/geometry.h"
#include <string_view>

#include <paplease/compiler.h>
#include <paplease/documents.h>
#include <paplease/game_screen.h>
#include <paplease/inspector.h>
#include <paplease/vision.h>
#include <vector>

void inspector::init()
{
	this->current_date = "";
	this->current_entrant_count = "";
	this->has_rules = false;
	this->expected_docs = {};

	this->seen_docs = {};
	this->scanned_docs = {};
}

static void instruct_user(std::string_view message)
{
	TODO("display message to user");
}

void handle_new_date(inspector &ins, const std::string &new_date)
{
	// reset inspector
	ins.init();
	instruct_user("You need to scan the rulebook in order to continue");
	ins.current_date = new_date;
}

void handle_new_entrant(inspector &ins, const std::string &new_entrant_count)
{
	// increment entrant count
	// reset entrant context
	ins.current_entrant_count = new_entrant_count;
}

bool scan_inspection_for_rulebook(rectangle &out, const game_screen &screen)
{
}

struct rulebook {};

bool extract_rulebook(rulebook &out, const cv::Mat &rulebook_region)
{
}

bool initialize_rules(const game_screen &screen)
{
	rectangle rulebook_rect;
	if (!scan_inspection_for_rulebook(rulebook_rect, screen))
		return false;

	cv::Mat rulebook_region = screen.pixels(rulebook_rect.to_cv());
	rulebook rb;
	if (!extract_rulebook(rb, rulebook_region))
		return false;

	return true;
}

void refresh_seen_documents(inspector &ins, const game_screen &screen)
{
	std::vector<doc_variant> docs_seen_in_booth =
		scan_booth_for_doc_variants(screen);
	std::vector<doc_variant> docs_seen_in_inspection =
		scan_inspection_for_doc_variants(screen);

	for (doc_variant variant : docs_seen_in_booth) {
		ins.seen_docs.emplace(variant);
	}
	for (doc_variant variant : docs_seen_in_inspection) {
		ins.seen_docs.emplace(variant);
	}
}

bool extract_doc_data(doc_data &out, const doc &document)
{
}

void refresh_scanned_documents(inspector &ins, const game_screen &screen)
{
	std::vector<doc> documents = scan_for_inspection_documents(screen);

	for (doc &document : documents) {
		if (ins.scanned_docs.find(document.variant) !=
		    ins.scanned_docs.end()) {
			// rescanned document
                        TODO("handle how rescanned documents are processed, whether to overwrite data or ignore");
                        continue;
		}

		doc_data data;
		if (!extract_doc_data(data, document))
			continue;

		ins.scanned_docs[document.variant] = data;
	}
}

void inspector::process_game_frame(const game_screen &screen)
{
	booth_info booth;
	if (!extract_booth_info(booth, screen))
		return;

	if (this->current_date != booth.current_date)
		handle_new_date(*this, booth.current_date);

	if (this->current_entrant_count != booth.entrant_count)
		handle_new_entrant(*this, booth.entrant_count);

	if (!this->has_rules) {
		if (!initialize_rules(screen))
			return;

		this->has_rules = true;
	}

	// scan documents driven by rulebook

	// some documents will be required
	// some will have failed analysis and need to reanalyzed
	// some will get discrepancies, that may get cleared after interrogation

	// list of expected documents
	// a) need to be viewed
	//      - if not viewed, then user needs to interrogate entrant
	// b) need to be analyzed
	//      - if not analyzed, then user needs to drag document into inspection
	// c) need to be confirmed as valid or invalid
	//      - action depends on discrepancy type
	//      - for some types we don't need to ask the user to interrogate, we can conclude the analysis
	//      - but for some types, we must ask for supplementary information, before we confirm validity
	//
	// for each scanned document, we can expect the list to change, which may require re-scanning

	// a) viewed documents - all documents that can be seen
	refresh_seen_documents(*this, screen);

	// b) analyzed documents - all documents that have been scanned in inspection
	refresh_scanned_documents(*this, screen);
}

/*
 * react on new date
 *  effect: reset inspector
 *
 * react on new entrant
 *  effect: keep day info, but reset entrant specific data
 *
 * react on analyzed document
 *  effect: update rulebook state
 **/



















/*
 * Rul
 **/









