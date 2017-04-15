/*------------------------------------------------------------------/
 PR0001_Reading_Writing_Image.cpp 
 Author: Van-Tuyen Dinh
 Read and write images
/-------------------------------------------------------------------*/


#include <stdio.h>
#include <iostream>
#include <stdio.h>


// OpenCV includes
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"


using namespace std;
using namespace cv;

/** @function main */
int main()
{
	string datapath = "d:/VienIndustry40/SourceCode/OPENCV/OPENCV_PRACTICE/"
		"Practice/Data/PR0001/";
	// name of the output video file
	string img01path = datapath + "Gray_Image.jpg";
	string img02path = datapath + "Color_Image.jpg";

	Mat img_1 = imread(img01path, CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_2 = imread(img02path, CV_LOAD_IMAGE_COLOR);

	// CV_LOAD_IMAGE_ANYCOLOR
	// CV_LOAD_IMAGE_COLOR
	// CV_LOAD_IMAGE_GRAYSCALE
	// CV_LOAD_IMAGE_ANYDEPTH
	
	if (!img_1.data || !img_2.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}

	// Write images
		imwrite(datapath + "Gray_Image_Save.jpg", img_1);
		imwrite(datapath + "Color_Image_Save.jpg", img_2);

	int myCol = img_2.cols-1;
	int myRow = img_2.rows-1;
	Vec3b pixel = img_2.at<Vec3b>(myRow, myCol);
	cout << "Pixel value (B, G, R): (" << (int)pixel[0] << "," <<
		(int)pixel[1] << "," << (int)pixel[2] << ")" << endl;

	//Access color pixel: Vec3b pixel= img.at<Vec3b>(row, col);
	//Access gray pixel:  uchar pixel= img.at<uchar>(row, col);

	//Create windows
	namedWindow("Image BGR", CV_GUI_NORMAL);
	namedWindow("Image Gray", CV_WINDOW_AUTOSIZE);

	// show images
	imshow("Image BGR", img_1);
	imshow("Image Gray", img_2);
	// wait for any key press
	waitKey(0);
	return 0;
}

