#pragma once
#include <array>

#include "base/documents_v2/doc_data.h"
#include "base/game_state/mediators/analysis_mediator.h"
#include "base/game_state/components/component.h"

class Profile
{
private:
	static constexpr size_t FieldsCapacity = 20;
private:
	static std::array<Documents::V2::DataFieldCategory, Profile::FieldsCapacity> ProfileFields();
	static int GetFieldIndexByCategoryType(Documents::V2::DataFieldCategory category);
private:
	std::array<Documents::V2::FieldData, Profile::FieldsCapacity> m_fields;
	size_t m_fieldCount;
};

class ProfilerState : public Component
{
public:
	using Component::Component;

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

	Profile m_currentProfile;
};