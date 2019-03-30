#pragma once
#include "line.h"
#include <armadillo>

using namespace arma;

void paint_lines(cv::Mat image, std::vector<cv::Vec4i> lines, std::string i);

void paint_vp(cv::Mat image, point::Point vp, std::string name);

