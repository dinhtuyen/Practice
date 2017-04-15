#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <opencv2/videoio.hpp> 
#include <opencv2/opencv.hpp>



#include "opencv2/core/utility.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"



using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
const int LOOP_NUM = 1;
const int GOOD_PTS_MAX = 500;
const float GOOD_PORTION = 0.5f;

int64 work_begin = 0;
int64 work_end = 0;

static void workBegin()
{
	work_begin = getTickCount();
}

static void workEnd()
{
	work_end = getTickCount() - work_begin;
}

static double getTime()
{
	return work_end / ((double)getTickFrequency())* 1000.;
}

struct SURFDetector
{
	Ptr<Feature2D> surf;
	SURFDetector(double hessian = 800.0)
	{
		surf = SURF::create(hessian);
	}
	template<class T>
	void operator()(const T& in, const T& mask, std::vector<cv::KeyPoint>& pts, T& descriptors, bool useProvided = false)
	{
		surf->detectAndCompute(in, mask, pts, descriptors, useProvided);
	}
};

template<class KPMatcher>
struct SURFMatcher
{
	KPMatcher matcher;
	template<class T>
	void match(const T& in1, const T& in2, std::vector<cv::DMatch>& matches)
	{
		matcher.match(in1, in2, matches);
	}
};

static Mat drawGoodMatches(
	const Mat& img1,
	const Mat& img2,
	const std::vector<KeyPoint>& keypoints1,
	const std::vector<KeyPoint>& keypoints2,
	std::vector<DMatch>& matches,
	std::vector<Point2f>& scene_corners_
)
{
	//-- Sort matches and preserve top 10% matches
	std::sort(matches.begin(), matches.end());
	std::vector< DMatch > good_matches;
	double minDist = matches.front().distance;
	double maxDist = matches.back().distance;

	const int ptsPairs = std::min(GOOD_PTS_MAX, (int)(matches.size() * GOOD_PORTION));
	for (int i = 0; i < ptsPairs; i++)
	{
		good_matches.push_back(matches[i]);
	}
	std::cout << "\nMax distance: " << maxDist << std::endl;
	std::cout << "Min distance: " << minDist << std::endl;

	std::cout << "Calculating homography using " << ptsPairs << " point pairs." << std::endl;

	// drawing the results
	Mat img_matches;

	drawMatches(img1, keypoints1, img2, keypoints2,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (size_t i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints1[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints2[good_matches[i].trainIdx].pt);
	}
	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = Point(0, 0);
	obj_corners[1] = Point(img1.cols, 0);
	obj_corners[2] = Point(img1.cols, img1.rows);
	obj_corners[3] = Point(0, img1.rows);
	std::vector<Point2f> scene_corners(4);

	Mat H = findHomography(obj, scene, RANSAC);
	if(H.data)
	{
	perspectiveTransform(obj_corners, scene_corners, H);

	scene_corners_ = scene_corners;

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches,
		scene_corners[0] + Point2f((float)img1.cols, 0), scene_corners[1] + Point2f((float)img1.cols, 0),
		Scalar(0, 255, 0), 2, LINE_AA);
	line(img_matches,
		scene_corners[1] + Point2f((float)img1.cols, 0), scene_corners[2] + Point2f((float)img1.cols, 0),
		Scalar(0, 255, 0), 2, LINE_AA);
	line(img_matches,
		scene_corners[2] + Point2f((float)img1.cols, 0), scene_corners[3] + Point2f((float)img1.cols, 0),
		Scalar(0, 255, 0), 2, LINE_AA);
	line(img_matches,
		scene_corners[3] + Point2f((float)img1.cols, 0), scene_corners[0] + Point2f((float)img1.cols, 0),
		Scalar(0, 255, 0), 2, LINE_AA);
	}
	return img_matches;
}

////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	string datapath = "d:/VienIndustry40/SourceCode/OPENCV/OPENCV_PRACTICE/"
		"Practice/Data/PR0005/";
	// name of the output video file
	string writevideofilename = datapath + "Video_write.avi";
	VideoWriter writer;
	int codec = CV_FOURCC('P', 'I', 'M', '1');
	// select desired codec (must be available at runtime)
	double fps = 25;
	// framerate of the created video stream

	writer.open(writevideofilename, codec, fps, Size(840,360), 1);
	// check if we succeeded
	if (!writer.isOpened()) {
		cerr << "Could not open the output video file for write\n";
		return -1;
	}



	string imgfilename = datapath + "LogoNTT.JPG";
	/*string imgfilename2 = datapath + "2015042003.jpg";*/
	Mat img_1 = imread(imgfilename, IMREAD_COLOR);
	resize(img_1,img_1,Size(200, 200));
	Mat img_1Gray;
	UMat img1, img2;
	cvtColor(img_1, img_1Gray, CV_BGR2GRAY);
	//Mat img_2 = imread(imgfilename2, IMREAD_GRAYSCALE);
	if (!img_1.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}
	//-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
	





	img_1.copyTo(img1);


	double surf_time = 0.;

	//declare input/output
	std::vector<KeyPoint> keypoints1, keypoints2;
	std::vector<DMatch> matches;

	UMat _descriptors1, _descriptors2;
	Mat descriptors1 = _descriptors1.getMat(ACCESS_RW),
		descriptors2 = _descriptors2.getMat(ACCESS_RW);
	//instantiate detectors/matchers
	SURFDetector surf;

	SURFMatcher<BFMatcher> matcher;

	//-- start of timing section



	////int minHessian = 100;
	////Ptr<SURF> detector = SURF::create();
	////detector->setHessianThreshold(minHessian);
	////std::vector<KeyPoint> keypoints_1, keypoints_2;
	////Mat descriptors_1, descriptors_2;
	////detector->detectAndCompute(img_1Gray, Mat(), keypoints_1, descriptors_1);
	////FlannBasedMatcher matcher;
	////std::vector< DMatch > matches;
	////
	Mat camimg, camimgGray;
	////// use default camera as video source
	//VideoCapture cap(0);

	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 800);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 600); 


	//////// check if we succeeded
	//if (!cap.isOpened()) {
	//	cerr << "ERROR! Unable to open camera\n";
	//	return -1;
	//}
	//// get one frame from camera to know frame size and type
	//cap >> camimg;

	for (int i = 0; i <= LOOP_NUM; i++)
	{
		if (i == 1) workBegin();
		surf(img1.getMat(ACCESS_READ), Mat(), keypoints1, descriptors1);
		
	}
	//workEnd();
	Mat img_matches;

	string readvideofilename = datapath + "Test_Logo_NTT.mp4";


	Mat src;

	// use video file as video source
	VideoCapture Cap;
	Cap.open(readvideofilename);
	if (!Cap.isOpened()) {
		cerr << "ERROR! Unable to read file\n";
		return -1;
	}


	




	while (Cap.read(camimg))
	{



	//for (;;)
	//{
	//	// check if we succeeded
	//	
	//	if (!cap.read(camimg)) {
	//		cerr << "ERROR! blank frame grabbed\n";
	//		break;
	//	}
		resize(camimg, camimg, camimg.size()/2);
		cvtColor(camimg, camimgGray, CV_BGR2GRAY);

		camimgGray.copyTo(img2);

		for (int i = 0; i <= LOOP_NUM; i++)
		{
			if (i == 1) workBegin();
			//surf(img1.getMat(ACCESS_READ), Mat(), keypoints1, descriptors1);
			surf(img2.getMat(ACCESS_READ), Mat(), keypoints2, descriptors2);
			
		}
		matcher.match(descriptors1, descriptors2, matches);

		workEnd();
		//std::cout << "FOUND " << keypoints1.size() << " keypoints on first image" << std::endl;
		//std::cout << "FOUND " << keypoints2.size() << " keypoints on second image" << std::endl;

		//surf_time = getTime();
		//std::cout << "SURF run time: " << surf_time / LOOP_NUM << " ms" << std::endl << "\n";



			//Mat img_matches = drawGoodMatches(img1.getMat(ACCESS_READ), img2.getMat(ACCESS_READ), keypoints1, keypoints2, matches, corner);
			std::vector<Point2f> corner;
			img_matches = drawGoodMatches(img_1, camimg, keypoints1, keypoints2, matches, corner);

			//img_matches = drawGoodMatches(img1.getMat(ACCESS_READ), img2.getMat(ACCESS_READ), keypoints1, keypoints2, matches, corner);


			//-- Show detected matches

			namedWindow("surf_matches", CV_GUI_NORMAL);
			imshow("surf_matches", img_matches);






			//////////detector->detectAndCompute(camimgGray, Mat(), keypoints_2, descriptors_2);
			////////////-- Step 2: Matching descriptor vectors using FLANN matcher
			//////////
			//////////matcher.match(descriptors_1, descriptors_2, matches);
			//////////double max_dist = 0; double min_dist = 100;
			////////////-- Quick calculation of max and min distances between keypoints
			//////////for (int i = 0; i < descriptors_1.rows; i++)
			//////////{
			//////////	double dist = matches[i].distance;
			//////////	if (dist < min_dist) min_dist = dist;
			//////////	if (dist > max_dist) max_dist = dist;
			//////////}
			//////////printf("-- Max dist : %f \n", max_dist);
			//////////printf("-- Min dist : %f \n", min_dist);
			////////////-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
			////////////-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
			////////////-- small)
			////////////-- PS.- radiusMatch can also be used here.
			//////////std::vector< DMatch > good_matches;
			//////////for (int i = 0; i < descriptors_1.rows; i++)
			//////////{
			//////////	if (matches[i].distance <= max(2 * min_dist, 0.001))
			//////////	{
			//////////		good_matches.push_back(matches[i]);
			//////////	}
			//////////}
			////////////-- Draw only "good" matches
			//////////Mat img_matches;
			//////////drawMatches(img_1, keypoints_1, camimg, keypoints_2,
			//////////	good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
			//////////	vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
			////////////-- Show detected matches
			//////////namedWindow("Good Matches", CV_GUI_NORMAL);
			//////////imshow("Good Matches", img_matches);
			writer.write(img_matches);
			if (waitKey(5) == 27)
				break;
		}

	waitKey(0);
	return 0;
}
