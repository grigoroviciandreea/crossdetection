#pragma once

#include "houghTransform.h"
#include "helperFunctions.h"

using namespace cv;
using namespace std;

namespace VP
{

	class vanishingPt
	{
	public:
		vanishingPt()
		{
			cv::namedWindow("win", 1);
			cv::namedWindow("Lines", 1);

			int flag = 0;

			image = cv::imread("./crosswalk_images/testC.png", CV_LOAD_IMAGE_UNCHANGED);   // Read the file
			image.copyTo(paint_lines_img);

			// define minimum length requirement for any line
			minlength = static_cast<int>(image.cols * image.cols * 0.001F);

			cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);


			//equalize histogram
			cv::equalizeHist(image, image);

			//initialize the line segment matrix in format y = m*x + c	
			init(image);

			//draw lines on image and display
			makeLines(flag);

			//approximate vanishing point
			eval();
		}

		point::Point getVP()
		{
			point::Point vp;
			vp.set_x(soln(0, 0));
			vp.set_y(soln(1, 0));
			
			return vp;
		}
	private:
		cv::Mat image;
		cv::Mat paint_lines_img;

		vector< vector<int> > points;
		mat A, b;
		mat Atemp, btemp, res, error, soln;

		//store slope (m) and y-intercept (c) of each lines
		float m, c;

		//store minimum length for lines to be considered while estimating vanishing point
		int minlength;

		//store (x1, y1) and (x2, y2) endpoints for each line segment
		vector<cv::Vec4i> lines_std;

		//to store intermediate errors
		double temperr;

		void init(cv::Mat image)
		{
			//temporary vector for intermediate storage
			vector<int> temp;

			//create OpenCV object for line segment detection
			cv::Ptr<cv::LineSegmentDetector> ls = cv::createLineSegmentDetector(cv::LSD_REFINE_STD);

			//initialize
			lines_std.clear();

			//detect lines in image and store in linse_std
			//store (x1, y1) and (x2, y2) endpoints for each line segment
			ls->detect(image, lines_std);

			for (int i = 0; i<lines_std.size(); i++)
			{
				//ignore if almost vertical
				if (abs(lines_std[i][0] - lines_std[i][2]) < 10 || abs(lines_std[i][1] - lines_std[i][3]) < 10) //check if almost vertical
					continue;
				//ignore shorter lines (x1-x2)^2 + (y2-y1)^2 < minlength
				if (((lines_std[i][0] - lines_std[i][2])*(lines_std[i][0] - lines_std[i][2]) + (lines_std[i][1] - lines_std[i][3])*(lines_std[i][1] - lines_std[i][3])) < minlength)
					continue;

				//store valid lines' endpoints for calculations
				for (int j = 0; j<4; j++)
				{
					temp.push_back(lines_std[i][j]);
				}

				points.push_back(temp);
				temp.clear();
			}
			cout<<"Detected:"<<lines_std.size()<<endl;
			cout<<"Filtered:"<<points.size()<<endl;
			paint_lines(paint_lines_img, lines_std, "PaintLinesVP");
		}

		void makeLines(int flag)
		{
			// to solve Ax = b for x
			A = zeros<mat>(points.size(), 2);
			b = zeros<mat>(points.size(), 1);

			//convert given end-points of line segment into a*x + b*y = c format for calculations
			//do for each line segment detected
			for (int i = 0; i<points.size(); i++)
			{

				A(i, 0) = -(points[i][3] - points[i][1]);			//-(y2-y1)
				A(i, 1) = (points[i][2] - points[i][0]);				//x2-x1
				b(i, 0) = A(i, 0)*points[i][0] + A(i, 1)*points[i][1];	//-(y2-y1)*x1 + (x2-x1)*y1
			}
		}

		//estimate the vanishing point
		void eval()
		{
			//stores the estimated co-ordinates of the vanishing point with respect to the image
			soln = zeros<mat>(2, 1);

			//initialize error
			double err = 9999999999;

			//calculate point of intersection of every pair of lines and
			//find the sum of distance from all other lines
			//select the point which has the minimum sum of distance
			for (int i = 0; i<points.size(); i++)
			{
				for (int j = 0; j<points.size(); j++)
				{
					if (i >= j)
						continue;

					//armadillo vector
					uvec indices;

					//store indices of lines to be used for calculation
					indices << i << j;

					//extract the rows with indices specified in uvec indices
					//stores the ith and jth row of matrices A and b into Atemp and btemp respectively
					//hence creating a 2x2 matrix for calculating point of intersection
					Atemp = A.rows(indices);
					btemp = b.rows(indices);

					//if lines are parallel then skip
					if (arma::rank(Atemp) != 2)
						continue;

					//solves for 'x' in A*x = b
					res = calc(Atemp, btemp);


					if (res.n_rows == 0 || res.n_cols == 0)
						continue;

					// calculate error assuming perfect intersection is 
					error = A*res - b;

					//reduce size of error
					error = error / 1000;

					// to store intermediate error values
					temperr = 0;
					//summation of errors
					for (unsigned int i = 0; i < error.n_rows; i++)
						temperr += (error(i, 0)*error(i, 0)) / 1000;

					//scale errors to prevent any overflows
					temperr /= 1000000;

					//if current error is smaller than previous min error then update the solution (point)
					if (err > temperr)
					{
						soln = res;
						err = temperr;
					}
				}
			}

			paint_vp(paint_lines_img, getVP(), "VP");

			//flush the vector
			points.clear();

		}

		//function to calculate and return the intersection point
		mat calc(mat A, mat b)
		{
			mat x = zeros<mat>(2, 1);
			solve(x, A, b);
			return x;
		}
	};


}