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
	std::vector<cv::Vec4i> lines;

	cv::Mat houghLinesSrc;
	cv::cvtColor(m_image, houghLinesSrc, CV_BGR2GRAY);

    //houghLinesSrc = sobelResult(i);
	houghLinesSrc = cannyResult(i);

	HoughLines(houghLinesSrc, houghLines, 30, CV_PI / 180, 30);

	lines.resize(houghLines.size());
	if (houghLines.size() > 0)
	{
		// hough coordinates to xy coordinates
		for (size_t i = 0; i < houghLines.size(); i++)
		{
			double rho = houghLines[i][0], theta = houghLines[i][1];
			double pt1x, pt1y, pt2x, pt2y;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			pt1x = x0 + 1000 * (-b);
			pt1y = y0 + 1000 * (a);
			pt2x = x0 - 1000 * (-b);
			pt2y = y0 - 1000 * (a);
			lines[i][0] = cvRound(pt1x);
			lines[i][1] = cvRound(pt1y);
			lines[i][2] = cvRound(pt2x);
			lines[i][3] = cvRound(pt2y);
			finalLines.push_back(line::Line(point::Point(pt1x, pt1y), point::Point(pt2x, pt2y), rho, theta));
		}
	}

	paint_lines(result, lines, "Hough");
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
	HoughLinesP(houghLinesSrc, lines, 1, CV_PI / 180, 10, 20, 70);
	
	finalLines.resize(lines.size());
	if (lines.size() > 0)
	{
		for (size_t i = 0; i < lines.size(); i++)
		{
			cv::Vec4i l = lines[i];
			if ((float)l[1] < (float)l[3])
			{
				finalLines[i].set_pointStart(point::Point((float)l[0], (float)l[1]));
				finalLines[i].set_pointEnd(point::Point((float)l[2], (float)l[3]));
			}
			else
			{ 
				finalLines[i].set_pointStart(point::Point((float)l[2], (float)l[3]));
				finalLines[i].set_pointEnd(point::Point((float)l[0], (float)l[1]));
			}
		}
	}

	paint_lines(result, finalLines, "HoughProb");
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
}

cv::Mat hough::Hough::cannyResult(char i) {
	cv::Mat image_canny;
	cv::Mat image_blur;
	cv::blur(m_image, image_blur, cv::Size(5, 5));
	//  use image_blur instead of m_image 

	cv::Canny(image_blur, image_canny, 30, 50, 3);

	for (int x = 0; x < (image_canny.size().height - 400); x++)
	{
		for (int y = 0; y < image_canny.size().width; y++)
		{
			image_canny.at<uchar>(cv::Point(y, x)) = 0;

		}
	}

	std::string imageName = "Canny Result" + std::string(1, i);

	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
	imshow(imageName, image_canny);
	return image_canny;
}


cv::Mat hough::Hough::sobelResult(char i) {
	cv::Mat image_sobel;
	cv::Mat image_blur;
	cv::blur(m_image, image_blur, cv::Size(5, 5));
	//  use image_blur instead of m_image 

	cv::Sobel(image_blur, image_sobel, CV_8UC1, 1, 0, 3);

	std::string imageName = "Sobel Result" + std::string(1, i);

	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
	imshow(imageName, image_sobel);
	return image_sobel;
}

cv::Mat hough::Hough::image() {
	
	return m_image;
}

