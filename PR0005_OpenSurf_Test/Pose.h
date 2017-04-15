#pragma once
#include "SiftGPU.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "query.h"

class Pose
{
public:
	Pose(void);
	~Pose(void);
	void Init(HMODULE  hsiftgpu, Query *pQuery);
	int PoseEstimate(float *pTDes, SiftGPU::SiftKeypoint *pTKeypoint, int nTKey,
		int nDb, float **pDbDes, SiftGPU::SiftKeypoint **pDbKeypoint, 
		float **pDb3DCoord, int *pDbKeyNum, IplImage **pDbImg, int *pSWLoc, 
		int *pFId, int *pDbImgId);
	int calcMatchPoints(float *pTDes, int nTKey, float *pDbDes, int pDbKeyNum, 
		int match_buffer[][2]);
	int GetInitMatch(float *pTDes, SiftGPU::SiftKeypoint *pTKeypoint, 
		int nTKey, int nDb, float **pDbDes, SiftGPU::SiftKeypoint **pDbKeypoint,
		float **pDb3DCoord, int *pDbKeyNum, IplImage **pDbImg, int *pSWLoc, 
		int *pFId, int *pDbImgId);
	int RansacHomography(CvMat *points1, CvMat *points2, int numMatches, 
		CvMat *homography, float ransac_threshold);

public:
    SiftMatchGPU* m_pMatcher;
	int (*m_pMatchBuff)[2];
	float *m_pMatchPoint1;
	float *m_pMatchPoint2;
	float *m_pMatch3DPoint;
	float *m_pMatchPoint1_t;
	float *m_pMatchPoint2_t;
	float *m_pMatch3DPoint_t;
	int m_nMatchPoint;
	Query *m_pQuery;
	int *m_pVisitBucket;
	CvMat *homography;
	CvMat *points1;
	CvMat *points2;
	int num_inliers;
	int best_ref_id;	// first view id of best match scene
	int ref_id;			// best match id
	int best_previous_id;
	int slide_previous_id;
	IplImage *best_ref_img;

private:
	CvMat *m_matDes1;
	CvMat *m_matDes2;
	CvMat *m_matDes2t;
	CvMat *m_simMatr;
	int *m_pBCoord;

};
