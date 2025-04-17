#include "pch.h"
#include "base/analysis/analysis_writer.h"

namespace paplease {
    namespace analysis {

        // Initialize the static member variable
        std::shared_ptr<AnalysisWriter> AnalysisWriter::s_Writer;

        // Initializes the writer by opening a file for writing
        void AnalysisWriter::Init(const std::string& fileName)
        {
            if (!s_Writer)
            {
                // s_Writer = std::make_shared<AnalysisWriter>(fileName);
            }
        }

        // Constructor to handle file initialization
        AnalysisWriter::AnalysisWriter(const std::string& fileName)
        {
            // Open file in output mode (create if doesn't exist)
            m_outputStream.open(fileName, std::ios::out | std::ios::app);  // Open in append mode
            if (!m_outputStream.is_open())
            {
                std::cerr << "Error: Could not open or create the file for writing!" << std::endl;
            }
        }

        // Destructor to close the file when done
        AnalysisWriter::~AnalysisWriter()
        {
            if (m_outputStream.is_open())
            {
                m_outputStream.close();
            }
        }

        // Method to write analysis data to the file
        void AnalysisWriter::WriteToFile(const std::string& data)
        {
            if (m_outputStream.is_open())
            {
                m_outputStream << data << std::endl;
            }
            else
            {
                std::cerr << "Error: Output stream is not open!" << std::endl;
            }
        }

    }  // namespace analysis
}  // namespace paplease
