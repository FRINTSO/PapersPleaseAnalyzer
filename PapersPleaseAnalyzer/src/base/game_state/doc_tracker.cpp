#include "pch.h"
#include "base/game_state/doc_tracker.h"

#include <iostream>

void DocTracker::Update(const GameView& gameView)
{
	std::cout << "Begin Doc Tracker Update\n";

	// Update passports
	// Cheap implementation
	

	std::cout << "End Doc Tracker Update\n";
}

void DocTracker::TrackDocuments(const GameView& gameView)
{
	for (size_t i = 0; i < DocTracker::DocCapacity; i++)
	{
		auto type = static_cast<Documents::V2::DocType>(i + 1);
		// m_docs[i] = Documents::V2::FindDocument(gameView, type);
		
	}
}