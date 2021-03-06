#pragma once
#include "line.h"
#include <armadillo>

using namespace arma;

void print_image(cv::Mat image, cv::String name);

void paint_lines(cv::Mat, std::vector<cv::Vec4i>, std::string);

cv::Mat paint_lines_for_bw(cv::Mat image, std::vector<line::Line> lines, std::string name);

void paint_lines(cv::Mat &, std::vector<line::Line>, std::string);

cv::Mat paint_vp(cv::Mat, point::Point, std::string);

std::vector<line::LineEquation> getLineEqVectorFromLineVector(std::vector<line::Line>);

void swapInt(float &x, float &y);

point::Point getVPfromBuff(cv::Mat buff);

cv::Mat applyDilation(cv::Mat input);

cv::Mat applyErosion(cv::Mat input);