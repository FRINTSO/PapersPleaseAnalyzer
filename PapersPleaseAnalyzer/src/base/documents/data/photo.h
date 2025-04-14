#pragma once
#include <opencv2/opencv.hpp>

namespace paplease {
	namespace documents {
		namespace data {

			struct Photo
			{
				cv::Mat m_mat;
				// bool wasPreprocessed;
				
				inline bool IsBinary() const
				{
					return m_mat.channels() == 1;
				}
			};

		}  // namespace data
	}  // namespace documents
}  // namespace paplease
