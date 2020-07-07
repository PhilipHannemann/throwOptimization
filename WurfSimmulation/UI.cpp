//
//  UI.cpp
//  WurfSimmulation
//
//  Created by Philip Hannemann on 9/24/19.
//  Copyright © 2019 Philip Hannemann. All rights reserved.
//

#include "UI.hpp"


UI::UI(int width, int hight){
    this->width = width;
    this->hight = hight;
    
    coordinateWidth = 20;
    coordinateHight = 10;
    
    displayConsole();
    
    
    if (!glfwInit()) {
        printf("Window couldn't be started!\n");
        return;
    }
    
    window = glfwCreateWindow(width, hight, "Wurfsimmulator", NULL, NULL);
    
    if (!window) {
        glfwTerminate();
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        return;
    }
    
    openWindow();
}

void UI::displayConsole(){
    std::cout << "Wurfsimmulation\n";
    std::cout << "===============\n";
    
    //std::cout << "\nWas ist die Abwurfhöhe? ";
    //std::cin >> h;
    //std::cout << "\n";
}

void UI::setupCoordinateSize(){
    double xMax = 19, yMax = 9;
    
    for (int i = 0; i < positions.size(); i++) {
        Point p = positions[i];
        if (p.x > xMax) {
            xMax = p.x;
        }
        if (p.y > yMax) {
            yMax = p.y;
        }
    }
    
    coordinateWidth = xMax + 1;
    coordinateHight = yMax + 1;
}

void UI::calculate(){
    Ball *ball = new Ball(h, alpha, m);
    //Simmulator simmulator(v, h, alpha, ball, 0.0001);
    Simmulator simmulator(h, ball);
    
    //simmulator.startSimmulation();
    //simmulator.startOptimalCalculationForTarget(20);
    Param angleMax, angleMin;
    angleMax.angle = M_PI_2;
    angleMin.angle = h <= 0 ? 0 : -M_PI_2;
    Result r = simmulator.fastOptimalCalculationForTarget(20, angleMin, angleMax);
    
    //positions = simmulator.getPositions();
    positions = r.positions;
    setupCoordinateSize();
    setupViewPort();
    
    startTime = glfwGetTime();
}


void UI::openWindow(){
    
    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return;
    }
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    glViewport(0.0f, 0.0f, width, hight);
    
    setupViewPort();
    
    do{
        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear( GL_COLOR_BUFFER_BIT );
        
        drawScene();
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        if((glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_ENTER)) && !simmulationComplete){
            
            Ball *ball = new Ball(h, alpha, m);
            //Simmulator simmulator(v, h, alpha, ball, 0.0001);
            Simmulator simmulator(h, ball);
            
            //simmulator.startSimmulation();
            //simmulator.startOptimalCalculationForTarget(20);
            Param angleMax, angleMin;
            angleMax.angle = M_PI_2;
            angleMin.angle = h <= 0 ? 0 : -M_PI_2;
            Result r = simmulator.fastOptimalCalculationForTarget(20, angleMin, angleMax);
            
            simmulationComplete = true;
            //positions = simmulator.getPositions();
            positions = r.positions;
            setupCoordinateSize();
            setupViewPort();
            
            startTime = glfwGetTime();
        }
        
        if (glfwGetKey(window, GLFW_KEY_UP) && !simmulationComplete) {
            h+=0.1;
            calculate();
            
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) && !simmulationComplete) {
            //if(h > 0)
            h-=0.1;
            calculate();
        }
        if (glfwGetKey(window, GLFW_KEY_R)) {
            simmulationComplete = false;
            positions = {};
        }
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

void UI::setupViewPort(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, coordinateWidth, -1, coordinateHight, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void UI::drawScene(){
    drawCoordinateSystem();
    drawBallMovement();
    drawBall();
}


void UI::drawCoordinateSystem(){
    glColor3f(1.0,0.0,0.0); // red x
    
    glBegin(GL_LINES);    // x aix
        glVertex2f(-1.0, 0.0f);
        glVertex2f(coordinateWidth, 0.0f);
    glEnd();
    
    glBegin(GL_TRIANGLES);    // arrow
        glVertex2f(coordinateWidth, 0);
        glVertex2f(coordinateWidth-0.5, -0.25);
        glVertex2f(coordinateWidth-0.5, 0.25);
    glEnd();
    
    glFlush();
    
    
    // y
    glColor3f(0.0,1.0,0.0); // green y
    glBegin(GL_LINES);
        glVertex3f(0.0, -1.0f, 0.0f);
        glVertex3f(0.0, coordinateHight, 0.0f);
    glEnd();
    
    glBegin(GL_TRIANGLES);    // arrow
        glVertex2f(0, coordinateHight);
        glVertex2f(-0.25, coordinateHight-0.5);
        glVertex2f(0.25, coordinateHight-0.5);
    glEnd();
    
    glFlush();
    
}

void UI::drawBallMovement(){
    glColor3f(1.0,0.0,0.0); // red x
    
    glBegin(GL_LINE_STRIP);    // x aix
    
    for (int i = 0; i < positions.size(); i++) {
        Point p = positions[i];
        //if(glfwGetTime() - startTime > p.t && startTime > 0){
            glVertex2f(p.x, p.y);
        //}
    }
    glEnd();
    
    glFlush();
}


void UI::drawBall(){
    glColor3f(1.0, 0.0, 0.0); // red x
    
    glBegin(GL_LINE_LOOP);
        double radius = 0.5;
        for (int i=0; i < 360; i++){
            float degInRad = i*DEG2RAD;
            glVertex2f(cos(degInRad)*radius,sin(degInRad)*radius + h);
        }

    glEnd();
    
    glFlush();
}
