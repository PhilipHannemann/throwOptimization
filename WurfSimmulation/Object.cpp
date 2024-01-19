//
//  Object.cpp
//  WurfSimmulation
//
//  Created by Philip Hannemann on 9/24/19.
//  Copyright © 2019 Philip Hannemann. All rights reserved.
//

#include "Object.hpp"

void Object::moveForTime(const double &dt){
    double dx = getHorizontalDistanceForTime(dt);
    double dy = getVerticalDistanceForTime(dt);
    double dv_h = getHorizontalVelocityForTime(dt);
    double dv_v = getVerticalVelocityForTime(dt);
    
    position.x += dx;
    position.y += dy;
    position.t += dt;
    
    v_h += dv_h;
    v_v += dv_v;
}

void Object::setInitialVelocity(const double &velocity, const double &angle) {
    v_h = cos(angle) * velocity;
    v_v = sin(angle) * velocity;
}

void Object::reset(const Point &position) {
    this->position = position;
}

double Object::isMoving() const {
    if(position.y > 0) return true;
    
    if (position.y < 0 && v_v > 0) return true;
    
    return false;
}


