//
//  dpCameraUnit_Input.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/18.
//
//

#include "dpCameraUnit_Input.h"

dpCameraUnit_input::dpCameraUnit_input(){

	mCameraList.push_back("None");
	for (int i = 0;i < mVideoGrabber.listDevices().size();i++){
		mCameraList.push_back(mVideoGrabber.listDevices()[i].deviceName);
	}
	mCameraList.push_back("ps3Eye");

	mGui.setup();
	mGui.addLabel("InputUnit",OFX_UI_FONT_LARGE);
	mGui.addDropDownList("InputSource", mCameraList);
	mGui.addToggle("Perspective", &mEnablePerspective);
	mGui.autoSizeToFitWidgets();

	ofAddListener(mGui.newGUIEvent, this, &dpCameraUnit_input::guiEvent);
	ofAddListener(ofEvents().mouseMoved, this, &dpCameraUnit_input::mouseMoved);
	ofAddListener(ofEvents().mousePressed, this, &dpCameraUnit_input::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &dpCameraUnit_input::mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &dpCameraUnit_input::mouseReleased);

	mWarpPoint[0].set(0.0			, 0.0);
	mWarpPoint[1].set(input_width	, 0.0);
	mWarpPoint[2].set(input_width	, input_height);
	mWarpPoint[3].set(0.0			, input_height);

	mFinalSource_Large.allocate(input_width,
								input_height,
								OF_IMAGE_COLOR);

	mFinalSource_Small.allocate(input_width * cvSrc_ratio,
								input_height * cvSrc_ratio,
								OF_IMAGE_COLOR);

	mIsFrameNew = false;
}

dpCameraUnit_input::~dpCameraUnit_input(){

}

void dpCameraUnit_input::update(){

	mVideoGrabber.update();

	if (mSourceType > 0){
		if (mCameraList[mSourceType] == "ps3Eye"){

		}else if (mSourceType > 0){
			if (mVideoGrabber.isFrameNew()){
				mIsFrameNew = true;

				if (mEnablePerspective){
					vector<ofxCv::Point2f> warpPt;
					for (int i = 0;i < 4;i++) warpPt.push_back(ofxCv::toCv(mWarpPoint[i]));
					ofxCv::unwarpPerspective(mVideoGrabber.getPixelsRef(),
											 mFinalSource_Large,
											 warpPt);
				}else{
					ofxCv::copy(mVideoGrabber.getPixelsRef(), mFinalSource_Large);
				}

				mFinalSource_Large.update();

				ofxCv::resize(mFinalSource_Large, mFinalSource_Small);
				mFinalSource_Small.update();

			}else{
				mIsFrameNew = false;
			}
		}
	}
}

void dpCameraUnit_input::draw(int x,int y){
	mGui.setPosition(x, y);

	mDrawnPoint.set(x, y);
	ofSetColor(255);

	ofPushMatrix();
	ofTranslate(x, y);
	mGui.draw();

	ofPushMatrix();
	ofTranslate(240, 0);
	glScaled(thumb_ratio, thumb_ratio, 1.0);
	if (mSourceType > 0){
		if (mCameraList[mSourceType] == "ps3Eye"){

		}else if (mSourceType > 0){
			mVideoGrabber.draw(0, 0, input_width, input_height);
			mFinalSource_Large.draw(0, input_height, input_width, input_height);

			ofSetColor(255, 255, 0);
			glBegin(GL_LINE_STRIP);
			glVertex2d(mWarpPoint[0].x, mWarpPoint[0].y);
			glVertex2d(mWarpPoint[1].x, mWarpPoint[1].y);
			glVertex2d(mWarpPoint[2].x, mWarpPoint[2].y);
			glVertex2d(mWarpPoint[3].x, mWarpPoint[3].y);
			glVertex2d(mWarpPoint[0].x, mWarpPoint[0].y);
			glEnd();
		}
	}
	ofPopMatrix();

	ofPopMatrix();
}


void dpCameraUnit_input::mouseMoved(ofMouseEventArgs &arg){

}

void dpCameraUnit_input::mousePressed(ofMouseEventArgs &arg){
	ofVec2f mp = arg - mDrawnPoint;

	if (ofRectangle(240,0,input_width/2,input_height/2).inside(mp)){
		ofVec2f tPos = (mp - ofVec2f(240,0))/thumb_ratio;

		mGuiParam_catchWarpPoint = -1;
		for (int i = 0;i < 4;i++){
			if (mWarpPoint[i].distanceSquared(tPos) < 900)
				mGuiParam_catchWarpPoint = i;
		}
	}
}

void dpCameraUnit_input::mouseDragged(ofMouseEventArgs &arg){
	ofVec2f mp = arg - mDrawnPoint;

	if (ofRectangle(240,0,input_width/2,input_height/2).inside(mp)){
		ofVec2f tPos = (mp - ofVec2f(240,0))/thumb_ratio;

		if (mGuiParam_catchWarpPoint > -1){
			mWarpPoint[mGuiParam_catchWarpPoint].set(tPos);
		}

	}
}

void dpCameraUnit_input::mouseReleased(ofMouseEventArgs &arg){
	mGuiParam_catchWarpPoint = -1;
}

void dpCameraUnit_input::guiEvent(ofxUIEventArgs &e){
	ofxUIWidget* w = e.widget;

	if (w->getName() == "InputSource"){
		ofxUIDropDownList* ww = (ofxUIDropDownList*)w;

		if ((ww->getSelectedNames().size() > 0) &&
			(ww->getSelectedIndeces()[0] != mSourceType)){

			mSourceType = ww->getSelectedIndeces()[0];

			string srcName = ww->getSelectedNames()[0];
			ww->setLabelText(srcName);

			if (srcName == "ps3Eye"){
				cout << "ps3eye initialize" << endl;
			}else if (ww->getSelectedIndeces()[0] > 0){
				mVideoGrabber.setDeviceID(ww->getSelectedIndeces()[0]-1);
				mVideoGrabber.initGrabber(input_width, input_height);
			}

		}

	}


}