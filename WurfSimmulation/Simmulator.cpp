//
//  Simmulator.cpp
//  WurfSimmulation
//
//  Created by Philip Hannemann on 9/24/19.
//  Copyright © 2019 Philip Hannemann. All rights reserved.
//

#include "Simmulator.hpp"

Simmulator::Simmulator(double v, double h, double alpha, Object *obj, double dt){
    this->obj = obj;
    double v_h = cos(alpha)*v;
    double v_v = sin(alpha)*v;
    this->dt = dt;
    
    obj->setHorizintalVelocity(v_h);
    obj->setVerticalVelocity(v_v);
    obj->setHight(h);
    startH = h;
}

Simmulator::Simmulator(double h, Object *obj, double dt){
    this->obj = obj;
    this->dt = dt;
    obj->setHight(h);
    startH = h;
}


std::vector<Point> Simmulator::startSimmulation(Object *obj){
    std::vector<Point> positions = {Point(0, obj->getHight(), 0)};
    double t = 0;
    while (obj->isGettingCloserTo0()){
        Point p;
        t += dt;
        
        obj->moveForTime(dt);
        p.y = obj->getHight();
        p.x = obj->getDistance();
        p.t = t;
        
        positions.push_back(p);
    }
    
    unsigned long size = positions.size();
    
    if(size >= 2 && positions[size-1].y < 0 && positions[size-2].y > 0){
        double y2 = positions[positions.size()-1].y;
        double y1 = positions[positions.size()-2].y;
        double x2 = positions[positions.size()-1].x;
        double x1 = positions[positions.size()-2].x;
        
        double yPercent = y1/(y2-y1);
        double newX = x1 + (x2-x1)*yPercent;
        
        positions[positions.size()-1].x = newX;
        positions[positions.size()-1].y = 0;
    }
    
    return positions;
}

void Simmulator::startOptimalCalculationForTarget(double s){
    double v = VELOCITY_INCREMENTOR, alpha = startH <= 0 ? 0 : -M_PI_2;
    printf("Alpha: %.2f\n", alpha);
    double vMin = -1, alphaMin = -1;
    std::vector<Point> optimalPositions;
    bool forward = true;
    double lastDistance;
    double lastDelta;
    
    do {
        positions = {};
        
        do {
            //printf("looking for velocity %.3f and alpha %.3f\n", v, alpha);
            double v_h = cos(alpha)*v;
            double v_v = sin(alpha)*v;
            obj->setVerticalVelocity(v_v);
            obj->setHorizintalVelocity(v_h);
            obj->reset(startH);
            
            lastDistance = getDistance();
            lastDelta = abs(getDistance()-s);
            //printf("delta: %.3f\n", lastDelta);
            //startSimmulation();
            
            if (abs(getDistance()-s) < DEST_VARIANCE && (vMin < 0 || v < vMin)) {
                vMin = v;
                alphaMin = alpha;
                optimalPositions = positions;
                //printf("Min: Der Erste Punkt ist: P(%.3f, %.3f) der letzte P(%.3f, %.3f) bei einem Winkel von: %.2f\n", positions[0].x, positions[0].y, positions[positions.size()-1].x, positions[positions.size()-1].y, alpha);
            }
            
            if (forward)
                v += VELOCITY_INCREMENTOR;
            else
                v -= VELOCITY_INCREMENTOR;
            
         } while (abs(getDistance()-s) > DEST_VARIANCE &&
                 lastDelta > abs(getDistance()-s));
        
        //printf("winkel %.3f hat die beste Geschwindigkeit, von %.3fm/s\n", alpha, v);
        
        if (vMin > 0) {
            forward = false;
            v = vMin;
        }else{
            v = 0;
        }
        
        alpha += ANGLE_INCREMENTOR;
    } while (alpha <= M_PI_2);
    
    positions = optimalPositions;
    
    printf("Die optimale Kurve benötigt eine Geschwindigkeit von %.3fm/s bei einem Winkel von %.2fº.\n", vMin, alphaMin*RAD2DEG);
    
    printf("Der Erste Punkt ist: P(%.3f, %.3f) der letzte P(%.3f, %.3f)\n", positions[0].x, positions[0].y, positions[positions.size()-1].x, positions[positions.size()-1].y);
}


Result Simmulator::getVMinForAngle(double angle, double s){
    Result r;
    r.angle = angle;
    double vMin = this->vMin;
    double lastDistance;
    double lastDelta;
    bool forward = vMin != DBL_MAX ? false : true;
    double v = vMin != DBL_MAX ? vMin : VELOCITY_INCREMENTOR;
    //double velocityIncrement = VELOCITY_INCREMENTOR;
    Object *obj = this->obj->clone();
    double pos = 0;
    std::vector<Point> positions;
    //double distanceToReach = 0;
    //double distanceIncrementor = s / 2.0;
    double part = 2.0;
    int failCounter = 0;
    
    do {
       double v_h = cos(angle)*v;
       double v_v = sin(angle)*v;
       obj->setVerticalVelocity(v_v);
       obj->setHorizintalVelocity(v_h);
       obj->reset(startH);
       
       double pos = getDistanceForPositions(positions);
       lastDistance = pos;
       lastDelta = pos == 0 && !forward ? DBL_MAX : s - pos;
       positions = startSimmulation(obj);
       
       pos = getDistanceForPositions(positions);
       if (abs(pos - s) < DEST_VARIANCE) {
           this->vMin = v;
           r.vMin = v;
           r.positions = positions;
           return r;
       }
        
        double delta = s - pos;
        double distanceVariance = abs(lastDelta) - abs(pos - s);
        
        if (lastDelta * delta < 0) {
            part *= 2;
        } else if (distanceVariance <= 0)
           return r;
        
    
        
        if (pos == 0 || !(pos == pos)) {
            v = VELOCITY_INCREMENTOR;
            pos = 0;
            if(failCounter++ > MAXERRORS){
                return r;
            }
        }else{
            double multipyer = s/pos;
            double factor = (multipyer - 1.0) / part + 1.0;
            v *= factor;
        }
        
        
       
    } while (abs(pos - s) > DEST_VARIANCE);
    
    return r;
}


Result Simmulator::fastOptimalCalculationForTarget(double s, Param angleMin, Param angleMax){

    Result resultHigh, resultLow;
    
    double angleDelta = angleMax.angle - angleMin.angle;
    double angle = angleMin.angle + angleDelta / 2.0;
    
    
    Result middle = getVMinForAngle(angle, s);
    
    bool equal = angleMax.vMin == angleMin.vMin;
    bool higherIsBetter = false;
    if(!equal)
        higherIsBetter = (angleMax.vMin != DBL_MAX) && (angleMax.vMin < angleMin.vMin);
    if(angleDelta > ANGLE_INCREMENTOR){
        if (equal || higherIsBetter) {
            Param min;
            min.angle = angle;
            min.vMin = middle.vMin;
            
            resultHigh = fastOptimalCalculationForTarget(s, min, angleMax);
        }
        
        if (equal || (!higherIsBetter && (angleMin.vMin != DBL_MAX))) {
            Param max;
            max.angle = angle;
            max.vMin = middle.vMin;
            
            resultLow = fastOptimalCalculationForTarget(s, angleMin, max);
        }
    }
    
    if (middle.vMin <= resultHigh.vMin && middle.vMin <= resultLow.vMin)
        return middle;
    
    if (resultHigh.vMin <= middle.vMin && resultHigh.vMin <= resultLow.vMin)
        return resultHigh;
    
    return resultLow;
}





double Simmulator::getDistance(){
    if (positions.size() == 0) {
        return DBL_MAX;
    }
    
    return positions[positions.size()-1].x;
}

double Simmulator::getDistanceForPositions(std::vector<Point> positions){
    if (positions.size() == 0) {
        return 0;
    }
    return positions[positions.size()-1].x;
}
