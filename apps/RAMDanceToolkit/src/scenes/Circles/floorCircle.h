#pragma once

class floorCircle
{
    ofColor changeColor;
    ofColor originColor;
    ofVec2f coord;
    float radius;
    ofColor color;
    int dir;
    
public:
    
    floorCircle() : coord(0,0), radius(10)
    {
        
    }
    
    void setup(float x, float y, float inputRadius, ofColor myColor, int myDir){
       
        coord.x = x;
        coord.y = y;
        radius = inputRadius;
        color = myColor;
        originColor = myColor;
        dir = myDir;
    }
    void update(float tx, float ty){
        
    }
    bool calculateDist(ofVec3f footLoc, ofVec3f directionVector, float speed){
        bool result;
        result = false;
        float dist = footLoc.distance(ofVec3f(coord.x,0,coord.y));
        if (dist < radius){
            updateLoc(directionVector*dir, speed*3);
            result = true;
            float opacity = ofMap(speed,0,1,100,255);
            changeColor = ofColor(255,opacity);
            color = changeColor;
        }else{
            color = originColor;
        }
        return result;
    }
    void updateLoc(ofVec3f directionVector, float speed){
        coord.x += directionVector.x*speed;
        coord.y += directionVector.z*speed;
    }
    
    void draw(){
        ofSetColor(color);
        ofDrawCircle(coord,radius);
    }
    
    
};


