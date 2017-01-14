//
// Created by piotrek on 30.11.16.
//

#include "Torus.h"

void Torus::calculateNormalVector(int k, point3& normalVector, GLfloat u, GLfloat v){
    GLfloat Xu, Xv, Yu, Yv, Zu, Zv;

    Xu = (GLfloat)(-2*M_PI*sin(2*M_PI*u)*(r*cos(2*M_PI*v)+R));
    Xv = (GLfloat)(-2*M_PI*r*cos(2*M_PI*u)*sin(2*M_PI*v));

    Yu = (GLfloat)(2*M_PI*cos(2*M_PI*u)*(r*cos(2*M_PI*v)+R));
    Yv = (GLfloat)(-2*M_PI*r*sin(2*M_PI*u)*sin(2*M_PI*v));

    Zu = (GLfloat)(0);
    Zv = (GLfloat)(2*M_PI*r*cos(2*M_PI*v));

    normalVector[0] = Yu*Zv - Zu*Yv;
    normalVector[1] = Zu*Xv - Xu*Zv;
    normalVector[2] = Xu*Yv - Yu*Xv;

    GLfloat vectorLength = (GLfloat)(sqrt(pow(normalVector[0], 2) + pow(normalVector[1], 2) + pow(normalVector[2], 2)));


    if (vectorLength != 0) {
        normalVector[0] = normalVector[0] / vectorLength;
        normalVector[1] = normalVector[1] / vectorLength;
        normalVector[2] = normalVector[2] / vectorLength;
    }
}

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
    normVec = new point3*[n];
    tekstury = new point2*[n];

    for(int i = 0; i < n; i++)
    {
        torus[i] = new point3[N];
        normVec[i] = new point3[N];
        tekstury[i] = new point2[N];
        for(int k = 0; k < n; k++)
        {
            calculateNormalVector(i, normVec[i][k], param2D[i][k][0], param2D[i][k][1]);
            tekstury[i][k][0] = param2D[i][k][0];
            tekstury[i][k][1] = param2D[i][k][1];
            /*Wyliczanie punktów torusa na podstawie wzorów z instrukcji*/
            torus[i][k][0] = (float)(( R + r*cos(2*M_PI*param2D[i][k][1]) )*cos(2*M_PI*param2D[i][k][0]));
            torus[i][k][1] = (float)(( R + r*cos(2*M_PI*param2D[i][k][1]) )*sin(2*M_PI*param2D[i][k][0]));
            torus[i][k][2] = (float)(r*sin(2*M_PI*param2D[i][k][1]));
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





