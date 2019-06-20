#include "crosswalk.h"
#include "birdEyeView.h"
#include <chrono>

point::Point findVP(cv::String path, cv::String ending);

bool saveLinesForManyImages()
{
	//while(read from a file path ->> more images)  -> make sure you have the images savede in a folder inside crosswalk_images
	// for each one do a File.csv with lines
	cv::String path("./crosswalk_images/*.png"); //select only jpg
	std::vector<cv::String> fn;
	std::vector<cv::Mat> data;
	cv::glob(path, fn, true); // recurse
	for (size_t k = 0; k<fn.size(); ++k)
	{
	 cv::Mat im = cv::imread(fn[k]);
	 if (im.empty()) continue; //only proceed if sucsessful

	// //process the image -> add it in a vector to use it later --- etc
	 data.push_back(im);
	}
	char i = '1';
	for (size_t k = 0; k < data.size(); ++k)
	{
	 std::string imageName = "Image" + std::string(1,i);
	 cv::imshow(imageName, data[k]);
	 i++;
	}
	i = '1';
	for (size_t k = 0; k < data.size(); ++k)
	{
	 std::string csvLocation = "./Files/Lines/cvsPicture" + std::string(1, i) + ".csv";
	 hough::Hough H;
	 H.setImage(data[k]);
	 cross::Crosswalk C(H.image(), H.probabilisticHoughLines(i));
	 C.saveLines(csvLocation);
	 C.drawLinesOnImage(i);
	 i++;
	}
	return true;
}

cv::Mat ComputeBirdEyeView()
{
	//http://answers.opencv.org/question/174548/inverse-perspective-mapping-ipm-on-the-capture-from-camera-feed/
	cv::String path("./crosswalk_images/trec3.png");
	cv::Mat img = cv::imread(path);
	BEV::BirdEyeView birdEyeView(img);
	cv::Mat output = birdEyeView.computeBirdEyeViewforImg();

	print_image(output, cv::String("./output_images/trec3_BEV.png"));
	return output;
}

void ProbHoughTest()
{
	hough::Hough H;
	H.readImage("./crosswalk_images/new/test01.png");
	cross::Crosswalk C(H.image(), H.probabilisticHoughLines('1'));
	//C.findParallelLinesInImage();
}

void HoughTest()
{
	hough::Hough H;
	H.readImage("./crosswalk_images/new/test1.png");
	//std::vector<line::Line> linesVectorHough  = H.houghLines('1');
	cross::Crosswalk C(H.image(), H.houghLines('1'));
	C.findParallelLinesInImage();
}

void VanishingPoint()
{
	cv::String path("./crosswalk_images/trec3.png");
	cv::Mat img = cv::imread(path);
	int mode = 1; //1 - for Detect with Hough; 2 - for detect clasic 
	VP::vanishingPt obj(img, mode);
	point::Point p = obj.getVP();
	cout << "\n\nResult:\n" << p.x() << "," << p.y() << "\nError:";
}

void drawPoly(std::vector<line::Line>, cv::Mat image);

void fct(std::vector<line::Line> lines, cv::Mat image);
void drawPolyFake(std::vector<line::Line> lines, cv::Mat image);

void findCross()
{
	cv::String ending("01.png");
	cv::String path("./crosswalk_images/new/test");
	path += ending;
	cv::Mat img = cv::imread(path);
	int mode = 1; //1 - for Detect with Hough; 2 - for detect clasic 
	VP::vanishingPt obj(img, mode);
	cv::Mat crossImg;

	const clock_t begin_time = std::clock();
	
	auto t_start = std::chrono::high_resolution_clock::now();

	img.copyTo(crossImg);

	hough::Hough H;
	H.setImage(img);
	
	//asta imi da toate liniile
    
	std::vector<line::Line> linesVectorHough = H.houghLines('1');
	std::vector<line::LineEquation>  lineEqVectorHough = getLineEqVectorFromLineVector(linesVectorHough);

	point::Point vanishingPointClasic = findVP(path, ending);

	// asta imi da doar liniile pe care le fol pt vp
	//std::vector<line::Line> linesVectorVP = obj.getLines();
	//point::Point vanishingPoint = obj.getVP();
	//std::vector<line::Line> linesVp = obj.getLines();
	//std::vector<line::LineEquation>  lineEqVectorVP = getLineEqVectorFromLineVector(linesVp);
	//paint_lines(img, linesVectorVP, "linesPAintedFinalVP");

	//cross::Crosswalk C(H.image(), linesVp, lineEqVectorVP, vanishingPoint);
	//C.findCrosswalkInImage();

	

	std::vector<line::Line> linesVectorHoughProb = H.probabilisticHoughLines('1');
	std::vector<line::LineEquation>  lineEqVectorHoughProb = getLineEqVectorFromLineVector(linesVectorHoughProb);



    paint_lines(img, linesVectorHough, "linesPAintedFinalHough" + ending);
	cv::Mat out_img = paint_vp(img, obj.getVP(), "VP");

	//print_image(out_img, cv::String("./output_images/trec_VP1.png"));

	cross::Crosswalk C2(H.image(), linesVectorHoughProb, lineEqVectorHoughProb, vanishingPointClasic);
	//C2.findCrosswalkInImage();
	cout << std::endl << C2.findLinesWithTheSameVP();
	std::vector<line::Line> linesVectorCrossWalk = C2.getCrossWalkLines();

	std::cout << "\nCPU time in ms: " << 1000.0 * float(std::clock() - begin_time) / CLOCKS_PER_SEC << endl;
	auto t_end = std::chrono::high_resolution_clock::now();
	std::cout << "\nReal time in ms: " << std::chrono::duration<double, std::milli>(t_end - t_start).count() << endl;

	paint_lines(crossImg, linesVectorCrossWalk, "linesPAintedFinalCrosswalk" + ending);
	print_image(crossImg, cv::String("./output_images/trec_crossDetect" + ending));

	drawPolyFake(linesVectorCrossWalk, crossImg);
	cv::namedWindow("aaa", cv::WINDOW_NORMAL); // Create a window for display.
	cv::resizeWindow("aaa", 432, 768);
	imshow("aaa", crossImg);

}

void drawPolyFake(std::vector<line::Line> lines, cv::Mat image)
{
	Point points[1][4];
	points[0][0] = Point(218, 1134);
	points[0][3] = Point(2, 1268);
	points[0][1] = Point(784, 1135);
	points[0][2] = Point(1022, 1338);
	const Point* ppt[1] = { points[0] };
	int npt[] = { 4 };
	cv::fillPoly(image,
		ppt,
		npt,
		1,
		Scalar(0, 0, 255),
		LINE_8);
}

void fct(std::vector<line::Line> lines, cv::Mat image)
{

	int size = lines.size();
	Point points[1][30];

	float min_x = lines.at(0).pointStart().x();
	float max_x = lines.at(0).pointStart().x();
	int i = 0;
	std::vector<line::Line>::iterator it;
	for (it = lines.begin(); it != lines.end(); it++, i += 2)
	{
		if (i < 29)
		{
			if (it->pointStart().y() > it->pointEnd().y())
			{
				points[0][i] = Point(it->pointStart().x(), it->pointStart().y());
				points[0][i + 1] = Point(it->pointEnd().x(), it->pointEnd().y());
			}
			else
			{
				points[0][i+1] = Point(it->pointStart().x(), it->pointStart().y());
				points[0][i] = Point(it->pointEnd().x(), it->pointEnd().y());
			}
		}
	}


	const Point* ppt[1] = { points[0] };
	int npt[] = { 30 };
	cv::fillPoly(image,
		ppt,
		npt,
		1,
		Scalar(0, 0, 255),
		LINE_8);
}

void drawPoly(std::vector<line::Line> lines, cv::Mat image)
{
	Point points[1][4];
	points[0][0] = Point(0, 0);
	points[0][1] = Point(0, 0);
	points[0][2] = Point(0, 0);
	points[0][3] = Point(0, 0);

	float min_x = lines.at(0).pointStart().x();
	float max_x = lines.at(0).pointStart().x();

	std::vector<line::Line>::iterator it;
	for (it = lines.begin(); it != lines.end(); it++)
	{
		if (it->pointStart().x() >= max_x)
		{
			max_x = it->pointStart().x();
			if (it->pointStart().y() > it->pointEnd().y())
			{
				points[0][2] = Point(it->pointStart().x(), it->pointStart().y());
				points[0][3] = Point(it->pointEnd().x(), it->pointEnd().y());
			}
			else
			{
				points[0][3] = Point(it->pointStart().x(), it->pointStart().y());
				points[0][2] = Point(it->pointEnd().x(), it->pointEnd().y());
			}
			
		}

		if (it->pointStart().x() <= min_x)
		{
			min_x = it->pointStart().x();
			if (it->pointStart().y() > it->pointEnd().y())
			{
				points[0][0] = Point(it->pointStart().x(), it->pointStart().y());
				points[0][1] = Point(it->pointEnd().x(), it->pointEnd().y());
			}
			else
			{
				points[0][1] = Point(it->pointStart().x(), it->pointStart().y());
				points[0][0] = Point(it->pointEnd().x(), it->pointEnd().y());
			}
		}
	}
	const Point* ppt[1] = { points[0] };
	int npt[] = { 4 };
	cv::fillPoly(image,
		ppt,
		npt,
		1,
		Scalar(0, 0, 255),
		LINE_8);
}

point::Point findVP(cv::String path, cv::String ending)
{
	cv::Mat img = cv::imread(path);

	hough::Hough H;
	H.setImage(img);

	std::vector<line::Line> linesVectorHough = H.houghLines('1');

	cv::Size size = cv::Size(img.size().width, img.size().height); //posibil sa fie invers, intai height apoi width
	cv::Mat buff = cv::Mat::zeros(size, CV_8U);

	cv::Mat buffBW = cv::Mat::zeros(size, CV_8U);

	const int BOUND_BOX_SIZE = 2; //sau sa pun 3,4
	std::vector<line::Line>::iterator it;
	//for each line 
	for (it = linesVectorHough.begin(); it != linesVectorHough.end(); it++) {
		double theta = it->get_theta();
		double rho = it->get_rho();

		//if direction is positive or negative
		if (abs(tan(theta)) > 1)
		{
			for (int y = 0; y < img.size().height; y++)
			{
				int x = (rho - y * sin(theta)) / cos(theta); //x = f(y)
				//if (x > 0 && y > 0 && x < img.size().width && y < img.size().height)
				//	buff.at<unsigned __int8>(y, x) += 10 ;

				//increment buffer around pixel in bounding box
				for (int i = -BOUND_BOX_SIZE; i <= BOUND_BOX_SIZE; i++) {
					for (int j = -BOUND_BOX_SIZE; j <= BOUND_BOX_SIZE; j++) {
						if ((x + i) < img.size().width && (y + j) < img.size().height &&
							(x + i) > 0 && (y + j) > 0)
						{
							if (buff.at<unsigned __int8>(y + j, x + i) < 255)
							{
								buff.at<unsigned __int8>(y + j, x + i)++;
								buffBW.at<unsigned __int8>(y + j, x + i) =255;
							}
						}
					}
				}
			}
		}
		else
		{
			for (int x = 0; x < img.size().width; x++)
			{
				int y = (rho - x* cos(theta)) / sin(theta); //y=f(x)		
				//if(x > 0 && y > 0 && x < img.size().width && y < img.size().height)
				//	buff.at<unsigned __int8>(y, x) += 10;

				//increment buffer around pixel in bounding bsox
				for (int i = -BOUND_BOX_SIZE; i <= BOUND_BOX_SIZE; i++) {
					for (int j = -BOUND_BOX_SIZE; j <= BOUND_BOX_SIZE; j++) {
						if((x + i) < img.size().width && (y + j) < img.size().height &&
						   (x + i) > 0 && (y + j) > 0)
						{
							if (buff.at<unsigned __int8>(y + j, x + i) < 255)
							{
								buff.at<unsigned __int8>(y + j, x + i)++;
								buffBW.at<unsigned __int8>(y + j, x + i) = 255;
							}
						}
					}
				}
			}
		}
	}
	cv::namedWindow("buffer", cv::WINDOW_NORMAL); // Create a window for display.
	cv::resizeWindow("buffer", 432, 768);
	imshow("buffer", buff);

	cv::namedWindow("bufferBW", cv::WINDOW_NORMAL); // Create a window for display.
	cv::resizeWindow("bufferBW", 432, 768);
	imshow("bufferBW", buffBW);
	
	print_image(buff, cv::String("./output_images/buff" + ending));
	print_image(buffBW, cv::String("./output_images/buffBW" + ending));

	point::Point vp = getVPfromBuff(buff);
	std::cout << "VP:" << vp.x() << ", " << vp.y();


	cv::Mat out_img = paint_vp(img, vp, "VP_original" + ending);
	print_image(out_img, cv::String("./output_images/vp" + ending));

	return vp;
}



int main()
{
	findCross();
	//VanishingPoint();
	//cv::destroyAllWindows();
	//ProbHoughTest();
	//HoughTest();
	//ComputeBirdEyeView();
//	findVP();
	cv::waitKey(0);
	return 0;
}
