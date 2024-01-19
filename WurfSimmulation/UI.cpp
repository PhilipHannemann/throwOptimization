//
//  UI.cpp
//  WurfSimmulation
//
//  Created by Philip Hannemann on 9/24/19.
//  Copyright © 2019 Philip Hannemann. All rights reserved.
//

#include "UI.hpp"


UI::UI(const int width, const int hight): simulator{0, std::make_shared<Ball>(0, DEFAULT_ALPHA, DEFAULT_MASS)} {
    this->width = width;
    this->hight = hight;
    
    coordinateWidth = 20;
    coordinateHight = coordinateWidth / WINDOW_ASPECT_RATIO;
    
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

void UI::displayConsole() const {
    std::cout << "Wurfsimmulation\n";
    std::cout << "===============\n";
}

void UI::setupCoordinateSize() {
    double yMax{0};
    
    for (const auto &position : positions) {
        if (position.y > yMax) {
            yMax = position.y;
        }
    }
    
    maxPointHeight = yMax;
    yMax = yMax < 10 ? 10 : yMax;
    coordinateHight = yMax + 1 ;
    coordinateWidth = coordinateHight * WINDOW_ASPECT_RATIO;
}

void UI::calculate() {
    //Simmulator simmulator(v, h, alpha, ball, 0.0001);

    const auto startTime = high_resolution_clock::now();
    const Param angleMax{M_PI_2}, angleMin{-M_PI_2};
    simulator.reset(startPosition);
    
    Result simulationResult;
    
    if (optimizedAlgorithm) {
        simulationResult = simulator.fastOptimalCalculationForTarget(20, angleMin, angleMax);
    } else {
        simulationResult = simulator.startOptimalCalculationForTarget(20);
    }
    
    const auto endTime = high_resolution_clock::now();
    
    this->positions = simulationResult.positions;
    this->optimalAngle = simulationResult.angle;
    this->optomalVelocity = simulationResult.vMin;
    
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    executionTimeInMilliseconds = duration.count();
    
    setupCoordinateSize();
    setupViewPort();
}


void UI::openWindow() {
    
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
    
    do {
        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear( GL_COLOR_BUFFER_BIT );
        
        drawScene();
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        if((glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_ENTER)) && !simmulationComplete){
            calculate();
            simmulationComplete = true;
        }
        
        if (glfwGetKey(window, GLFW_KEY_UP) && !simmulationComplete) {
            startPosition.y += 0.1;
            calculate();
        } else if (glfwGetKey(window, GLFW_KEY_DOWN) && !simmulationComplete) {
            startPosition.y -= 0.1;
            calculate();
        } else if (glfwGetKey(window, GLFW_KEY_RIGHT) && !simmulationComplete) {
            startPosition.x += 0.1;
            calculate();
        } else if (glfwGetKey(window, GLFW_KEY_LEFT) && !simmulationComplete) {
            startPosition.x -= 0.1;
            calculate();
        } else if (glfwGetKey(window, GLFW_KEY_R)) {
            simmulationComplete = false;
            positions = {};
        } else if (glfwGetKey(window, GLFW_KEY_1)){
            optimizedAlgorithm = false;
            calculate();
        } else if (glfwGetKey(window, GLFW_KEY_2)){
            optimizedAlgorithm = true;
            calculate();
        }
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

void UI::setupViewPort() const {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, coordinateWidth, -1, coordinateHight, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void UI::drawScene() {
    drawCoordinateSystem();
    drawBall();
    drawBallMovement();
    drawExecutionTime();
}

void UI::drawCoordinateSystem(){
    glColor3f(1.0,0.0,0.0); // red x
    glLineWidth(5.0f);
    
    glBegin(GL_LINES);    // x aix
        glVertex2f(-1.0, 0.0f);
        glVertex2f(coordinateWidth, 0.0f);
    glEnd();
    
    glBegin(GL_TRIANGLES);    // arrow
        glVertex2f(coordinateWidth, 0);
        glVertex2f(coordinateWidth - 0.5, -0.25);
        glVertex2f(coordinateWidth - 0.5, 0.25);
    glEnd();
    
    glBegin(GL_LINES);    // goal
        glVertex2f(20, -0.25);
        glVertex2f(20, 0.25);
    glEnd();
    
    glFlush();
    
    drawText(19.5f, -0.75f, {1, 1, 1}, "20m");
    
    
    // y
    glColor3f(0.0f,1.0f,0.0f); // green y
    glBegin(GL_LINES);
        glVertex3f(0.0, -1.0f, 0.0f);
        glVertex3f(0.0, coordinateHight, 0.0f);
    glEnd();
    
    glBegin(GL_TRIANGLES);    // arrow
        glVertex2f(0, coordinateHight);
        glVertex2f(-0.25, coordinateHight-0.5);
        glVertex2f(0.25, coordinateHight-0.5);
    glEnd();
    
    glBegin(GL_LINES);    // highest point
        glVertex2f(-0.25, maxPointHeight);
        glVertex2f(0.25, maxPointHeight);
    glEnd();
    
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << maxPointHeight;
    const auto maxHeightString = stream.str() + "m";
    drawText(0.25f, maxPointHeight, {1, 1, 1},  maxHeightString);
    
    glFlush();
}

void UI::drawExecutionTime() const {
    const auto algorithmus = optimizedAlgorithm ? "Alg2: " : "Alg1: ";
    drawText(coordinateWidth - 3.0, coordinateHight - 1.0, {1, 1, 1}, algorithmus + std::to_string(executionTimeInMilliseconds) + "ms");
    glFlush();
}

void UI::drawBallMovement() {
    glColor3f(1.0,1.0,1.0); // red x
    
    glBegin(GL_LINE_STRIP);    // x aix
    
    for (const auto &position : positions) {
        glVertex2f(position.x, position.y);
    }
    
    glEnd();
    
    glFlush();
}


void UI::drawBall() {
    glColor3f(1.0, 1.0, 1.0); // red x
    
    glBegin(GL_LINE_LOOP);
        const float radius = 0.5f;
        for (auto angle = 0U; angle < 360; angle += 3) {
            const float angleInRad = angle * DEG2RAD;
            glVertex2f(cos(angleInRad) * radius + startPosition.x, sin(angleInRad) * radius + startPosition.y);
        }

    glEnd();
    
    glFlush();
}

void UI::drawText(const float &x, const float &y, const Color &color, const string &text) const {
    glPushMatrix();
    glTranslatef(x, y, 0);
    const float scaleFactor{1.f / 300.f};
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    
    glColor3f(color.r, color.g, color.b);
    
    for (const auto &character : text) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, (unsigned char)character);
    }
    
    glPopMatrix();
}
