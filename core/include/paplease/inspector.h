#ifndef PAPLEASE_INSPECTOR_H
#define PAPLEASE_INSPECTOR_H

#include <unordered_map>
#include <unordered_set>

#include <paplease/documents.h>
#include <paplease/game_screen.h>
#include <paplease/types.h>

struct doc_lookup {
	doc_type type;
	country passport;
};

namespace std
{
template <> struct hash<doc_lookup> {
	static_assert(sizeof(doc_lookup) == sizeof(u16));
	size_t operator()(const doc_lookup key) const noexcept
	{
		const doc_lookup *lookup = &key;
		u16 value = *reinterpret_cast<const u16 *>(lookup);
		return std::hash<u16>{}(value);
	}
};
}

struct inspector {
	std::string current_date;
	std::string current_entrant_count;

	bool has_rules;

	std::unordered_set<doc_lookup> expected_docs;

	std::unordered_set<doc_variant> seen_docs;
        std::unordered_map<doc_variant, doc_data> scanned_docs;


	void init();
	void process_game_frame(const game_screen &screen);
};

#endif // PAPLEASE_INSPECTOR_H
