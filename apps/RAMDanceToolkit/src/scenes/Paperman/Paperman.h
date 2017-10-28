//
//  boids.hpp
//  RAMDanceToolkit
//
//  Created by NAOYA IWAMOTO on 2017/10/26.
//
//

#pragma once

#include "ramMain.h"
#include "Extractor.h"

struct Plane {
    int index;
    ofColor col;
    ofVec3f previousPos;
    ofVec3f currentPos;
    ofVec3f vel;
    ofMesh pathLines;
    deque<ofVec3f> pathVertices;
};

class Paperman : public rdtk::BaseScene
{
public:
    
    Paperman();
    void setup();
    void update();
    void draw();
    void drawImGui();
    string getName() const { return "Paperman"; }
    
    void modelingPlane(ofMesh& mesh);
    void setScalePlane(const float scale);
    void addPlane();
    void removePlane();
    void resetPos();

private:
    bool mIsAddPlane;
    bool mIsRemovePlane;
    
    bool mIsPlayAuto;
    bool mIsPlayManual;
    bool mIsPlayBlackBox;
    
    float mTimestep;
    
    ofMesh mMesh;
    vector<Plane> mPlanes;
    rdtk::MotionExtractor mEx;  // for picking the joint


    void updateAuto();
    void updateManual();
    void checkNumPlaneWithActor();
};
