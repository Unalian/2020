//
//  main.cpp
//  Shading_mode
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

#include <math.h>
int mode = 0;
GLfloat windowWidth,windowHeight;

void choseMode(){
    if (mode == 0)
        glShadeModel(GL_SMOOTH);
    if (mode == 1) {
        glShadeModel(GL_FLAT);
    }
}
void RenderScene(){
    //clean windows with current clean colors
    glClear(GL_COLOR_BUFFER_BIT);
    choseMode();
    
    //draw triangle
    glBegin(GL_TRIANGLES);
    glColor3ub((GLubyte)255, (GLubyte)0, (GLubyte)0);
    glVertex3f(-1.0,-1.0,0.0);
    glColor3ub((GLubyte)209, (GLubyte)12, (GLubyte)23);
    glVertex3f(1.0,0.0,0.0);
    glColor3ub((GLubyte)103, (GLubyte)255, (GLubyte)0);
    glVertex3f(0.0,1.0,0.0);
    glEnd();
    
    
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
void processSpecialKey(int key,int x, int y){
    switch (key) {
        case GLUT_KEY_F1:
            mode = 0;
            break;
        case GLUT_KEY_F2:
            mode = 1;
            break;
        default:
            break;
    }
}


int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);//使用雙緩衝區有利於動畫的流暢性 //在renderScene中調用glutSwapBuffers() 雙緩衝區的切換
    glutInitWindowSize(100, 100);
    glutInitWindowPosition(0, 100);
    glutCreateWindow("Rotary Triangle");
    
    // register the callbacks
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    //鍵盤事件
    glutSpecialFunc(processSpecialKey);
    
    
    //enter envent processing loop
    glutMainLoop();
    
    return 0;
}
