#include"helperFunctions.h"

void print_image(cv::Mat image, cv::String name)
{
	cv::imwrite(name, image);
}

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
	cv::namedWindow(name, cv::WINDOW_NORMAL); // Create a window for display.
	cv::resizeWindow(name, 432, 768);
	imshow(name, result);
}

void paint_lines(cv::Mat image, std::vector<line::Line> lines, std::string name)
{
	cv::Mat result(image);

	if (lines.size() > 0)
	{
		// paint detected lines
		for (size_t i = 0; i < lines.size(); i++)
		{
			cv::line(result, cv::Point(lines[i].pointStart().x(), lines[i].pointStart().y()), cv::Point(lines[i].pointEnd().x(), lines[i].pointEnd().y()), cv::Scalar(0, 150, 255), 1, cv::LINE_AA);
		}
	}
	cv::namedWindow(name, cv::WINDOW_NORMAL); // Create a window for display.
	cv::resizeWindow(name, 432, 768);
	imshow(name, result);
}

cv::Mat paint_vp(cv::Mat image, point::Point vp, std::string name)
{
	// draw a circle to visualize the approximate vanishing point
	if (vp.x() > 0 && vp.x() < image.cols && vp.y() > 0 && vp.y() < image.rows)
		cv::circle(image, cv::Point(vp.x(), vp.y()), 5, cv::Scalar(0, 0, 255), 10);
	
	cv::namedWindow(name, cv::WINDOW_NORMAL); // Create a window for display.
	cv::resizeWindow(name, 432, 768);
	cv::imshow(name, image);

	return image;
}

std::vector<line::LineEquation> getLineEqVectorFromLineVector(std::vector<line::Line> lineVector)
{
	std::vector<line::LineEquation> lineEqVector;
	if (lineVector.size() > 0)
	{
		// paint detected lines
		for (size_t i = 0; i < lineVector.size(); i++)
		{
			line::LineEquation eq(lineVector[i]);
			lineEqVector.push_back(eq);
		}
	}
	return lineEqVector;
}

void swapInt(float &x, float &y)
{
	float aux = x;
	x = y;
	y = aux;
}

point::Point getVPfromBuff(cv::Mat buff)
{
	int max = 0;
	point::Point P;
	for (int x = 0; x < buff.size().height; x++)
	{
		for (int y = 0; y < buff.size().width; y++)
		{
			if (max < buff.at<unsigned __int8>(x, y))
			{
				P.set_x(y);
				P.set_y(x);
				max = buff.at<unsigned __int8>(x, y);
			}
		}
	}
	return P;
}