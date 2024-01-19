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
#include <memory>
#include <tuple>

#include "Constants.h"
#include "Ball.hpp"
#include "Object.hpp"
#include "Point.h"

#define RAD2DEG 180/3.14159
#define DEST_VARIANCE 0.1
#define VELOCITY_INCREMENTOR 0.1
#define ANGLE_INCREMENTOR 0.1
#define MAXERRORS 10
#define MAX_POSIBLE_VELOCITY 10000

using std::vector;
using std::shared_ptr;
using std::tuple;

struct Result {
    vector<Point> positions;
    double vMin = DBL_MAX, angle = 0;
};

struct Param {
    double angle = 0;
    double vMin = DBL_MAX;
};

struct Accuracy {
    double distance{DEST_VARIANCE}, angle{ANGLE_INCREMENTOR}, velocity{VELOCITY_INCREMENTOR};
};

class Simmulator {
    shared_ptr<Object> obj;
    double dt;
    vector<Point> positions;
    Point startPosition;
    double vMin = DBL_MAX;
    Accuracy accuracy;
    
public:
    Simmulator(double v, double h, double alpha, shared_ptr<Object> obj, double dt = 0.001);
    Simmulator(double h, shared_ptr<Object> obj, double dt = 0.001);
    
    vector<Point> startSimmulation(const double &maxDistance = DBL_MAX) const;
    Result startOptimalCalculationForTarget(const double &targetDistance);
    Result getVMinForAngle(const double &angle, const double &targetDistance);
    Result getMinimumVelocityForAngle(const double &angle, const double &targetDistance);
    bool isDistanceReachable(const double &angle, const double &targetDistance);
    Result fastOptimalCalculationForTarget(const double &targetDistance, const Param &angleMin, const Param &angleMax);
    
    double getDistance();
    vector<Point> getPositions() { return positions; }
    double getDistanceForPositions(const vector<Point> &positions) const;
    
    void reset(const Point &startPosition);
    void setAccuracy(const Accuracy &accuracy);
};



#endif /* Simmulator_hpp */
