#pragma once

#include <fstream>
#include <memory>

namespace paplease {
    namespace analysis {

        class AnalysisWriter
        {
        public:
            static void Init(const std::string& fileName);
            static inline std::shared_ptr<AnalysisWriter>& GetWriter() { return s_Writer; }

            void WriteToFile(const std::string& data);

        private:
            AnalysisWriter(const std::string& fileName);
            ~AnalysisWriter();

            static std::shared_ptr<AnalysisWriter> s_Writer;
            std::ofstream m_outputStream;
        };

#define WRITE_NEW_FILE(name)
#define WRITE_FILE(msg)

    }  // namespace analysis
}  // namespace paplease
