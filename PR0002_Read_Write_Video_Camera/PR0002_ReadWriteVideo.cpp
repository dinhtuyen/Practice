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
		"Practice/Data/PR0002/";
	// name of the output video file
	string writevideofilename = datapath + "Video_write.avi";
	string readvideofilename  = datapath + "Video_read.avi";

	Mat src;

	// use video file as video source
	VideoCapture Cap;
	Cap.open(readvideofilename);
	if (!Cap.isOpened()) {
		cerr << "ERROR! Unable to read file\n";
		return -1;
	}
	while (Cap.read(src))
	{
		// show live and wait for a key with timeout long enough to show images
		imshow("Live", src);
		if (waitKey(5) == 27)
			break;
	}
	// use default camera as video source
	VideoCapture cap(0);
	// check if we succeeded
	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}
	// get one frame from camera to know frame size and type
	cap >> src;
	// check if we succeeded
	if (src.empty()) {
		cerr << "ERROR! blank frame grabbed\n";
		return -1;
	}
	bool isColor = (src.type() == CV_8UC3);
	//--- INITIALIZE VIDEOWRITER
	VideoWriter writer;
	int codec = CV_FOURCC('P', 'I', 'M', '1'); 
	// select desired codec (must be available at runtime)
	double fps = 25;                          
	// framerate of the created video stream

	writer.open(writevideofilename, codec, fps, src.size(), isColor);
	// check if we succeeded
	if (!writer.isOpened()) {
		cerr << "Could not open the output video file for write\n";
		return -1;
	}
	//--- GRAB AND WRITE LOOP
	cout << "Writing videofile: " << writevideofilename << endl
		<< "Press any key to terminate" << endl;
	for (;;)
	{
		// check if we succeeded
		if (!cap.read(src)) {
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}
		// encode the frame into the videofile stream
		writer.write(src);
		// show live and wait for a key with timeout long enough to show images
		imshow("Live", src);
		if (waitKey(5) == 27)
			break;
	}
	// the videofile will be closed and released automatically in VideoWriter destructor
	return 0;

}
