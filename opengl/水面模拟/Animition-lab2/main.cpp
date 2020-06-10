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



#define PI              3.14159f


GLuint texture[0];//材质

float windowWidth,windowHeight;//初始化窗口大小变量
float amplitude = 0.01;
float wavelength = 0.3;
float speed = -0.2;
int numWaves = 3;//多震源时镇远
float centers[3][2] = {
    0.0,5.0,
    3.0,2.0,
    4.0,1.0
};
float center[1][2] = {0.05,0.1};
static float    rotate_x = 30;
static float    rotate_y = 15;
static float    translate_z = 4;
static int RESOLUTION = 40.0f;
static float surface[6*40*(40+1)];
static float normal[6*40*(40+1)];
bool normals = 1;
bool wire_frame = 1;
float xold,yold;// the past location of the mouse
int k = 3;//type2
int Q = 0;//type3

int vaSource = 0;
int type = 0;


GLfloat LightAmbient[] ={0.9f,0.9f,0.9f,1.0f};
GLfloat LightDiffuse[] ={0.9f,0.9f,0.9f,1.0f};
GLfloat LightPosition[] ={1.0f,1.0f,-0.5f,0.0f};
int LoadGLTextures(){
    char *file = "./reflection.jpg";
    
    std::cout<<file<<std::endl;
    if (file == NULL) {
        printf("123");
    }
    texture[0]=SOIL_load_OGL_texture(file,
                                     SOIL_LOAD_AUTO,
                                     SOIL_CREATE_NEW_ID,
                                     SOIL_FLAG_INVERT_Y
                                     );
    if(texture[0] == 0)
        return false;
    
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    
    return true;
}


int InitGL(){
    if (!LoadGLTextures()) {
        return false;
    }
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    
    glColorMaterial(GL_FRONT,GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    return true;
}

// caculate the distance between two dots
float dot(float x, float y){
    float cx = x - center[0][0];
    float cy = y - center[0][1];
    return sqrt(cx * cx + cy * cy);
    
}

float dots(int i, float x, float y){
    float xc = x - centers[i][0];
    float yc = y - centers[i][1];
    return sqrt(xc*xc + yc*yc);
    
}

float wave1(int i,float x, float y, float time){
    float frequency = 2*PI / wavelength;
    float phase = speed * frequency;
    float theta = dots(i, x, y);
    return amplitude * sin(theta * frequency + time * phase);
}

float wave2(int i,float x, float y, float time){
    float frequency = 2*PI / wavelength;
    float phase = speed * frequency;
    float theta = dots(i, x, y);
    return 2 * amplitude * pow(((sin(theta * frequency + time * phase)+1)/2),k);
}

float waveHeight(float x, float y,float time){
    if(type == 0){
        float frequency = 2*PI / wavelength;
        float phase = speed * frequency;
        float theta = (dot(x, y));
        return amplitude * sin(theta * frequency + time * phase);}
    else if(type == 1 || type == 3){
        float height = 0.0;
        for(int a = 0; a < numWaves; a ++)
            height += wave1(a, x, y, time);
        return height;
    }else{
        float height = 0.0;
        for(int a = 0; a < numWaves; a ++)
            height += wave2(a, x, y, time);
        return height;
    }
}

float dWavedx(float x, float y, float time){
    float frequency = 2*PI / wavelength;
    float phase = speed * frequency;
    float theta = dot(x, y);
    if (type == 0||type == 1) {
        float A = amplitude * x * frequency / theta;
        return A * cos(theta * frequency + time * phase);
    }else{
        float A = amplitude * k * x * frequency / theta * pow(((sin(theta * frequency + time * phase)+1)/2),k-1);
        return A * cos(theta * frequency + time * phase);
    }
}

float dWavedy(float x, float y, float time){
    float frequency = 2*PI / wavelength;
    float phase = speed * frequency;
    float theta = dot(x, y);
    if (type == 0||type == 1) {
        float A = amplitude * y * frequency / theta;
        return A * cos(theta * frequency + time * phase);
    }else{
        float A = amplitude *frequency* k * y  / theta * pow(((sin(theta * frequency + time * phase)+1)/2),k-1);
        return A * cos(theta * frequency + time * phase);
    }
}


float findX(float x,float y, float t){
 
    float finalX = x;
    float frequency = 2*PI / wavelength;
    float phase = speed * frequency;
    float theta = 0.0;
    for(int a = 0;a<numWaves;a++){
        theta = dots(a, x, y);
        finalX += 0.02*(x - centers[a][0]) / theta* cos(theta  * frequency + t * phase);}
    return finalX;
    
}

float findY(float x,float y, float t){

    float finalY = y;
    float frequency = 2*PI / wavelength;
    float phase = speed * frequency;
    float theta = 0.0;
    for(int a = 0;a<numWaves;a++){
        theta = dots(a, x, y);
  
        finalY += 0.02 * (y - centers[a][1]) / theta *cos(theta  * frequency + t * phase);}
    return finalY;
    
}

struct Vector3{
    float x;
    float y;
    float z;
};

Vector3 waveNormal(float x, float y, float time){
    float dx = dWavedx(x, y, time);
    float dy = dWavedy(x, y, time);
    Vector3 n;
    n.x = -dx;
    n. y = 1.0;
    n. z = -dy;
    
    float l = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
    if (l!=0) {
        n.x /= l;
        n.y /= l;
        n.z /= l;
    }else{
        n.x = 0;
        n.y = 1;
        n.z = 0;
    }
    return n;
}



// Particles structure



void ChangeSize(int w, int h)
{
    if (h == 0) {
        h = 1;
    }
    float ratio = 1.0*w/h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(20, ratio, 0.1, 15);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}




void renderScene(void)
{
    const float t = glutGet (GLUT_ELAPSED_TIME) / 1000.0;
    const float delta = 2. / RESOLUTION;
    const unsigned int length = 2 * (RESOLUTION + 1);
    const float xn = (RESOLUTION + 1) * delta + 1;
    unsigned int i;
    unsigned int j;
    float x;
    float y;
    unsigned int indice;
    unsigned int preindice;
    
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    glTranslatef(0, 0, -translate_z);
    glRotatef(rotate_y, 1, 0, 0);
    glRotatef(rotate_x, 0, 1, 0);
    
    /* Vertices */
    for (j = 0; j < RESOLUTION; j++)
    {
        y = (j + 1) * delta - 1;
        
        
        for (i = 0; i <= RESOLUTION; i++)
        {
            indice = 6 * (i + j * (RESOLUTION + 1));
            
            x = i * delta - 1;
            
            if(type == 3){surface[indice + 3] = findX(x, y , t);}
            else{surface[indice + 3] = x;}
            surface[indice + 4] = waveHeight(x, y, t);
            if(type == 3){surface[indice + 5] = findY(x, y, t);}
            else{ surface[indice + 5] = y;}
 
            if (j != 0)
            {
                /* Values were computed during the previous loop */
                preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
                surface[indice] = surface[preindice + 3];
                surface[indice + 1] = surface[preindice + 4];
                surface[indice + 2] = surface[preindice + 5];
            }
            else
            {
                surface[indice] = x;
                surface[indice + 1] = waveHeight(x, -1, t);
                surface[indice + 2] = -1;
            }
            /* Normals */
            Vector3 n = waveNormal(surface[indice],surface[indice + 2],t);
            normal[indice] = n.x;
            normal[indice+1] = n.y;
            normal[indice+2] = n.z;
            
            n = waveNormal(surface[indice + 3],surface[indice + 5],t);
            normal[indice +3] = n.x;
            normal[indice+4] = n.y;
            normal[indice+5] = n.z;
            
        }
    }
    
    
    /* The ground */
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    
    glColor4f (1.0f, 0.9f, 0.7f,1.0f);
    glBegin (GL_QUADS);
    glNormal3f(0.0f,1.0f,0.0f);
    glVertex3f (-1.0f, 0.0f, -1.0f);
    glVertex3f (-1.0f, 0.0f, 1.0f);
    glVertex3f ( 1.0f, 0.0f, 1.0f);
    glVertex3f ( 1.0f, 0.0f, -1.0f);
    glEnd ();
    
    glTranslatef (0, 0.2, 0);
    
    /* Render wireframe? */
    if (wire_frame != 0)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    /* The water */
    glEnable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.5f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);// open array
    glNormalPointer(GL_FLOAT, 0, normal);
    glVertexPointer(3, GL_FLOAT, 0, surface);
    for (i = 0; i < RESOLUTION; i++)
        glDrawArrays(GL_TRIANGLE_STRIP, i * length, length);
    
    /* Draw normals? */
    if (normals != 0)
    {
        glDisable (GL_TEXTURE_2D);
        glColor3f (1, 0, 0);
        glBegin (GL_LINES);
        for (j = 0; j < RESOLUTION; j++)
            for (i = 0; i <= RESOLUTION; i++)
            {
                indice = 6 * (i + j * (RESOLUTION + 1));
                glVertex3fv (&(surface[indice]));
                glVertex3f (surface[indice] + normal[indice] / 50,
                            surface[indice + 1] + normal[indice + 1] / 50,
                            surface[indice + 2] + normal[indice + 2] / 50);
            }
        glEnd ();
    }
    
    /* End */
    glutSwapBuffers();
    glutPostRedisplay();
}



void Keyboard(unsigned char key, int x, int y){
    switch (key) {
        case 'q':case 27:
            exit(0);
            break;
        case 'l':
            wire_frame = !wire_frame;
            break;
        case 'n':
            normals = !normals;
            break;
        case 'v':
            vaSource = !vaSource;
            break;
        case '0':
            type = 0;
            break;
        case '1':
            type = 1;
            break;
        case '2':
            type = 2;
            break;
        case '3':
            type = 3;
            break;
        default:
            break;
    }
}
int left_click;
int right_click;

void Mouse(int button, int state,int x, int y){
    if (GLUT_LEFT_BUTTON == button)
        left_click = state;
    if (GLUT_RIGHT_BUTTON == button)
        right_click = state;
    xold = x;
    yold = y;
    
    
}

void mouseMotion(int x,int y){
    if (GLUT_DOWN == left_click) {
        rotate_y = rotate_y + (y-yold)/100.0;
        rotate_x = rotate_x + (x-yold)/100.0;
        if (rotate_y > 90)
            rotate_y = 90;
        if(rotate_y < -90)
            rotate_y = -90;
        glutPostRedisplay();
    }
    if (GLUT_DOWN == right_click) {
        translate_z = translate_z + (y-yold)/5.0;
        if (translate_z> 10)
            translate_z = 10;
        if(translate_z < 0.5)
            translate_z = 0.5;
        glutPostRedisplay();
    }
    xold = x;
    yold = y;
    
}

int main(int argc, char **argv) {
    
    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);//双缓冲区，应用于动画
    glutInitWindowPosition(100,100);
    glutInitWindowSize(640,640);
    glutCreateWindow("Water");
    
    InitGL();
    
    // register callbacks
    glutReshapeFunc(ChangeSize);//窗口的大小
    glutDisplayFunc(renderScene);
    //glutSpecialFunc(processSpecialKey);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc(mouseMotion);
    // enter GLUT event processing loop
    glutMainLoop();//处理队列，死循环的方式等待时间
    
    return 0;
}
