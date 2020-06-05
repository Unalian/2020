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
#include <math.h>
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
float angle = 0.0f;
float lx = 0.0f, lz = -1.0f;
float x = 0.0f, z = 5.0f;
GLfloat gray[] = {0.0f, 0.9f, 0.9f, 1.0f};

GLfloat LightAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat LightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f};
GLfloat light = 1;


void drawSnowMan(){
    glColor3f(1.0f, 1.0f, 1.0f);
    
    //body
    glTranslatef(0.0f, 0.75f, 0.0f);
    glutSolidSphere(0.75f, 20, 20);
    //draw head
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f, 20, 20);
    //draw eyes
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(0.05f, 0.1f, 0.18f);
    glutSolidSphere(0.05f, 10, 10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f, 10,  10);
    glPopMatrix();
    //draw nose
    glColor3f(1.0f, 0.5f, 0.5f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.08f, 0.5f, 10,2);
}

void RenderScene(){
    //clean windows with current clean colors
    //depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //重置位置
    glLoadIdentity();
    //set camera
    gluLookAt(x, 1.0f, z,
              x+lx, 1.0f, z+lz,
              0.0f, 1.0f, 0.0f);
    
    //draw ground
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();
    
    //Draw 36 SnowMen
    for(int i = -3; i < 3; i++)
        for (int j = -3; j < 3; j++) {
            glPushMatrix();
            glTranslatef(i*10.0, 0, j*10.0);
            drawSnowMan();
            glPopMatrix();
        }
    glutSwapBuffers();
    
    //flush drawing commands 告诉opengl应该执行提供给他的绘图命令
    // glFlush();
}
void ChangeSize(int w,int h){
    // 透视投影
    GLfloat fAspect;
    
    if (h == 0)
        h = 1;
    
    glViewport(0, 0, w, h);
    
    fAspect = (GLfloat)w / (GLfloat)h;
    
    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //produce perspective projection
    gluPerspective(60.0f, fAspect, 1.0, 400.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void processNormalKey(unsigned char key,int x,int y){
    if(key == 27){
        exit(0);
    }
    if (key == 'l') {
        light =!light;
        light?glEnable(GL_LIGHTING):glDisable(GL_LIGHTING);
    }
    
}
void processSpecialKey(int key,int xx, int yy){
    float fraction = 0.1f;
    switch (key) {
        case GLUT_KEY_LEFT:
            angle  -= 0.01f;
            lx = sin(angle);
            lz = - cos(angle);
            break;
        case GLUT_KEY_RIGHT:
            angle  += 0.01f;
            lx = sin(angle);
            lz = - cos(angle);
            break;
        case GLUT_KEY_UP:
            x += lx*fraction;
            z += lz*fraction;
            break;
        case GLUT_KEY_DOWN:
            x -= lx*fraction;
            z -= lz*fraction;
            break;
    }
}

void initGL(){
    
    glLightfv(GL_LIGHT1,GL_AMBIENT,LightAmbient);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,LightDiffuse);
    glLightfv(GL_LIGHT1,GL_POSITION,LightPosition);
    glEnable(GL_LIGHT1);
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE,gray);
    
}
int main (int argc, char *argv[])
{   //init glut and creat window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);//使用雙緩衝區有利於動畫的流暢性
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(320, 320);
    glutCreateWindow("Snowmen World");
    
    
    // register the callbacks
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutIdleFunc(RenderScene);
    glutKeyboardFunc(processNormalKey);
    glutSpecialFunc(processSpecialKey);
    initGL();
    
    glEnable(GL_DEPTH_TEST);
    
    //enter envent processing loop
    glutMainLoop();
    
    return 0;
    
    
}
