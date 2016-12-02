#include "Torus.h"


GLint _nT = 20;
GLfloat _chainR = 5.0;
GLint _N = 20;  //wymiar dziedzin_N
point3** colors;
/*************************************************************************************/
static GLfloat theta[] = {0.0, 0.0, 0.0}; // trzy kąty obrotu
// Funkcja rysująca osie układu współrzędnych
bool spin = true;

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
void RenderScene(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszczącym

    glLoadIdentity();
    // Czyszczenie macierzy bieżącej

    //Axes();
    // Narysowanie osi przy pomocy funkcji zdefiniowanej wyżej

    glRotatef(theta[0], 1.0, 0.0, 0.0);

    glRotatef(theta[1], 0.0, 1.0, 0.0);

    glRotatef(theta[2], 0.0, 0.0, 1.0);


    drawChain(_nT, _chainR);


    glFlush();
    // Przekazanie poleceń rysujących do wykonania



    glutSwapBuffers();
    //

}

/*************************************************************************************/

// Funkcja ustalająca stan renderowania

void keys(unsigned char key, int x, int y)
{
    if(key == 'p') model = POINTS;
    if(key == 'w') model = NET;
    if(key == 's') model = TRIANGLES;
    if(key == ' ') spin = !spin;
    if(key == '+') _nT += 2;
    if(key == '-' && _nT > 10) _nT -= 2;

    RenderScene(); // przerysowanie obrazu sceny
}

void spinEgg()
{

    if(spin)
    {
        theta[0] -= 0.5;
        if( theta[0] > 360.0 ) theta[0] -= 360.0;

        theta[1] -= 0.5;
        if( theta[1] > 360.0 ) theta[1] -= 360.0;

        theta[2] -= 0.5;
        if( theta[2] > 360.0 ) theta[2] -= 360.0;

        glutPostRedisplay(); //odświeżenie zawartości aktualnego okna
    }

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

    AspectRatio = (GLfloat)horizontal/(GLfloat)vertical;
    // Wyznaczenie współczynnika  proporcji okna
    // Gdy okno nie jest kwadratem wymagane jest określenie tak zwanej
    // przestrzeni ograniczającej pozwalającej zachować właściwe
    // proporcje rysowanego obiektu.
    // Do okreslenia przestrzeni ograniczjącej służy funkcja
    // glOrtho(...)

    if(horizontal <= vertical)

        glOrtho(-7.5,7.5,-7.5/AspectRatio,7.5/AspectRatio,10.0, -10.0);

    else

        glOrtho(-7.5*AspectRatio,7.5*AspectRatio,-7.5,7.5,10.0,-10.0);

    glMatrixMode(GL_MODELVIEW);
    // Przełączenie macierzy bieżącej na macierz widoku modelu

    glLoadIdentity();
    // Czyszcenie macierzy bieżącej

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

    glutCreateWindow("TorusChain");

    glutDisplayFunc(RenderScene);
    // Określenie, że funkcja RenderScene będzie funkcją zwrotną
    // (callback function).  Bedzie ona wywoływana za każdym razem
    // gdy zajdzie potrzba przeryswania okna

    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
    // zazmiany rozmiaru okna

    MyInit();
    // Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
    // inicjalizacje konieczne  przed przystąpieniem do renderowania

    glEnable(GL_DEPTH_TEST);
    // Włączenie mechanizmu usuwania powierzchni niewidocznych

    glutKeyboardFunc(keys);

    glutIdleFunc(spinEgg);

    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT
    return 0;
}