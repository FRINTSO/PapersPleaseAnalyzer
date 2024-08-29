#pragma once

enum class SpeakerRole
{
	Invalid = 0,
	Applicant,
	Inspector
};

class TranscriptEntry
{
private:
	SpeakerRole m_speakerRole;
	std::string m_dialogueLine;
};

class Transcript
{
private:
	std::array<TranscriptEntry, 10> m_entries;
};