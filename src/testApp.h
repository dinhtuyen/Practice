#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOpenCv.h"

#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file

#include "ofxSimpleGuiToo.h"

#include "surflib.h"
//#include "kmeans.h"
#include "ofxOpenSurf.h"

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		//void makeMarkerFromImg(string url, std::vector<Ipoint> &ipts);
		//void ofxDrawIpoints(int x, int y, float s, std::vector<Ipoint> &ipts, int tailSize=0);

        ofVideoGrabber cam;
        bool bCapturing;

        //ofxCvColorImage marker;
        //ofxCvColorImage camImg;
        //ofxCvColorImage mtcImg;
        ofxCvColorImage vtcImg;
        IpVec ipts; //static img
        IpVec vpts; //video only
        IpVec vspts;//video src
        IpVec vtpts;//video trg
        IpVec spts; //static src
        IpVec tpts; //static trg
        IpPairVec matches;
        IpPairVec vmatches;

        ofPoint src_corners[4];
        ofPoint dst_corners[4];

        string opts[6];
        int opt;
        int oldOpt;

        ofxSurfImage sImg;
        ofxSurfImage dImg;
        ofxSurfImage camImg;
        ofxSurfImage dvImg;

        ofxSurfMotion mov;
        //ofxSurfStaticMatch staticMatch;
};

#endif
