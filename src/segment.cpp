//
// Created by max on 07/06/18.
//
#include "segment.h"
#include "ofGraphics.h"

void Segment::update(ofPoint offset, float dt)
{
    this->offset = offset;
    end.x = offset.x + (length * 10) * sin(angle);
    end.y = offset.y + (length * 10) * cos(angle);
}

void Segment::draw()
{
    ofSetColor(0);
    ofDrawLine(offset,end);
    ofDrawCircle(end, mass*2.0f);
    prevEnd = end;

}
