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
#include <opencv2/videoio.hpp> 

using namespace std;
using namespace cv;

int main()
{
	string datapath = "d:/VienIndustry40/SourceCode/OPENCV/OPENCV_PRACTICE/"
		"Practice/Data/PR0003/";
	// name of the output video file
	string datafilename = datapath + "test.yml";

	FileStorage fs(datafilename, FileStorage::WRITE);
	// Save an int
	int fps = 5;
	fs << "fps" << fps;
	// Create some mat sample
	Mat m1 = Mat::eye(10, 10, CV_32F);
	Mat m2 = Mat::ones(10, 10, CV_32F);
	Mat result = (m1 + 1).mul(m1 + 3);
	// write the result
	fs << "Result" << result <<"m1" <<m1 <<"m2"<<m2;
	// release the file
	fs.release();

	// read the yml file

	FileStorage fs2(datafilename, FileStorage::READ);
	Mat r,n1,n2;
	int ii;
	fs2["Result"] >> r;
	fs2["m1"] >> n1;
	fs2["m2"] >> n2;
	fs2["fps"] >> ii;
	std::cout << r << std::endl;
	fs2.release();
	return 0;

}
