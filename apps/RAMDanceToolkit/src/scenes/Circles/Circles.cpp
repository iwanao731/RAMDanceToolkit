#include "Circles.h"

Circles::Circles(){
}

void Circles::setupControlPanel(){

    player.load("1.aif");
    player.setLoop(true);
    
    numCircle = 100;
    checkSetPort = false;

    ofEnableDepthTest();
    ofSetSmoothLighting(true);
    
    spotLight.setSpotlight();
    spotLight.setSpotlightCutOff(50);
    spotLight.setSpotConcentration(45);
    spotLight.setAttenuation(1,0,0);
    spotLight.setSpecularColor(ofColor(255));
    spotLight.setDiffuseColor(ofColor(255));
    spotLight.setAmbientColor(ofColor(100));
    
    //material
    material.setShininess(120);
    material.setSpecularColor(ofColor(0,0,0));
    material.setDiffuseColor(ofColor(0,0,200));
    material.setAmbientColor(ofColor(0,0,100));
    
    
    ofPushMatrix();
    ofRotate(90.0f, 1.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 0);
    
    for (int i = 0;i < numCircle;i++){
        floorCircle newCircle;
        float ranNum_c = ofRandom(0, 255);
        ofColor myColor = ofColor(255,ranNum_c,255);
        float ranNum_x = ofRandom(-300, 300);
        float ranNum_y = ofRandom(-300, 300);
        float ranNum_r = ofRandom(40, 100);
        
        float ranNum = ofRandom(0,1);
        bool myDir;
        if (ranNum >0.5){
            myDir = -1;
        }else{
            myDir = 1;
        }
        newCircle.setup(ranNum_x,ranNum_y,ranNum_r,myColor,myDir);
        circles.push_back(newCircle);
    }
    
    ofPopMatrix();
    
    ofxUICanvas* gui = rdtk::GetGUI().getCurrentUIContext();
    motionExtractor.setupControlPanel(this);
	
}

void Circles::drawImGui()
{

    motionExtractor.drawImGui();
}

void Circles::update(){

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
    
//    for (int i=0 ; i<23; i++){
//        checkingCircles(i);
//    }
    checkingCircles(7);
    checkingCircles(1);
//    checkingCircles(11);

}

void Circles::draw(){

    ofEnableDepthTest();
    
    rdtk::BeginCamera();

    example_drawBars(9, 22, ofColor(255), 0,0);
    const ofVec3f centerPos(0, 0, 0);
    
    const ofVec3f mousePos(ofGetMouseX(), ofGetMouseY(), 400);
    spotLight.setPosition(mousePos);
    spotLight.lookAt(centerPos);
    ofPushMatrix();
    ofRotate(90.0f, 1.0f, 0.0f, 0.0f);
    
   
    //const ofVec3f mousePos(100, 400, 0);
    
    
    
    for (int i = 0;i < numCircle;i++){
        circles[i].draw();
    }
    ofPopMatrix();
    
//    if (mDrawPreview)    motionExtractor.draw();

//    example_drawUnderbars(9, 10, 6, 5, ofColor(255,0,255,100), ofColor(255,70,255,100), ofColor(255,140,255,100), ofColor(255,210,255,100), -4, 1, 3, -3);
    
//    example_drawUnderbars(10, 11, 7, 6, ofColor(255,0,255,100), ofColor(255,70,255,100), ofColor(255,140,255,100), ofColor(255,210,255,100), 2, 2, -0.75, -0.75);

    rdtk::EndCamera();

    ofDisableDepthTest();
}

void Circles::example_drawBars(int index1, int index2, ofColor color, float numX,float numY){
    
    
    ofVec3f vec_b = motionExtractor.getPositionAt(index2);
    ofVec3f vec_a = motionExtractor.getPositionAt(index1);
    ofVec3f new_vec_b_origin = ofVec3f(vec_b.x,0,vec_b.z);
    ofEnableLighting();
    spotLight.enable();
    material.begin();
    ofPushMatrix();
    //ofSetColor(255,0,0);
    ofDrawSphere(vec_b, 10);
    //ofSetColor(255,255,0);
    ofDrawSphere(vec_a, 5);
    ofPopMatrix();
    material.end();
    spotLight.disable();
    
    ofDisableLighting();
    
    spotLight.draw();
//    ofVec3f direction = motionExtractor.getNodeAt(index2).getGlobalPosition().normalize();
//    float speed = motionExtractor.getVelocitySpeedAt(index2)/10;
//    ofVec3f vec_r_shoulder = motionExtractor.getPositionAt(7);
//    ofVec3f vec_r_hand = motionExtractor.getPositionAt(8);
//
//    bool result;
//
////    for (int i = 0;i < numCircle; i++){
////
////    }
//    int i = 0;
//    while (i<numCircle){
//        float ranNum = ofRandom(0,1);
//        if (vec_r_shoulder.y < vec_r_hand.y){
//            result = circles[i].calculateDist(new_vec_b_origin, direction, speed);
//        }else{
//            result = circles[i].calculateDist(new_vec_b_origin, direction*(-1), speed);
//        }
//        if (result){
//            break;
//        }
//    }
//
//    player.setVolume(speed*5);
//
    ofSetColor(255);
}

void Circles::checkingCircles(int index2){
    
    ofVec3f vec_b = motionExtractor.getPositionAt(index2);
    ofVec3f new_vec_b_origin = ofVec3f(vec_b.x,0,vec_b.z);
    
    ofVec3f direction = motionExtractor.getNodeAt(index2).getGlobalPosition().normalize();
    float speed = motionExtractor.getVelocitySpeedAt(index2)/10;
    ofVec3f vec_r_shoulder = motionExtractor.getPositionAt(7);
    ofVec3f vec_r_hand = motionExtractor.getPositionAt(8);
    
//    for (int i = 0;i < numCircle;i++){
//        float ranNum = ofRandom(0,1);
//        if (vec_r_shoulder.y < vec_r_hand.y){
//            circles[i].calculateDist(new_vec_b_origin, direction, speed);
//        }else{
//            circles[i].calculateDist(new_vec_b_origin, direction*(-1), speed);
//        }
//    }
    bool result;
    int i = 0;
    while (i<numCircle){
        result = circles[i].calculateDist(new_vec_b_origin, direction, speed);
//        if (vec_r_shoulder.y < vec_r_hand.y){
//            result = circles[i].calculateDist(new_vec_b_origin, direction, speed);
//        }else{
//            result = circles[i].calculateDist(new_vec_b_origin, direction*(-1), speed);
//        }
        if (result){
            break;
        }else{
            i++;
        }
    }
    player.setVolume(speed*5);
    
    ofSetColor(255);
}

void Circles::example_drawUnderbars(int index1, int index2, int index3, int index4, ofColor color1, ofColor color2, ofColor color3, ofColor color4, float numX1, float numY1, float numX2, float numY2){

    example_drawBars(index1, index2, color1, numX1, numY1);
    example_drawBars(index3, index4, color2, numX2, numY2);
}


