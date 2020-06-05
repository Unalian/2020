//
//  main.cpp
//  openglDemo2
//
//  Created by una on 2020/4/10.
//  Copyright © 2020 cetacis. All rights reserved.
// Animation lab: rotate triangle

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
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif
GLfloat angle = 0.0;
GLfloat angleChange = 2.0;
GLfloat red = 0.0f,green = 1.0f,blue = 1.0f;
GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat windowWidth,windowHeight;


void setupRC(){
    glClearColor(0.3f, 0.1f, 0.4f, 1.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
}

void RenderScene(){
    //clean windows with current clean colors
    glClear(GL_COLOR_BUFFER_BIT);
    
    glPushMatrix();
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glColor3f(red, green, blue);
    //draw triangle
    glBegin(GL_TRIANGLES);
    glVertex3f(-1.0,-1.0,0.0);
    glVertex3f(1.0,0.0,0.0);
    glVertex3f(0.0,1.0,0.0);
    glEnd();
    glPopMatrix();
    
    glutSwapBuffers();
    //flush drawing commands 告诉opengl应该执行提供给他的绘图命令
    // glFlush();
}

void ChangeSize(int w,int h){
    GLfloat aspectRatio;
    
    if (h == 0) h = 1;
    
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    aspectRatio = (GLfloat)w/(GLfloat)h;
    
    if(w <= h){
        
        glOrtho(-1.0, 1.0, -1.0/aspectRatio, 1.0/aspectRatio, 1.0, -1.0);
        
    }
    else{
        windowWidth = 1.0 * aspectRatio;
        windowHeight = 1.0;
        glOrtho(-windowWidth, windowWidth, -1.0, 1.0, 1.0, -1.0);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
}

// 遞歸調用每幀時間更新動畫
void TimeFunction(int value){

    angle += angleChange;
    glutPostRedisplay();
    glutTimerFunc(23, TimeFunction, 1);
}

void processNormalKey(unsigned char key,int x,int y){
    if(key == 27){
        exit(0);
    }
}
void processSpecialKey(int key,int x, int y){
    switch (key) {
        case GLUT_KEY_F1:
            red = 1.0f;
            green = 0.0f;
            blue = 0.0f;
            break;
        case GLUT_KEY_F2:
            red = 1.0f;
            green = 0.0f;
            blue = 1.0f;
            break;
        case GLUT_KEY_F3:
            red = 0.0f;
            green = 1.0f;
            blue = 1.0f;
            break;
        case GLUT_KEY_F4:
            angleChange = 4.0;
            break;
        case GLUT_KEY_F5:
            angleChange = 1.0;
            break;
        default:
            break;
    }
}

int main (int argc, char *argv[])
{   //init glut and creat window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);//使用雙緩衝區有利於動畫的流暢性 //在renderScene中調用glutSwapBuffers() 雙緩衝區的切換
    glutInitWindowSize(100, 100);
    glutInitWindowPosition(0, 100);
    glutCreateWindow("Rotary Triangle");
    
    
    // register the callbacks
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(23, TimeFunction, 1);
    //鍵盤事件
    glutKeyboardFunc(processNormalKey);
    glutSpecialFunc(processSpecialKey);
    
    
    
    //set up the render state
    setupRC();
    
    //enter envent processing loop
    glutMainLoop();
    
    return 0;
    
    
}
