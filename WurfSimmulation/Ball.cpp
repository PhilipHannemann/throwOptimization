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
    x = 0;
    y = 0;
}

double Ball::getHorizontalDistanceForTime(double dt){
    return v_h * dt;
}

double Ball::getVerticalDistanceForTime(double dt){
    return v_v * dt;
}

double Ball::getHorizontalVelocityForTime(double dt){
    return (1.0/m) * (-c * sqrt(pow(v_h, 2.0) + pow(v_h, 2.0)) * v_h) * dt;
}

double Ball::getVerticalVelocityForTime(double dt){
    return (1.0/m) * (-m * GRAVITY - c * sqrt(pow(v_h, 2.0) + pow(v_h, 2.0)) * v_v) * dt;
}

Object* Ball::clone() const{
    return new Ball(*this);
}
