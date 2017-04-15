//-------------------------------------------------------------------
// .cpp -- 
// Author: Van-Tuyen Dinh
//-------------------------------------------------------------------


#include <stdio.h>
#include <iostream>
#include <stdio.h>


// OpenCV includes
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"


using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

void readme();

/** @function main */
int main()
{
	string datapath = "d:/VienIndustry40/SourceCode/OPENCV/OPENCV_PRACTICE/"
		"Practice/Data/PR0001/";
	// name of the output video file
	string img01path = datapath + "view0001.jpg";
	string img02path = datapath + "view0002.jpg";

	Mat img_1 = imread(img01path, IMREAD_GRAYSCALE);
	Mat img_2 = imread(img02path, IMREAD_GRAYSCALE);

	
	if (!img_1.data || !img_2.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}

	waitKey(0);
	return 0;
}

/** @function readme */
void readme()
{
	std::cout << " Usage: ./SURF_detector <img1> <img2>" << std::endl;
}