#pragma once

class VisualStudio : public rdtk::BaseScene
{
    vector<float> mSizeArray;
    float mBox1Height;
    float mBox2Height;

    bool mUseSingleColor;
    ofFloatColor mColor;
    
public:
    
    VisualStudio() : mBox1Height(200.0), mBox2Height(1250.0), mColor(255, 0, 255)
    {
        mSizeArray.clear();
        mSizeArray.resize(8);
        
        mSizeArray.at(0) = 2.0;
        mSizeArray.at(1) = 2.0;
        mSizeArray.at(2) = 3.0;
        mSizeArray.at(3) = 3.0;
        mSizeArray.at(4) = -2.5;
        mSizeArray.at(5) = -2.5;
        mSizeArray.at(6) = -1.0;
        mSizeArray.at(7) = 4.0;
    }
    
    void setupControlPanel()
    {
#ifdef RAM_GUI_SYSTEM_OFXUI
        
        //rdtk::GetGUI().addToggle("Use single color", &mUseSingleColor);
        rdtk::GetGUI().addColorSelector("Master color", &mColor);
        rdtk::GetGUI().addSlider("Box1 height", 0.0, 1500.0, &mBox1Height);
        rdtk::GetGUI().addSlider("Box2 height", 0.0, 1500.0, &mBox1Height);

        
        rdtk::GetGUI().addSlider("distance1x", -6.0, 6.0, &mSizeArray.at(0));
        rdtk::GetGUI().addSlider("distance1y", -6.0, 6.0, &mSizeArray.at(1));
        rdtk::GetGUI().addSlider("distance2x", -6.0, 6.0, &mSizeArray.at(2));
        rdtk::GetGUI().addSlider("distance2y", -6.0, 6.0, &mSizeArray.at(3));
        rdtk::GetGUI().addSlider("distance3x", -6.0, 6.0, &mSizeArray.at(4));
        rdtk::GetGUI().addSlider("distance3y", -6.0, 6.0, &mSizeArray.at(5));
        rdtk::GetGUI().addSlider("distance4x", -6.0, 6.0, &mSizeArray.at(6));
        rdtk::GetGUI().addSlider("distance4y", -6.0, 6.0, &mSizeArray.at(7));

        ofAddListener(rdtk::GetGUI().getCurrentUIContext()->newGUIEvent, this, &VisualStudio::onPanelChanged);
        
#endif
    }
    
    void onPanelChanged(ofxUIEventArgs& e)
    {
        string name = e.widget->getName();
\
    }
    
    void drawImGui()
    {

        ImGui::ColorEdit3("Master color", &mColor[0]);
        ImGui::DragFloat("Box1 height", &mBox1Height, 10, 0.0, 1500.0);
        ImGui::DragFloat("Box2 height", &mBox2Height, 10, 0.0, 1500.0);
        
        ImGui::Columns(2, NULL, true);

        ImGui::DragFloat("distance1x", &mSizeArray.at(0), 0.1, -6, 6);
        ImGui::NextColumn();
        ImGui::DragFloat("distance1y", &mSizeArray.at(1), 0.1, -6, 6);
        ImGui::NextColumn();
        ImGui::DragFloat("distance2x", &mSizeArray.at(2), 0.1, -6, 6);
        ImGui::NextColumn();
        ImGui::DragFloat("distance2y", &mSizeArray.at(3), 0.1, -6, 6);
        ImGui::NextColumn();
        ImGui::DragFloat("distance3x", &mSizeArray.at(4), 0.1, -6, 6);
        ImGui::NextColumn();
        ImGui::DragFloat("distance3y", &mSizeArray.at(5), 0.1, -6, 6);
        ImGui::NextColumn();
        ImGui::DragFloat("distance4x", &mSizeArray.at(6), 0.1, -6, 6);
        ImGui::NextColumn();
        ImGui::DragFloat("distance4y", &mSizeArray.at(7), 0.1, -6, 6);

        ImGui::Columns(1);
        
    }
    
    void draw()
    {
        rdtk::BeginCamera();
        
        for(int i=0; i<getNumNodeArray(); i++)
        {
            const rdtk::NodeArray &NA = getNodeArray(i);
            drawVisualStudio(NA, i);
        }
        
        rdtk::EndCamera();
    }
    
    void drawVisualStudio(const rdtk::NodeArray& NA, int colorNum)
    {
        ofMesh line, line2, line3, line4, line5, line6, line7;
        ofVec3f savedPos = ofVec3f(0,0,0);
        ofVec3f newPos;
        for (int i=0; i<NA.getNumNode(); i++)
        {
            const int keyJoint = NA.isActor() ? rdtk::Actor::JOINT_HEAD : 0;
            
            const rdtk::Node &node = NA.getNode(i);
            
            ofVec3f pos = node.getPosition();
            newPos = savedPos + pos;

            //hands
            if (i == 17 || i == 22){
                line.addVertex(ofVec3f(newPos.x*mSizeArray.at(0),mBox1Height,newPos.z*mSizeArray.at(1)));
                line.addVertex(ofVec3f(newPos.x*mSizeArray.at(0),0,newPos.z*mSizeArray.at(1)));
                line5.addVertex(ofVec3f(newPos.x*mSizeArray.at(0),0,newPos.z*mSizeArray.at(1)));
                line7.addVertex(ofVec3f(newPos.x*mSizeArray.at(0),mBox1Height,newPos.z*mSizeArray.at(1)));
            }
            //toes
            if (i == 8 || i == 12){
                line2.addVertex(ofVec3f(newPos.x*mSizeArray.at(2),mBox2Height,newPos.z*mSizeArray.at(3)));
                line2.addVertex(ofVec3f(newPos.x*mSizeArray.at(2),0,newPos.z*mSizeArray.at(3)));
                line6.addVertex(ofVec3f(newPos.x*mSizeArray.at(2),0,newPos.z*mSizeArray.at(3)));
            }
            //ankles
            if (i == 7 || i == 11){
                line3.addVertex(ofVec3f(newPos.x*mSizeArray.at(4),mBox1Height,newPos.z*mSizeArray.at(5)));
                line3.addVertex(ofVec3f(newPos.x*mSizeArray.at(4),0,newPos.z*mSizeArray.at(5)));
                line5.addVertex(ofVec3f(newPos.x*mSizeArray.at(4),0,newPos.z*mSizeArray.at(5)));
                line7.addVertex(ofVec3f(newPos.x*mSizeArray.at(4),mBox1Height,newPos.z*mSizeArray.at(5)));
            }
            //shoulders
            if (i == 14 || i == 19){
                line4.addVertex(ofVec3f(newPos.x*mSizeArray.at(6),mBox2Height,newPos.z*mSizeArray.at(7)));
                line4.addVertex(ofVec3f(newPos.x*mSizeArray.at(6),0,newPos.z*mSizeArray.at(7)));
                line6.addVertex(ofVec3f(newPos.x*mSizeArray.at(6),0,newPos.z*mSizeArray.at(7)));
            }
            
            savedPos = newPos;
            
        }

        
        ofFill();
        if (colorNum%2 == 0){
            ofSetColor(255, 0, 255,100);
        }else{
            ofSetColor(0, 0, 255,100);
        }
        
        line.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        line.addIndex(0);
        line.addIndex(1);
        line.addIndex(3);
        line.addIndex(2);
        
        line.draw();
        
        if (colorNum%2 == 0){
            ofSetColor(255,70,255,100);
        }else{
            ofSetColor(70,70,255,100);
        }
        
        line2.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        line2.addIndex(0);
        line2.addIndex(1);
        line2.addIndex(3);
        line2.addIndex(2);
        
        line2.draw();
        
        if (colorNum%2 == 0){
            ofSetColor(255,140,255,100);
        }else{
            ofSetColor(140,140,255,100);
        }
        
        line3.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        line3.addIndex(0);
        line3.addIndex(1);
        line3.addIndex(3);
        line3.addIndex(2);
        
        line3.draw();
        
        if (colorNum%2 == 0){
            ofSetColor(255,210,255,100);
        }else{
            ofSetColor(210,210,255,100);
        }
        
        line4.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        line4.addIndex(0);           // 10
        line4.addIndex(1);           // 1
        line4.addIndex(3);       // 11
        line4.addIndex(2);           // 10
        
        line4.draw();
        
        if (colorNum%2 == 0){
            ofSetColor(255,0,255);
        }else{
            ofSetColor(0,0,255);
        }
        
        //ofSetColor(255,0,255);
        line5.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        line5.addIndex(0);
        line5.addIndex(1);
        line5.addIndex(3);
        line5.addIndex(2);
        
        line5.draw();
        
        //ofSetColor(255,0,255);
        line6.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        line6.addIndex(0);
        line6.addIndex(1);
        line6.addIndex(3);
        line6.addIndex(2);
        
        line6.draw();
        
        
        if (colorNum%2 == 0){
            ofSetColor(255,0,255,100);
        }else{
            ofSetColor(0,0,255,100);
        }
        
        line7.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        line7.addIndex(0);
        line7.addIndex(1);
        line7.addIndex(3);
        line7.addIndex(2);
        
        line7.draw();
    }
    
    string getName() const { return "VisualStudio"; }
};

