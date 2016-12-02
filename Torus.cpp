//
// Created by piotrek on 30.11.16.
//

#include "Torus.h"

Torus::Torus(GLint N, GLfloat _r, GLfloat _R)
{
    this->n = N;
    this->r = _r;
    this->R = _R;

    point2** param2D = new point2*[n];

    for(int i = 0; i < n; i++)
    {
        /*Tworzenie dziedziny*/
        param2D[i] = new point2[n];
        for(int k = 0; k < n; k++)
        {
            param2D[i][k][0] = (float)i/(n-1);
            param2D[i][k][1] = (float)k/(n-1);
        }
    }

    torus = new point3*[n];

    for(int i = 0; i < n; i++)
    {
        torus[i] = new point3[N];
        for(int k = 0; k < n; k++)
        {
            /*Wyliczanie punktów torusa na podstawie wzorów z instrukcji*/
            torus[i][k][0] = ( R + r*cos(2*M_PI*param2D[i][k][1]) )*cos(2*M_PI*param2D[i][k][0]);
            torus[i][k][1] = ( R + r*cos(2*M_PI*param2D[i][k][1]) )*sin(2*M_PI*param2D[i][k][0]);
            torus[i][k][2] = r*sin(2*M_PI*param2D[i][k][1]);
        }
    }

    delete[] param2D;
}

Torus::~Torus()
{
    if(torus != nullptr)
    {
        for(int i = 0; i < n; i++)
            delete[] torus[i];

        delete[] torus;
    }
}

GLfloat Torus::getX(GLint u, GLint v)
{
    return torus[u][v][0];
}

GLfloat Torus::getY(GLint u, GLint v)
{
    return torus[u][v][1];
}

GLfloat Torus::getZ(GLint u, GLint v)
{
    return torus[u][v][2];
}

GLint Torus::getN()
{
    return n;
}

GLfloat Torus::getOuterR()
{
    return R;
}

GLfloat Torus::getInnerR()
{
    return r;
}

