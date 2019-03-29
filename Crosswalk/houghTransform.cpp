#include"houghTransform.h"

void hough::Hough::setImage(cv::Mat image)
{
	m_image = image;
}

std::vector<line::Line> hough::Hough::houghLines(char i) {
	cv::Mat result;
	m_image.copyTo(result);
	std::vector<line::Line> finalLines;
	//calculate Hough lines from image
	std::vector<cv::Vec2f> houghLines;
	cv::Mat houghLinesSrc;
	cv::cvtColor(m_image, houghLinesSrc, CV_BGR2GRAY);

    //houghLinesSrc = sobelResult(i);
	houghLinesSrc = cannyResult(i);

	HoughLines(houghLinesSrc, houghLines, 1, CV_PI / 180, 50);

	if (houghLines.size() > 0)
	{
		// hough coordinates to xy coordinates
		for (size_t i = 0; i < houghLines.size(); i++)
		{
			float rho = houghLines[i][0], theta = houghLines[i][1];
			float pt1x, pt1y, pt2x, pt2y;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			pt1x = (float)(x0 + 1000 * (-b));
			pt1y = (float)(y0 + 1000 * (a));
			pt2x = (float)(x0 - 1000 * (-b));
			pt2y = (float)(y0 - 1000 * (a));

			finalLines.push_back(line::Line(point::Point(pt1x, pt1y), point::Point(pt2x, pt2y)));
		}
		// paint detected lines
		for (size_t i = 0; i < finalLines.size(); i++)
		{
			line::Line l = finalLines[i];
			int p1 = cvRound(l.pointStart().x());
			int p2 = cvRound(l.pointStart().y());
			int p3 = cvRound(l.pointEnd().x());
			int p4 = cvRound(l.pointEnd().y());
			cv::line(result, cv::Point(p1, p2), cv::Point(p3, p4), cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
		}
	}

	std::string imageName = "Hough" + std::string(1, i);

	cv::resize(result, result, cv::Size(), 0.4, 0.4);

	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);// Create a window for display.
	imshow(imageName, result);
	return finalLines;
}

std::vector<line::Line> hough::Hough::probabilisticHoughLines(char i) {
	cv::Mat result;
	m_image.copyTo(result);
	std::vector<line::Line> finalLines;
	//calculate Hough lines from image
	cv::Mat houghLinesSrc;
	
	//I could use Sobel instead!!!
	houghLinesSrc = cannyResult(i);
	//houghLinesSrc = sobelResult(i);
	std::vector<cv::Vec4i> lines;
	HoughLinesP(houghLinesSrc, lines, 1, CV_PI / 180, 10, 20, 160);
	
	finalLines.resize(lines.size());
	if (lines.size() > 0)
	{
		// paint detected lines
		for (size_t i = 0; i < lines.size(); i++)
		{
			cv::Vec4i l = lines[i];
			cv::line(result, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
			finalLines[i].set_pointStart(point::Point((float)l[0],(float) l[1]));
			finalLines[i].set_pointEnd(point::Point((float)l[2], (float)l[3]));
		}
	}

	std::string imageName = "Probabilistic Hough" + std::string(1, i);
	//resize frame to 480x320 cv::resize(image, image, cv::Size(480, 320));
	cv::resize(result, result, cv::Size(), 0.4, 0.4);
	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE); // Create a window for display.
	imshow(imageName, result);
	return finalLines;
}


void  hough::Hough::readImageAndDisplay(cv::Mat image) {
	image = cv::imread("./crosswalk_images/crosswalk1.png", CV_LOAD_IMAGE_UNCHANGED);   // Read the file
	if (!image.data)                              // Check for invalid input
	{
		std::cout << "Could not open or find the image" << std::endl;
	}
	cv::namedWindow("My image", cv::WINDOW_AUTOSIZE);// Create a window for display.
	imshow("My image", image);
}

void  hough::Hough::displayImage() {
	if (!m_image.data)                              // Check for invalid input
	{
		std::cout << "Could not find the image" << std::endl;
	}
	cv::namedWindow("My image", cv::WINDOW_AUTOSIZE);// Create a window for display.
	imshow("My image", m_image);
}

void  hough::Hough::readImage(std::string path) {
	m_image = cv::imread(path, CV_LOAD_IMAGE_UNCHANGED);   // Read the file from the path specified
	if (!m_image.data)                              // Check for invalid input
	{
		std::cout << "Could not open or find the image" << std::endl;
	}
	cv::resize(m_image, m_image, cv::Size(), 0.4, 0.4);
}

cv::Mat hough::Hough::cannyResult(char i) {
	cv::Mat image_canny;
	cv::Mat image_blur;
	cv::blur(m_image, image_blur, cv::Size(5, 5));
	//  use image_blur instead of m_image 

	cv::Canny(image_blur, image_canny, 100, 150, 3);


	std::string imageName = "Canny Result" + std::string(1, i);

	cv::Mat out;

	cv::resize(image_canny, out, cv::Size(), 0.4, 0.4);
	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
	imshow(imageName, out);
	return image_canny;
}


cv::Mat hough::Hough::sobelResult(char i) {
	cv::Mat image_sobel;
	cv::Mat image_blur;
	cv::blur(m_image, image_blur, cv::Size(5, 5));
	//  use image_blur instead of m_image 

	cv::Sobel(image_blur, image_sobel, CV_8UC1, 1, 0, 3);


	std::string imageName = "Sobel Result" + std::string(1, i);

	//cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
	//imshow(imageName, image_sobel);
	return image_sobel;
}

cv::Mat hough::Hough::image() {
	
	return m_image;
}

