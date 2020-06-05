//
//  main.cpp
//  Lab3
//
//  Created by una on 2020/5/22.
//  Copyright © 2020 cetacis. All rights reserved.
//


#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>
#include <OpenGL/gl.h>


#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>


#endif

                                                     // Header File For NeHeGL
#include <stdio.h>
#include <stdlib.h>
#include "Physics2.h"                                                        // Header File For Physics2.h

#ifndef GLUT_DISABLE_ATEXIT_HACK
#define GLUT_DISABLE_ATEXIT_HACK
#endif
#define GLEW_STATIC

RopeSimulation* ropeSimulation = new RopeSimulation(
                                                    20,20,                        // 80 Particles (Masses)
                                                    0.01f,                    // Each Particle Has A Weight Of 50 Grams
                                                    300.0f,                // springConstant In The Rope
                                                    0.2f,                    // Normal Length Of Springs In The Rope
                                                    0.1f,                    // Spring Inner Friction Constant
                                                    Vector3D(0, -9.81f, 0)); // Gravitational Acceleration

int InitGL(GLvoid)                                        // All Setup For OpenGL Goes Here
{
    //ropeSimulation->getMass(ropeSimulation->vertinumOfMasses - 1,ropeSimulation->horinumOfMasses - 1)->vel.z = 3.0f;

    glClearColor (0.0f, 0.0f, 0.0f, 0.5f);                                    // Black Background
    glClearDepth (1.0f);                                                    // Depth Buffer Setup
    glShadeModel (GL_SMOOTH);                                                // Select Smooth Shading
    glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);                        // Set Perspective Calculations To Most Accurate

    return true;                                        // Initialization Went OK
}

void Deinitialize (void)                                                    // Any User DeInitialization Goes Here
{
    ropeSimulation->release();                                                // Release The ropeSimulation
    delete(ropeSimulation);                                                    // Delete The ropeSimulation
    ropeSimulation = NULL;
}

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;
    float ratio =  w * 1.0 / h;

        // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

        // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

void Update ()                                            // Perform Motion Updates Here
{
     ropeSimulation->operate(0.001);
  
    
}

void pri(float x, float y, float z){
    //printf("%f, %f, %f",x,y,z);
}

void renderScene(void) {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();                                                        // Reset The Modelview Matrix
    
    // Position Camera 40 Meters Up In Z-Direction.
    // Set The Up Vector In Y-Direction So That +X Directs To Right And +Y Directs To Up On The Window.
    gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                    // Clear Screen And Depth Buffer

    //update
    Update();

    // Start Drawing The Rope.
                                              // Set Color To Yellow
    
    for( int x=0; x< ropeSimulation->vertinumOfMasses-1; x++ ){
        for( int y=0; y<ropeSimulation->horinumOfMasses; y++){
            Mass* mass1 = ropeSimulation->getMass(x,y);
            Vector3D* pos1 = &mass1->pos;

            Mass* mass2 = ropeSimulation->getMass(x+1,y);
            Vector3D* pos2 = &mass2->pos;
            
            glColor3ub(255, 255, 0);
            glLineWidth(4);
            glBegin(GL_LINES);
                glVertex3f(pos1->x, pos1->y, pos1->z);
                glVertex3f(pos2->x, pos2->y, pos2->z);
               
            glEnd();
        }
    }
    for( int x=0; x< ropeSimulation->vertinumOfMasses; x++ ){
        for( int y=0; y<ropeSimulation->horinumOfMasses-1; y++){
            Mass* mass1 = ropeSimulation->getMass(x,y);
            Vector3D* pos1 = &mass1->pos;

            Mass* mass2 = ropeSimulation->getMass(x,y+1);
            Vector3D* pos2 = &mass2->pos;
            
            glColor3ub(255, 255, 0);
            glLineWidth(4);
            glBegin(GL_LINES);
                glVertex3f(pos1->x, pos1->y, pos1->z);
                glVertex3f(pos2->x, pos2->y, pos2->z);
               
            glEnd();
        }
    }
  
    glFlush ();                                                                // Flush The GL Rendering Pipeline
    glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

    if(key == 's'){
        Vector3D ropeConnectionVel;
        ropeConnectionVel.x = 0.0;
        ropeConnectionVel.y = 0.0;
        ropeConnectionVel.z = 0.0;
        ropeSimulation->setRopeConnectionVel(ropeConnectionVel);
    }
if (key == 27){
    
            // Application Is Finished
            Deinitialize ();                                            // User Defined DeInitialization

        exit(0);
}
 }

void processSpecialKeys(int key, int x, int y) {

    Vector3D ropeConnectionVel;                                                // Create A Temporary Vector3D
    switch(key) {
        case GLUT_KEY_RIGHT :                                              // Is The Right Arrow Being Pressed?
           ropeConnectionVel.x += 3.0f;                                      // Add Velocity In +X Direction
           break;
        case GLUT_KEY_LEFT :                                                    // Is The Left Arrow Being Pressed?
            ropeConnectionVel.x -= 3.0f;                                        // Add Velocity In -X Direction
            break;
        case GLUT_KEY_UP :
            ropeConnectionVel.y +=3.0f;                                        // Add Velocity In +Z Direction
            break;
        case GLUT_KEY_DOWN :
            ropeConnectionVel.y -= 3.0f;                                        // Add Velocity In -Z Direction
            break;
        case GLUT_KEY_PAGE_UP:
            ropeConnectionVel.z +=3.0f;                                        // Add Velocity In +Y Direction
            break;
        case GLUT_KEY_PAGE_DOWN:
            ropeConnectionVel.z -= 3.0f;                                        // Add Velocity In +Y Direction
            break;
    }

    ropeSimulation->setRopeConnectionVel(ropeConnectionVel);                // Set The Obtained ropeConnectionVel In The Simulation
    
}

int main(int argc, char **argv) {
    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(320,320);
    glutCreateWindow("Rope Simulation");

    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    if(!InitGL()){
        printf("Initialization Failed.");
        return false;
    }

    // process key
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}



