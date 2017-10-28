//
//  boids.hpp
//  RAMDanceToolkit
//
//  Created by NAOYA IWAMOTO on 2017/10/26.
//
//

#pragma once

#include "ramMain.h"

struct Particle{
    ofVec3f pos;
    ofVec3f vel;
    ofColor col;
    int life;
    int linkIndex;
    float sumDistance;
    float theta;
    float speed;
    float angle;
};

class Emitter : public rdtk::BaseScene
{
public:
    
    Emitter();
    void update();
    void draw();
    void drawImGui();
    string getName() const { return "Emitter"; }

    // original
    int getNumParticles() { return mParticles.size(); }
    float gaussFunction(float sumDistance);
    ofColor errorToRGB(float err, float errMin, float errMax);
    void addParticle(ofVec3f pos, ofVec3f vel, float life);
    
private:
    
    bool mIsFirstFrame;
    bool mEnableColor;
    bool mIsDrawEdge;
    int mLife;
    float mSize;
    float mDamping;
    float mThreshold;
    float mTimestep;
    float mGravity;
    vector<ofVec3f> mPreviousPos;
    vector<Particle> mParticles;
    vector<Particle> mPreviousJoints;
};
