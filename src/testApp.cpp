#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0,0,0);
    ofSetVerticalSync(true);

    camImg.allocate(320,240);
    bCapturing = false;

    sImg.loadImage("Images/img1.jpg");
    sImg.calcSurf(OFX_SURF_STATIC);

    dImg.loadImage("Images/img2.jpg");

    dvImg.loadImage("Images/marker.jpg");
    dvImg.calcSurf(OFX_SURF_VIDEOMATCH);

    mov.setup(&camImg);

    //makeMarkerFromImg("Images/img1.jpg",ipts);

    /*ofImage img;
    img.loadImage("Images/img2.jpg");
    mtcImg.allocate(img.width,img.height);
    unsigned char * pix = img.getPixels();
    mtcImg.setFromPixels(pix,img.width,img.height);
    surfDetDes(mtcImg.getCvImage(),tpts,false,4,4,2,0.0006f);
    surfDetDes(marker.getCvImage(),spts,false,4,4,2,0.0006f);
    getMatches(spts,tpts,matches);*/

    /*ofImage img2;
    img2.loadImage("Images/marker.png");
    vtcImg.allocate(img2.width,img2.height);
    pix = img2.getPixels();
    vtcImg.setFromPixels(pix,img2.width,img2.height);
    surfDetDes(vtcImg.getCvImage(),vtpts,false,4,4,2,0.0001f);*/

    src_corners[0].set(0,0);
    src_corners[1].set(vtcImg.width,0);
    src_corners[2].set(vtcImg.width,vtcImg.height);
    src_corners[3].set(0,vtcImg.height);

    opt = 0;
    opts[0]="Image";
    opts[1]="Video";
    opts[2]="Match";
    opts[3]="MotionPts";
    opts[4]="StaticMatch";
    opts[5]="Kmeans";

    gui.addTitle("ofxOpenSURF",20);
    gui.addComboBox("Options",opt,6,opts);
    gui.show();
}

//--------------------------------------------------------------
void testApp::update(){
    if(bCapturing){
        if(opt!=1 && opt!=2 && opt!=3){
            cam.close();
            bCapturing=false;
        }else{
            cam.update();
            if(cam.isFrameNew()){
                camImg.setFromPixels(cam.getPixels(),320,240);
                switch(opt){
                    case 1:
                        //video only
                        camImg.calcSurf(OFX_SURF_VIDEO);
                        break;
                    case 2:
                        //video match
                        //surfDetDes(camImg.getCvImage(),vspts,false,4,4,2,0.001f);
                        //getMatches(vspts,vtpts,vmatches);
                        ofxSurfVideoMatch(&camImg,&dvImg,&vmatches);
                        break;
                    case 3:
                        //motion
                        mov.update();
                        break;
                }
            }
        }
    }else if(!bCapturing && (opt==1 || opt==2 || opt==3)){
        cam.initGrabber(320,240);
        bCapturing = true;
    }

    oldOpt=opt;
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(0xffffff);
    glPushMatrix();
    glTranslatef(250,100,0);
    switch(opt){
        case 0:
            //single img
            //marker.draw(0,0,marker.width/2,marker.height/2);
            sImg.draw(0,0,sImg.width/2,sImg.height/2);
            ofxDrawIpoints(0,0,.5,sImg.ipts);
            break;
        case 1:
            //video only
            camImg.draw(0,0);
            ofxDrawIpoints(0,0,1,camImg.ipts);
            break;
        case 2:
            //video match
            camImg.draw(0,0);
            dvImg.draw(camImg.width,0);
            ofxDrawIpoints(0,0,1,camImg.ipts);
            ofxDrawIpoints(camImg.width,0,1,dvImg.ipts);
            ofSetColor(0xffff00);
            if(ofxSurfObjCorners(vmatches,src_corners,dst_corners)){

            }
            for(uint i=0;i<vmatches.size();i++){
                ofLine(vmatches[i].first.x,vmatches[i].first.y,vmatches[i].second.x+camImg.width,vmatches[i].second.y);
            }

            ofSetColor(0xff0000);
            ofDrawBitmapString(ofToString((int)(vmatches.size()))+" matches found",10,camImg.height+10);
            break;
        case 3:
            //motion
            camImg.draw(0,0);
            mov.draw();
            break;
        case 4:
            //static match
            if(oldOpt!=opt){
                    ofxSurfStaticMatch(&sImg,&dImg,&matches);
            }

            sImg.draw(0,0,sImg.width/2,sImg.height/2);
            dImg.draw(sImg.width/2,0,dImg.width/2,dImg.height/2);
            glPushMatrix();
            glScalef(.5,.5,1);
            for(uint i=0;i<matches.size();i++){
                ofSetColor(0xff0000);
                ofNoFill();
                ofCircle(matches[i].first.x,matches[i].first.y,3);
                ofCircle(matches[i].second.x+sImg.width,matches[i].second.y,3);
                ofSetColor(0xffff00);
                ofLine(matches[i].first.x,matches[i].first.y,matches[i].second.x+sImg.width,matches[i].second.y);
            }
            glPopMatrix();
            ofSetColor(0xff0000);
            ofDrawBitmapString(ofToString((int)(matches.size()))+" matches found",10,sImg.height/2+10);
            break;

    }
    glPopMatrix();

    gui.draw();
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){


}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

/*void testApp::makeMarkerFromImg(string url,std::vector<Ipoint> &ipts){
    ofImage img;
    img.loadImage(url);
    marker.allocate(img.width,img.height);
    unsigned char * pix = img.getPixels();
    marker.setFromPixels(pix,img.width,img.height);

    //IpVec ipts;
    surfDetDes(marker.getCvImage(),ipts,true,3,4,2,0.0004f);
}*/

/*void testApp::ofxDrawIpoints(int x, int y, float sz, std::vector<Ipoint> &ipts, int tailSize){
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
}*/
