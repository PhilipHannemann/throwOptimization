//
//  Object.cpp
//  WurfSimmulation
//
//  Created by Philip Hannemann on 9/24/19.
//  Copyright © 2019 Philip Hannemann. All rights reserved.
//

#include "Object.hpp"

void Object::moveForTime(double dt){
    double dx = getHorizontalDistanceForTime(dt);
    double dy = getVerticalDistanceForTime(dt);
    double dv_h = getHorizontalVelocityForTime(dt);
    double dv_v = getVerticalVelocityForTime(dt);
    
    x += dx;
    y += dy;
    v_h += dv_h;
    v_v += dv_v;
}

void Object::reset(double h){
    x = 0.0;
    y = h;
}

double Object::isGettingCloserTo0(){
    if(y > 0)
        return true;
    if (y < 0 && v_v > 0)
        return true;
    
    return false;
}


