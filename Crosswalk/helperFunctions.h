#pragma once
#include "line.h"
#include <armadillo>

using namespace arma;

void print_image(cv::Mat image, cv::String name);

void paint_lines(cv::Mat, std::vector<cv::Vec4i>, std::string);

void paint_lines(cv::Mat, std::vector<line::Line>, std::string);

cv::Mat paint_vp(cv::Mat, point::Point, std::string);

std::vector<line::LineEquation> getLineEqVectorFromLineVector(std::vector<line::Line>);
