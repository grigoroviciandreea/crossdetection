#include"crosswalk.h"

void cross::Crosswalk::printLines()
{
	for (int i = 0; i < m_lines.size(); i++)
	{
		std::cout << "Start: " << m_CrossWalkLines[i].pointStart().x() << " " << m_CrossWalkLines[i].pointStart().y() << '\n';
		std::cout << "End: " << m_CrossWalkLines[i].pointEnd().x() << " " << m_CrossWalkLines[i].pointEnd().y() << "\n\n";
	}
  std::cout << '\n';
}

cv::Mat cross::Crosswalk::Vdisparity(cv::Mat ImgToProcess, int max_disp) {
	cv::Size size = cv::Size(max_disp, ImgToProcess.size().height);
	cv::Mat result = cv::Mat::zeros(size, CV_8U);
	for (int i = 0; i < ImgToProcess.size().height; i++)
	{
		for (int j = 0; j < ImgToProcess.size().width; j++)
		{

			int disp = (int)ImgToProcess.at<unsigned char>(i, j);
			if (disp > 0)
				result.at<unsigned __int8>(i, disp)++;
		}
	}
	return result;
}

void cross::Crosswalk::saveLines(std::string path)
{
	std::ofstream outputFile;
	outputFile.open(path);
	for (int i = 0; i < m_lines.size(); i++)
	{
		outputFile << "Start: " << m_CrossWalkLines[i].pointStart().x() << " " << m_CrossWalkLines[i].pointStart().y() << '\n';
		outputFile << "End: " << m_CrossWalkLines[i].pointEnd().x() << " " << m_CrossWalkLines[i].pointEnd().y() << "\n\n";
	}
	outputFile << '\n';
	outputFile.close();
}

std::vector<line::Line> cross::Crosswalk::getCrossWalkLines()
{
	bool isFound = findCrosswalkInImage();
	if (isFound)
		return m_CrossWalkLines;
	else
		return std::vector<line::Line> {0};
}

int cross::Crosswalk::findLinesWithTheSameVP()
{
	int howManyValues = 0;
	std::vector<line::LineEquation>::iterator it;
	for (it = m_lineEcuation.begin(); it != m_lineEcuation.end(); it++) {
		float euqation = it->getParam_a() * m_vanishingPoint.x() + it->getParam_b() * m_vanishingPoint.y() + it->getParam_c();
		float result = 0.0f;
		if (std::abs(euqation - result) < 2.1f)
		{
			m_CrossWalkLines.push_back(it->getLine());
			howManyValues++;
		}
	}
	return howManyValues;
}

bool cross::Crosswalk::findCrosswalkInImage()
{
	bool isFound = false;
	std::multimap<float, line::Line> mapLines;
	std::vector<line::Line> myLinesInMap;
	std::vector<line::LineEquation>::iterator it;
    

	//add lines into map with their slope
	for (it = m_lineEcuation.begin(); it != m_lineEcuation.end(); it++) {
		it->calculateSlope();	
		mapLines.insert(std::pair<float, line::Line>(it->getSlope(), it->getLine()));
	}

	//print map

	std::cout << "map: \n";
	std::multimap<float, line::Line>::iterator itMap = mapLines.begin();
	for (; itMap != mapLines.end(); itMap++) {
		std::cout << itMap->first << " x= " << itMap->second.pointStart().x() << " y= " << itMap->second.pointStart().y() 
			<< ": x= " << itMap->second.pointEnd().x() << " y= " << itMap->second.pointEnd().y() << "\n";
	}

	cv::Mat binary(m_picture);
	cv::cvtColor(binary, binary, cv::COLOR_BGR2GRAY);
	cv::threshold(binary, binary, 100, 255, cv::THRESH_BINARY);

	cv::floodFill(binary, cv::Point(0, 0), Scalar(255));
	std::string imageName = "Threshold";

	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);// Create a window for display.
	imshow(imageName, binary);

	//poate am nevoie la ceva
	cv::Mat v_disp = Vdisparity(binary, 256);
	std::string imageNameDisp = "V-disp";
	cv::namedWindow(imageNameDisp, cv::WINDOW_AUTOSIZE);// Create a window for display.
	imshow(imageNameDisp, v_disp);


	return isFound;
}

void cross::Crosswalk::drawLinesOnImage(char i)
{
	cv::Mat result;
	m_picture.copyTo(result);
	if (m_CrossWalkLines.size() > 0)
	{
		// paint detected lines
		for (size_t i = 0; i < m_lines.size(); i++)
		{
			cv::line(result, cv::Point((int)m_CrossWalkLines[i].pointStart().x(),(int)m_CrossWalkLines[i].pointStart().y()),
				cv::Point((int)m_CrossWalkLines[i].pointEnd().x(), (int)m_CrossWalkLines[i].pointEnd().y()), cv::Scalar(0, 255, 255), 1, cv::LINE_AA);
		}
	}

	std::string imageName = "LinesDrawn" + std::string(1, i);

	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE); // Create a window for display.
	imshow(imageName, result);
}