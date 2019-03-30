#pragma once
#include "helperfunctions.h"

namespace hough
{
  class Hough
  {
  public:
    inline Hough(cv::Mat image, std::vector<line::Line> lines);
	inline Hough();
	void setImage(cv::Mat image);
    std::vector<line::Line> houghLines(char i);
	std::vector<line::Line> probabilisticHoughLines(char i);
	void readImageAndDisplay(cv::Mat image);
	void displayImage();
	void readImage(std::string path);
	cv::Mat cannyResult(char i);
	cv::Mat sobelResult(char i);
	cv::Mat image();

  private:
    cv::Mat m_image; 
    std::vector<line::Line> m_lines; 
  };

  inline Hough::Hough(cv::Mat image, std::vector<line::Line> lines)
  {
	m_image = image;
    m_lines.swap(lines);
  }

  inline Hough::Hough()
  {
	  m_image = 0;
	  m_lines.swap(std::vector<line::Line> {0});
  }

}