#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "paplease/common/shape.h"
#include "paplease/types.h"

namespace paplease {

	enum class ViewArea : u8
	{
		BoothView,
		InspectionView,
	};

	class GameView
	{
	public:
		GameView() = default;
		GameView(const std::string& filename);
		explicit GameView(cv::Mat&& image) noexcept
#if IS_DOWNSCALED
			: m_image(ScaleImage(std::move(image), SCALE)),
			m_booth(m_image(BoothRegion)),
			m_inspection(m_image(InspectionRegion))
#else
			: m_image(std::move(image)),
			m_booth(m_image(BoothRegion)),
			m_inspection(m_image(InspectionRegion))
#endif
		{
			assert(m_image.cols == ViewSize.width && m_image.rows == ViewSize.height && "GameView image has to be 1142x672 in size!");
		}

		constexpr const cv::Mat& image() const
		{
			return m_image;
		}
		constexpr cv::Mat& booth()
		{
			return m_booth;
		}
		constexpr const cv::Mat& booth() const
		{
			return m_booth;
		}
		constexpr cv::Mat& inspection()
		{
			return m_inspection;
		}
		constexpr const cv::Mat& inspection() const
		{
			return m_inspection;
		}

		static constexpr ScaledShape ViewSize{ 1142, 672 };
#if IS_DOWNSCALED
		static constexpr Rectangle BoothRegion{ 1, 119, 178, 217 };
		static constexpr Rectangle InspectionRegion{ 179, 119, 392, 217 };
#else
		static constexpr Rectangle BoothRegion{ 1, 237, 356, 434 };
		static constexpr Rectangle InspectionRegion{ 357, 237, 784, 434 };
#endif

	private:
		cv::Mat m_image{};
		cv::Mat m_booth{};
		cv::Mat m_inspection{};
	};

	GameView GetGameView(const std::string& number);
	bool GetNextGameSimView(const std::string& num, GameView& view);

}  // namespace paplease
