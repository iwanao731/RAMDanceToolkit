//
//  boids.cpp
//  RAMDanceToolkit
//
//  Created by NAOYA IWAMOTO on 2017/10/26.
//
//

#include "Emitter.h"

Emitter::Emitter() :
mIsFirstFrame(false),
mSize(5.0f),
mDamping(1.0f),
mTimestep(0.66f),
mThreshold(5.0f),
mGravity(-6.0f),
mLife(200),
mEnableColor(false),
mIsDrawEdge(false)
{
    mPreviousPos.clear();
    mPreviousPos.resize(rdtk::Actor::NUM_JOINTS);
}

void Emitter::update()
{
    // update simulation
    int num = 0;
    for(auto& particle : mParticles)
    {
        ofVec3f prevPos = particle.pos;
        
        particle.vel *= mDamping;
        particle.pos += (particle.vel + ofVec3f(0.0f, mGravity, 0.0f)) * mTimestep;
        particle.pos.y += gaussFunction(particle.sumDistance);
        
        if(particle.pos.y < 0.0f){
            particle.pos.y = 0.0f;
            particle.vel = ofVec3f::zero();
        }
        
        particle.life = particle.life - 1;
        particle.sumDistance = (particle.pos - prevPos).length();
        
        if(particle.life < 0)
        {
            num++;
        }
    }
    
    // remove considered to those life
    for(int i=0; i<num; i++){
        mParticles.erase(mParticles.begin());
    }
    
    // number of person
    for(int i=0; i<getNumNodeArray(); i++)
    {
        const rdtk::NodeArray &NA = getNodeArray(i);
        
        // number of joints
        for (int j=0; j<NA.getNumNode(); j++)
        {
            const rdtk::Node &node = NA.getNode(j);
            
            if(!mIsFirstFrame)
            {
                mPreviousPos[j] = node.getGlobalPosition();
                mIsFirstFrame = true;
            }
            else{
                ofVec3f p0 = mPreviousPos[j];
                ofVec3f p1 = node.getGlobalPosition();
                ofVec3f vel = p1 - p0;
                float length = vel.length();
                
                if(mThreshold < length)
                {
                    this->addParticle(node.getGlobalPosition(), vel, mLife);
                }
                
                // save current position for next frame
                mPreviousPos[j] = node.getGlobalPosition();
            }
        }
    }
}

void Emitter::draw()
{
    rdtk::BeginCamera();
    
    for(auto& particle : mParticles)
    {
        if(mEnableColor){
            ofSetColor(particle.col);
        }else{
            ofSetColor(255);
        }
        ofVec3f p = particle.pos;
        ofPushMatrix();
        ofTranslate(p.x, p.y, p.z);
        ofDrawBox(0,0,0, mSize);
        //drawPaperPlane();
        ofPopMatrix();
        
    }
    
    if(mIsDrawEdge)
    {
        for(int i=0; i<mParticles.size(); i++){
            
            if(mEnableColor){
                ofSetColor(mParticles[i].col);
            }else{
                ofSetColor(255);
            }
            
            int r = ofRandom(mParticles.size());
            int idx = mParticles[i].linkIndex;
            ofDrawLine(mParticles[i].pos, mParticles[idx].pos);
        }
    }
    
    rdtk::EndCamera();
}

void Emitter::drawImGui()
{
    ImGui::Checkbox("Enable Color", &mEnableColor);
    ImGui::Checkbox("Draw Edge", &mIsDrawEdge);
    ImGui::DragInt("Life", &mLife, 10, 50.0, 10000.0);
    ImGui::DragFloat("Size", &mSize, 1.0, 0.0, 20.0);
    ImGui::DragFloat("Damping", &mDamping, 0.01, 0.0, 1.0);
    ImGui::DragFloat("Timestep", &mTimestep, 0.01, 0.0, 1.0);
    ImGui::DragFloat("Gravity", &mGravity, 0.1f, -9.8f, 9.8f);
    ImGui::DragFloat("Threshold", &mThreshold, 0.1, 0.0, 100.0);
}

void Emitter::addParticle(ofVec3f pos, ofVec3f vel, float life)
{
    Particle p;
    p.pos = pos;
    p.vel = vel;
    p.life = life;
    p.col = errorToRGB(vel.length(), 0.0, 10);
    p.sumDistance = 0.0f;
    p.linkIndex = ofRandom(mParticles.size());
    mParticles.push_back(p);
}

float Emitter::gaussFunction(float sumDistance)
{
    float myu = 20.f;
    float sigma = 4.0f;
    float alpha = 1.0f/(sqrt(2.0f*M_PI)*sigma);
    float inside = - (sumDistance - myu)*(sumDistance - myu) / (2.0f * sigma*sigma);
    float gauss = alpha * exp(inside);
    return gauss;
}

ofColor Emitter::errorToRGB(float err, float errMin, float errMax)
{
    int r, g, b;
    float norm_err = (err - errMin) / (errMax - errMin);
    float H, Hi, f, p, q, t, S = 1.0f, V = 1.0f;
    
    H = 360.0f - (240.0f * norm_err + 120.0f);
    
    if(H < 0.0f)
    {
        H = 0.0f;
    }
    
    Hi = (float)floor(H / 60.0f);
    
    f = H / 60.0f - Hi;
    
    p = V * (1.0f - S);
    q = V * (1.0f - f * S);
    t = V * (1.0f - (1.0f - f) * S);
    
    r = g = b = 0;
    
    if(Hi == 0)
    {
        r = (int)(255.0f * V);
        g = (int)(255.0f * t);
        b = (int)(255.0f * p);
    }
    if(Hi == 1)
    {
        r = (int)(255.0f * q);
        g = (int)(255.0f * V);
        b = (int)(255.0f * p);
    }
    if(Hi == 2)
    {
        r = (int)(255.0f * p);
        g = (int)(255.0f * V);
        b = (int)(255.0f * t);
    }
    if(Hi == 3)
    {
        r = (int)(255.0f * p);
        g = (int)(255.0f * q);
        b = (int)(255.0f * V);
    }
    if(Hi == 4)
    {
        r = (int)(255.0f * t);
        g = (int)(255.0f * p);
        b = (int)(255.0f * V);
    }
    if(Hi == 5)
    {
        r = (int)(255.0f * V);
        g = (int)(255.0f * p);
        b = (int)(255.0f * q);
    }
    
    ofColor error(r,g,b);
    return error;
}
