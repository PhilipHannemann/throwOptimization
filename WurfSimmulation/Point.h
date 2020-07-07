//
//  Point.h
//  WurfSimmulation
//
//  Created by Philip Hannemann on 9/24/19.
//  Copyright © 2019 Philip Hannemann. All rights reserved.
//

#ifndef Point_h
#define Point_h

struct Point {
    Point(double x = 0, double y = 0, double t = 0){
        this->x = x;
        this->y = y;
        this->t = t;
    }
    double x, y;
    double t;
};

#endif /* Point_h */
