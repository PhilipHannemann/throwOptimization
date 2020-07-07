//
//  Simmulator.hpp
//  WurfSimmulation
//
//  Created by Philip Hannemann on 9/24/19.
//  Copyright © 2019 Philip Hannemann. All rights reserved.
//

#ifndef Simmulator_hpp
#define Simmulator_hpp

#include <stdio.h>
#include <math.h>
#include <vector>
#include <float.h>

#include "Constants.h"
#include "Ball.hpp"
#include "Object.hpp"
#include "Point.h"

#define RAD2DEG 180/3.14159
#define DEST_VARIANCE 0.01
#define VELOCITY_INCREMENTOR 0.01
#define ANGLE_INCREMENTOR 0.001
#define MAXERRORS 10

struct Result {
    std::vector<Point> positions;
    double vMin = DBL_MAX, angle = 0;
};

struct Param{
    double angle = 0;
    double vMin = DBL_MAX;
};

class Simmulator {
    Object *obj;
    double dt;
    std::vector<Point> positions;
    double startH;
    double vMin = DBL_MAX;
    
public:
    Simmulator(double v, double h, double alpha, Object *obj, double dt = 0.001);
    Simmulator(double h, Object *obj, double dt = 0.001);
    
    std::vector<Point> startSimmulation(Object *obj);
    void startOptimalCalculationForTarget(double s);
    Result getVMinForAngle(double angle, double s);
    Result fastOptimalCalculationForTarget(double s, Param angleMin, Param angleMax);
    
    double getDistance();
    std::vector<Point> getPositions() {return positions;}
    double getDistanceForPositions(std::vector<Point> positions);

};



#endif /* Simmulator_hpp */
