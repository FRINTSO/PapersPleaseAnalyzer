#include "pch.h"
#include "paplease/analysis/data/transcript.h"

#include "paplease/documents/bounding_box_finder.inc"
#include "paplease/ocr/font.h"
#include "paplease/ocr/ocr.h"

namespace paplease {
	namespace analysis {
		namespace data {

			struct DialogueBuffer
			{
				static constexpr size_t Size = 1024;
				char data[Size]{};
			};
			static constinit DialogueBuffer g_TranscriptDialogue;


			struct DialogueBox
			{
				Rectangle box;
				SpeakerRole speaker;
			};

			static inline cv::Mat AdjustBinaryTextColorOfTranscript(const cv::Mat& transcript_bin)
			{
				constexpr auto layout = documents::DocLayout::GetInstant(documents::AppearanceType::Transcript);
				constexpr auto box = layout.GetLayout(documents::FieldCategory::TranscriptPage).GetBox();

				cv::Mat transcript(transcript_bin, cv::Rect(box.x, box.y, box.width, box.height));

				constexpr int minColLimit = 20;
				constexpr int minRowLimit = 15;

				for (int row = 0; row < transcript.rows; row++)
				{
					int left = documents::TryFindBlackPixel(transcript, row);
					if (left == -1) continue;

					int right = documents::FindValidRightEdge(transcript, row, left, minColLimit);
					if (right == -1) continue;

					int bottom = documents::FindValidBottomEdge(transcript, row, right, minRowLimit);
					if (bottom == -1) continue;

					documents::InvertRegion(transcript, left, row, right, bottom);

					row = bottom;
				}

				return transcript_bin;
			}

			static core::FixedArray<DialogueBox, 10> GetDialogueBoxes(const cv::Mat& transcript)
			{
				core::FixedArray<DialogueBox, 10> dialogueBoxes{};

				using namespace documents;

				constexpr const auto box = documents::DocLayout::GetInstant(documents::AppearanceType::Transcript)
					.GetLayout(documents::FieldCategory::TranscriptPage)
					.GetBox();

				const auto cutout = transcript(box);

				constexpr u8 Black = 0;
				constexpr u8 White = 255;
				auto MatchBlack = [&](int row, int col) { return cutout.ptr<u8>(row)[col] == Black; };
				auto MatchWhite = [&](int row, int col) { return cutout.ptr<u8>(row)[col] == White; };

				// Find First dialogue box
				auto topLeft = FindTopLeftCorner(cutout, MatchBlack);
				int width = TraceEdgeLengthFromCorner(topLeft, Direction::Right, cutout, MatchBlack);
				int height = 0;
				do {
					Point localTopLeft{ topLeft.x, topLeft.y + height };
					Point whiteTopLeft = localTopLeft;
					int whiteHeight = TraceEdgeLengthFromCorner(localTopLeft, Direction::Down, cutout, MatchWhite);

					localTopLeft.y += whiteHeight;
					int blackHeight = TraceEdgeLengthFromCorner(localTopLeft, Direction::Down, cutout, MatchBlack);
					if (blackHeight == 0)
					{
						// This will be text
						auto bottomLeft = FindCorner(Corner::BottomLeft, cutout, MatchBlack);
						if (bottomLeft != Point{ -1, -1 })
						{
							height = bottomLeft.y - topLeft.y + 1;
						}


						dialogueBoxes.EmplaceBack(
							DialogueBox{
								Rectangle{
									whiteTopLeft.x + box.x, whiteTopLeft.y + box.y, width, bottomLeft.y - whiteTopLeft.y
								},
								SpeakerRole::Entrant
							}
						);
						break;
					}

					if (whiteHeight != 0)
					{
						dialogueBoxes.EmplaceBack(
							DialogueBox{
								Rectangle{
									whiteTopLeft.x + box.x, whiteTopLeft.y + box.y, width, whiteHeight
								},
								SpeakerRole::Entrant
							}
						);
					}
					dialogueBoxes.EmplaceBack(DialogueBox{ Rectangle{ localTopLeft.x + box.x, localTopLeft.y + box.y, width, blackHeight }, SpeakerRole::Inspector });

					height += whiteHeight + blackHeight;
				} while (true);



				return dialogueBoxes;
			}

			static std::string ExtractDialogueString(const cv::Mat& transcript, const Rectangle& box, SpeakerRole speaker)
			{
				const auto textBox = transcript(box);
				if (speaker == SpeakerRole::Inspector)
				{
					return ocr::ImageToString(textBox ^ 255, ocr::GetFontInfo(documents::DocType::Transcript));
				}
				return ocr::ImageToString(textBox, ocr::GetFontInfo(documents::DocType::Transcript));
			}

			std::optional<Transcript> CreateTranscript(const documents::Doc& document)
			{
				Transcript my_transcript{};


				const auto binary = document.PreprocessDocument();
				for (auto& [box, speaker] : GetDialogueBoxes(binary))
				{
					my_transcript.m_entries.EmplaceBack(
						Transcript::Entry{
							ExtractDialogueString(binary, box, speaker),
							speaker
						}
					);
					//cv::rectangle(binary, box.box, cv::Scalar(127));
				}

				//cv::imshow("Transcript", binary);
				//cv::waitKey();

				return my_transcript;
			}

		}  // namespace data
	}  // namespace analysis
}  // namespace paplease
