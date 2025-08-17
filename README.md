# Papers Please Analyzer 📑

An automated analyzer for the game Papers, Please that catches rule violations and helps you avoid fines.

## About The Project

Papers, Please starts simple, but the rules and documents you have to check quickly become complex. It's easy to miss a small detail, which can lead to hefty in-game penalties. This project, Papers Please Analyzer, was built to solve that problem.

It's a console application that acts as a real-time assistant, scanning the game window multiple times per second to analyze documents and applicant data. It builds a profile on each applicant as you scan their documents, comparing the information to the current day's rules. If a rule is broken or if information is conflicting, the analyzer will immediately flag it, ensuring you don't miss a thing.

## Features

- **Real-time Analysis**: Scans the game window continuously to keep track of all on-screen documents
- **Applicant Profiling**: Builds a detailed profile for each applicant as you scan their passport and other documents
- **Dynamic Rule Checking**: Prompts you to scan the rulebook at the start of each day to understand the current rules
- **Violation Flagging**: Automatically flags any conflicting information or rule violations, helping you avoid mistakes and fines
- **Console-Based Interface**: Runs in a separate console window for easy use alongside the game

## Getting Started

To get the analyzer up and running, you'll need to build the project from its source code. Once built, the executable can be run on any Windows machine with the necessary dependencies.

### Prerequisites

- **Visual Studio**: This project requires Visual Studio to be built
- **A C++ Compiler**: Ensure you have a C++ compiler installed, which is included with Visual Studio

### Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/FRINTSO/PapersPleaseAnalyzer.git
   ```

2. **Open the project in Visual Studio**:
   Navigate to the cloned directory and open the `.sln` file in Visual Studio.

3. **Build the project**:
   Build the project to generate the executable.

## Usage

Using the analyzer is straightforward:

1. **Launch the application**:
   Double-click the generated executable to launch the console window.

2. **Launch Papers, Please**:
   Start the game and begin a new day.

3. **Begin Using**:
   The analyzer will begin prompting you to scan documents as you play. Simply follow the on-screen instructions to get started.

## Built With

- **C++**: The core programming language
- **OpenCV**: Used for image processing and analysis of the game window

## License

This project is not currently licensed.

## Contact

**FRINTSO** - [GitHub Profile](https://github.com/FRINTSO)
