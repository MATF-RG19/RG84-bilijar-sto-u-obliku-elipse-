#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <math.h>

#define TIMER_ID 0
#define TIMER_INTERVAL 20

/* Dimenzije prozora */
static int window_width, window_height;
static float v_x, v_y;          /* Komponente vektora brzine kretanja
                                 * kugle. */
static float e = sqrt(1*1 - 0.7*0.7);
static float x_curr, y_curr;    /* Tekuce koordinate centra kugle. */
static int animation_ongoing;   /* Fleg koji odredjuje da li je
                                 * animacija u toku. */
static float x_cen, y_cen;      /* koordinate leve zize(centra pocetne kugle) */

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);
static void on_display(void);
static void on_reshape(int width, int height);
static void draw_elipse(float a, float b);
static void draw_circle(float r, float a);

int main(int argc, char **argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    /* Registruju se funkcije za obradu dogadjaja. */
    glutKeyboardFunc(on_keyboard);
	glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* Inicijalizuju se globalne promjenljive. */
   
   e = sqrt(1*1 - 0.7*0.7);
   x_curr=-sqrt(1*1 - 0.7*0.7);
   y_curr=0; 
   
   x_cen=-sqrt(1*1 - 0.7*0.7);
   y_cen=0;

    v_x = 0.01;
    v_y = 0.01;

    /* Na pocetku je animacija neaktivna */
    animation_ongoing = 0;

    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(1, 1, 1, 0);
    glEnable(GL_DEPTH_TEST);

    /* Ulazi se u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;

    case 'g':
    case 'G':
        /* Pokrece se animacija. */
        if (!animation_ongoing) {
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            animation_ongoing = 1;
        }
        break;

    case 's':
    case 'S':
        /* Zaustavlja se animacija. */
        animation_ongoing = 0;
        break;
    
        /*Restartuje se apk(vraca se na pocetak)*/
    case 'r':
    case 'R':
      
         x_curr=x_cen-0.01;
         y_curr=y_cen-0.01;
           animation_ongoing = 0;
         break;
    }
}

static void on_reshape(int width, int height)
{
	window_width = width;
    window_height = height;
}

static void on_timer(int value)
{
    /*
     * Proverava se da li callback dolazi od odgovarajuceg tajmera.
     */
    if (value != TIMER_ID)
        return;

    /* Azuriraju se koordinate centra kruga. */
    x_curr += v_x;
    if (y_curr+0.06 > sqrt(0.49 - 0.49*x_curr*x_curr)) {
        v_x *= -1;
    }

    y_curr += v_y;
    if ( x_curr+0.06 > sqrt( (0.49 - y_curr*y_curr) / 0.49)) {
        v_y *= -1;
    }

    /* Forsira se ponovno iscrtavanje prozora. */
    glutPostRedisplay();

    /* Po potrebi se ponovo postavlja tajmer. */
    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    }
}

static void on_display(void)
{
    /* Postavlja se boja svih piksela na zadatu boju pozadine. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    glViewport(0,0,window_width,window_height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) window_width / (float)window_height, 0, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    /* Crta se elipsa na odgovarajucoj poziciji. */
    
	glPushMatrix();
        glColor3f(0,0.6,0);
        draw_elipse(1,0.7);
    glPopMatrix();
      
    //Pocetna pozicija(prva ziza)
    glPushMatrix();
        glTranslatef(-e,0,0);
        glColor3f(1,1,1);
        draw_circle(0.06,-0.1);
    glPopMatrix();
    
    //Zavrsna pozicija(druga ziza)
    glPushMatrix();
        glTranslatef(e,0,0);
        glColor3f(0,0,0);
        draw_circle(0.06,-0.3);
    glPopMatrix();
    
    //Pocetna pozicija kugle(prva ziza)
    glPushMatrix();
    glTranslatef(-e,0,0);
    glTranslatef(x_curr-x_cen,y_curr-y_cen,0);
        glColor3f(0, 0, 1);
        draw_circle(0.06,-0.2);
    glPopMatrix();
    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}

static void draw_elipse(float a, float b) {
	glBegin(GL_POLYGON);
	int i=0;
		for(i=0; i<360; i++)
		glVertex3f(a*cos(i),
				   b*sin(i),
                   0
                  );
		glEnd();
}

static void draw_circle(float r,float a) {
	glBegin(GL_POLYGON);
	int i=0;
		for(i=0; i<360; i++)
		glVertex3f(r*cos(i),
				   r*sin(i),
                   a
                  );
		glEnd();
}









