//
//  ramMotionExtractorExampleScene.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/05.
//
//

#include "VisualStudio.h"

VisualStudio::VisualStudio(){
    
    mDrawLines        = true;
    mDrawTriangle    = true;
    mDrawPreview    = true;
    mDrawDump        = true;
    
}

void VisualStudio::setupControlPanel(){
    
    player.load("1.aif");
    player.setLoop(true);
    //player.play();
    
    checkSetPort = false;
    
    ofxUICanvas* gui = rdtk::GetGUI().getCurrentUIContext();
    gui->addToggle("Lines"        , &mDrawLines, 20, 20);
    gui->addToggle("Triangle"    , &mDrawTriangle, 20, 20);
    gui->addToggle("Preview"    , &mDrawPreview, 20, 20);
    gui->addToggle("Dump"        , &mDrawDump, 20, 20);
    
    motionExtractor.setupControlPanel(this);
    
}

void VisualStudio::drawImGui()
{
    ImGui::Checkbox("Preview", &mDrawPreview);
    ImGui::Checkbox("Dump", &mDrawDump);
    ImGui::Checkbox("Lines", &mDrawLines);
    ImGui::Checkbox("Triangle", &mDrawTriangle);
    motionExtractor.drawImGui();
}

void VisualStudio::update(){
    
    /*=== update ===*/
    motionExtractor.update();
    
    if (!checkSetPort){
        
        player.play();
        
        for (int i=0 ; i<23; i++){
            rdtk::NodeIdentifer n;
            n.set("jin" , i);
            rdtk::MotionPort* mp = new rdtk::MotionPort(n);
            motionExtractor.pushPort(mp);
            
        }
        
        checkSetPort = true;
        
    }
    
    
}

void VisualStudio::draw(){
    
    rdtk::BeginCamera();
    
    if (mDrawPreview)    motionExtractor.draw();
    
    example_drawUnderbars(9, 10, 6, 5, ofColor(255,70,255,150), ofColor(255,180,255,150), ofColor(255,0,255), ofColor(255,210,255,100), -4, 1, 3, -3);
    
    example_drawUnderbars(10, 11, 7, 6, ofColor(255,70,255,150), ofColor(255,180,255,150), ofColor(255,0,255), ofColor(255,210,255,100), 2, 2, -0.75, -0.75);
    
    example_drawUnderbars(17, 22, 12, 8, ofColor(140,140,255,150), ofColor(70,70,255,150), ofColor(0,0,255), ofColor(210,210,255,100), 3, 3, 2, 2);
    
    example_drawUnderbars(19, 20, 17, 4, ofColor(140,140,255,150), ofColor(70,70,255,150), ofColor(0,0,255), ofColor(210,210,255,100), 1, -4, -2, -4);

    rdtk::EndCamera();
    
}

void VisualStudio::example_drawBars(int index1, int index2, ofColor color, float numX,float numY){
    
    ofMesh line;
    
    ofVec3f vec_a = motionExtractor.getPositionAt(index1);
    
    ofVec3f new_vec_a = ofVec3f(vec_a.x *numX,0,vec_a.z*numY);
    ofVec3f new_vec_a_up = ofVec3f(vec_a.x*numX,300,vec_a.z*numY);
    
    ofVec3f vec_b = motionExtractor.getPositionAt(index2);
    ofVec3f new_vec_b_origin = ofVec3f(vec_b.x,0,vec_b.z);
    
    ofVec3f direction = motionExtractor.getNodeAt(index2).getGlobalPosition().normalize();
    float speed = motionExtractor.getVelocitySpeedAt(index2)/10;
    
    ofVec3f new_vec_b = ofVec3f(vec_b.x*numX,0,vec_b.z*numY);
    ofVec3f new_vec_b_up = ofVec3f(vec_b.x*numX,300,vec_b.z*numY);
    ofVec3f vec_r_shoulder = motionExtractor.getPositionAt(19);
    ofVec3f vec_r_hand = motionExtractor.getPositionAt(22);
    
    player.setVolume(speed*5);
    
    line.addVertex(new_vec_a);
    line.addVertex(new_vec_a_up);
    
    line.addVertex(new_vec_b);
    line.addVertex(new_vec_b_up);
    
    ofSetColor(color);
    line.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    line.addIndex(0);
    line.addIndex(1);
    line.addIndex(3);
    line.addIndex(2);
    line.draw();
    
    ofSetColor(255);
}

void VisualStudio::example_drawUnderbars(int index1, int index2, int index3, int index4, ofColor color1, ofColor color2, ofColor color3, ofColor color4, float numX1, float numY1, float numX2, float numY2){
    
    ofMesh line,line2;
    
    example_drawBars(index1, index2, color1, numX1, numY1);
    example_drawBars(index3, index4, color2, numX2, numY2);
    
    ofVec3f vec_a = motionExtractor.getPositionAt(index1);
    ofVec3f new_vec_a = ofVec3f(vec_a.x *numX1,0,vec_a.z*numY1);
    //ofVec3f new_vec_a_up = ofVec3f(vec_a.x*numX,400,vec_a.z*numY);
    
    ofVec3f vec_b = motionExtractor.getPositionAt(index2);
    ofVec3f new_vec_b = ofVec3f(vec_b.x*numX1,0,vec_b.z*numY1);
    
    ofVec3f vec_c = motionExtractor.getPositionAt(index3);
    ofVec3f new_vec_c = ofVec3f(vec_c.x*numX2,0,vec_c.z*numY2);
    
    ofVec3f vec_d = motionExtractor.getPositionAt(index4);
    ofVec3f new_vec_d = ofVec3f(vec_d.x*numX2,0,vec_d.z*numY2);
    //ofVec3f new_vec_b_up = ofVec3f(vec_b.x*numX,400,vec_b.z*numY);
    
    
    line.addVertex(new_vec_a);
    line.addVertex(new_vec_b);
    line.addVertex(new_vec_c);
    line.addVertex(new_vec_d);
    
    ofSetColor(color3);
    line.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    line.addIndex(0);
    line.addIndex(1);
    line.addIndex(3);
    line.addIndex(2);
    line.draw();
    
    ofSetColor(255);
}



