#include "pch.h"
#include "base/game_state/states/profiler_state.h"

void ProfilerState::Update()
{
	std::cout << "\tProfilerState::Update() Begin\n";


	std::cout << "\tProfilerState::Update() End\n";
	// Do comparison and build profile
}

void ProfilerState::ReceiveApplicantDocument(const Documents::V2::Doc& document)
{
	this->RegisterDocument(document);
}

void ProfilerState::RegisterDocument(const Documents::V2::Doc& document)
{
	assert(m_documentCount + 1 < ProfilerState::DocumentCapacity);

	std::cout << "\t - Registered a document to Profiler!\n";

	m_comparableDocuments[m_documentCount] = document;
	m_documentCount++;
}