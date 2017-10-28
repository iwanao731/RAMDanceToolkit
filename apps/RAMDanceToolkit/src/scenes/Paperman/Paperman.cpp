//
//  boids.cpp
//  RAMDanceToolkit
//
//  Created by NAOYA IWAMOTO on 2017/10/26.
//
//

#include "Paperman.h"

Paperman::Paperman() :
    mIsAddPlane(false),
    mIsRemovePlane(false),
    mTimestep(0.33),
    mTrackDistance(256),
    mPlayingMethod(0), // automatic
    mManualControlMethod(0),   // velocity control in manual
    mEnableSound(false)
{
    ofSetVerticalSync(true);
    this->modelingPlane(mMesh);
    this->addPlane();
}

void Paperman::setup()
{
    mEx.setup(this);
    
    mWEAudioL.assign(mTrackDistance, 0.0);
    mWEAudioR.assign(mTrackDistance, 0.0);
}

void Paperman::update()
{
    mEx.update();

    // automatic
    if(mPlayingMethod == 0)
    {
        this->updateAuto();
    }
    // manually
    else if(mPlayingMethod == 1)
    {
        this->updateManual();
    }
    
    if(mIsAddPlane){
        addPlane();
        mIsAddPlane = false;
    }
    
    if(mIsRemovePlane){
        removePlane();
        mIsRemovePlane = false;
    }
}

void Paperman::draw()
{
    rdtk::BeginCamera();

    for(auto &p : mPlanes)
    {
        ofPushMatrix();
        
        ofSetLineWidth(2);
        ofSetColor(p.col);
        p.pathLines.draw();
    
        // translate and rotate to the current position and orientation
        ofTranslate(p.currentPos.x, p.currentPos.y, p.currentPos.z);
        
        if( (p.currentPos - p.previousPos).length() > 0.0 ) {
            rotateToNormal(p.currentPos - p.previousPos);
        }
        
        ofSetColor(255);
        mMesh.draw();

        ofPopMatrix();
    }
    
    mEx.draw();
    
    rdtk::EndCamera();
}

void Paperman::drawImGui()
{
    ImGui::Checkbox("Enable Sound", &mEnableSound);
    
    ImGui::DragFloat("Tracking distance", &mTrackDistance, 100.0, 0.0, 10000.0);

    ImGui::Checkbox("Add plane", &mIsAddPlane); ImGui::SameLine();
    ImGui::Checkbox("Remove plane", &mIsRemovePlane);
    
    // playing method
    ImGui::RadioButton("Auto play", &mPlayingMethod, 0); ImGui::SameLine();
    ImGui::RadioButton("Manual play", &mPlayingMethod, 1);
    
    // control method in manual play
    ImGui::RadioButton("Control Velocity", &mManualControlMethod, 0); ImGui::SameLine();
    ImGui::RadioButton("Control Direction", &mManualControlMethod, 1);
    
    mEx.drawImGui();
}

void Paperman::updateAuto()
{
    for(auto& p : mPlanes)
    {
        p.previousPos = p.currentPos;
        
        // generate a noisy 3d position over time
        float t = (2 + ofGetElapsedTimef()) * 0.5;
        p.currentPos.x = ofSignedNoise(t, 0, 0, p.index);
        p.currentPos.y = ofSignedNoise(0, t, 0, p.index);
        p.currentPos.z = ofSignedNoise(0, 0, t, p.index);
        p.currentPos *= 400; // scale from -1,+1 range to -400,+400
        
        // add the current position to the pathVertices deque
        p.pathVertices.push_back(p.currentPos);
        
        // if we have too many vertices in the deque, get rid of the oldest ones
        while(p.pathVertices.size() > mTrackDistance) {
            p.pathVertices.pop_front();
        }
        
        // clear the pathLines ofMesh from any old vertices
        p.pathLines.clear();
        
        // add all the vertices from pathVertices
        for(unsigned int i = 0; i < p.pathVertices.size(); i++) {
            p.pathLines.addVertex(p.pathVertices[i]);
        }
    }
}

void Paperman::checkNumPlaneWithActor()
{
    int numPeople = getNumNodeArray();
    
    while(numPeople != mPlanes.size())
    {
        // check the number of plane
        if(mPlanes.size() < numPeople){
            this->addPlane();
        }else if(mPlanes.size() > numPeople){
            this->removePlane();
        }
    }
}

void Paperman::updateManual()
{
    //checkNumPlaneWithActor();
    // number of people == number of plane
//    for(int i=0; i<getNumNodeArray(); i++)
//    {
//        const rdtk::NodeArray &NA = getNodeArray(i);
//        
//        // number of joints
//        
//        if(mPlanes[i].index < NA.getNumNode()){
//            
//            const rdtk::Node &node = NA.getNode(17);
//            
//            ofVec3f v = (node.getGlobalPosition() - node.getParent()->getGlobalPosition()).normalize();
//            
//            auto& p = mPlanes[i];
//            
//            p.previousPos = p.currentPos; // save for next frame
//            
//            p.currentPos += v / mTimestep;
//            
//            // add the current position to the pathVertices deque
//            p.pathVertices.push_back(p.currentPos);
//            
//            // if we have too many vertices in the deque, get rid of the oldest ones
//            while(p.pathVertices.size() > 200) {
//                p.pathVertices.pop_front();
//            }
//            
//            // clear the pathLines ofMesh from any old vertices
//            p.pathLines.clear();
//            
//            // add all the vertices from pathVertices
//            for(unsigned int i = 0; i < p.pathVertices.size(); i++) {
//                p.pathLines.addVertex(p.pathVertices[i]);
//            }
//        }
//    }
    
    // picking joint drawing
    for (int i = 0;i < mEx.getNumPort();i++) {
        
        //! Get extractor informations
        const ofMatrix4x4 mat = mEx.getNodeAt(i).getGlobalTransformMatrix();
        const ofVec3f pos = mEx.getPositionAt(i);
        const ofVec3f next = mEx.getPositionAt((i+1) % mEx.getNumPort());
        ofVec3f vel = mEx.getVelocityAt(i);
        ofVec3f p0 = mEx.getNodeAt(i).getParent()->getGlobalPosition();
        ofVec3f p1 = mEx.getNodeAt(i).getGlobalPosition();
        
        ofVec3f direction = (p1 - p0).normalize();
        
        auto& p = mPlanes[i];
        p.previousPos = p.currentPos; // save for next frame
        
        // velocity control
        if(mManualControlMethod == 0)
        {
            //vel.x = -vel.x;
            p.currentPos += vel / mTimestep;
        }
        // direction control
        else if(mManualControlMethod == 1)
        {
            p.currentPos += direction / mTimestep;
        }
        
        // add the current position to the pathVertices deque
        p.pathVertices.push_back(p.currentPos);

        // if we have too many vertices in the deque, get rid of the oldest ones
        while(p.pathVertices.size() > mTrackDistance) {
            p.pathVertices.pop_front();
        }

        // clear the pathLines ofMesh from any old vertices
        p.pathLines.clear();

        // add all the vertices from pathVertices
        for(unsigned int i = 0; i < p.pathVertices.size(); i++) {
            p.pathLines.addVertex(p.pathVertices[i]);
        }
    }
}

void Paperman::modelingPlane(ofMesh& mesh)
{
    // vertices
    mesh.addVertex(ofVec3f(0, 0, 0));
    mesh.addVertex(ofVec3f(-20, 0, -60));
    mesh.addVertex(ofVec3f(-2, 0,  -60));
    mesh.addVertex(ofVec3f(0, 13, -60));
    mesh.addVertex(ofVec3f(2, 0, -60));
    mesh.addVertex(ofVec3f(20, 0, -60));
    
    // normals
    mesh.addNormal(ofVec3f(0, -0.0285722, 0.999592));
    mesh.addNormal(ofVec3f(0, 0, 1));
    mesh.addNormal(ofVec3f(0.650801, -0.0216934, 0.758938));
    mesh.addNormal(ofVec3f(0, -0.211753, 0.977323));
    mesh.addNormal(ofVec3f(-0.650801, -0.0216934, 0.758938));
    mesh.addNormal(ofVec3f(0, 0, 1));
    
    // indices
    mesh.addIndex(0);
    mesh.addIndex(1);
    mesh.addIndex(2);
    mesh.addIndex(0);
    mesh.addIndex(2);
    mesh.addIndex(3);
    mesh.addIndex(0);
    mesh.addIndex(3);
    mesh.addIndex(4);
    mesh.addIndex(0);
    mesh.addIndex(4);
    mesh.addIndex(5);
}

void Paperman::setScalePlane(const float scale)
{
    for(auto& p : mMesh.getVertices())
    {
        p *= scale;
    }
}

void Paperman::addPlane()
{
    Plane n;
    
    // draw the vertices in pathLines as a line strip
    n.pathLines.setMode(OF_PRIMITIVE_LINE_STRIP);
    n.index = mPlanes.size();
    
    if(mPlanes.size() == 0) {
        n.col = ofColor::fromHex(0x00abec); // cyan
    }else{
        n.col = ofColor::fromHsb(ofRandom(255), 255, 255);
    }

    mPlanes.push_back(n);
}

void Paperman::removePlane()
{
    if(mPlanes.size() > 0)
        mPlanes.erase(mPlanes.begin());
}

void Paperman::resetPos()
{
    for(auto &p : mPlanes){
        p.previousPos = ofVec3f::zero();
        p.currentPos = ofVec3f::zero();
    }
}

void Paperman::audioOut(float * output, int bufferSize, int nChannels)
{
    if(!mEnableSound) return;
    
    int i=0;
    while(i < mPlanes[0].pathLines.getNumVertices()){
        float x = mPlanes[0].pathVertices[i].x;
        float y = mPlanes[0].pathVertices[i].y;
        mWEAudioL[i] = output[i*nChannels    ] = x;
        mWEAudioR[i] = output[i*nChannels + 1] = y;
        i++;
    }
}
