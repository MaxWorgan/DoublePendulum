//
// Created by max on 07/06/18.
//

#ifndef OFAPP_PENDULUM_H
#define OFAPP_PENDULUM_H


#include "segment.h"

class Pendulum
{
public:
    struct State{
        Segment seg1;
        Segment seg2;
        float g;
    };
    struct NextStep
    {
        float dv1,dv2;
    };
    struct Derivative
    {
        float dx1; // dx/dt = velocity
        float dv1; // dv/dt = acceleration
        float dx2; // dx/dt = velocity
        float dv2; // dv/dt = acceleration
    };
    Pendulum(const State &state);
    State state;
    void draw();
    void update(float dt, float drag);
    NextStep acceleration(const State & state);
    Pendulum::Derivative evaluate(const State state, float dt, const Derivative &next);
};

#endif // PENDULUM_H
