//
//  DDA.h
//  RAMDanceToolkit
//

#pragma once
#include "ramMain.h"

class DDA: public rdtk::BaseScene
{
public:
    
    DDA();
    void drawImGui();
    void draw();
    void drawBigBox(const rdtk::NodeArray& NA);
    string getName() const { return "DDA"; }
    void onPanelChanged(ofxUIEventArgs& e);

private:
    bool mDrawImage;
    float mBoxLineWidth;
    float mMasterBoxSize;
    vector<ofImage> mImages;
    vector<float> mSizeArray;
};
