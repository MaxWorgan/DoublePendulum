//
// Created by max on 07/06/18.
//

#ifndef OFAPP_SEGMENT_H
#define OFAPP_SEGMENT_H

#include "../../of/src/openframeworks/types/ofBaseTypes.h"
#include "../../of/src/openframeworks/math/ofVec3f.h"

class Segment
{
public:
    float mass;
    float length;
    float angle;
    float velocity, acceleration;
    float scale;

    ofPoint end, offset;
    ofPoint prevEnd;



    Segment(){ }\

    Segment(float mass, float length) {
        this->mass = mass;
        this->length = length;
        this->angle = 0;
        this->end = ofPoint(0,length);
        this->prevEnd = end;
        this->velocity = 0;
        this->acceleration = 0;
        this->scale = 10;
    }

    void update(ofPoint offset = ofPoint(0,0), float dt = 1);

    void draw();

};

#endif //OFAPP_SEGMENT_H
