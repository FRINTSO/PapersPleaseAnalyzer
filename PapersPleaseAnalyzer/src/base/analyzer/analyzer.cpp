#include "pch.h"
#include "base/analyzer/analyzer.h"

#include <opencv2/opencv.hpp>



Analysis Analyze(const GameView& gameView) {
	// Get Game state
	// Find what documents are present
	// Get all documents
	// Validate document against itself and game state (exp data, seal, etc)
	// Add Document in comparision que, and compare same fields of all documents against each other
	cv::imshow("Inspection Area", gameView.inspection);
	cv::imshow("Booth Area", gameView.booth.mat);

	cv::waitKey();

	return { true };
}