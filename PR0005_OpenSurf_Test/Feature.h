#pragma once

#include <vector>
#include <cmath>
#include <string.h>

#include "surflib.h"
#include "kmeans.h"

using namespace std;

#include "SiftGPU.h"
#include "glut.h"

class Feature
{
public:
	SiftGPU* m_pSiftGPU;
	SiftGPU::SiftKeypoint* m_pKeypoints;

	float *m_pDescriptor;
	unsigned char *m_pIDes;
	int m_nFr;
	HMODULE m_hsiftgpu;

	// Initial sampling step (default 2)
	int samplingStep;
	// Number of analysed octaves (default 4)
	int octaves;
	// Set this flag "true" to double the image size
	bool doubleImageSize;
	// Initial lobe size, default 3 and 5 (with double image size)
	int initLobe;
	// Upright SURF or rotation invaraiant
	bool upright;
	// If the extended flag is turned on, SURF 128 is used
	bool extended;
	// Spatial size of the descriptor window (default 4)
	int indexSize;
	// verbose output
	bool bVerbose;
	// skip sign of laplacian
	bool bLaplacian;
	// threshold
	float thr;
	// number of levels
	int numLevels;
	// descriptor length
	int VLength;

public:
	Feature(void);
	~Feature(void);

	// SURF initialization
	int initializeSurf();
	int DetectSURF(unsigned char* frame_image, int frame_width, int frame_height);
};
