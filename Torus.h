//
// Created by piotrek on 30.11.16.
//

#ifndef TORUSCHAIN2_TORUS_H
#define TORUSCHAIN2_TORUS_H


#include <GL/glut.h>
#include <iostream>
#include <math.h>

typedef float point3[3];

typedef float point2[2];

/*Klasa reprezentująca torus w przestrzeni 3D*/
class Torus
{
private:
    point3** torus;

    GLfloat r;
    GLfloat R;
    GLint n;
    void calculateNormalVector(int i, point3& normalVector, GLfloat u, GLfloat v);
public:
    point3** normVec;
    Torus(GLint N, GLfloat r, GLfloat R);
    ~Torus();

    GLfloat getX(GLint u, GLint v);
    GLfloat getY(GLint u, GLint v);
    GLfloat getZ(GLint u, GLint v);

    GLint getN();
    GLfloat getOuterR();
    GLfloat getInnerR();
};


#endif //TORUSCHAIN2_TORUS_H
