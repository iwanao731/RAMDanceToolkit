
#pragma once

#include "ramMain.h"

class MakeSound : public rdtk::BaseScene {
public:
    void drawImGui(){
        ImGui::Checkbox("sound", &isSound);
        ImGui::Checkbox("draw line", &isDrawLine);
        ImGui::SliderFloat("volume", &volume, 0., 1.);
        ImGui::Checkbox("head synth", &isHeadSynth);
        
    }
    void setup(){
        vector<ofVec3f> hl;
        hl.assign(6, ofVec3f());
        handsline.assign(3, hl);
        
        soundline.assign(3, ofPolyline());
        
        vector<float> v;
        v.assign(256, 0.);
        soundBuffer.assign(3, v);
        
        frequency.assign(3, 1.);
        phase.assign(3, 0.);;
        phaseAdder.assign(3, 0.01);;
    };
    void update(){
        ofVec3f actor1Position;
        
        for (int j = 0; j < getNumNodeArray(); j++) {
            rdtk::NodeArray actor = getNodeArray(j);
            
            for (int i = 0; i < pointsIndex.size(); i++) {
                ofVec3f p = actor.getNode(pointsIndex[i]).getGlobalPosition();
                handsline[j][i] = p;
            }
            
            float least = handsline[j][0].y;
            float most = handsline[j][0].y;
            
            for (int i = 1; i < pointsIndex.size(); i++) {
                
                float value = handsline[j][i].y;
                if (least > value) least = value;
                if (most < value) most = value;
                
            }
            
            soundline[j].clear();
            
            for (int i = 0; i < pointsIndex.size(); i++) {
                
                float val = ofMap(handsline[j][i].y, least, most, -1., 1.);
                soundline[j].curveTo(1. * i, val);
                
            }
            
            soundline[j] = soundline[j].getResampledByCount(128 + 1);
            
            int divNum = floor(256 / (frequency[j] * 2));
            divNum = divNum == 0 ? 1 : divNum;
            int nind = 0;
            bool isEven = true;
            int index = 0;
                
            for (int i = 0; i < 256; i++) {
                    
                nind = (i % divNum) * frequency[j];
                int num = (int)floor(i / divNum);
                isEven = (num % 2) == 0;
                    
                if (isEven) {
                    index = nind;
                } else {
                    index = 127 - nind;
                }
                if(j<soundBuffer.size()){
                    soundBuffer[j][i] = soundline[j][index].y * volume;
                }
                
            }
            
            if (phase[j] > TWO_PI) phase[j] -= TWO_PI;
            {
                ofVec3f p = actor.getNode(4).getGlobalPosition();
                if(j == 0){
                    actor1Position = p;
                }
                
                if(j == 1 ){
                    float d1 = actor1Position.distance(p);
                    float d2 = actor1Position.distance(p);
                    leftVolume = ofMap(d1,0,500,1.0,0,true);
                    rightVolume = ofMap(d2,0,500,1.0,0,true);
                }
            }
            if (isHeadSynth) {
                ofVec3f p = actor.getNode(4).getGlobalPosition();
             
                frequency[j] = ofMap(p.z, -300., 300., 0.1, 5.);
                phaseAdder[j] = ofMap(p.x, -300., 300., 0.01, 0.05);
            }
            
        }
        
        
    };
    
    void draw(){
        
        if (isDrawLine) {
            for (int j = 0; j < getNumNodeArray(); j++) {
                ofPushMatrix();
                ofScale(100, -100);
                ofTranslate(0, -1);
                ofSetColor(255);
                soundline[j].draw();
                ofPopMatrix();
            }
            
        }
    };
    
    void audioOut(float * output, int bufferSize, int nChannels){
        if (!isEnabled() || !isSound || getNumNodeArray() == 0) return;
        
        for (int j = 0; j < getNumNodeArray(); j++) {
            while (phase[j] > TWO_PI){
                phase[j] -= TWO_PI;
            }
        }
        int length = 2;//soundBuffer.size() < 2 ? soundBuffer.size() : 2;
        for (int i = 0; i < bufferSize; i++) {
            
            for (int j = 0; j < length ; j++) {
                output[i * nChannels] += (soundBuffer[j][i] * sin(phase[j]))*leftVolume;
                output[i * nChannels + 1] += soundBuffer[j][i] * sin(phase[j])*rightVolume;
                phase[j] += phaseAdder[j];
            }
            
        }
    };
    string getName() const { return "MakeSound"; }

private:
    vector<int> pointsIndex = {
        22, //rdtk::Actor::JOINT_RIGHT_HAND,
        21, //rdtk::Actor::JOINT_RIGHT_WRIST,
        20, //rdtk::Actor::JOINT_RIGHT_ELBOW,
        19, //rdtk::Actor::JOINT_RIGHT_SHOULDER,
        18, //rdtk::Actor::JOINT_RIGHT_COLLAR,
        
        3 //rdtk::Actor::JOINT_NECK,
        
    };
//    vector<int> pointsIndex = {
//        22, //rdtk::Actor::JOINT_RIGHT_HAND,
//        21, //rdtk::Actor::JOINT_RIGHT_WRIST,
//        20, //rdtk::Actor::JOINT_RIGHT_ELBOW,
//        19, //rdtk::Actor::JOINT_RIGHT_SHOULDER,
//        18, //rdtk::Actor::JOINT_RIGHT_COLLAR,
//
//        3, //rdtk::Actor::JOINT_NECK,
//
//        13, //rdtk::Actor::JOINT_LEFT_COLLAR,
//        14, //rdtk::Actor::JOINT_LEFT_SHOULDER,
//        15, //rdtk::Actor::JOINT_LEFT_ELBOW,
//        16, //rdtk::Actor::JOINT_LEFT_WRIST,
//        17  //rdtk::Actor::JOINT_LEFT_HAND
//    };
    vector<int> pointsIndex2 = {
        rdtk::Actor::JOINT_HEAD,
        rdtk::Actor::JOINT_NECK, //rdtk::Actor::JOINT_RIGHT_WRIST,
        rdtk::Actor::JOINT_CHEST, //rdtk::Actor::JOINT_RIGHT_ELBOW,
        rdtk::Actor::JOINT_ABDOMEN, //rdtk::Actor::JOINT_RIGHT_SHOULDER,
        rdtk::Actor::JOINT_HIPS, //rdtk::Actor::JOINT_RIGHT_COLLAR,
        
        rdtk::Actor::JOINT_LEFT_HIP, //rdtk::Actor::JOINT_NECK,
        
        rdtk::Actor::JOINT_HIPS, //rdtk::Actor::JOINT_LEFT_COLLAR,
        rdtk::Actor::JOINT_ABDOMEN, //rdtk::Actor::JOINT_LEFT_SHOULDER,
        rdtk::Actor::JOINT_CHEST, //rdtk::Actor::JOINT_LEFT_ELBOW,
        rdtk::Actor::JOINT_NECK, //rdtk::Actor::JOINT_LEFT_WRIST,
        rdtk::Actor::JOINT_HEAD  //rdtk::Actor::JOINT_LEFT_HAND
    };
    vector<vector<ofVec3f>> handsline;
    vector<ofPolyline> soundline;
    vector<vector<float>> soundBuffer;
    vector<float> frequency;
    vector<float> phase;
    vector<float> phaseAdder;
    
    bool isDrawLine = false;
    bool isSound = false;
    bool isHeadSynth = false;
    float volume = 0.1;
    float leftVolume = 0.1;
    float rightVolume = 0.1;
    
};
