#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //setup sound
//    soundStream.printDeviceList();
    soundStream.setup(this,8,0,48000, 512, 16);

    //General setup
    ofSetVerticalSync(true);
    ofBackground(255,255,255);
    ofSetFrameRate(60);

    //SIM setup
    dt 		= 0.001;
    drag 	= 9.998;
    gravity = 9.81;
    state.g = gravity;
    state.seg1 = Segment(2.06,60);
    state.seg2 = Segment(5.06,100);
    state.seg1.angle = (float)PI;
    state.seg2.angle = (float)PI+0.001f;
    state.seg1.velocity = 0.01;
    state.seg2.velocity = 0.01;

    pendulum = new Pendulum(state);

    //FBO setup
    fbo.allocate(ofGetScreenWidth(), ofGetScreenHeight(), GL_RGBA);
    fbo.begin();
    ofClear(255, 255,255,0);
    fbo.end();

    chartFbo.allocate(ofGetScreenWidth() * 5,ofGetScreenHeight(), GL_RGBA);
    chartFbo.begin();
    ofClear(255,255,255,0);
    chartFbo.end();

    // GUI setup
    bHide = false;


    seg1Mass.addListener(this,&ofApp::mass1Updated);
    seg2Mass.addListener(this,&ofApp::mass2Updated);

    seg1Length.addListener(this, &ofApp::length1Updated);
    seg2Length.addListener(this, &ofApp::length2Updated);

    gravitySlider.addListener(this, &ofApp::gravityUpdated);

    dtSlider.addListener(this, &ofApp::dtUpdated);
    dragSlider.addListener(this, &ofApp::dragUpdated);

    gui.setup(); // most of the time you don't need a namxe
    gui.add(seg1Mass.setup("Mass 1", 5.0, 1.0, 10.0));
    gui.add(seg1Length.setup("Length 1", 5.0,1.0,20.0));
    gui.add(seg2Mass.setup("Mass 2", 5.0, 1.0, 10.0));
    gui.add(seg2Length.setup("Length 2", 7.0,1.0,20));
    gui.add(gravitySlider.setup("gravity", 9.98, 1, 20));
    gui.add(dtSlider.setup("dt", dt, 0.001, 0.1));
    gui.add(dragSlider.setup("drag", 0.01, 0.0, 0.01));
    gui.add(angle1.setup("angle1", "0"));
    gui.add(angle2.setup("angle2", "0"));
    gui.add(velocity1.setup("velocity1", "0"));
    gui.add(velocity2.setup("velocity2", "0"));
    gui.add(acceleration1.setup("acceleration1", "0"));
    gui.add(acceleration2.setup("acceleration2", "0"));
    gui.add(end1x.setup("posx", "0"));
    gui.add(end1y.setup("posy", "0"));

    gui.loadFromFile("settings.xml");

    ofEnableSmoothing();

//    soundStream.start();

}

//--------------------------------------------------------------
void ofApp::update(){
    pendulum->update(dt, drag);
    fbo.begin();
    //fade
    ofFill();
    ofSetColor(255,255,255,1);
    ofDrawRectangle(0,0,0,ofGetWidth(),ofGetHeight());
    //draw the trail
    if(ofGetFrameNum() > 1) {
        ofSetColor(10,10,60,100);
        ofSetLineWidth(2);
        ofTranslate(ofGetWidth()/2,500);
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofDrawLine(pendulum->state.seg2.prevEnd,pendulum->state.seg2.end);
        ofDisableBlendMode();
        ofEnableAlphaBlending();
    }
    fbo.end();


    float newAngle1 = pendulum->state.seg1.angle;
    float newAngle2 = pendulum->state.seg2.angle;
    float newVel1 = pendulum->state.seg1.velocity;
    float newVel2 = pendulum->state.seg2.velocity;
    float newAcc1 = pendulum->state.seg1.acceleration;
    float newAcc2 = pendulum->state.seg2.acceleration;

    angle1 			= ofToString(newAngle1);
    angle2 			= ofToString(newAngle2);
    velocity1 =  ofToString(newVel1);
    velocity2 =  ofToString(newVel2);
    acceleration1 	= ofToString(newAcc1);
    acceleration2 	= ofToString(newAcc2);
    end1x			= ofToString(atan2(pendulum->state.seg2.end.x, pendulum->state.seg2.end.y));

}

//--------------------------------------------------------------
void ofApp::draw(){

    fbo.draw(0,0);
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2,500);
    pendulum->draw();
    ofPopMatrix();
    if(!bHide){
        gui.draw();
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'h'){
        bHide = !bHide;
    }
    else if(key == 'r'){
        pendulum->state.seg1.angle = static_cast<float>(PI);
        pendulum->state.seg2.angle = static_cast<float>(PI + 0.001);
        pendulum->state.seg1.velocity = 0;
        pendulum->state.seg2.velocity = 0;
        fbo.begin();
        ofFill();
        ofSetColor(0);
        ofDrawRectangle(0,0,0,ofGetWidth(),ofGetHeight());
        fbo.end();
    }
    else if(key == 'p'){
        pendulum->state.seg1.velocity = 1;
        pendulum->state.seg2.velocity = 1;
    }
    else if(key == 'x'){
        pendulum->state.seg1.angle = static_cast<float>(PI);
        pendulum->state.seg2.angle = static_cast<float>(PI + 0.001);
        pendulum->state.seg1.velocity = 3;
        pendulum->state.seg2.velocity = 3;
        fbo.begin();
        ofFill();
        ofSetColor(0);
        ofDrawRectangle(0,0,0,ofGetWidth(),ofGetHeight());
        fbo.end();
    }
}

void ofApp::audioOut(ofSoundBuffer & buffer){

    float maxDistance = (pendulum->state.seg1.length + pendulum->state.seg2.length) * 10.0f;
    float diff  	  = abs(pendulum->state.seg1.length - pendulum->state.seg2.length) * 10.0f;
    for (size_t i = 0; i < buffer.getNumFrames(); i++){
        // seg2.end position on channel 1 + 2
        buffer[i*buffer.getNumChannels()    ] = static_cast<float>(pendulum->state.seg2.end.x / pendulum->state.seg2.length * 0.1);
        buffer[i*buffer.getNumChannels() +1 ] = static_cast<float>(pendulum->state.seg2.end.y / pendulum->state.seg2.length * 0.1);
        // seg2.end distance from seg1.start on 3
        float relDistance = (pendulum->state.seg2.end.distance(ofPoint(0,0)) - diff) / maxDistance;
        float relDistance2 = pendulum->state.seg2.end.distance(ofPoint(0,0)) / maxDistance;
        buffer[i*buffer.getNumChannels() + 2 ] = static_cast<float>(pow(relDistance, 2));
        // gate when seg2.end touches outer circle on 4
        if(relDistance2 > 0.99) buffer[i*buffer.getNumChannels() + 3] = 1;
        else 				  buffer[i*buffer.getNumChannels() + 3] = 0;
        // velocity of seg2.end on 5
        buffer[i*buffer.getNumChannels() + 4 ] = static_cast<float>(abs(pendulum->state.seg2.velocity) * 0.5);
        // acceleration of seg2.end on 6
        buffer[i*buffer.getNumChannels() + 5 ] = pendulum->state.seg2.acceleration;
        // total velocity on 7
        buffer[i*buffer.getNumChannels() + 6 ] = static_cast<float>(abs(pendulum->state.seg2.velocity) + abs(pendulum->state.seg1.velocity) / 10.0);
        // angle seg2 on 8
        auto angle 		  = pendulum->state.seg2.end;
        buffer[i*buffer.getNumChannels() + 7 ] = static_cast<float>((abs(atan2(angle.x, angle.y)) / PI) * 0.5f);

    }


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

void ofApp::mass1Updated(float &mass)
{
    pendulum->state.seg1.mass = mass;
}

void ofApp::mass2Updated(float &mass)
{
    pendulum->state.seg2.mass = mass;
}

void ofApp::length1Updated(float &length)
{
    pendulum->state.seg1.length = length;
}

void ofApp::length2Updated(float &length)
{
    pendulum->state.seg2.length = length;
}

void ofApp::gravityUpdated(float &g)
{
    pendulum->state.g = g;
}

void ofApp::dtUpdated(float &dt)
{
    this->dt = dt;
}

void ofApp::dragUpdated(float &drag){
    this->drag = drag;
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
