#include "ofxOpenSurf.h"

ofxOpenSurf::ofxOpenSurf()
{
    //ctor
}

ofxOpenSurf::~ofxOpenSurf()
{
    //dtor
}

void ofxSurfStaticMatch(ofxSurfImage * src, ofxSurfImage * dst, IpPairVec * matches){
     //IpPairVec matches;
     surfDetDes(src->getCvImage(),src->ipts,false,4,4,2,0.0006f);
     surfDetDes(dst->getCvImage(),dst->ipts,false,4,4,2,0.0006f);
     getMatches(src->ipts,dst->ipts,*matches);
     //return matches;
}

void ofxSurfVideoMatch(ofxSurfImage * cam, ofxSurfImage * mrk, IpPairVec * matches){
    surfDetDes(cam->getCvImage(),cam->ipts,false,4,4,2,0.001f);
    getMatches(cam->ipts,mrk->ipts,*matches);
}

int ofxSurfObjCorners(IpPairVec & matches,const ofPoint src_crn[4],ofPoint dst_crn[4]){
    double h[9];
    CvMat _h = cvMat(3,3,CV_64F,h);
    vector<CvPoint2D32f> pt1,pt2;
    CvMat _pt1,_pt2;

    int n = (int)(matches.size());
    if(n<4)return 0;

    pt1.resize(n);
    pt2.resize(n);

    for(int i=0;i<n;i++){
        pt1[i] = cvPoint2D32f(matches[i].second.x,matches[i].second.y);
        pt2[i] = cvPoint2D32f(matches[i].first.x,matches[i].first.y);
    }
    _pt1 = cvMat(1,n,CV_32F,&pt1[0]);
    _pt2 = cvMat(1,n,CV_32F,&pt2[0]);



    //if(!cvFindHomography(&_pt1,&_pt2,&_h,CV_RANSAC,5))return 0;
    /*for(int i=0;i<4;i++){
        double x = (double)src_crn[i].x;
        double y = (double)src_crn[i].y;
        double Z = 1./(h[6]*x + h[7]*y + h[8]);
        double X = (h[0]*x + h[1]*y + h[2])*Z;
        double Y = (h[3]*x + h[4]*y + h[5])*Z;
        dst_crn[i].set(cvRound(X),cvRound(Y));
    }*/

    return 1;
}

void ofxDrawIpoints(int x, int y, float sz, std::vector<Ipoint> &ipts, int tailSize){
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(sz,sz,1);
    Ipoint * ipt;
    float s,o;
    int r1,c1,r2,c2,lap;
    for(unsigned int i=0;i<ipts.size();i++){
        ipt = &ipts.at(i);
        s = ((9.0f/1.2f) * ipt->scale) / 3.0f;
        o = ipt->orientation;
        lap = ipt->laplacian;
        r1 = fRound(ipt->y);
        c1 = fRound(ipt->x);
        c2 = fRound(s * cos(o)) + c1;
        r2 = fRound(s * sin(o)) + r1;

        if(o){ //green line = orientation
            ofSetColor(0x00ff00);
            ofLine(c1,r1,c2,r2);
        }else{ //green dot = upright conversion
            ofSetColor(0x00ff00);
            ofCircle(c1,r1,1);
        }

        if(lap>=0){ //blue circle = dark blob on light
            ofSetColor(0x0000ff);
            ofNoFill();
            ofCircle(c1,r1,fRound(s));
        }else{ // red circle = light blob on dark
            ofSetColor(0xff0000);
            ofNoFill();
            ofCircle(c1,r1,fRound(s));
        }

        if(tailSize){ //draw motion ipoint dx dy
            ofSetColor(0xffffff);
            ofLine(c1,r1,int(c1+ipt->dx*tailSize),int(r1+ipt->dy*tailSize));
        }
    }
    glPopMatrix();
}

void ofxSurfMotion::setup(ofxSurfImage * _cam){
    cam = _cam;
}

void ofxSurfMotion::update(){
    oldIpts = cam->ipts;
    motion.clear();
    surfDetDes(cam->getCvImage(),cam->ipts,true,3,4,2,0.0004f);
    getMatches(cam->ipts,oldIpts,matches);
    for(uint i=0;i<matches.size();i++){
        Motion m;
        m.src.set(matches[i].first.x,matches[i].first.y);
        m.dst.set(matches[i].second.x,matches[i].second.y);
        float dx = matches[i].first.dx;
        float dy = matches[i].first.dy;
        m.speed = sqrtf(dx*dx+dy+dy);
        motion.push_back(m);
    }
}

void ofxSurfMotion::draw(int x, int y, float sz){
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(sz,sz,1);
    ofSetColor(0xff0000);
    ofNoFill();
    for(uint i=0;i<motion.size();i++){
        Motion m = motion[i];
        ofLine(m.src.x,m.src.y,m.dst.x,m.dst.y);
        ofCircle(m.src.x,m.src.y,m.speed);
    }
    glPopMatrix();
}

/*void ofxDrawMatch(int x,int y, float sz, IpPairVec match){
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(sz,sz,1);
    for(uint i=0;i<matches.size();i++){
        ofSetColor(0xff0000);
        ofNoFill();
        ofCircle(matches[i].first.x,matches[i].first.y,3);
        ofCircle(matches[i].second.x+marker.width,matches[i].second.y,3);
        ofSetColor(0xffff00);
        ofLine(matches[i].first.x,matches[i].first.y,matches[i].second.x+marker.width,matches[i].second.y);
    }
    glPopMatrix();
}*/
