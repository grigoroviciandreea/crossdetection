#pragma once
#include "vanishingPoint.h"

///iau serie de linii hough din imagine (gasite cu ajutorul clasei hough, ce indeplinesc anumite conditii => crosswalk
namespace cross
{
  class Crosswalk
  {
  public:
	inline Crosswalk();
    inline Crosswalk(cv::Mat picture, std::vector<line::Line> lines);
	inline Crosswalk(cv::Mat picture, std::vector<line::Line> lines, std::vector<line::LineEquation> m_lineEcuation);
    void printLines();
	cv::Mat Vdisparity(cv::Mat ImgToProcess, int max_disp);
	void saveLines(std::string path);
	void drawLinesOnImage(char i);
	bool findCrosswalkInImage();
	std::vector<line::Line> getCrossWalkLines();
  private:
    cv::Mat m_picture; //type image
    std::vector<line::Line> m_lines; //type Line
	std::vector<line::LineEquation> m_lineEcuation; //type LineEq

	std::vector<line::Line> m_CrossWalkLines; //basically what I need
  };

  inline Crosswalk::Crosswalk(cv::Mat picture, std::vector<line::Line> lines, std::vector<line::LineEquation> lineEcuation)
  {
	  m_picture = picture;
	  m_lines.swap(lines);
	  m_lineEcuation.swap(lineEcuation);
	  m_CrossWalkLines.swap(lines);
  }
  inline Crosswalk::Crosswalk(cv::Mat picture, std::vector<line::Line> lines)
  {
    m_picture = picture;
    m_lines.swap(lines);
	std::vector<line::Line>::iterator itLine;
	for ( itLine = m_lines.begin(); itLine != m_lines.end(); itLine++) {
		m_lineEcuation.push_back(line::LineEquation(*itLine));
	}
	m_CrossWalkLines.swap(lines);
  }
  inline Crosswalk::Crosswalk()
  {
	  m_picture = 0;
	  m_lines.swap(std::vector<line::Line> {0});
	  m_lineEcuation.swap(std::vector<line::LineEquation> {0});
	  m_CrossWalkLines.swap(std::vector<line::Line> {0});
  }
}