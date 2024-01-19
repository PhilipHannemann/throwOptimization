//
//  UI.hpp
//  WurfSimmulation
//
//  Created by Philip Hannemann on 9/24/19.
//  Copyright © 2019 Philip Hannemann. All rights reserved.
//

#ifndef UI_hpp
#define UI_hpp

#define DEG2RAD 3.14159 / 180
#define DEFAULT_VELOCITY 550;
#define WINDOW_ASPECT_RATIO 2.0 / 1.0;

#define DEFAULT_ALPHA 0.263
#define DEFAULT_MASS 7.5

#include <chrono>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GLUT/glut.h>
#include <GLFW/glfw3.h>
#include "Point.h"
#include "Simmulator.hpp"
#include "Ball.hpp"

using std::vector;
using std::string;
using std::chrono::high_resolution_clock;

struct Color {
    float r,  g, b;
};

class UI {
    GLFWwindow *window;
    Simmulator simulator;
    
    int width, hight;
    float coordinateWidth;
    float coordinateHight;
    float maxPointHeight{0};
    long executionTimeInMilliseconds{0};
    
    bool optimizedAlgorithm{true};
    
    double startTime = -1;
    double optomalVelocity, optimalAngle;
    
    Point startPosition{0.0, 0.0};
    vector<Point> positions;
    bool simmulationComplete = false;
    
    void displayConsole() const;
    void openWindow();
    void drawScene();
    void drawCoordinateSystem();
    void drawBallMovement();
    void setupCoordinateSize();
    void drawBall();
    void drawExecutionTime() const;
    void setupViewPort() const;
    void drawText(const float &x, const float &y, const Color &color, const string &text) const;
    void calculate();
    
public:
    UI(int width = 1000, int hight = 500);
    
};


#endif /* UI_hpp */
