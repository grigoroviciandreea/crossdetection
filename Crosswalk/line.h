#pragma once
#include "point.h"

namespace line
{
  class Line
  {
  public:
	inline Line::Line(point::Point p1, point::Point p2, double rho, double theta);
    inline Line(point::Point, point::Point);
    inline Line();
	point::Point pointStart();
	point::Point pointEnd();
    void set_pointStart(point::Point);
    void set_pointEnd(point::Point);
	void set_rho_theta(double rho, double theta);
  private:
    point::Point m_pStart;
	point::Point m_pEnd;
	double m_rho;   //distance
	double m_theta; //tangent
  };

  inline Line::Line(point::Point p1, point::Point p2)
  {
	  m_pStart = p1;
	  m_pEnd = p2;
	  m_rho = 0.0F;
	  m_theta = 0.0F;
  }

  inline Line::Line(point::Point p1, point::Point p2, double rho, double theta)
  {
	  m_pStart = p1;
	  m_pEnd = p2;
	  m_rho = rho;
	  m_theta = theta;
  }

  inline Line::Line()
  {
	  m_pStart = point::Point(0.0f, 0.0f);
	  m_pStart = point::Point(0.0f, 0.0f);
	  m_rho = 0.0F;
	  m_theta = 0.0F;
  }

  class LineEquation 
  {
  public:
	  inline LineEquation();
	  inline LineEquation(Line line);
	  void operator =(const Line line)
	  {
		  m_line = line;
	  }
	  void calculateSlope();  
	  void calculateEcuationParameters();
	  Line getLine();
	  double getSlope();
	  double getParam_a();
	  double getParam_b();
	  double getParam_c();
	  double getDistanceFromPointToLine(point::Point);
  private:
	  Line m_line;
	  double m_slope;
	  double m_a;
	  double m_b;
	  double m_c;
  };

  inline LineEquation::LineEquation(Line line)
  {
	  m_line = line;

	  calculateSlope();
	  calculateEcuationParameters();
  }

  inline LineEquation::LineEquation()
  {
	  m_line = Line(point::Point(0,0),point::Point(0,0));
  }
}