#include "ramMain.h"
#include "Extractor.h"

class VisualStudio : public rdtk::BaseScene{
public:
    
    VisualStudio();
    virtual ~VisualStudio(){};
    
    void setupControlPanel();
    void drawImGui();
    
    void update();
    void draw();
    bool checkSetPort;

    ofSoundPlayer player;

    inline string getName() const {
        return "VisualStudio";
    }
    
private:
    
    rdtk::MotionExtractor    motionExtractor;
    
    bool mDrawLines;
    bool mDrawTriangle;
    bool mDrawPreview;
    bool mDrawDump;
    
    void example_drawBars(int index1, int index2, ofColor color, float numX, float numY);
    void example_drawUnderbars(int index1, int index2, int index3, int index4, ofColor color1, ofColor color2, ofColor color3, ofColor color4, float numX1, float numY1, float numX2, float numY2);
    
};


