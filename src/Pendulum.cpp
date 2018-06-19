//
// Created by max on 07/06/18.
//
#include "Pendulum.h"
#include "ofGraphics.h"

Pendulum::Pendulum(const State& state){
    this->state = state;
}

void Pendulum::draw(){
    state.seg1.draw();
    state.seg2.draw();
}

Pendulum::Derivative Pendulum::evaluate(const State initial, float dt, const Derivative &d) {

    State nextState;
    nextState.g = initial.g;

    nextState.seg1.mass = initial.seg1.mass;
    nextState.seg2.mass = initial.seg2.mass;

    nextState.seg1.length= initial.seg1.length;
    nextState.seg2.length = initial.seg2.length;

    nextState.seg1.angle = initial.seg1.angle + d.dx1 * dt;
    nextState.seg2.angle = initial.seg2.angle + d.dx2 * dt;

    nextState.seg1.velocity = initial.seg1.velocity + d.dv1 * dt;
    nextState.seg2.velocity = initial.seg2.velocity + d.dv2 * dt;

    Derivative output;

    output.dx1 = nextState.seg1.velocity;
    output.dx2 = nextState.seg2.velocity;
    NextStep n = acceleration(nextState);
    output.dv1 = n.dv1;
    output.dv2 = n.dv2;
    return output;

}

Pendulum::NextStep Pendulum::acceleration(const State &state)
{
    double num1 = -state.g * (2 * state.seg1.mass + state.seg2.mass) * sin(state.seg1.angle);
    double num2 = -state.seg2.mass * state.g * sin(state.seg1.angle - 2 * state.seg2.angle);
    double num3 = -2 * sin(state.seg1.angle - state.seg2.angle) * state.seg2.mass;
    double num4 = pow(state.seg2.velocity,2) * state.seg2.length + pow(state.seg1.velocity, 2) * state.seg1.length * cos(state.seg1.angle - state.seg2.angle);
    double den = state.seg1.length * (2 * state.seg1.mass + state.seg2.mass - state.seg2.mass* cos(2 * state.seg1.angle - 2 * state.seg2.angle));

    NextStep next;
    next.dv1 = (num1 + num2 + num3 * num4) / den;

    num1 = 2 * sin(state.seg1.angle - state.seg2.angle);
    num2 = (pow(state.seg1.velocity,2) * state.seg1.length * (state.seg1.mass + state.seg2.mass));
    num3 = state.g * (state.seg1.mass + state.seg2.mass) * cos(state.seg1.angle);
    num4 = pow(state.seg2.velocity,2) * state.seg2.length * state.seg2.mass * cos(state.seg1.angle - state.seg2.angle);
    den = state.seg2.length * (2 * state.seg1.mass + state.seg2.mass - state.seg2.mass* cos(2 * state.seg1.angle - 2 * state.seg2.angle));

    next.dv2 = (num1 * (num2 + num3 + num4)) / den;

    return next;

}

void Pendulum::update(float dt, float drag){
    Derivative a = evaluate(state, 0.0f, Derivative() );
    Derivative b = evaluate(state, dt*0.5f, a );
    Derivative c = evaluate(state, dt*0.5f, b );
    Derivative d = evaluate(state, dt, c );

    float dxdt1 = 1.0f / 6.0f *  ( a.dx1 + 2.0f * ( b.dx1 + c.dx1 ) + d.dx1 );
    float dvdt1 = 1.0f / 6.0f *  ( a.dv1 + 2.0f * ( b.dv1 + c.dv1 ) + d.dv1 );
    float dxdt2 = 1.0f / 6.0f *  ( a.dx2 + 2.0f * ( b.dx2 + c.dx2 ) + d.dx2 );
    float dvdt2 = 1.0f / 6.0f *  ( a.dv2 + 2.0f * ( b.dv2 + c.dv2 ) + d.dv2 );

    state.seg1.angle += dxdt1 * dt;
    state.seg2.angle += dxdt2 * dt;

    state.seg1.velocity += dvdt1 * dt;
    state.seg2.velocity += dvdt2 * dt;
    auto dir = 0;
    if(state.seg1.velocity > 0) dir = -1;
    else dir = 1;
    state.seg1.velocity += pow(state.seg1.velocity,2) * drag * dir;
    if(state.seg2.velocity > 0) dir = -1;
    else dir = 1;
    state.seg2.velocity += pow(state.seg2.velocity,2) * drag * dir;

//   state.seg1.acceleration = dvdt1 * dt;
//   state.seg2.acceleration = dvdt2 * dt;

    state.seg1.update();
    state.seg2.update(state.seg1.end);
}
