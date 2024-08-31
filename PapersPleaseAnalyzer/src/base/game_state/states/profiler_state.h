#pragma once
#include "base/game_state/mediators/analysis_mediator.h"
#include "base/game_state/states/state.h"

class ProfilerState : public State
{
public:
	using State::State;

	void Update();
public: // Mediator functions
	void ReceiveApplicantDocument(const Documents::V2::Doc& document);
private:
	void RegisterDocument(const Documents::V2::Doc& document);
private:
	static constexpr size_t DocumentCapacity = 10;
private:
	std::array<Documents::V2::Doc, ProfilerState::DocumentCapacity> m_comparableDocuments;
	size_t m_documentCount;
};