#pragma once

#include <string>
#include <opencv/cv.h>
#include "SiftGPU.h"

using namespace std;

class SceneControllor
{
public:
	int num_scenes, num_references, num_views;
	int current_scene_id;
	int start_scene_id;
	int dimension_index;

	// parameters for surf
	int sampling_step;
	int response_threshold;
	int octave_num;
	bool double_size;
	int window_size;

	// define max feature numbers
	int max_feature_num;

	// related to matchings
	int max_feature_num_used_matching;
	float matching_dist_ratio;

	// RANSAC parameters
	float ransac_threshold; 
	int ransac_loop_num;

	// scene image dimension
	int image_width;
	int image_height;

	// max image dimension
	int max_image_width;
	int max_image_height;

	// status flags
	bool status_database_openned;
	bool status_track;
	bool status_query_needed;
	bool status_current_view_recognized;

	// number of inliers needed to be recognized
	int recognition_num_inliers_threshold;
	int recognition_max_inliers;

	// recognition confidence value (0-1)
	float recognition_confidence;

	// homography of current view
	CvMat *current_to_live_homography;
	CvMat *live_to_current_homography;
	float *first_to_all_homography;
	CvMat *first_to_current_homography;
	CvMat *first_to_live_homography;
	CvMat *live_to_first_homography;

	// for testing annotations
	CvMat *annotation_points_scenes;	// for first view
	CvMat *annotation_points_scenes1;	// for current view (could be first view)
	CvMat *annotation_points_scenes2;	// for live view

	// related to file names
	char fileName[300];
	string str_fileName;
	// CString cstr_fileName;

	// the live image
	IplImage* pCurImg;
	IplImage *pGrayImg;

	// the database images, keypoints and descriptors
	IplImage **pDbImg;
	float **pDbDes;
	float **pDb3DCoord;
	SiftGPU::SiftKeypoint **pDbKey;
	int *pDbKeyNum;
	int *pDbImgId;
	float *pSWDes;
	SiftGPU::SiftKeypoint *pSWKey;
	int nSWFr;

	// number of top image retrived
	int nTop;

public:
	SceneControllor();
	~SceneControllor();
	void initialize();

};