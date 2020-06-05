//
//  main.cpp
//  Animition-lab2
//
//  Created by una on 2020/4/24.
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


# include <math.h>
# include "SOIL.h"
#include <time.h>
#include <stdlib.h>

#define    MAX_PARTICLES    200            // Number of particles to create

#define PI              3.14159f
#define RHO             50
#define LIFE            3.0f
#define TIME            0.01f
#define SIZE            0.5f
float size = 2;
float windowHeight, windowWidth;
float red = 1.0, green = 1.0, blue = 1.0;
float xx=0 , yy=50, zz=0 ;
float num = 180;
GLuint texture[0];
float k = 1,k2 = 1,k3 = 90,a = -60,k4 = 0;
float vy;
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


typedef struct                        // Create a Structure for Particle
{
    float    life;                    // Particle Life
    float   init_life;
    float    speed_aging;            // Aging speed
    
    float    r;                        // Red value
    float    g;                        // Green value
    float    b;                        // Blue value
    
    float    x;                        // X position
    float    y;                        // Y position
    float    z;                        // Z position
    
    float    v_x;                    // X velocity
    float    v_y;                    // Y velocity
    float    v_z;                    // Z velocity
    
    float    a_x;                    // X acceleration
    float    a_y;                    // Y acceleration
    float    a_z;                    // Z acceleration
}
particle;                            // Particles structure

particle particles[MAX_PARTICLES];    // Particle array (Room for particle info)


void ChangeSize(int w, int h)
{
    float aspectRatio;
    
    
    if (h == 0) h = 1;
    
    
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    aspectRatio = (float)w / (float)h;
    
    if (w <= h){
        windowWidth = 100;
        windowHeight = 100 / aspectRatio;
        glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
    }
    else{
        windowWidth = 100.0 * aspectRatio;
        windowHeight = 100;
        glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int InitPaticleSystem(void)
{
    float theta, phi, rho;
    
    for (int i = 0; i < MAX_PARTICLES;i++)                         // Initials all particles
    {
        particles[i].init_life = LIFE + rand() % 10 / 10.0;            // Give All The Particles Full Life
        particles[i].life = particles[i].init_life;
        particles[i].speed_aging = TIME;
        
        srand((unsigned)time(NULL));
        
        particles[i].r = red;                                    // Set color for particle
        particles[i].g = green;
        particles[i].b = blue;
        
        
        particles[i].x = 0.0f;                                   // Set position for particle
        particles[i].y = 50.0f;
        particles[i].z = 0.0f;
        
        theta = (rand() % 90 + 45) * PI / 180;
        phi = 90 * PI / 180;
        rho = rand() % RHO;
        particles[i].v_x = float(sinf(phi) * cosf(theta) * rho);  // Set X Axis Speed And Direction for particle
        particles[i].v_y = float(sinf(phi) * sin(theta) * rho);      // Set Y Axis Speed And Direction for particle
        particles[i].v_z = float(cosf(phi) * rho);                // Set Z Axis Speed And Direction for particle
        
        particles[i].a_x = 0.0f;                            // Set X Axis acceleration
        particles[i].a_y = a;                            // Set Y Axis acceleration
        particles[i].a_z = 0.0f;                            // Set Z Axis acceleration
    }
    
    return true;
}

void RenderScene(void) {
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);          // Clear screen and depth buffer //无动态模糊时
    /*glClear(GL_DEPTH_BUFFER_BIT);    //动态模糊
     glColor4f(0.0f, 0.0f,0.0f, 0.1f);
     glRectf(-windowWidth, -windowHeight, windowWidth, windowHeight);*/
    
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture[0]);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    for (int i = 0;i < MAX_PARTICLES; i++)                             // All particles
    {
        float x = particles[i].x;                                // Position of particle
        float y = particles[i].y;
        float z = particles[i].z;
        
        
        // Draw particle using RGB values, alpha value based on it's life
        glColor4f(red,green,blue,particles[i].life);
        
        glPointSize(size);
        glBegin(GL_TRIANGLE_STRIP);            // Build Quad From A Triangle Strip
        glTexCoord2d(1,1);glVertex3f(x+SIZE,y+SIZE,z); // Top Right
        glTexCoord2d(0,1); glVertex3f(x-SIZE,y+SIZE,z); // Top Left
        glTexCoord2d(1,0); glVertex3f(x+SIZE,y-SIZE,z); // Bottom Right
        glTexCoord2d(0,0); glVertex3f(x-SIZE,y-SIZE,z); // Bottom Left
        glEnd();
    }
    
    glClear(GL_DEPTH_BUFFER_BIT);    //动态模糊
    glColor4f(0.0f, 0.0f,0.0f, 0.1f);
    glRectf(-windowWidth, -windowHeight, windowWidth, windowHeight);
    
    
    glutSwapBuffers();
}
/*void RenderScene2(void) {
 //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);          // Clear screen and depth buffer //无动态模糊时
 glClear(GL_DEPTH_BUFFER_BIT );    //动态模
 glColor4f(1.0f,1.0f,1.0f,0.1);
 glBegin(GL_QUADS);
 glVertex3f(-20.0f  , -20.0f  ,  20.0f  );
 glVertex3f( 20.0f  , -20.0f  ,  20.0f  );
 glVertex3f( 20.0f  ,  20.0f  ,  20.0f  );
 glVertex3f(-20.0f  ,  20.0f  ,  20.0f  );
 glEnd();
 glutSwapBuffers();
 
 }*/

void Update(){//在glTime中，每隔一段时间调用一下
    for (int i = 0;i < MAX_PARTICLES; i++)                    // All The Particles
    {   if(particles[i].y - SIZE < -30.0 && particles[i].x<20.0f &&particles[i].x>-20.0f || particles[i].y-SIZE > 80.0f && particles[i].x<5.0f &&particles[i].x>-5.0f)
        particles[i].v_y = -k*particles[i].v_y;
        
        particles[i].x += particles[i].v_x * TIME;         // update position of particle
        particles[i].y += particles[i].v_y * TIME;
        particles[i].z += particles[i].v_z * TIME;
        
        particles[i].v_x += particles[i].a_x * TIME;            // update velocity
        particles[i].v_y += a * TIME;
        particles[i].v_z += particles[i].a_z * TIME;
        
        particles[i].life -= particles[i].speed_aging;        // reduce particles life
        
        float theta, phi, rho;
        if (particles[i].life < 0.0f)                    // if particle has reached end of life
        {
            particles[i].init_life = LIFE + rand()%10/10.0;        // Give it new life
            particles[i].life = particles[i].init_life;
            
            particles[i].x = xx;                        // Initialize position
            particles[i].y = yy;
            particles[i].z = zz;
            
            theta = (rand() % 360) * PI/num+k3;
            phi = 90 * PI / 180;
            rho = rand() % RHO;
            vy = float(sinf(phi) * sin(theta) * rho)*k2;
            particles[i].v_x = float(sinf(phi) * cosf(theta) * rho)*k2;    // Initialize velocity
            particles[i].v_y = vy;
            particles[i].v_z = float(cosf(phi) * rho)*k2;
        }
    }
}

void TimerFunction(int value){
    Update();
    glutPostRedisplay();
    glutTimerFunc(10,TimerFunction,1);// 10ms后执行TimerFuncion
}

// Setup the rendering state
int SetupRC(void)
{
    if(!LoadGLTextures()){
        printf("Load texture fail");
        return false;
    }
    //glClearColor(0.0, 0.0, 0.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    return true;
}
void processSpecialKey(int key,int x, int y){
    switch (key) {
        case GLUT_KEY_F1:
            
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            break;
        case GLUT_KEY_F2:
            glDisable(GL_TEXTURE_2D);
            break;
        case GLUT_KEY_F3:
            red = 0.3;
            blue = 0.5;
            green = 0.9;
            break;
        case GLUT_KEY_F4:
            red = 1;
            blue = 1;
            green = 1;
            break;
        case GLUT_KEY_F5:
            yy = 0;
            red = 0.2;
            blue = 1;
            green = 0.2;
            num = 1540;
            k = 0.3;
            k2 = 0.4;
            k3 = 180;
            k4 = 30;
            a = -20;
            break;
        case GLUT_KEY_F6:
            yy = 0;
            red = 1;
            blue = 0;
            green = 0;
            num = 180;
            k = 0.3;
            k2 = 0.1;
            k4 = 0;
            a = 1;
            size = 5;
            vy = abs(vy);
            break;
            
            
    }
}

int main(int argc, char **argv) {
    
    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);//双缓冲区，应用于动画
    glutInitWindowPosition(100,100);
    glutInitWindowSize(640,640);
    glutCreateWindow("Particle system");
    
    // register callbacks
    //glutDisplayFunc(RenderScene);// 绑定回调函数，传入的函数是在窗体绘制的时候调用
    //glutDisplayFunc(RenderScene2);
    
    glutReshapeFunc(ChangeSize);//窗口的大小
    glutDisplayFunc(RenderScene);
    glutTimerFunc(10, TimerFunction, 1);//每隔一段时间绘制一帧，产生连续的动画
    glutSpecialFunc(processSpecialKey);
    // Setup the rendering state
    
    SetupRC();//绘图初始化 设置清除色，由glClear(GL_COLOR_BUFFER_BIT)调用-在renderScene中；
    InitPaticleSystem();
    
    // enter GLUT event processing loop
    glutMainLoop();//处理队列，死循环的方式等待时间
    
    return 0;
}
