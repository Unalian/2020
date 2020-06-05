//
//  main.cpp
//  Light_Rotate_Rectangle
//
//  Created by una on 2020/5/6.
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

# include "SOIL.h"

GLfloat LightAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat LightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f};
GLfloat light = 1;

GLfloat xrot;//x rotation
GLfloat yrot;//y rotation
GLfloat xspeed;//rotation speed
GLfloat yspeed;//rotation speed
GLfloat z = -5.0f;
//1. material
// GLfloat gray[] = {0.9f, 0.0f, 0.0f, 1.0f};

GLuint texture[1];


int LoadGLTextures(){
    char *file = "./img_test.bmp";
    
    std::cout<<file<<std::endl;
    if (file == NULL) {
        printf("123");
    }
    texture[0]=SOIL_load_OGL_texture(file,
                                     SOIL_LOAD_AUTO,
                                     SOIL_CREATE_NEW_ID,
                                     SOIL_FLAG_INVERT_Y
                                     );
    if(texture[0] == 0 )
        return false;
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return true;
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
    gluPerspective(45.0f, fAspect, 1.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
}

int InitGL(GLvoid){
    if (!LoadGLTextures()) {
       
    }
    
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);//Build up the depth buffer.
    glEnable(GL_DEPTH_TEST);//Enable the depth buffer
    glDepthFunc(GL_LEQUAL);//The type of the depth testing.
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//Perfect perspective calculation.
    
    //light
    glLightfv(GL_LIGHT1,GL_AMBIENT,LightAmbient);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,LightDiffuse);
    glLightfv(GL_LIGHT1,GL_POSITION,LightPosition);
    glEnable(GL_LIGHT1);
    
    //1. material
    //glMaterialfv(GL_FRONT, GL_DIFFUSE,gray);
    
    //2.material
    //glColorMaterial(GL_FRONT, GL_DIFFUSE);
    //glEnable(GL_COLOR_MATERIAL);
    
    
    
    return true;
    
}

void renderScene(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, z);
    
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);
    //2. material
    //glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //glutSolidCube(1.0); (no texture)
    // texture
    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f,1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f,1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,1.0f);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,-1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f,-1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f,-1.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f,1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f,1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f,-1.0f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f,-1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f,-1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f,1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,1.0f);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f,-1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f,-1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f,1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f,1.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,-1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f,1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);
    glEnd();
    
    glutSwapBuffers();
    
    xrot += xspeed;
    yrot += yspeed;
}

void processSpecialKey(int key, int x, int y){
    if (key == GLUT_KEY_PAGE_UP) {
        z -= 0.02f;
    }
    if (key == GLUT_KEY_PAGE_DOWN) {
        z += 0.02f;
    }
    if (key == GLUT_KEY_PAGE_UP) {
        xspeed -= 0.01f;
    }
    if (key == GLUT_KEY_PAGE_DOWN) {
        xspeed += 0.01f;
    }
    if (key == GLUT_KEY_LEFT) {
        yspeed -= 0.01f;
    }
    if (key == GLUT_KEY_RIGHT) {
        yspeed -= 0.01f;
    }
    
}

void processNomalKey(unsigned char key, int x, int y){
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'l':
            light =!light;
            light?glEnable(GL_LIGHTING):glDisable(GL_LIGHTING);
            
        default:
            break;
    }
    
}


int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);//使用雙緩衝區有利於動畫的流暢性
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 640);
    glutCreateWindow("Rotate Cube");
    
    // register the callbacks
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(processSpecialKey);
    glutKeyboardFunc(processNomalKey);
    
    
    InitGL();
    
    //enter envent processing loop
    glutMainLoop();
    
    return 0;
}

