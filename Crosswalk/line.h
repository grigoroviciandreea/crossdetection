#pragma once
#include "point.h"

namespace line
{
  class Line
  {
  public:
    inline Line(point::Point, point::Point);
    inline Line();
	point::Point pointStart();
	point::Point pointEnd();
    void set_pointStart(point::Point);
    void set_pointEnd(point::Point);
  private:
    point::Point m_pStart;
	point::Point m_pEnd;
  };

  inline Line::Line(point::Point p1, point::Point p2)
  {
	  m_pStart = p1;
	  m_pEnd = p2;
  }
  inline Line::Line()
  {
	  m_pStart = point::Point(0.0f, 0.0f);
	  m_pStart = point::Point(0.0f, 0.0f);
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
	  float getSlope();
	  float getParam_a();
	  float getParam_b();
	  float getParam_c();
  private:
	  Line m_line;
	  float m_slope;  
	  float m_a;
	  float m_b;
	  float m_c;
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