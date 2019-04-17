#include"birdEyeView.h"

#define PI 3.1415926

void BEV::BirdEyeView::setImage(cv::Mat img)
{
	m_image = img;
}


cv::Mat BEV::BirdEyeView::computeBirdEyeView()
{
	cv::Mat BEV;
	cv::Mat src = cv::imread("./crosswalk_images/crosswalk1.png");

	cv::Point2f src_vertices[4];
	src_vertices[0] = cv::Point(11, 424);
	src_vertices[1] = cv::Point(940, 414);
	src_vertices[2] = cv::Point(950, 663);
	src_vertices[3] = cv::Point(20, 670);

	cv::Point2f dst_vertices[4];
	dst_vertices[0] = cv::Point(0, 0);
	dst_vertices[1] = cv::Point(src.size().width, 0);
	dst_vertices[2] = cv::Point(src.size().width, src.size().height);
	dst_vertices[3] = cv::Point(0, src.size().height);

	cv::Mat M = cv::getPerspectiveTransform(src_vertices, dst_vertices);
	cv::Mat dst(src.size().height, src.size().width, CV_8UC3);
	warpPerspective(src, dst, M, dst.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

	cv::imshow("src", src);
	cv::imshow("dst", dst);

	return BEV;
}

cv::Mat BEV::BirdEyeView::computeBirdEyeViewforImg()
{
	cv::Mat src = this->m_image;
	//alpha pitch, gamma roll, azimuth beta --> incercarile mele de atunci
	//pe net zice: roll = gamma, pitch - beta, azimuth alpha (sau alta sursa zice azimuth gamma)

	//ultim data si o mers bine: --- alpha = roll = 18 --- beta = pitch = -83 --- gamma = azim = 107 pt trec3
	int alpha_ = 18, beta_ = 83, gamma_ = 97;
	int f_ = 426; //4.26 mm inainte: 560
    int dist_ = 600; 

	double w = (double)src.size().width, h = (double)src.size().height;
	double focalLength, dist, alpha, beta, gamma;

	//transform from degrees in radians
	alpha = ((double)alpha_ - 90) * PI / 180;
	beta =((double)beta_ - 90) * PI / 180;
	gamma = ((double)gamma_ - 90) * PI / 180;
	focalLength = (double)f_;
	dist = (double)dist_;

	//ungiuri rotatie dintre sistemul de referinta al telefonului fata de sistemul lumii (z de-a lungul drumului, x laterl, y sus)


	//aplicatii ce masoara unghiurile: android senzors multitools ceva de genu. pt rigidazare cand iau secventa. rotation angles (fata de ce sistem de referinta se masoara unghiurile)
	//cel mai rau caz scad 90 - ungiul de la teelfon

	// Projecion matrix 2D -> 3D
	cv::Mat A1 = (cv::Mat_<float>(4, 3) <<
		1, 0, -w / 2,
		0, 1, -h / 2,
		0, 0, 0,
		0, 0, 1);


	// Rotation matrices Rx, Ry, Rz

	cv::Mat RX = (cv::Mat_<float>(4, 4) <<
		1, 0, 0, 0,
		0, cos(alpha), -sin(alpha), 0,
		0, sin(alpha), cos(alpha), 0,
		0, 0, 0, 1);

	cv::Mat RY = (cv::Mat_<float>(4, 4) <<
		cos(beta), 0, -sin(beta), 0,
		0, 1, 0, 0,
		sin(beta), 0, cos(beta), 0,
		0, 0, 0, 1);

	cv::Mat RZ = (cv::Mat_<float>(4, 4) <<
		cos(gamma), -sin(gamma), 0, 0,
		sin(gamma), cos(gamma), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);


	// R - rotation matrix
	cv::Mat R = RX * RY * RZ;

	// T - translation matrix
	cv::Mat T = (cv::Mat_<float>(4, 4) <<
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, dist,
		0, 0, 0, 1);

	// K - intrinsic matrix 
	cv::Mat K = (cv::Mat_<float>(3, 4) <<
		focalLength, 0, w / 2, 0,
		0, focalLength, h / 2, 0,
		0, 0, 1, 0
		);


	cv::Mat M = K * (T * (R * A1));


	cv::Mat BEV(src.size().height, src.size().width, CV_8UC3);
	warpPerspective(src, BEV, M, BEV.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

	cv::imshow("src", src);
	cv::imshow("dst", BEV);

	return BEV;
}
