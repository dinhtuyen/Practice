#pragma once
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "ff_voctree.h"
#include "ff_database.h"
#include "Feature.h"

class Query
{
public:
	unsigned int *m_pVW;	
	ff_voctree* m_pVocTree;
	ff_database* m_pDb;
	int m_nFile;
	int m_nDbFr;
	// -------------------- 3D parameters ----------------
	SiftGPU::SiftKeypoint *m_p3DKeypoint;
	float *m_p3DDes;
	float *m_p3DCoord;
	int *m_p3DKeyNum;
	int m_n3DImages;
	int m_n3DTotalKeyNum;
	int *m_p3DStart;
	// ----------------------------------------------------

	SiftGPU::SiftKeypoint *m_pKeypoint;
	int *m_pKeyNum;
	int *m_pStart;
	char *m_pFileName;
	IplImage **m_pDbImg;
	int *m_pSImgSrc; //identify the src images of the sub-imges
	int *m_pSImgLoc; //identify the locations of the sub-images within the src images	
	int *m_pSImgFrNum; 
	int m_nSImg;
	int *m_pBucketStart; 
	unsigned int *m_pDbVW;
	short *m_pBucketNum;
	int *m_pSWBucket;
	int *m_pSWBucketStart;
	int m_maxNSW;
    unsigned char *m_pIDes_t;
	unsigned char *m_pIDes_t1;
	float *m_pDes_t;
	SiftGPU::SiftKeypoint *m_pKeypoint_t;
	short *m_pBucketNum_t;
	int *m_pFrBId;
	int *m_pBucketStart_t;
	int *m_pSortBucket;
	int *m_pFindSImgLoc;
	int *m_pSWRect;
	int m_iQueryLoc;
	int m_nQueryFr;
	float *m_pBucketWt;
	float *m_pBucketPri;
	float *m_pSWPri;

public:
	Query(void);
	~Query(void);
	void FindImage(unsigned char* pIDes, SiftGPU::SiftKeypoint *pKey, float * pDes, int nFr, int nTop, IplImage** pTopImg, float **pDbDes, SiftGPU::SiftKeypoint **pDbKey, float **pDb3DCoord, int *pDbKeyNum, float *pSWDes, SiftGPU::SiftKeypoint * pSWKey, int &nSWFr, int *pDbImgId);
	void Init(void);
	int *m_pFId;
	float *m_pScore;
	void BuildDb(void);
	void CreateBucket(void);
};
