//
//  Object.hpp
//  WurfSimmulation
//
//  Created by Philip Hannemann on 9/24/19.
//  Copyright © 2019 Philip Hannemann. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

class Object {
protected:
    double m, c;
    double x, y;
    double v_h, v_v;
    
    virtual double getHorizontalDistanceForTime(double dt) = 0;
    virtual double getHorizontalVelocityForTime(double dt) = 0;
    virtual double getVerticalDistanceForTime(double dt) = 0;
    virtual double getVerticalVelocityForTime(double dt) = 0;
    
public:
    Object(){
        x = 0.0;
        y = 0.0;
    }
    virtual Object* clone() const = 0;
    
    void setHorizintalVelocity(double v) {v_h = v;}
    void setVerticalVelocity(double v) {v_v = v;}
    void setHight(double h) {y = h;}
    void setDistance(double x) {this->x = x;}
    
    void moveForTime(double dt);
    void reset(double h);
    
    double getHorizintalVelocity(){return v_h;}
    double getVerticalVelocity(){return v_v;}
    double getHight(){return y;}
    double getDistance(){return x;}
    double getMass(){return m;}
    double getDrag(){return c;}
    double isGettingCloserTo0();
    
};



#endif /* Object_hpp */
