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
    double getHorizontalDistanceForTime(const double &dt) const;
    double getHorizontalVelocityForTime(const double &dt) const;
    double getVerticalDistanceForTime(const double &dt) const;
    double getVerticalVelocityForTime(const double &dt) const;
    
public:
    Ball(double h, double alpha, double m);
    Object* clone() const;
};


#endif /* Ball_hpp */
