/////////////////////
#include "Torus.h"
static bool keepMoving = true;
// inicjalizacja położenia obserwatora
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy kąty obrotu
// inicjalizacja położenia obserwatora


int x_pos_old = 0;

int y_pos_old = 0;

int status = 0;

static int delta_x = 0;

static int delta_y = 0;

static GLfloat pix2angle_X;     // przelicznik pikseli na stopnie
static GLfloat pix2angle_Y;

float x_swiatla = -200;
float y_swiatla = 0;
float x_swiatla2 = 200;
float y_swiatla2 = 0;
float z_swiatla = 10;
float z_swiatla2 = 10;

/*********************************/

GLint _nT = 20;
GLfloat _chainR = 5.0;
GLint _N = 20;  //wymiar dziedzin_N
GLfloat D = 0.01;
point3** colors;
float A = 0.0;
/*************************************************************************************/

enum ChainType{
    STRAIGHT, CIRCLE
};

ChainType chainType = STRAIGHT;

/*Funkcja rysująca torus na podstawie przekazanego obiektu w trzech formach w zależności
od globalnego parametru model*/
void drawTorus(Torus& t)
{
    GLint N = t.getN();


            for(int i = 0; i < N-1; i++)
            {
                for(int k = 0; k < N-1; k++)
                {
                    if(i < N-2) //usuwanie linii łączenia (dla ostatniego i przypisujey kolory z 0)
                    {
                        glBegin(GL_TRIANGLES);
                        glNormal3fv(t.normVec[i][k]);
                        glColor3f(colors[i][k][0], colors[i][k][1], colors[i][k][2]);
                        glVertex3f(t.getX(i,k), t.getY(i,k), t.getZ(i,k));
                        glNormal3fv(t.normVec[i][k+1]);
                        glColor3f(colors[i][k+1][0], colors[i][k+1][1], colors[i][k+1][2]);
                        glVertex3f(t.getX(i,k+1), t.getY(i,k+1), t.getZ(i,k+1));
                        glNormal3fv(t.normVec[i+1][k]);
                        glColor3f(colors[i+1][k][0], colors[i+1][k][1], colors[i+1][k][2]);
                        glVertex3f(t.getX(i+1,k), t.getY(i+1,k), t.getZ(i+1,k));
                        glEnd();

                        glBegin(GL_TRIANGLES);
                        glNormal3fv(t.normVec[i][k+1]);
                        glColor3f(colors[i][k+1][0], colors[i][k+1][1], colors[i][k+1][2]);
                        glVertex3f(t.getX(i,k+1), t.getY(i,k+1), t.getZ(i,k+1));
                        glNormal3fv(t.normVec[i][k+1]);
                        glColor3f(colors[i+1][k+1][0], colors[i+1][k+1][1], colors[i+1][k+1][2]);
                        glVertex3f(t.getX(i+1,k+1), t.getY(i+1,k+1), t.getZ(i+1,k+1));
                        glNormal3fv(t.normVec[i+1][k]);
                        glColor3f(colors[i+1][k][0], colors[i+1][k][1], colors[i+1][k][2]);
                        glVertex3f(t.getX(i+1,k), t.getY(i+1,k), t.getZ(i+1,k));
                        glEnd();
                    }
                    else
                    {
                        glBegin(GL_TRIANGLES);
                        glNormal3fv(t.normVec[i][k]);
                        glColor3f(colors[i][k][0], colors[i][k][1], colors[i][k][2]);
                        glVertex3f(t.getX(i,k), t.getY(i,k), t.getZ(i,k));
                        glNormal3fv(t.normVec[i][k+1]);
                        glColor3f(colors[i][k+1][0], colors[i][k+1][1], colors[i][k+1][2]);
                        glVertex3f(t.getX(i,k+1), t.getY(i,k+1), t.getZ(i,k+1));
                        glNormal3fv(t.normVec[0][k]);
                        glColor3f(colors[0][k][0], colors[0][k][1], colors[0][k][2]);
                        glVertex3f(t.getX(i+1,k), t.getY(i+1,k), t.getZ(i+1,k));
                        glEnd();

                        glBegin(GL_TRIANGLES);
                        glNormal3fv(t.normVec[i][k+1]);
                        glColor3f(colors[i][k+1][0], colors[i][k+1][1], colors[i][k+1][2]);
                        glVertex3f(t.getX(i,k+1), t.getY(i,k+1), t.getZ(i,k+1));
                        glNormal3fv(t.normVec[0][k+1]);
                        glColor3f(colors[0][k+1][0], colors[0][k+1][1], colors[0][k+1][2]);
                        glVertex3f(t.getX(i+1,k+1), t.getY(i+1,k+1), t.getZ(i+1,k+1));
                        glNormal3fv(t.normVec[0][k]);
                        glColor3f(colors[0][k][0], colors[0][k][1], colors[0][k][2]);
                        glVertex3f(t.getX(i+1,k), t.getY(i+1,k), t.getZ(i+1,k));
                        glEnd();
                    }
                }
            }


}

void Mouse(int btn, int state, int x, int y)
{


    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora
        y_pos_old = y;
        status = 1;           // wcięnięty został lewy klawisz myszy
    }
    else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        x_pos_old = x;
        y_pos_old = y;
        status = 2;
    }
    else

        status = 0;          // nie został wcięnięty żaden klawisz
}

/*************************************************************************************/
// Funkcja "monitoruje" położenie kursora myszy i ustawia wartości odpowiednich
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

    delta_x = x - x_pos_old;     // obliczenie różnicy położenia kursora myszy
    delta_y = y - y_pos_old;
    x_pos_old = x;
    y_pos_old = y;            // podstawienie bieżącego położenia jako poprzednie

    glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/


/*
Funkcja rysująca łańcuch torusów
nT - ilość torusów w łańcuchu
chainR - promień okręgu, na którym narysowany jest łańcuch
*/
void drawChain(GLint nT, GLint chainR)
{
    GLfloat _R = 2*chainR*M_PI/nT;

    GLfloat _r = _R/7; //mały promień torusa (przekroju brzegu)
    _R = 5*_r; // duży promień torusa+1


    Torus torus = *(new Torus(_N, _r, _R));
    // std::cout << R << std::endl << r << std::endl << N << std::endl;
    GLfloat* chain1D = new GLfloat[nT];
    for(int i = 0; i < nT; i++)
        chain1D[i] = (float)i/nT;

    point3* chain3D = new point3[nT];

    /*Obliczanie punktów na okręgu, na którym będzie narysowany torus. Każdy z punktów
    to środek kolejnego torusa*/
    for(int i = 0; i < nT; i++)
    {
        chain3D[i][0]= chainR*cos(2*M_PI*chain1D[i]);
        chain3D[i][1]= chainR*sin(2*M_PI*chain1D[i]);
        chain3D[i][2]= 0;
    }

    /*Tu są rysowane torusy o środkach wyliczonych wyżej*/
    for(int i = 0; i < nT; i++)
    {
        point3 vec; //vector wyznaczający prostą, dookoła której obrócimy torus
        GLint id1; //indeks  następnego torusa
        GLint id2; //indeks poprzedniego torusa

        if( i == nT - 1)
            id1 = 0;
        else
            id1 = i + 1;

        if( i == 0 )
            id2 = nT-1;
        else
            id2 = i - 1;

        vec[0] = chain3D[id1][0] - chain3D[id2][0];
        vec[1] = chain3D[id1][1] - chain3D[id2][1];
        vec[2] = chain3D[id1][2] - chain3D[id2][2];

        /*Obracamy co drugi torus w łańcuchu*/
        if(i%2)
        {
            /*Co drugi torus przesuwamy tak, aby jego środek był
            na odcinku łączącym poprzedni i następny torus w łańcuchu.
            Robimy to, aby po obrocie, krawędzie torusów na siebie nie zachodziły*/
            point3 med_vec;
            med_vec[0] = (-1)*vec[0]/2;
            med_vec[1] = (-1)*vec[1]/2;
            med_vec[2] = (-1)*vec[2]/2;

            point3 u;
            u[0] = chain3D[id1][0] - chain3D[i][0];
            u[1] = chain3D[id1][1] - chain3D[i][1];
            u[2] = chain3D[id1][2] - chain3D[i][2];

            point3 x;
            x[0] = u[0] + med_vec[0];
            x[1] = u[1] + med_vec[1];
            x[2] = u[2] + med_vec[2];

            chain3D[i][0] += x[0];
            chain3D[i][1] += x[1];
            chain3D[i][2] += x[2];
        }

        //Przesunięcie  z (0,0,0) do punktu w którym będzie narysowany torus
        glTranslated(chain3D[i][0], chain3D[i][1], chain3D[i][2]);

        //Obrót o 90 stopni co drugiego torusa wokół osi wyznaczanej przez wektor vec
        if(i%2)
            glRotated(90.0f, vec[0], vec[1], vec[2]);


        drawTorus(torus);

        //Obrót z powrotem
        if(i%2)
            glRotated(-90.0f, vec[0], vec[1], vec[2]);

        //Przesunięcie z powrotem do (0,0,0)
        glTranslated((-1)*chain3D[i][0], (-1)*chain3D[i][1], (-1)*chain3D[i][2]);
    }
}

void drawStraightchain(GLint nT, GLfloat torusR, GLfloat d){

    GLfloat r = torusR/8; //promien przekroju brzegu torusa
    if( d < 0)
        d = 0;
    if( d > torusR - 3*r)
        d = torusR - 3*r;

    GLfloat chainR = 2*torusR - (2*r + d); //odlegołość między środkami torusów

    Torus torus = *(new Torus(_N, r, torusR));


    point3* chain3D = new point3[nT];

    GLfloat firstTorus = -(nT/2)*chainR;
    int beg;
    for (int i = 0; i < nT; ++i) {
         float X = firstTorus + i*chainR;
        if(X==0)
            beg = i;

        /*wyliczamy kolejne punkty należące do funkcji kwadratowej*/
        chain3D[i][0] = X;
        chain3D[i][1] = A*(float)pow(chain3D[i][0], 2);
        chain3D[i][2] = 0;
    }

    std::cout <<"BEG " << beg <<std::endl;

    /*liczymy punkty dla x > 0 */
    if( A != 0){
        for(int i = beg; i < nT-1; ++i){
            point3 vector;
            /*Wektor pokazujący kierunek mieszy punktem a punktem następnym*/
            vector[0] = chain3D[i+1][0] - chain3D[i][0];
            vector[1] = chain3D[i+1][1] - chain3D[i][1];
            vector[2] = chain3D[i+1][2] - chain3D[i][2];

            /*Tworzymy wektor jednostkowy*/
            double vectorLength = sqrt( vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
            vector[0] = (float)(vector[0]/vectorLength);
            vector[1] = (float)(vector[1]/vectorLength);
            vector[2] = (float)(vector[2]/vectorLength);

            /*Mnożymy przez odległośc między torusami*/
            vector[0] = vector[0]*chainR;
            vector[1] = vector[1]*chainR;
            vector[2] = vector[2]*chainR;

            /*Wyliczamy środek następnego torusa*/
            chain3D[i+1][0] = chain3D[i][0] + vector[0];
            chain3D[i+1][1] = chain3D[i][1] + vector[1];
            chain3D[i+1][2] = chain3D[i][2] + vector[2];

        }

        /*liczymy punkty dla x > 0 */
        for(int i = beg; i > 0; --i){
            point3 vector;
            /*Wektor pokazujący kierunek mieszy punktem a punktem następnym*/
            vector[0] = chain3D[i-1][0] - chain3D[i][0];
            vector[1] = chain3D[i-1][1] - chain3D[i][1];
            vector[2] = chain3D[i-1][2] - chain3D[i][2];

            /*Tworzymy wektor jednostkowy*/
            double vectorLength = sqrt( vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
            vector[0] = (float)(vector[0]/vectorLength);
            vector[1] = (float)(vector[1]/vectorLength);
            vector[2] = (float)(vector[2]/vectorLength);

            /*Mnożymy przez odległośc między torusami*/
            vector[0] = vector[0]*chainR;
            vector[1] = vector[1]*chainR;
            vector[2] = vector[2]*chainR;

            /*Wyliczamy środek następnego torusa*/
            chain3D[i-1][0] = chain3D[i][0] + vector[0];
            chain3D[i-1][1] = chain3D[i][1] + vector[1];
            chain3D[i-1][2] = chain3D[i][2] + vector[2];
        }
    }



    for (int i = 0; i < nT ; ++i) {

        point3 vec; //vector wyznaczający prostą, dookoła której obrócimy torus
        /*Obracamy co drugi torus w łańcuchu*/
        bool obrot = !(i%2 == 0);
        if(obrot)
        {

            GLint id1; //indeks  następnego torusa
            GLint id2; //indeks poprzedniego torusa

            if( i == nT - 1)
                id1 = i;
            else
                id1 = i + 1;


            id2 = i - 1;

            vec[0] = chain3D[id1][0] - chain3D[id2][0];
            vec[1] = chain3D[id1][1] - chain3D[id2][1];
            vec[2] = chain3D[id1][2] - chain3D[id2][2];
        }

        //Przesunięcie  z (0,0,0) do punktu w którym będzie narysowany torus
        glTranslated(chain3D[i][0], chain3D[i][1], chain3D[i][2]);

        //Obrót o 90 stopni co drugiego torusa wokół osi wyznaczanej przez wektor vec
        if(obrot)
            glRotated(90.0f, vec[0], vec[1], vec[2]);

        drawTorus(torus);

        //Obrót z powrotem
        if(obrot)
            glRotated(-90.0f, vec[0], vec[1], vec[2]);

        //Przesunięcie z powrotem do (0,0,0)
        glTranslated((-1)*chain3D[i][0], (-1)*chain3D[i][1], (-1)*chain3D[i][2]);
    }
}

void RenderScene(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszczącym

    glLoadIdentity();
    // Czyszczenie macierzy bie??cej

    gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // Zdefiniowanie położenia obserwatora

    if (status == 1)                     // jeśli lewy klawisz myszy wcięnięty
    {
        x_swiatla += delta_x*pix2angle_X;
        y_swiatla -=delta_y*pix2angle_Y;
        z_swiatla = 10;
    }
    else if (status == 2) {
        x_swiatla2 += delta_x*pix2angle_X;
        y_swiatla2 -=delta_y*pix2angle_Y;
        z_swiatla2 = 10;
    }
    gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    GLfloat light_position[] = { x_swiatla/5, y_swiatla/5 , z_swiatla, 1.0f };
    GLfloat light_position2[] = { x_swiatla2/5 , y_swiatla2/5, z_swiatla2, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position2);

    glRotatef(theta[0], 1.0, 0.0, 0.0);
    glRotatef(theta[1], 0.0, 1.0, 0.0);
    glRotatef(theta[2], 0.0, 0.0, 1.0);
    switch (chainType){
        case STRAIGHT:{ drawStraightchain(_nT, 0.5, D); break;}
        case CIRCLE: { drawChain(_nT, (GLint )_chainR); break;}
    }

    glFlush();

    glutSwapBuffers();
}

/*************************************************************************************/

// Funkcja ustalająca stan renderowania

void keys(unsigned char key, int x, int y)
{
    switch (key) {

        case '+': { _nT += 2; break;}
        case '-': {
            if (_nT > 10)
                _nT -= 2;
            break;
        }

        case 't': {
            if (chainType == STRAIGHT)
                chainType = CIRCLE;
            else
                chainType = STRAIGHT;
            break;
        }

        case ' ': {
            keepMoving = !keepMoving;
            break;
        }

    }

    RenderScene(); // przerysowanie obrazu sceny
}

void spinEgg()
{
    if(keepMoving){
        theta[0] -= 0.5;
        if (theta[0] > 360.0) theta[0] -= 360.0;

        theta[1] -= 0.5;
        if (theta[1] > 360.0) theta[1] -= 360.0;

        theta[2] -= 0.5;
        if (theta[2] > 360.0) theta[2] -= 360.0;
    }


    glutPostRedisplay(); //odświeżenie zawartości aktualnego okna
}

void MyInit(void)
{


    /*************************************************************************************/

    //  Definicja materiału z jakiego zrobiony jest czajnik
    //  i definicja źródła światła

    /*************************************************************************************/
    // Definicja materiału z jakiego zrobiony jest czajnik

    GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    // współczynniki ka =[kar,kag,kab] dla światła otoczenia

    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    // współczynniki kd =[kdr,kdg,kdb] światła rozproszonego

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    // współczynniki ks =[ksr,ksg,ksb] dla światła odbitego

    GLfloat mat_shininess = { 100.0 };
    // współczynnik n opisujący połysk powierzchni

    /*************************************************************************************/
    // Definicja źródła światła

    GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
    // położenie źródła


    GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    // składowe intensywności świecenia źródła światła otoczenia
    // Ia = [Iar,Iag,Iab]

    GLfloat light_diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    // składowe intensywności świecenia źródła światła powodującego
    // odbicie dyfuzyjne Id = [Idr,Idg,Idb]

    GLfloat light_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    // składowe intensywności świecenia źródła światła powodującego
    // odbicie kierunkowe Is = [Isr,Isg,Isb]

    GLfloat att_constant = { 1.0f };
    // składowa stała ds dla modelu zmian oświetlenia w funkcji
    // odległości od źródła

    GLfloat att_linear = { 0.05f };
    // składowa liniowa dl dla modelu zmian oświetlenia w funkcji
    // odległości od źródła

    GLfloat att_quadratic = { 0.001f };
    // składowa kwadratowa dq dla modelu zmian oświetlenia w funkcji
    // odległości od źródła


    GLfloat light_position2[] = { 10.0f, 0.0f, 0.0f, 1.0f };
    GLfloat light_ambient2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat light_diffuse2[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    GLfloat light_specular2[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    GLfloat att_constant2 = { 1.0f };
    GLfloat att_linear2 = { 0.05f };
    GLfloat att_quadratic2 = { 0.001f };
    /*************************************************************************************/
    // Ustawienie parametrów materiału i źródła światła

    /*************************************************************************************/
    // Ustawienie patrametrów materiału


    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    /*************************************************************************************/
    // Ustawienie parametrów 1 źródła

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

    // Ustawienie parametrów 2 źródła

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular2);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant2);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear2);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic2);


    /*************************************************************************************/
    // Ustawienie opcji systemu oświetlania sceny

    glShadeModel(GL_SMOOTH); // właczenie łagodnego cieniowania
    glEnable(GL_LIGHTING);   // właczenie systemu oświetlenia sceny
    glEnable(GL_LIGHT0);     // włączenie źródła o numerze 0
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST); // włączenie mechanizmu z-bufora

/*************************************************************************************/
}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical )
{
    pix2angle_X = 360/(float)horizontal;  // przeliczenie pikseli na stopnie
    pix2angle_Y = 360/(float)vertical;  // przeliczenie pikseli na stopnie

    GLfloat AspectRatio;
    // Deklaracja zmiennej AspectRatio  określającej proporcję
    // wymiarów okna

    if(vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0

        vertical = 1;

    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielkościokna okna widoku (viewport)
    // W tym przypadku od (0,0) do (horizontal, vertical)

    glMatrixMode(GL_PROJECTION);
    // Przełączenie macierzy bieżącej na macierz projekcji

    glLoadIdentity();
    // Czyszcznie macierzy bieżącej

    gluPerspective(70, 1.0, 1.0, 50.0);
    // Ustawienie parametrów dla rzutu perspektywicznego


    if(horizontal <= vertical)
        glViewport(0, (vertical-horizontal)/2, horizontal, horizontal);

    else
        glViewport((horizontal-vertical)/2, 0, vertical, vertical);
    // Ustawienie wielkości okna okna widoku (viewport) w zależności
    // relacji pomiędzy wysokością i szerokością okna

    glMatrixMode(GL_MODELVIEW);
    // Przełączenie macierzy bieżącej na macierz widoku modelu

    glLoadIdentity();
    // Czyszczenie macierzy bieżącej


}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli



int main(int argc, char** argv)
{
    srand(time(NULL));

    colors = new point3*[_N];

    for(int i = 0; i < _N; i++)
    {
        colors[i] = new point3[_N];

        for(int k = 0; k < _N; k++)
        {
            colors[i][k][0] = (float)(rand()%1001)/1000;
            colors[i][k][1] = (float)(rand()%1001)/1000;
            colors[i][k][2] = (float)(rand()%1001)/1000;
        }
    }

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);

    glutInitWindowSize(300, 300);

    glutCreateWindow("TorusChain interactive");

    glutDisplayFunc(RenderScene);
    // Określenie, że funkcja RenderScene będzie funkcją zwrotną
    // (callback function).  Bedzie ona wywoływana za każdym razem
    // gdy zajdzie potrzba przeryswania okna

    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
    // zazmiany rozmiaru okna

    glutIdleFunc(spinEgg);

    glutMouseFunc(Mouse);
    // Ustala funkcję zwrotną odpowiedzialną za badanie stanu myszy

    glutMotionFunc(Motion);
    // Ustala funkcję zwrotną odpowiedzialną za badanie ruchu myszy


    MyInit();
    // Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
    // inicjalizacje konieczne  przed przystąpieniem do renderowania

    glEnable(GL_DEPTH_TEST);
    // Włączenie mechanizmu usuwania powierzchni niewidocznych

    glutKeyboardFunc(keys);

    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT
    return 0;
}