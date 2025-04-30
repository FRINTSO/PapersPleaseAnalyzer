#include "pch.h"
#include "test/test_faces.h"

#include <array>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#include "paplease/scannable/booth.h"
#include "paplease/scannable/inspection.h"
#include "paplease/documents/data/photo.h"
#include "paplease/documents/doc_type.h"
#include "paplease/documents/doc_data_type.h"
#include "paplease/game_view.h"

namespace test {

    static consteval std::array<std::string_view, 4> GetGameViewFolders()
    {
        return {
            "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images",
            "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\1",
            "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\2",
            "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\3",
        };
    }

    static int GetGameViewFolderEntryCount(const std::string_view& path)
    {
        int count = 0;
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_regular_file())
            {
                count++;
            }
        }
        return count;
    }

    static bool GetNextGameView(paplease::GameView& gameView)
    {
        static constexpr auto gameViewFolders = GetGameViewFolders();

        static size_t currentFolderIndex = 0;
        static std::string_view currentFolder = gameViewFolders[currentFolderIndex];
        static int gameViewsInFolder = GetGameViewFolderEntryCount(currentFolder);
        static int currentGameView = 1;

        if (currentGameView < gameViewsInFolder + 1)  // 1 indexed
        {
            // Read next game view
            std::string path = fmt::format("{}\\game_{}.png", currentFolder, currentGameView);
            paplease::LOG_RAW("[Game view: {}]", path);

            gameView = paplease::GameView(path);
            currentGameView++;
            return true;
        }
        else if (currentFolderIndex + 1 < gameViewFolders.size())
        {
            // Read next game view folder
            currentFolderIndex++;

            currentFolder = gameViewFolders[currentFolderIndex];
            gameViewsInFolder = GetGameViewFolderEntryCount(currentFolder);
            currentGameView = 1;

            // then read next game view
            return GetNextGameView(gameView);
        }
        else
        {
            return false;
        }
    }

    std::vector<paplease::documents::data::Photo> load_faces()
    {
        std::vector<paplease::documents::data::Photo> faces{};

        paplease::GameView view;
        while (GetNextGameView(view))
        {
            // Load booth
            auto boothData = paplease::scannable::ScanBooth(view);
            auto headshotOpt = boothData.ExtractBinaryHeadshot();
            if (headshotOpt)
            {
                faces.push_back({ *headshotOpt });
            }

            // Load documents
            auto inspectionData = paplease::scannable::ScanInspection(view);
            for (const auto& docView : inspectionData.scannedDocuments)
            {
                auto doc = docView.ToDocument(view);
                if (doc.GetDocumentType() == paplease::documents::DocType::Invalid)
                {
                    break;
                }

                auto docData = doc.GetDocumentData();
                auto photoOpt = docData.GetField(paplease::documents::FieldCategory::Photo, false);
                if (!photoOpt)
                    continue;

                const auto& photoField = photoOpt->get();

                if (photoField.Type() == paplease::documents::FieldType::Image)
                {
                    if (const auto& photo = photoField.GetFieldData<paplease::documents::FieldCategory::Photo>(); photo)
                    {
                        faces.push_back(photo->get());
                    }
                }
            }
        }

        return faces;
    }

    void test_faces()
    {
        auto faces = load_faces();
        for (const auto& face : faces)
        {
            cv::imshow("Face", face.m_mat);
            cv::waitKey();
        }
    }

}  // namespace test
