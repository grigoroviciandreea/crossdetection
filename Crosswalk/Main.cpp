#include "crosswalk.h"
#include "birdEyeView.h"

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
	BEV::BirdEyeView birdEyeView;
	cv::Mat img = birdEyeView.computeBirdEyeViewforImg();
	return img;
}

void ProbHoughTest()
{
	hough::Hough H;
	H.readImage("./crosswalk_images/testC.png");
	cross::Crosswalk C(H.image(), H.probabilisticHoughLines('1'));
	//C.findCrosswalkInImage();
}

void HoughTest()
{
	hough::Hough H;
	H.readImage("./crosswalk_images/testC.png");
	cross::Crosswalk C(H.image(), H.houghLines('1'));
	//C.findCrosswalkInImage();
}

int main()
{	
	ProbHoughTest();
	//HoughTest();
	//ComputeBirdEyeView();
	cv::waitKey(0);
	return 0;
}