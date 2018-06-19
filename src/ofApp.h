#pragma once

#include "ofMain.h"
#include "Pendulum.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp{

public:

    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void mass1Updated(float &mass);
    void mass2Updated(float &mass);
    void length1Updated(float &length);
    void length2Updated(float &length);
    void gravityUpdated(float &g);
    void dtUpdated(float &dt);
    void dragUpdated(float &drag);
    void audioOut(ofSoundBuffer & buffer);

    ofxLabel angle1, angle2;
    ofxLabel velocity1, velocity2;
    ofxLabel acceleration1, acceleration2;
    ofxLabel end1x, end1y;

    ofSoundStream soundStream;

    Pendulum *pendulum;
    Pendulum::State state;
    ofFbo fbo;
    ofFbo chartFbo;

    float gravity;
    float dt;
    float drag;

    ofxFloatSlider massScale;
    ofxFloatSlider lengthScale;
    ofxFloatSlider seg1Mass;
    ofxFloatSlider seg2Mass;
    ofxFloatSlider seg1Length;
    ofxFloatSlider seg2Length;
    ofxFloatSlider gravitySlider;
    ofxFloatSlider dragSlider;

    ofxFloatSlider dtSlider;

    ofxButton reset;

    ofxPanel gui;

    bool bHide;

};
