//
//  DDA.cpp
//  RAMDanceToolkit
//

#include "DDA.h"

DDA::DDA() :
    mDrawImage(false),
    mBoxLineWidth(2.0),
    mMasterBoxSize(10.0)
{
    mSizeArray.clear();
    mSizeArray.resize(rdtk::Actor::NUM_JOINTS);
    for (int i=0; i<mSizeArray.size(); i++)
        mSizeArray.at(i) = mMasterBoxSize;
    
    // image
    ofImage img;
    img.load("oshushi.jpg");
    img.resize(40, 40);
    
    for (int i=0; i<mSizeArray.size(); i++)
        mImages.push_back(img);
}

void DDA::onPanelChanged(ofxUIEventArgs& e)
{
    string name = e.widget->getName();
    
    if (name == "Master box size")
    {
        for (int i=0; i<mSizeArray.size(); i++)
            mSizeArray.at(i) = mMasterBoxSize;
    }
}

void DDA::drawImGui()
{
    ImGui::Checkbox("Draw Image", &mDrawImage);    
}

void DDA::draw()
{
    rdtk::BeginCamera();
    
    for(int i=0; i<getNumNodeArray(); i++)
    {
        const rdtk::NodeArray &NA = getNodeArray(i);
        drawBigBox(NA);
    }
    
    rdtk::EndCamera();
}

void DDA::drawBigBox(const rdtk::NodeArray& NA)
{
    ofMatrix4x4 mat1;
    for (int i=0; i<NA.getNumNode(); i++)
    {
        const int keyJoint = NA.isActor() ? rdtk::Actor::JOINT_HEAD : 0;
        
        const rdtk::Node &node = NA.getNode(i);
        float boxSize = (i==keyJoint) ? 6 : 3;
        float bigBoxSize = mSizeArray.at(i);
        
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushMatrix();
        {
            ofPushStyle();
            ofNoFill();
            
            glEnable(GL_DEPTH_TEST);

            // model view matrix of camera(rotation and translation)
            auto MVM = this->getCameraManager().getCamera(0)->getModelViewMatrix();
            
            // joint's global tranform matrix (rotation and translation)
            auto GTM = node.getGlobalTransformMatrix();
            
            ofMatrix4x4 m = GTM * GTM.getInverse().getRotate() * MVM.getInverse().getRotate();
            glMultMatrixf(m.getPtr());

            if(mDrawImage){
                mImages[i].draw(0,0,0);
            }else{
                ofSetLineWidth(mBoxLineWidth);
                ofDrawBox(bigBoxSize);
                ofDrawAxis(bigBoxSize);
            }
            ofPopStyle();
        }
        
        glPopMatrix();
        glPopAttrib();
    }
}
