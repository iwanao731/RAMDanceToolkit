//
//  DDA.cpp
//  RAMDanceToolkit
//

#include "WeightEffort.h"

WeightEffort::WeightEffort() :
    mIsFirstFrame(false),
    mFrameIndex(0),
    mScaleY(0.3),
    mBufferSize(512),
    mDrawWidth(512),
    mVolume(1.0)
{
    mPrevAngles.clear();
    mPrevAngles.resize(rdtk::Actor::NUM_JOINTS);
    std::fill(mPrevAngles.begin(), mPrevAngles.end(), ofVec3f(0.0));
}

void WeightEffort::setup()
{
    mWEAudioL.assign(mBufferSize, 0.0);
    mWEAudioR.assign(mBufferSize, 0.0);
}

void WeightEffort::drawImGui()
{
    ImGui::DragInt("Buffer size", &mBufferSize, 10, 0.0, 10000.0);
    ImGui::DragFloat("ScaleY", &mScaleY, 0.1, 0.0, 2.0);
    ImGui::DragFloat("Volume", &mVolume, 0.1, 0.0, 2.0);
}

void WeightEffort::update()
{
    if(mIsFirstFrame)
        mFrameIndex++;
    
    if(mWEPoints.size() > mDrawWidth)
    {
        mWEPoints.erase(mWEPoints.begin());
    }
}

void WeightEffort::draw()
{
    ofPushStyle();
    ofSetColor(255);
    
    rdtk::BeginCamera();
    
    for(int i=0; i<getNumNodeArray(); i++)
    {
        const rdtk::NodeArray &NA = getNodeArray(i);
        drawBeat(NA);
    }

    rdtk::EndCamera();
    ofPopStyle();
}

void WeightEffort::drawBeat(const rdtk::NodeArray& NA)
{
    ofPushMatrix();
    ofTranslate(-mFrameIndex + 300, 0, 0);
    
    float WEValue = computeWeightEffort(NA);
    
    // ignore sometimes framerate is faster than captured data
    if(WEValue != 0.0f){
        ofVec3f pos = ofVec3f(mFrameIndex, WEValue, 0);
        mWEPoints.push_back(pos);
    }
    
    for(int i = 1; i<mWEPoints.size(); i++){
        ofVec3f p0 = mWEPoints[i-1];
        ofVec3f p1 = mWEPoints[i];        
        p0.y *= mScaleY;
        p1.y *= mScaleY;
        ofDrawLine(p0, p1);
    }
    
    ofPopMatrix();
}

const float WeightEffort::computeWeightEffort(const rdtk::NodeArray& NA)
{
    float totalWE = 0.0f;

    if(!mIsFirstFrame)
    {
        for (int i=0; i<NA.getNumNode(); i++)
        {
            const rdtk::Node &node = NA.getNode(i);
            mPrevAngles[i] = node.getGlobalOrientation().getEuler();
        }
        mIsFirstFrame = true;
    }else{
        for (int i=0; i<NA.getNumNode(); i++)
        {
            const rdtk::Node &node = NA.getNode(i);
            ofVec3f v0 = mPrevAngles[i];
            ofVec3f v1 = node.getGlobalOrientation().getEuler();
            ofQuaternion q;
            q.makeRotate(v0, v1);
            float angle;
            //ofVec3f v = q.getEuler();
            ofVec3f v = (v0.cross(v1)).normalize();
            q.getRotate(angle, v.x, v.y, v.z);
            totalWE += angle;//(abs(v.x) + abs(v.y) + abs(v.z));
            
            // save for next frame
            mPrevAngles[i] = v1;
        }
    }
    return totalWE;
}

void WeightEffort::audioOut(float * output, int bufferSize, int nChannels)
{
    if(mWEPoints.size() > mBufferSize)
    {
        for (int i = 1; i <= bufferSize; i++){
            mWEAudioL[i] = output[i*nChannels    ] = mWEPoints[mWEPoints.size() - mBufferSize - i ].y * mVolume;
            mWEAudioR[i] = output[i*nChannels + 1] = mWEPoints[mWEPoints.size() - mBufferSize - i ].y * mVolume;
        }
    }
}

