#include "VisualStudio.h"

VisualStudio::VisualStudio(){
    
    mBox1H = 300;
    mBox2H = 400;
    mBox3H = 550;
    mBox4H = 1100;
    
}

void VisualStudio::setupControlPanel(){
    
    checkSetPort = false;
    
    ofxUICanvas* gui = rdtk::GetGUI().getCurrentUIContext();
    gui->addSlider("box1", 0, 1200, &mBox1H);
    gui->addSlider("box2", 0, 1200, &mBox2H);
    gui->addSlider("box3", 0, 1200, &mBox3H);
    gui->addSlider("box4", 0, 1200, &mBox4H);
    
    motionExtractor.setupControlPanel(this);
    
}

void VisualStudio::drawImGui()
{
    
    ImGui::DragFloat("box1", &mBox1H);
    ImGui::DragFloat("box2", &mBox2H);
    ImGui::DragFloat("box3", &mBox3H);
    ImGui::DragFloat("box4", &mBox4H);
    
    motionExtractor.drawImGui();
}

void VisualStudio::update(){
    
    /*=== update ===*/
    motionExtractor.update();
    
    if (!checkSetPort){
        
        //player.play();
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
    
    ramBeginCamera();
    
    //    if (mDrawPreview)    motionExtractor.draw();
    
    float box1Length = example_drawUnderbars(9, 10, 6, 5, ofColor(255,0,255,100), ofColor(255,100,255,100), ofColor(255,0,255), ofColor(255,180,255,100), -4, 1, 3, -3, mBox1H);
    
    float box2Length =example_drawUnderbars(10, 11, 7, 6, ofColor(255,0,255,100), ofColor(255,100,255,100), ofColor(255,0,255), ofColor(255,180,255,100), 2, 2, -0.75, -0.75, mBox2H);
    
    float box3Length = example_drawUnderbars(17, 22, 12, 8, ofColor(0,0,255,100), ofColor(100,100,255,100), ofColor(0,0,255), ofColor(140,140,255,100), 3, 3, 2, 2, mBox3H);
    
    float box4Length = example_drawUnderbars(19, 20, 17, 4, ofColor(0,0,255,100), ofColor(100,100,255,100), ofColor(0,0,255), ofColor(140,140,255,100), 1, -4, -2, -4, mBox4H);
    
    ramEndCamera();
    
}

void VisualStudio::example_drawBars(int index1, int index2, ofColor color, float numX,float numY, float height){
    
    ofMesh line;
    
    ofVec3f vec_a = motionExtractor.getPositionAt(index1);
    
    ofVec3f new_vec_a = ofVec3f(vec_a.x *numX,0,vec_a.z*numY);
    ofVec3f new_vec_a_up = ofVec3f(vec_a.x*numX,height,vec_a.z*numY);
    
    ofVec3f vec_b = motionExtractor.getPositionAt(index2);
    ofVec3f new_vec_b_origin = ofVec3f(vec_b.x,0,vec_b.z);
    
    ofVec3f direction = motionExtractor.getNodeAt(index2).getGlobalPosition().normalize();
    float speed = motionExtractor.getVelocitySpeedAt(index2)/10;
    
    ofVec3f new_vec_b = ofVec3f(vec_b.x*numX,0,vec_b.z*numY);
    ofVec3f new_vec_b_up = ofVec3f(vec_b.x*numX,height,vec_b.z*numY);
    ofVec3f vec_r_shoulder = motionExtractor.getPositionAt(19);
    ofVec3f vec_r_hand = motionExtractor.getPositionAt(22);
    
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

float VisualStudio::example_drawUnderbars(int index1, int index2, int index3, int index4, ofColor color1, ofColor color2, ofColor color3, ofColor color4, float numX1, float numY1, float numX2, float numY2, float height){
    
    ofMesh line,line2;
    
    example_drawBars(index1, index2, color1, numX1, numY1, height);
    example_drawBars(index3, index4, color2, numX2, numY2, height);
    
    ofVec3f vec_a = motionExtractor.getPositionAt(index1);
    ofVec3f new_vec_a = ofVec3f(vec_a.x *numX1,0,vec_a.z*numY1);
    ofVec3f new_vec_a_up = ofVec3f(vec_a.x *numX1,height,vec_a.z*numY1);
    
    ofVec3f vec_b = motionExtractor.getPositionAt(index2);
    ofVec3f new_vec_b = ofVec3f(vec_b.x*numX1,-1,vec_b.z*numY1);
    ofVec3f new_vec_b_up = ofVec3f(vec_b.x*numX1,height,vec_b.z*numY1);
    
    ofVec3f new_vec_a_b = (new_vec_a_up + new_vec_b_up)*0.5;
    
    ofVec3f vec_c = motionExtractor.getPositionAt(index3);
    ofVec3f new_vec_c = ofVec3f(vec_c.x*numX2,-2,vec_c.z*numY2);
    ofVec3f new_vec_c_up = ofVec3f(vec_c.x*numX2,height,vec_c.z*numY2);
    
    ofVec3f vec_d = motionExtractor.getPositionAt(index4);
    ofVec3f new_vec_d = ofVec3f(vec_d.x*numX2,-3,vec_d.z*numY2);
    ofVec3f new_vec_d_up = ofVec3f(vec_d.x*numX2,height,vec_d.z*numY2);
    
    ofVec3f new_vec_c_d = (new_vec_c_up + new_vec_d_up)*0.5;
    
    float boxLength = new_vec_c_d.distance(new_vec_a_b);
    float boxLengthMapped = ofMap(boxLength,0,640,0,1);
    
    line.addVertex(new_vec_a);
    line.addVertex(new_vec_b);
    line.addVertex(new_vec_c);
    line.addVertex(new_vec_d);
    
    line2.addVertex(new_vec_a_up);
    line2.addVertex(new_vec_b_up);
    line2.addVertex(new_vec_c_up);
    line2.addVertex(new_vec_d_up);
    
    ofSetColor(color3);
    line.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    line.addIndex(0);
    line.addIndex(1);
    line.addIndex(3);
    line.addIndex(2);
    line.draw();
    
    ofSetColor(color4);
    line2.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    line2.addIndex(0);
    line2.addIndex(1);
    line2.addIndex(3);
    line2.addIndex(2);
    line2.draw();
    
    ofSetColor(255);
    
    return boxLengthMapped;
}

