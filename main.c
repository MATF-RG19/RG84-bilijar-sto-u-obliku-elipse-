#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TIMER_ID 0
#define TIMER_INTERVAL 20

#define PI 3.1415926535898


/* Dimenzije prozora */
static int window_width, window_height;
static float v_x, v_z, v_x_new, v_z_new;          /* Komponente vektora brzine kretanja
                                 * kugle. */
static float e = sqrt(1*1 - 0.7*0.7);
static float x_curr, y_curr, z_curr, x_start, z_start,
             x_end, z_end, x_coll, z_coll;    /* Tekuce koordinate centra kugle,pocetna, zavrsna pozicija i koordinate kolizije */
static int animation_ongoing;   /* Fleg koji odredjuje da li je
                                 * animacija u toku. */
/* Deklaracije callback funkcija. */
static int kamera = 1;
static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);
static void on_display(void);
static void on_reshape(int width, int height);
static void draw_elipse(float a, float b);
static void draw_circle(float r, float a);
static void draw_cylinder(GLfloat a, GLfloat b, GLfloat height, GLubyte R, GLubyte G, GLubyte B);
static void getParameters(); //funkcija za opciju unosenje vektora brzine
static void restart(); //funkcija za restartovanje aplikacije



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
    x_curr = -e;
    y_curr = 0;
    z_curr = 0; 
   
    x_start = -e;
    z_start = 0;
   
    x_end = e;
    z_end = 0;

    v_x = 0.01;
    v_z = -0.01;
    
    v_x_new = 0;
    v_z_new = 0;

    /* Na pocetku je animacija neaktivna */
    animation_ongoing = 0;

    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(1, 1, 1, 0);
    glEnable(GL_DEPTH_TEST);
    
    glClearColor(0.9, 0.9, 0.9, 0);

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
         restart();
         break;
        
    case '1':
        kamera = 1;
        glutPostRedisplay();
        break;
    
    case '2':
        kamera = 2;
        glutPostRedisplay();
        break;
        
    case 'e':
    case 'E':
        getParameters();
        break;
        
    }
}

static void on_reshape(int width, int height)
{
    if (width == 0 || height == 0) return;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40, (GLdouble)width/(GLdouble)height,
                  0.5, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);
    
}

static void on_timer(int value)
{
    /*
     * Proverava se da li callback dolazi od odgovarajuceg tajmera.
     */
    if (value != TIMER_ID)
        return;
    
    
    /* proverava se da li je kugla dosla do rupe */
    if(y_curr < -1)
        animation_ongoing = 0;
    
    else if(x_coll < e && z_coll > 0 && x_curr+0.01 > e && z_curr-0.01 < 0) {
        y_curr -= 0.005;
    }
    else if(x_coll < e && z_coll < 0 && x_curr+0.01 > e && z_curr+0.01 > 0){
        y_curr -= 0.005;
    }
    else if(x_coll > e && z_coll > 0 && x_curr-0.02 < e && z_curr-0.02 < 0){
        y_curr -= 0.005;
    }
    else if(x_coll > e && z_coll < 0 && x_curr-0.02 < e && z_curr+0.02 > 0){
        y_curr -= 0.005;
    }
    else if(x_coll < -e && z_coll == 0 && x_curr+0.02 > e){
        y_curr -= 0.005;
    }
    
    else{
    
    /* Azuriraju se koordinate centra kruga. */
    z_curr += v_z;
    double z_compare = sqrt(0.49 - 0.49*x_curr*x_curr);
    if (z_curr+0.05 >= z_compare || z_curr-0.05 <= -z_compare) {
        x_coll = x_curr;
        z_coll = z_curr;
        v_x = (x_end - x_curr)/70 ;
        v_z = (z_end - z_curr)/70 ;  
        }

    x_curr += v_x;
    double x_compare = sqrt((0.49 - z_curr*z_curr) / 0.49);
    if ( x_curr+0.05 >= x_compare || x_curr-0.05 <= -x_compare) {
        x_coll = x_curr;
        z_coll = z_curr;
        v_x = (x_end - x_curr)/70 ;
        v_z = (z_end - z_curr)/70 ;  
        } 
        
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
    
    glLoadIdentity();
    
    
    if(kamera == 1)
        gluLookAt(0, 3, 2, 0, 0.5, 0, 0, 1, 0);
    else
        gluLookAt(0, 5, 0.1, 0, 0.5, 0, 0, 1, 0);
    
   
   
    draw_cylinder(1, 0.7, 1, 0, 160, 0);
        
      
    /*Pocetna pozicija(prva ziza)*/
    glPushMatrix();
        glTranslatef(-e,1.001,0);
        glColor3f(1,1,1);
        draw_circle(0.01,0);
    glPopMatrix();
    
    /*Zavrsna pozicija(druga ziza)*/
    glPushMatrix();
        glTranslatef(e,1.001,0);
        glColor3f(0,0,0);
        draw_circle(0.06,0);
    glPopMatrix();
    
    /*Pocetna pozicija kugle(prva ziza)*/
    glPushMatrix();
        glTranslatef(-e,1.05,0);
        glTranslatef(x_curr-x_start,y_curr,z_curr-z_start);
        glColor3f(1, 1, 1);
        glutSolidSphere(0.05, 50, 50);
    glPopMatrix(); 
    
    
    /* Nova slika se salje na ekran. */
    
    glutSwapBuffers(); 
}

static void draw_elipse(float a, float b) {
	glBegin(GL_POLYGON);
	int i=0;
		for(i=0; i<360; i++)
		glVertex3f(a*cos(i),
                   0,
                   b*sin(i)
                  );
		glEnd();
}

static void draw_circle(float r,float a) {
	glBegin(GL_POLYGON);
	int i=0;
		for(i=0; i<360; i++)
		glVertex3f(r*cos(i),
                   a,
                   r*sin(i)
                  );
		glEnd();
}

static void draw_cylinder(GLfloat a,
                   GLfloat b,
                   GLfloat height,
                   GLubyte R,
                   GLubyte G,
                   GLubyte B)
{
    GLfloat x              = 0.0;
    GLfloat z              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    /** Crta se valjak */
    glColor3ub(60,0,0);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = a * cos(angle);
            z = b * sin(angle);
            glVertex3f(x, height , z);
            glVertex3f(x, 0, z);
            angle = angle + angle_stepsize;
        }
        glVertex3f(a, height, 0);
        glVertex3f(b, 0.0, 0.0);
    glEnd();

    /** Crta se elipsa na vrhu valjka */
    glColor3ub(R,G,B);
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = a * cos(angle);
            z = b * sin(angle);
            glVertex3f(x, height , z);
            angle = angle + angle_stepsize;
        }
        glVertex3f(a, height, 0);
    glEnd();    
    
}

static void getParameters() {
    
    animation_ongoing = 0;
    
    scanf("%f%f", &v_x_new, &v_z_new);
    
    v_x = v_x_new;
    v_z = v_z_new;
    
    x_curr = x_start;
    y_curr = 0;
    z_curr = z_start;
    
    glutPostRedisplay();
}

static void restart() {
    
    animation_ongoing = 0;
    
    if(v_x_new != 0 || v_z_new != 0) {
        v_x = v_x_new;
        v_z = v_z_new;
    }
    else {
        v_x = 0.01;
        v_z = -0.01;
    }
    
    x_curr = x_start;
    y_curr = 0;
    z_curr = z_start;
    
    glutPostRedisplay();
}










