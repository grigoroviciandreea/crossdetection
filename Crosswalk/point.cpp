#include "point.h"

float point::Point::x()
{
	return m_x;
}

float point::Point::y()
{
	return m_y;
}

void point::Point::set_x(float x)
{
	m_x = x;
}

void point::Point::set_y(float y)
{
	m_y = y;
}