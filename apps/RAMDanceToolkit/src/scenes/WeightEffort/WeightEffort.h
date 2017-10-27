//
//  DDA.h
//  RAMDanceToolkit
//

#pragma once
#include "ramMain.h"

class WeightEffort: public rdtk::BaseScene
{
public:    
    WeightEffort();
    void update();
    void draw();
    
    void drawBeat(const rdtk::NodeArray& NA);
    void drawImGui();
    
    const float computeWeightEffort(const rdtk::NodeArray& NA);
    string getName() const { return "WeightEffort"; }
    
private:
    bool mIsFirstFrame;
    int mFrameIndex;
    int mBufferSize;
    float mScaleY;
    vector<ofVec3f> mPrevAngles;
    vector<ofVec3f> mWEPoints;  // weight effort points for display the result as a line
    vector<vector<ofVec3f>> mWEWave;
};
