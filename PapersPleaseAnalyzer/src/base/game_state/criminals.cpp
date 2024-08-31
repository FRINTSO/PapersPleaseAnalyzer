#include "pch.h"
#include "base/game_state/criminals.h"

using namespace Documents::V2;

CriminalData CreateCriminalData(const Doc& document)
{
	if (!document.IsValid())
	{
		std::cerr << "Tried creating criminal data from invalid document\n";
		return {};
	}

	return {};
}