#include <cstdio>

#include <paplease/compiler.h>
#include <paplease/documents.h>
#include <paplease/game_screen.h>
#include <paplease/inspector.h>
#include <paplease/vision.h>

void update_entrant_classification(inspector &ins)
{
	// Check each piece of evidence
	//if (ins.scanned_docs.count(doc_variant::passport_arstotzka)) {
	//	ins.classification.is_citizen = true;
	//	ins.classification.nationality_known = true;
	//}
	//if (ins.scanned_docs.count(doc_variant::identity_card)) {
	//	ins.classification.is_citizen =
	//		true; // only citizens have ID cards
	//	ins.classification.nationality_known = true;
	//}
	//if (ins.scanned_docs.count(doc_variant::work_pass)) {
	//	ins.classification.visit_purpose = purpose::worker;
	//}
	// ... etc for each evidence type
}

std::vector<doc_type> get_required_documents(inspector &ins)
{
	std::vector<doc_type> required;

	// Universal
	if (ins.rules.require_passport)
		required.push_back(doc_type::passport);

	// Conditional on classification
	if (ins.classification.is_citizen && ins.rules.citizens_need_id)
		required.push_back(doc_type::identity_card);

	if (!ins.classification.is_citizen &&
	    ins.classification.nationality_known &&
	    ins.rules.foreigners_need_permit)
		required.push_back(doc_type::entry_permit);

	//if (ins.classification.visit_purpose == purpose::worker &&
	//    ins.rules.workers_need_work_pass)
	//	required.push_back(doc_type::work_pass);

	// ... etc

	return required;
}

static bool scan_and_parse_rulebook(const game_screen &screen)
{
	// find rulebook
	doc rulebook;
	//if (!scan_for_rulebook_in_inspection(rulebook, screen))
        //        return false;
        return false;
}

void inspector::init()
{
	this->current_date = "";
	this->rules = {};
	this->has_rules = {};

	// Entrant state
	this->current_entrant_count = "";
	this->classification = {};

	this->documents = {};
}

void inspector::process_game_frame(const game_screen &screen)
{
	booth_info booth;
	if (!extract_booth_info(booth, screen))
		return;

	if (this->current_date != booth.current_date) {
		// reset inspector
		this->init();
		this->current_date = booth.current_date;
		printf("=== NEW DAY: %s ===\n", booth.current_date.c_str());
		return;
	}

	if (this->current_entrant_count != booth.entrant_count) {
		this->current_entrant_count = booth.entrant_count;
		this->classification = {};
		this->documents = {};
		printf("--- Entrant #%s ---\n", booth.entrant_count.c_str());
		return;
	}

	// 1. Make sure we have rules
	if (!this->has_rules) {
		if (!scan_and_parse_rulebook(screen))
			return;
		this->has_rules = true;
	}

	// 2. Scan any visible documents we haven't processed
	//	scan_visible_documents(screen);
	//std::vector<doc_variant> booth_docs =
	//	scan_booth_for_doc_variants(screen);
	//for (auto variant : booth_docs) {
	//	seen_docs.insert(variant);
	//}
        //
	//// Scan INSPECTION - read actual data from open documents
	//std::vector<doc> inspection_docs =
	//	scan_for_inspection_documents(screen);
	//for (auto &doc : inspection_docs) {
	//	seen_docs.insert(doc.variant); // also counts as seen
        //
	//	if (scanned_docs.count(doc.variant) == 0) {
	//		// Haven't extracted data yet - do it now
	//		doc_data data;
	//		if (extract_doc_data(data, doc)) {
	//			scanned_docs[doc.variant] = data;
	//		}
	//	}
	//}
        //
	//// 3. Update classification based on what we've seen
	//update_entrant_classification();
        //
	//// 4. Determine what's required based on classification + rules
	//auto required = get_required_documents();
        //
	//// 5. Check what we have vs what we need
	//auto missing = compute_missing_documents(required);
	//auto discrepancies = check_for_discrepancies();

	// 6. Output state
	//if (!discrepancies.empty()) {
	//	printf("DISCREPANCIES FOUND: ...\n");
	//} else if (!missing.empty()) {
	//	printf("WAITING FOR: ...\n");
	//} else {
	//	printf("ALL CLEAR - APPROVE\n");
	//}
}
