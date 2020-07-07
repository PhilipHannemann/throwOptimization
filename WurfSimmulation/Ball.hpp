//
//  Ball.hpp
//  WurfSimmulation
//
//  Created by Philip Hannemann on 9/24/19.
//  Copyright © 2019 Philip Hannemann. All rights reserved.
//

#ifndef Ball_hpp
#define Ball_hpp

#include <stdio.h>
#include <math.h>
#include "Object.hpp"
#include "Constants.h"

class Ball: public Object {
protected:
    double getHorizontalDistanceForTime(double dt);
    double getHorizontalVelocityForTime(double dt);
    double getVerticalDistanceForTime(double dt);
    double getVerticalVelocityForTime(double dt);
    
public:
    Ball(double h, double alpha, double m);
    Object* clone() const;
};


#endif /* Ball_hpp */
