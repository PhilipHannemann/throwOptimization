//
//  UI.hpp
//  WurfSimmulation
//
//  Created by Philip Hannemann on 9/24/19.
//  Copyright © 2019 Philip Hannemann. All rights reserved.
//

#ifndef UI_hpp
#define UI_hpp

#define DEG2RAD 3.14159/180

#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Point.h"
#include "Simmulator.hpp"
#include "Ball.hpp"

class UI {
    GLFWwindow* window;
    int width, hight;
    float coordinateWidth;
    float coordinateHight;
    
    double startTime = -1;
    
    double h, v = 550, alpha = 0.263, m = 7.5;
    std::vector<Point> positions;
    bool simmulationComplete = false;
    
    void displayConsole();
    void openWindow();
    void drawScene();
    void drawCoordinateSystem();
    void drawBallMovement();
    void setupCoordinateSize();
    void drawBall();
    void setupViewPort();
    void calculate();
    
public:
    UI(int width = 1000, int hight = 500);
    
};


#endif /* UI_hpp */
