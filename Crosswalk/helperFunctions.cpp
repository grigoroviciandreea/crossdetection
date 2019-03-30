#include"helperFunctions.h"

void paint_lines(cv::Mat image, std::vector<cv::Vec4i> lines, std::string name)
{
	cv::Mat result(image);

	if (lines.size() > 0)
	{
		// paint detected lines
		for (size_t i = 0; i < lines.size(); i++)
		{
			cv::Vec4i l = lines[i];
			cv::line(result, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
		}
	}
	cv::namedWindow(name, cv::WINDOW_AUTOSIZE); // Create a window for display.
	imshow(name, result);
}


void paint_vp(cv::Mat image, point::Point vp, std::string name)
{
	// draw a circle to visualize the approximate vanishing point
	if (vp.x() > 0 && vp.x() < image.cols && vp.y() > 0 && vp.y() < image.rows)
		cv::circle(image, cv::Point(vp.x(), vp.y()), 20, cv::Scalar(0, 0, 255), 10);

	cv::imshow(name, image);
}
