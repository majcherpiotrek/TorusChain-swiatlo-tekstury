//
// Created by piotrek on 30.11.16.
//

#include "Torus.h"

void Torus::calculateNormalVector(int i, point3& normalVector, GLfloat u, GLfloat v){
    GLfloat Xu, Xv, Yu, Yv, Zu, Zv;

    Xu = (GLfloat)(cos(M_PI*v)*((-450) * pow(u, 4) + 900 * pow(u, 3) + (-810) * pow(u, 2) +360 * u - 45));
    Xv = (GLfloat)(M_PI*(90 * pow(u, 5) + (-225) * pow(u, 4) + 270 * pow(u, 3) + (-180) * pow(u, 2) + 45 * u)*sin(M_PI*v));
    Yu = (GLfloat)(640 * pow(u, 3) + (-960) * pow(u, 2) + 320 * u);
    Yv = 0;
    Zu = (GLfloat)(sin(M_PI*v)*((-450) * pow(u, 4) + 900 * pow(u, 3) + (-810) * pow(u, 2) + 360 * u - 45));
    Zv = (GLfloat)((-1)*M_PI*cos(M_PI*v)*(90 * pow(u, 5) + (-225) * pow(u, 4) + 270 * pow(u, 3) + (-180) * pow(u, 2) + 45 * u));

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

    for(int i = 0; i < n; i++)
    {
        torus[i] = new point3[N];
        normVec[i] = new point3[N];
        for(int k = 0; k < n; k++)
        {
            calculateNormalVector(i, normVec[i][k], param2D[i][k][0], param2D[i][k][1]);
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



