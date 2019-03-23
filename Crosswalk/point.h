#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <windows.h>

namespace point
{
	class Point
	{
	public:
		inline Point(float x, float y);
		inline Point();
		float x();
		float y();
		void set_x(float x);
		void set_y(float y);
	private:
		float m_x;
		float m_y;
	};

	inline Point::Point(float x, float y)
	{
		m_x = x;
		m_y = y;
	}
	inline Point::Point()
	{
		m_x = 0.0f;
		m_y = 0.0f;
	}
}
