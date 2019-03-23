#include"line.h"

point::Point line::Line::pointStart()
{
  return m_pStart;
}

point::Point line::Line::pointEnd()
{
  return m_pEnd;
}

void line::Line::set_pointStart(point::Point pointStart)
{
  m_pStart = pointStart;
}

void line::Line::set_pointEnd(point::Point pointEnd)
{
  m_pEnd = pointEnd;
}

void line::LineEquation::calculateSlope()
{
	m_slope = (m_line.pointEnd().y() - m_line.pointStart().y()) /
		(m_line.pointEnd().x() - m_line.pointStart().x());
}

void line::LineEquation::calculateEcuationParameters()
{
	m_a = m_line.pointStart().x() - m_line.pointEnd().x();
	m_b = m_line.pointEnd().y() - m_line.pointStart().y();
	m_c = m_line.pointStart().x() * m_line.pointEnd().x() - m_line.pointStart().x() * m_line.pointStart().x() +
		m_line.pointStart().y() * m_line.pointStart().y() - m_line.pointStart().y() * m_line.pointEnd().y();
}

float line::LineEquation::getSlope()
{
	return m_slope;
}

line::Line line::LineEquation::getLine()
{
	return m_line;
}

float line::LineEquation::getParam_a()
{
	return m_a;
}

float line::LineEquation::getParam_b()
{
	return m_b;
}

float line::LineEquation::getParam_c()
{
	return m_c;
}