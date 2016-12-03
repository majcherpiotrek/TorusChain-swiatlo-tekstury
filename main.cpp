#include "Torus.h"

/*DODANE*/
static GLfloat viewer[]= {0.0, 0.0, 10.0};
static GLfloat R = 10.0;
static const GLfloat R_min = 2.6;
static const GLfloat R_max = 23.0;

// inicjalizacja położenia obserwatora

static GLfloat theta = 0.0;   // kąt obrotu obiektu
static GLfloat fi = 0.0;   // kąt obrotu obiektu
static GLfloat pix2angle_X;     // przelicznik pikseli na stopnie
static GLfloat pix2angle_Y;

static GLint status = 0;       // stan klawiszy myszy
// 0 - nie naciśnięto żadnego klawisza
// 1 - naciśnięty został lewy klawisz
// 2 - naciśnięty został prawy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy

static int delta_x = 0;        // różnica pomiędzy pozycją bieżącą
// i poprzednią kursora myszy

static int y_pos_old = 0;

static int delta_y = 0;
/*********************************/

GLint _nT = 20;
GLfloat _chainR = 5.0;
GLint _N = 20;  //wymiar dziedzin_N
GLfloat D = 0.01;
point3** colors;
/*************************************************************************************/

enum ChainType{
    STRAIGHT, CIRCLE
};

ChainType chainType = STRAIGHT;

enum Model{
    POINTS, NET, TRIANGLES
};

Model model = POINTS;

void Axes(void)
{

    point3  x_min = {-7.0, 0.0, 0.0};
    point3  x_max = { 7.0, 0.0, 0.0};
    // początek i koniec obrazu osi x

    point3  y_min = {0.0, -7.0, 0.0};
    point3  y_max = {0.0,  7.0, 0.0};
    // początek i koniec obrazu osi y

    point3  z_min = {0.0, 0.0, -7.0};
    point3  z_max = {0.0, 0.0,  7.0};
    //  początek i koniec obrazu osi y

    glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
    glBegin(GL_LINES); // rysowanie osi x

    glVertex3fv(x_min);
    glVertex3fv(x_max);

    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
    glBegin(GL_LINES);  // rysowanie osi y

    glVertex3fv(y_min);
    glVertex3fv(y_max);

    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
    glBegin(GL_LINES); // rysowanie osi z

    glVertex3fv(z_min);
    glVertex3fv(z_max);

    glEnd();

}

/*Funkcja rysująca torus na podstawie przekazanego obiektu w trzech formach w zależności
od globalnego parametru model*/
void drawTorus(Torus& t)
{
    GLint N = t.getN();

    switch(model)
    {
        case POINTS:
        {
            for(int i = 0; i < N-1; i++)
            {
                for(int k = 0; k < N-1; k++)
                {
                    glBegin(GL_POINTS);
                    glColor3f(0.0f, 1.0f, 1.0f);
                    glVertex3f(t.getX(i,k), t.getY(i,k), t.getZ(i,k));
                    glEnd();
                }
            }
            break;
        }/*end case POINTS*/

        case NET:
        {
            for(int i = 0; i < N-1; i++)
            {
                for(int k = 0; k < N-1; k++)
                {
                    glBegin(GL_LINES);

                    glColor3f(0.0f, 0.0f, 1.0f);
                    glVertex3f(t.getX(i,k), t.getY(i,k), t.getZ(i,k));
                    glVertex3f(t.getX(i,k+1), t.getY(i,k+1), t.getZ(i,k+1));
                    glEnd();

                    glBegin(GL_LINES);

                    glColor3f(0.0f, 0.0f, 1.0f);
                    glVertex3f(t.getX(i,k), t.getY(i,k), t.getZ(i,k));
                    glVertex3f(t.getX(i+1,k), t.getY(i+1,k), t.getZ(i+1,k));
                    glEnd();

                    glBegin(GL_LINES);

                    glColor3f(0.0f, 0.0f, 1.0f);
                    glVertex3f(t.getX(i,k), t.getY(i,k), t.getZ(i,k));
                    glVertex3f(t.getX(i+1,k+1), t.getY(i+1,k+1), t.getZ(i+1,k+1));
                    glEnd();

                }
            }
            break;
        }/*end case NET*/

        case TRIANGLES:
        {
            for(int i = 0; i < N-1; i++)
            {
                for(int k = 0; k < N-1; k++)
                {
                    if(i < N-2) //usuwanie linii łączenia (dla ostatniego i przypisujey kolory z 0)
                    {
                        glBegin(GL_TRIANGLES);
                        glColor3f(colors[i][k][0], colors[i][k][1], colors[i][k][2]);
                        glVertex3f(t.getX(i,k), t.getY(i,k), t.getZ(i,k));
                        glColor3f(colors[i][k+1][0], colors[i][k+1][1], colors[i][k+1][2]);
                        glVertex3f(t.getX(i,k+1), t.getY(i,k+1), t.getZ(i,k+1));
                        glColor3f(colors[i+1][k][0], colors[i+1][k][1], colors[i+1][k][2]);
                        glVertex3f(t.getX(i+1,k), t.getY(i+1,k), t.getZ(i+1,k));
                        glEnd();

                        glBegin(GL_TRIANGLES);
                        glColor3f(colors[i][k+1][0], colors[i][k+1][1], colors[i][k+1][2]);
                        glVertex3f(t.getX(i,k+1), t.getY(i,k+1), t.getZ(i,k+1));
                        glColor3f(colors[i+1][k+1][0], colors[i+1][k+1][1], colors[i+1][k+1][2]);
                        glVertex3f(t.getX(i+1,k+1), t.getY(i+1,k+1), t.getZ(i+1,k+1));
                        glColor3f(colors[i+1][k][0], colors[i+1][k][1], colors[i+1][k][2]);
                        glVertex3f(t.getX(i+1,k), t.getY(i+1,k), t.getZ(i+1,k));
                        glEnd();
                    }
                    else
                    {
                        glBegin(GL_TRIANGLES);
                        glColor3f(colors[i][k][0], colors[i][k][1], colors[i][k][2]);
                        glVertex3f(t.getX(i,k), t.getY(i,k), t.getZ(i,k));
                        glColor3f(colors[i][k+1][0], colors[i][k+1][1], colors[i][k+1][2]);
                        glVertex3f(t.getX(i,k+1), t.getY(i,k+1), t.getZ(i,k+1));
                        glColor3f(colors[0][k][0], colors[0][k][1], colors[0][k][2]);
                        glVertex3f(t.getX(i+1,k), t.getY(i+1,k), t.getZ(i+1,k));
                        glEnd();

                        glBegin(GL_TRIANGLES);
                        glColor3f(colors[i][k+1][0], colors[i][k+1][1], colors[i][k+1][2]);
                        glVertex3f(t.getX(i,k+1), t.getY(i,k+1), t.getZ(i,k+1));
                        glColor3f(colors[0][k+1][0], colors[0][k+1][1], colors[0][k+1][2]);
                        glVertex3f(t.getX(i+1,k+1), t.getY(i+1,k+1), t.getZ(i+1,k+1));
                        glColor3f(colors[0][k][0], colors[0][k][1], colors[0][k][2]);
                        glVertex3f(t.getX(i+1,k), t.getY(i+1,k), t.getZ(i+1,k));
                        glEnd();
                    }
                }
            }
        }/*end case TRIANGLES*/
    }
}

/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia wartości odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{


    if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        x_pos_old=x;        // przypisanie aktualnie odczytanej pozycji kursora
        y_pos_old=y;                     // jako pozycji poprzedniej
        status = 1;          // wcięnięty został lewy klawisz myszy
    }
    else
    if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        y_pos_old = y;
        status = 2;
    }
    else
        status = 0;          // nie został wcięnięty żaden klawisz

}

/*************************************************************************************/
// Funkcja "monitoruje" położenie kursora myszy i ustawia wartości odpowiednich
// zmiennych globalnych

void Motion( GLsizei x, GLsizei y )
{


    delta_x = x - x_pos_old;     // obliczenie różnicy położenia kursora myszy

    x_pos_old = x;            // podstawienie bieżącego położenia jako poprzednie

    delta_y = y - y_pos_old;

    y_pos_old = y;

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
    for (int i = 0; i < nT; ++i) {
        chain3D[i][0] = firstTorus + i*chainR;
        chain3D[i][1] = 0;
        chain3D[i][2] = 0;
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

            if( i == 0 )
                id2 = i;
            else
                id2 = i - 1;

            vec[0] = chain3D[id1][0] - chain3D[id2][0];
            vec[1] = chain3D[id1][1] - chain3D[id2][1];
            vec[2] = chain3D[id1][2] - chain3D[id2][2];
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
    // Czyszczenie macierzy bieżącej

    gluLookAt(viewer[0],viewer[1],viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // Zdefiniowanie położenia obserwatora

    if(status == 1)                     // jeśli lewy klawisz myszy wcięnięty
    {


        theta += delta_x*pix2angle_X;
        fi += delta_y*pix2angle_Y;   // modyfikacja kąta obrotu o kat proporcjonalny
        // do różnicy położeń kursora myszy

        //viewer[0] = R * cos(theta) * cos(fi);
        //viewer[1] = R * sin(fi);
        //viewer[2] = R * sin(theta) * cos(fi);
    }
    if(status == 2)
    {
        viewer[2] += delta_y*0.05;
        if (viewer[2] > R_max)
            viewer[2] = R_max;

        if(viewer[2] < R_min)
            viewer[2] = R_min;

        std::cout<<viewer[2]<<std::endl;
    }

    gluLookAt(viewer[0],viewer[1],viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy kąt
    glRotatef(fi, 1.0, 0.0, 0.0);


    Axes();
    // Narysowanie osi przy pomocy funkcji zdefiniowanej wyżej


    switch (chainType){
        case STRAIGHT:{ drawStraightchain(_nT, 0.5, D); break;}
        case CIRCLE: { drawChain(_nT, (GLint )_chainR); break;}
        }


    glFlush();
    // Przekazanie poleceń rysujących do wykonania



    glutSwapBuffers();
    //

}

/*************************************************************************************/

// Funkcja ustalająca stan renderowania

void keys(unsigned char key, int x, int y)
{
    switch (key) {
        case 'p': { model = POINTS; break;}
        case 'w': { model = NET; break;}
        case 's': { model = TRIANGLES; break;}
        case '+': { _nT += 2; break;}
        case '-': {
            if (_nT > 10)
                _nT -= 2;
            break;
        }
        case '>': {
            if(chainType == STRAIGHT)
                D += 0.01; break;
        }
        case '<': {
            if(chainType == STRAIGHT)
                D -= 0.01;
            break;
        }
        case 't': {
            if(chainType == STRAIGHT)
                chainType = CIRCLE;
            else
                chainType = STRAIGHT;
            break;
        }
    }





    RenderScene(); // przerysowanie obrazu sceny
}

void MyInit(void)
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
// Kolor czyszcący (wypełnienia okna) ustawiono na czarny

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