//
//  Object.hpp
//  WurfSimmulation
//
//  Created by Philip Hannemann on 9/24/19.
//  Copyright © 2019 Philip Hannemann. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include <math.h>
#include "Point.h"

class Object {
protected:
    double m, c;
    Point position{0.0, 0.0};
    double v_h, v_v;
    
    virtual double getHorizontalDistanceForTime(const double &dt) const = 0;
    virtual double getHorizontalVelocityForTime(const double &dt) const = 0;
    virtual double getVerticalDistanceForTime(const double &dt) const = 0;
    virtual double getVerticalVelocityForTime(const double &dt) const = 0;
    
public:
    Object() {}
    virtual Object* clone() const = 0;
    
    void setHorizintalVelocity(const double &velocity) {v_h = velocity;}
    void setVerticalVelocity(const double &velocity) {v_v = velocity;}
    void setInitialVelocity(const double &velocity, const double &angle);
    void setHeight(const double &height) {position.y = height;}
    void setDistance(const double &xDistance) {position.x = xDistance;}
    
    void moveForTime(const double &dt);
    void reset(const Point &position);
    
    double getHorizintalVelocity() const {return v_h;}
    double getVerticalVelocity() const {return v_v;}
    double getHeight() const {return position.y;}
    double getDistance() const {return position.x;}
    Point getPosition() const { return position; }
    double getMass() const {return m;}
    double getDrag() const {return c;}
    double isMoving() const;
    
};



#endif /* Object_hpp */
