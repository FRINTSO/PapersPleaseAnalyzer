#pragma once
#include <array>

#include "base/game_view.h"
#include "base/documents_v2/doc_class.h"

class DocTracker
{
public:
	void Update(const GameView& gameView);
private:
	void TrackDocuments(const GameView& gameView);
private:
	static constexpr size_t DocCapacity = 10;
private:
	std::array<Documents::V2::Doc, DocCapacity> m_docs; // there can be max 10 docs
	size_t m_documentCount;
};