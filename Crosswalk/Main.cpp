#include "crosswalk.h"
#include "birdEyeView.h"

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


void findCross()
{
	cv::String ending("01.png");
	cv::String path("./crosswalk_images/new/test");
	path += ending;
	cv::Mat img = cv::imread(path);
	int mode = 1; //1 - for Detect with Hough; 2 - for detect clasic 
	VP::vanishingPt obj(img, mode);
	cv::Mat crossImg;

	img.copyTo(crossImg);

	hough::Hough H;
	H.setImage(img);
	
	//asta imi da toate liniile
    std::vector<line::Line> linesVectorHoughProb = H.probabilisticHoughLines('1');
	std::vector<line::Line> linesVectorHough = H.houghLines('1');

	// asta imi da doar liniile pe care le fol pt vp
	//std::vector<line::Line> linesVectorVP = obj.getLines();
	//point::Point vanishingPoint = obj.getVP();
	//std::vector<line::Line> linesVp = obj.getLines();
	//std::vector<line::LineEquation>  lineEqVectorVP = getLineEqVectorFromLineVector(linesVp);
	//paint_lines(img, linesVectorVP, "linesPAintedFinalVP");

	//cross::Crosswalk C(H.image(), linesVp, lineEqVectorVP, vanishingPoint);
	//C.findCrosswalkInImage();

	point::Point vanishingPointClasic = findVP(path, ending);
	std::vector<line::LineEquation>  lineEqVectorHough = getLineEqVectorFromLineVector(linesVectorHough);


    paint_lines(img, linesVectorHough, "linesPAintedFinalHough" + ending);
	cv::Mat out_img = paint_vp(img, obj.getVP(), "VP");

	//print_image(out_img, cv::String("./output_images/trec_VP1.png"));

	cross::Crosswalk C2(H.image(), linesVectorHough, lineEqVectorHough, vanishingPointClasic);
	//C2.findCrosswalkInImage();
	cout << std::endl << C2.findLinesWithTheSameVP();
	std::vector<line::Line> linesVectorCrossWalk = C2.getCrossWalkLines();

	paint_lines(crossImg, linesVectorCrossWalk, "linesPAintedFinalCrosswalk" + ending);
	print_image(crossImg, cv::String("./output_images/trec_crossDetect" + ending));

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

	const int BOUND_BOX_SIZE = 1; //sau sa pun 4
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
				for (int i = -BOUND_BOX_SIZE; i < BOUND_BOX_SIZE; i++) {
					for (int j = -BOUND_BOX_SIZE; j < BOUND_BOX_SIZE; j++) {
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
				for (int i = -BOUND_BOX_SIZE; i < BOUND_BOX_SIZE; i++) {
					for (int j = -BOUND_BOX_SIZE; j < BOUND_BOX_SIZE; j++) {
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
//	findCross();
	//VanishingPoint();
	//cv::destroyAllWindows();
	ProbHoughTest();
	//HoughTest();
	//ComputeBirdEyeView();
//	findVP();
	cv::waitKey(0);
	return 0;
}
