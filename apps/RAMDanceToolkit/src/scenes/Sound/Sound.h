#pragma once

#include "ramMain.h"

class Sound : public rdtk::BaseScene {
public:
    
    Sound();
    void setup();
    void update();
    void draw();
    void drawImGui();
    string getName() const { return "Sound"; }
    
    void audioOut(float * input, int bufferSize, int nChannels);

private:
    
    ofSoundStream soundStream;
    
    float 	pan;
    int		sampleRate;
    bool 	bNoise;
    float 	volume;
    
    vector <float> lAudio;
    vector <float> rAudio;
    
    //------------------- for the simple sine wave synthesis
    float 	targetFrequency;
    float 	phase;
    float 	phaseAdder;
    float 	phaseAdderTarget;
};
