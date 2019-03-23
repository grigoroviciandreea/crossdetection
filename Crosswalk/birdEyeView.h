#pragma once
#include <opencv2/opencv.hpp>

namespace BEV
{
	class BirdEyeView
	{
	public:
		inline BirdEyeView(cv::Mat img);
		inline BirdEyeView();
		void setImage(cv::Mat img);
		cv::Mat computeBirdEyeView();
		cv::Mat computeBirdEyeViewforImg();
	private:
		cv::Mat m_image;
	};

	inline BirdEyeView::BirdEyeView(cv::Mat img)
	{
		m_image = img;
	}
	inline BirdEyeView::BirdEyeView()
	{
		m_image = 0;
	}
}