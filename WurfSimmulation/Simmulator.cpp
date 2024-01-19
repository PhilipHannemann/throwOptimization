//
//  Simmulator.cpp
//  WurfSimmulation
//
//  Created by Philip Hannemann on 9/24/19.
//  Copyright © 2019 Philip Hannemann. All rights reserved.
//

#include "Simmulator.hpp"

Simmulator::Simmulator(double v, double h, double alpha, shared_ptr<Object> obj, double dt){
    this->obj = obj;
    double v_h = cos(alpha) * v;
    double v_v = sin(alpha) * v;
    this->dt = dt;
    
    obj->setHorizintalVelocity(v_h);
    obj->setVerticalVelocity(v_v);
    obj->setHeight(h);
    startPosition.y = h;
}

Simmulator::Simmulator(const double h, shared_ptr<Object> obj, const double dt){
    
    this->obj = obj;
    this->dt = dt;
    this->obj->setHeight(h);
    startPosition.y = h;
}


vector<Point> Simmulator::startSimmulation(const double &maxDistance) const {
    vector<Point> positions{obj->getPosition()};
    while (obj->isMoving()) {
        obj->moveForTime(dt);
        const auto position = obj->getPosition();
        
        positions.push_back(position);
        
        if(position.x > maxDistance) break;
    }
    
    const auto size = positions.size();
    auto &lastPoint = positions[size - 1];
    
    // hits the ground
    if(size >= 2 && lastPoint.y < 0 && positions[size - 2].y > 0) {
        const auto &priorPoint = positions[size - 2];
        
        const double yPercent = priorPoint.y / (lastPoint.y - priorPoint.y);
        
        lastPoint.x = priorPoint.x + (lastPoint.x - priorPoint.x) * yPercent;
        lastPoint.y = 0;
    }
    
    return positions;
}

Result Simmulator::startOptimalCalculationForTarget(const double &targetDistance) {
    double currentVelocity{accuracy.velocity}, angle{0.0};
    printf("Alpha: %.2f\n", angle);
    Result optimum{{}, DBL_MAX, 0};
    double currentDistance;
    
    do {
        while(!isDistanceReachable(angle, targetDistance) && angle <= M_PI_2) {
            angle += accuracy.angle;
        }
        
        if(angle > M_PI_2) break;
        
        do {
            obj->setInitialVelocity(currentVelocity, angle);
            obj->reset(startPosition);

            const auto positions = startSimmulation();
            
            currentDistance = getDistanceForPositions(positions);
            const double missingDistance{abs(currentDistance - targetDistance)};
            
            if (missingDistance < accuracy.distance && currentVelocity < optimum.vMin) {
                optimum = { positions, currentVelocity, angle};
            }
            
            currentVelocity += accuracy.velocity;
            
         } while (currentDistance < targetDistance);
        
        currentVelocity = 0.0;
        angle += accuracy.angle;
    } while (angle <= M_PI_2);
    
    return optimum;
}


Result Simmulator::getVMinForAngle(const double &angle, const double &targetDistance) {

    double &vMin = this->vMin;
    double lastDistance, lastDelta;
    bool forward = vMin == DBL_MAX;
    double v = vMin != DBL_MAX ? vMin : accuracy.velocity;
    double newDistance = 0;
    vector<Point> positions;
    double part = 2.0;
    int failCounter = 0;
    
    if(!isDistanceReachable(angle, targetDistance)) {
        return {{}, DBL_MAX, angle};
    }
    
    do {
        obj->setInitialVelocity(v, angle);
        obj->reset(startPosition);
       
        lastDistance = getDistanceForPositions(positions);

        lastDelta = lastDistance == 0 && !forward ? DBL_MAX : targetDistance - lastDistance;
        positions = startSimmulation();
       
        newDistance = getDistanceForPositions(positions);
        if (abs(newDistance - targetDistance) < accuracy.distance) {
           this->vMin = v;
           return {positions, v, angle};
        }
        
        const double newDelta = targetDistance - newDistance;
        const double distanceVariance = abs(lastDelta) - abs(newDelta);
        
        if (lastDelta * newDelta < 0) {
            part *= 2;
        } else if (distanceVariance <= 0){
            return {{}, DBL_MAX, angle};
        }
        
        if (newDistance == 0 || newDistance == NAN) {
            v = accuracy.velocity;
            newDistance = 0;
            if(failCounter++ > MAXERRORS) {
                return {{}, DBL_MAX, angle};
            }
        } else {
            const double multiplyer = targetDistance / newDistance;
            const double factor = (multiplyer - 1.0) / part + 1.0;
            v *= factor;
        }
        
    } while (abs(newDistance - targetDistance) > accuracy.distance);
    
    return {positions, v, angle};
}

Result Simmulator::getMinimumVelocityForAngle(const double &angle, const double &targetDistance) {
    
    if(!isDistanceReachable(angle, targetDistance)) {
        return {{}, DBL_MAX, angle};
    }
    
    vector<Point> positions;
    double currentVelocity = this->vMin == DBL_MAX ? accuracy.velocity : this->vMin;
    
    struct Try { double distance; double velocity; };
    
    Try notEnough{0.0, 0.0}, tooMuch{DBL_MAX, DBL_MAX};
    
    do {
        obj->setInitialVelocity(currentVelocity, angle);
        obj->reset(startPosition);
        
        positions = startSimmulation(targetDistance * 2);
        const auto distance = getDistanceForPositions(positions);
        
        const auto missingDistance = distance - targetDistance;
        
        if (abs(missingDistance) < accuracy.distance) break;
        
        const auto notEnoughVeleocity = missingDistance < 0;
        
        if (notEnoughVeleocity) {
            notEnough = {distance, currentVelocity};
        } else {
            tooMuch = {distance, currentVelocity};
        }
        
        if (tooMuch.velocity == DBL_MAX || notEnough.velocity == 0.0) {
            currentVelocity = targetDistance / distance * currentVelocity;
        } else {
            const auto missingTargetDistance = targetDistance - notEnough.distance;
            const auto distanceVariance = tooMuch.distance - notEnough.distance;
            const auto velocityVariance = tooMuch.velocity - notEnough.velocity;
            
            const auto velocityIncrement = missingTargetDistance / distanceVariance * velocityVariance;
            if (velocityIncrement < accuracy.velocity) break;
            
            currentVelocity = notEnough.velocity + velocityIncrement;
        }
        
    } while (true);
    
    return {positions, currentVelocity, angle};
}

bool Simmulator::isDistanceReachable(const double &angle, const double &targetDistance) {
    obj->setInitialVelocity(MAX_POSIBLE_VELOCITY, angle);
    obj->reset(startPosition);
    
    const auto positions = startSimmulation(targetDistance);
    
    const auto distance = getDistanceForPositions(positions);
    
    return distance > targetDistance;
}


Result Simmulator::fastOptimalCalculationForTarget(const double &targetDistance, const Param &angleMin, const Param &angleMax) {

    Result resultHigh, resultLow;
    
    const double angleDelta = abs(angleMax.angle - angleMin.angle);
    const double angle = angleMin.angle + angleDelta / 2.0;
    
    const auto middle = getMinimumVelocityForAngle(angle, targetDistance);
    
    const bool equalVelocity = angleMax.vMin == angleMin.vMin;
    const bool higherIsBetter = (angleMax.vMin != DBL_MAX) && (angleMax.vMin < angleMin.vMin);
    
    if (angleDelta < accuracy.angle) {
        return middle;
    }
    
    if (equalVelocity || higherIsBetter) {
        resultHigh = fastOptimalCalculationForTarget(targetDistance, {angle, middle.vMin}, angleMax);
    }
    
    if (equalVelocity || (!higherIsBetter && (angleMin.vMin != DBL_MAX))) {
        resultLow = fastOptimalCalculationForTarget(targetDistance, angleMin, {angle, middle.vMin});
    }
    
    if (middle.vMin <= resultHigh.vMin && middle.vMin <= resultLow.vMin)
        return middle;
    
    if (resultHigh.vMin <= resultLow.vMin)
        return resultHigh;
    
    return resultLow;
}


double Simmulator::getDistance() {
    return getDistanceForPositions(positions);
}

double Simmulator::getDistanceForPositions(const vector<Point> &positions) const {
    if (positions.size() == 0) {
        return 0;
    }
    return positions[positions.size() - 1].x;
}

void Simmulator::reset(const Point &startPosition) {
    obj->reset(startPosition);
    positions.clear();
    vMin = DBL_MAX;
    this->startPosition = startPosition;
}

void Simmulator::setAccuracy(const Accuracy &accuracy) {
    this->accuracy = accuracy;
}
