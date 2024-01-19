//
//  Ball.cpp
//  WurfSimmulation
//
//  Created by Philip Hannemann on 9/24/19.
//  Copyright © 2019 Philip Hannemann. All rights reserved.
//

#include "Ball.hpp"

Ball::Ball(double h, double alpha, double m):Object(){
    this->m = m;
    c = 0.45;
}

double Ball::getHorizontalDistanceForTime(const double &dt) const {
    return v_h * dt;
}

double Ball::getVerticalDistanceForTime(const double &dt) const {
    return v_v * dt;
}

double Ball::getHorizontalVelocityForTime(const double &dt) const {
    return (1.0/m) * (-c * sqrt(pow(v_h, 2.0) + pow(v_h, 2.0)) * v_h) * dt;
}

double Ball::getVerticalVelocityForTime(const double &dt) const {
    return (1.0/m) * (-m * GRAVITY - c * sqrt(pow(v_h, 2.0) + pow(v_h, 2.0)) * v_v) * dt;
}

Object* Ball::clone() const {
    return new Ball(*this);
}
