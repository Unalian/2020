//
//  main.cpp
//  lab1_1
//
//  Created by una on 2020/5/5.
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
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

GLfloat red = 0.0f,green = 0.0f,blue = 0.0f;
GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat rsize = 25;
GLfloat windowWidth,windowHeight;
GLfloat xstep = 1.0f,ystep = 1.0f;

void setupRC(){
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
}
void RenderScene(){
    //clean windows with current clean colors
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    
    glColor3f(red, green, blue);
    //draw triangle
    glBegin(GL_TRIANGLES);
    glVertex3f(x+rsize,y,0);
    glVertex3f(x,y,0.0);
    glVertex3f(x+rsize,y-rsize,0.0);
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
        windowWidth = 100;
        windowHeight = 100/aspectRatio;
        glOrtho(-100.0, 100.0, -windowWidth, windowHeight, 1.0, -1.0);
        
    }
    else{
        windowWidth = 100 * aspectRatio;
        windowHeight = 100;
        glOrtho(-windowWidth, windowWidth, -100, 100, 1.0, -1.0);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
}

// 遞歸調用每幀時間更新動畫
void TimeFunction(int value){

    //當遇到左右邊改變方向
    if(x>windowWidth-rsize||x<-windowWidth)
        xstep = -xstep;
    //當遇到上下邊改變方向
    if(y>windowHeight||y<-windowHeight+rsize)
        ystep = -ystep;
    //move
    x += xstep;
    y += ystep;
    //防止特殊情況s
    if(x>(windowWidth-rsize+xstep))
        x = windowWidth-rsize-1;
    else if(x<-(windowWidth +xstep))
        x = -windowWidth -1;
    
    if(y>(windowHeight+ystep))
        y = windowHeight - 1;
    else if(y < -(windowHeight -rsize + ystep))
        y = -windowHeight + rsize -1;
    
    
    glutPostRedisplay();
    glutTimerFunc(33, TimeFunction, 1);
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
            if(xstep >0)xstep = 2.0f;
            else xstep = -2.0f;
           if(ystep >0)ystep = 2.0f;
            else ystep = -2.0f;
            break;
        case GLUT_KEY_F5:
           if(xstep >0)xstep = 3.0f;
            else xstep = -3.0f;
            if(ystep >0)ystep = 3.0f;
            else ystep = -3.0f;
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
    glutInitWindowPosition(100, 100);
    glutCreateWindow("First Glut");
    
    
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
