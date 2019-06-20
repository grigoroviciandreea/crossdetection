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
	m_a = m_line.pointEnd().y() - m_line.pointStart().y();
	m_b = m_line.pointStart().x() - m_line.pointEnd().x();

	m_c = m_line.pointEnd().x() * m_line.pointStart().y() - m_line.pointEnd().y() * m_line.pointStart().x();
}

double line::LineEquation::getDistanceFromPointToLine(point::Point P)
{
	double distance = 0.0F;
	double absolute_val = 0.0F, sqrt_val = 0.0F;
	absolute_val = std::abs(m_a * P.x() + m_b * P.y() + m_c);
	sqrt_val = std::sqrt(m_a * m_a + m_b * m_b);
	distance = absolute_val / sqrt_val;
	return distance;
}


void line::Line::set_rho_theta(double rho, double theta)
{
	m_rho = rho;
	m_theta = theta;
}

double line::LineEquation::getSlope()
{
	return m_slope;
}

line::Line line::LineEquation::getLine()
{
	return m_line;
}

double line::LineEquation::getParam_a()
{
	return m_a;
}

double line::LineEquation::getParam_b()
{
	return m_b;
}

double line::LineEquation::getParam_c()
{
	return m_c;
}