#include "ramMain.h"
#include "Extractor.h"
#include "floorCircle.h"

class Circles : public rdtk::BaseScene{
public:
	
	Circles();
	virtual ~Circles(){};

    void setupControlPanel();
    void drawImGui();

	void update();
	void draw();
    bool checkSetPort;
    ofVec2f circle1, circle2, circle3;
    ofSoundPlayer player;
    vector<floorCircle> circles;
    int numCircle;
    ofLight spotLight;
    ofMaterial material;
	inline string getName() const {
		return "Circles";
	}

private:

    rdtk::MotionExtractor	motionExtractor;
    void checkingCircles(int index2);
    void example_drawBars(int index1, int index2, ofColor color, float numX, float numY);
    void example_drawUnderbars(int index1, int index2, int index3, int index4, ofColor color1, ofColor color2, ofColor color3, ofColor color4, float numX1, float numY1, float numX2, float numY2);

};

