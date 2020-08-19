#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"


/*Imena fajlova sa teksturama*/

#define GRASS "gras.bmp"

/*Identifikatori tekstura*/
static GLuint names[1];


/* Vreme proteklo od pocetka simulacije*/
static float hours; 

/*fleg koji odredjuje stanje tajmera*/
static int timer_active;
//static int timer_active2;

//Dimenzije prozora
static int window_w, window_h;

//fjA ZA inicijalizaciju u openglu
static void initialize_lights(void);
static void initialize(void);
static void initialize_material(int boja);
//Deklaracije callback funkcija
static void on_display(void);
static void on_reshape(int w,int h);
static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);




int main(int argc,char **argv){
    
    //Inicijalizuje se GLUT
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB  | GLUT_DEPTH | GLUT_DOUBLE);
    
    //Kreira se prozor
    glutInitWindowSize(1000,800);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Bunny");
    
    //Registruju se callback funkcije 
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);

    //Obavlja se OpenGL inicijalizacija
    
    glClearColor(0,0.8,1,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    
    glLineWidth(2);

    hours = 0;
    timer_active = 0;
   
    initialize();        
    //Program ulazi u glavnu petlju    
    glutMainLoop();
    
    return 0;
}

static void initialize(void)
{
    /* Objekat koji predstavlja teskturu ucitanu iz fajla. */
    Image * image;

    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    image = image_init(0, 0);

    
    /* Generisu se identifikatori tekstura. */
    glGenTextures(1, names);

    /* Kreira se  tekstura. */
    image_read(image, GRASS);

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);
 
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
       case 27:
       /* Zavrsava se program na komandu esc */
        exit(0);
        break;

        case 'b':
        case 'B':
          /*pokrece se simulacija*/ 
            if(!timer_active){
              glutTimerFunc(50,on_timer, 0);
              timer_active = 1;              
            }
            
            break;

        case 'r':
        case 'R':
        /* Restartuje se se simulacija. */
            hours = 0;
            glutPostRedisplay();           
            break;
        case 'z':
        case 'Z':
        /* Zaustavlja se simulacija. */
            timer_active = 0;                       
            break;
        /*Ukljucuje se ravno sencenje i ponovo se iscratava scena*/    
        case 'P':
        case 'p':
        glShadeModel(GL_FLAT);
        glutPostRedisplay();
        break;

    }
}

static void on_timer(int value)
{
    /* Proverava se da li callback dolazi od odgovarajuceg tajmera. */
    if (value != 0)
        return;

    /* Azurira se vreme simulacije. */
    hours +=10;

    /* Forsira se ponovno iscrtavanje prozora. */
    glutPostRedisplay();

    /* Po potrebi se ponovo postavlja tajmer. */
    if (timer_active)
        glutTimerFunc(50, on_timer, 0);
}


static void on_reshape(int w,int h)
{
    //pamti se sirina i visina prozora
    window_w = w;
    window_h = h;
    /* Podesavanje projekcije*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //prvi argument naredne fje-odredjuje ugao vidnog polja u stepenima u smeru y
    //drugi-odnos koji odredjuje vidno polje,treci i cetvrti odredjuju rastojanje izmedju view i clipping plane 
    gluPerspective(130,window_w/(float)window_h,1,6);

}


static void initialize_lights(){
 //Podesavanje boje i svetla
     /* Pozicija svetla (u pitanju je direkcionalno svetlo). */
    GLfloat light_position[] = { 1, 1, 0, 0 };

    /* Ambijentalna boja svetla. */
    GLfloat light_ambient[] = { 0.4, 0.4, 0.4, 1 };

    /* Difuzna boja svetla. */
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };

    /* Spekularna boja svetla. */
    GLfloat light_specular[] = { 0.8, 0.8, 0.9, 1 };

    /* Ukljucuje se osvjetljenje i podesavaju parametri svetla. */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

static void initialize_material(int boja){
    switch(boja){
		case 1:{
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 1, 1, 1, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 1, 1, 1, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 100;

    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, shininess);
		}
		case 2:{
    GLfloat ambient_coeffs1[] = { 1, 0.5, 0,0 };
    GLfloat diffuse_coeffs1[] = { 1, 0.5, 0, 0 };
    GLfloat specular_coeffs1[] = { 1, 0.5, 0, 0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   ambient_coeffs1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse_coeffs1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specular_coeffs1);    
		}
		case 3:{
		
    GLfloat ambient_coeffs2[] = { 0, 0.8, 0,0 };
    GLfloat diffuse_coeffs2[] = { 0, 0.8, 0, 0 };
    GLfloat specular_coeffs2[] = { 0, 0.8, 0, 0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   ambient_coeffs2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse_coeffs2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specular_coeffs2);
		}
  
	}
  
}

void draw_bunny(){
    glPushMatrix();
    glScalef(1, 1, 1);
    
   
    initialize_material(1);
    
    //crtamo telo(sfera)
    glTranslatef(-7,-1,1); 
    glColor3f(1, 1, 1);
    glutSolidSphere(1.5,25,25);
    glTranslatef(7,1,-1);

    //glava(sfera)
    
    glTranslatef(-4,1,1);
    glColor3f(1, 1, 1);
    glutSolidSphere(0.5,20,20);    
    glTranslatef(4,-1,-1);
    

	GLUquadricObj *obj = gluNewQuadric();
    GLUquadricObj *obj2 = gluNewQuadric();
	//prvo uvo
	glPushMatrix();
	glTranslatef(-8,-1,0);
	glRotatef(90,-8,-1,0);
	gluCylinder(obj,0.25,0.25,3.5,3,3);
	glPopMatrix();
    
    //drugo uvo 
    glPushMatrix();
    glTranslatef(-7,-1,0);
    glRotatef(95,-7,-1,0);
    gluCylinder(obj2,0.25,0.25,3.5,3,3);
    glPopMatrix();

    //noge
    //prva noga
    glTranslatef(-8,-3,2);
    glColor3f(1, 1, 1);
    glutSolidSphere(0.5,20,20);   
    glTranslatef(8,3,-2); 
    //DRUGA NoGA
    glTranslatef(-7,-3,1.5);
    glColor3f(1, 1, 1);
    glutSolidSphere(0.5,20,20); 
    glTranslatef(7,3,-1.5);  
    //treca noga 

    glTranslatef(-9,-3,2);
    
    glColor3f(1, 1, 1);
    glutSolidSphere(0.5,20,20);
   
    glTranslatef(9,3,-2);
    //cetvra noga se ne vidi

    glTranslatef(-9,-3,1);
    
    glColor3f(1, 1, 1);
    glutSolidSphere(0.5,20,20);
   
    glTranslatef(9,3,-1);
    //rep 
    glTranslatef(-8,0,2);
    glRotatef(360,-8,0,2); 
    glColor3f(1, 1, 1);
    glutSolidSphere(0.25,20,20);
    glRotatef(-360,8,0,-2); 
    glTranslatef(8,0,-2);    
}



void draw_carrots(){

    glPushMatrix();  
    initialize_material(2);
    
     /*sargarepa*/
    glTranslatef(1, 0, -2);
    glColor3f(1, .4, 0);
    glutSolidCone(0.25,1.75,10,10);
    glTranslatef(-1, 0, 2);

    /*nastavak sargarepe*/
      
    initialize_material(3);
    glTranslatef(1, 0, -3);
    glColor3f(0, 1, 0);
    glutSolidCone(0.15,2.75,9,9);      
    glTranslatef(1, 0, 3);
    glPopMatrix();
     

    /*druga sargarepa*/
    initialize_material(2);
    glTranslatef(1,0,0);
    glPushMatrix();  
    /*sargarepa*/
    glTranslatef(1, 0, -2);
    glColor3f(1, .4, 0);
    glutSolidCone(0.25,1.75,10,10);
    glTranslatef(-1, 0, 2);

    /*nastavak sargarepe*/

    initialize_material(3);

    glTranslatef(1, 0, -3);    
    glColor3f(0, 1, 0);
    glutSolidCone(0.15,2.75,9,9);

      
    glTranslatef(1, 0, 3);
    glPopMatrix();  
    glTranslatef(-1,0,0);
      
    //treca sargarepa
    initialize_material(2);

    glTranslatef(-1,0,1);
    glPushMatrix();  
    /*sargarepa*/
    glTranslatef(1, 0, -2);    
    glColor3f(1, .4, 0);
    glutSolidCone(0.25,1.75,10,10);
    glTranslatef(-1, 0,2);

    /*nastavak sargarepe*/
    initialize_material(3);
    glTranslatef(1, 0, -3);
    glColor3f(0, 1, 0);
    glutSolidCone(0.15,2.75,9,9);
    glTranslatef(1, 0, 3);

    glPopMatrix();  
    glTranslatef(1,0,-1);
}


static void on_display(void)
{   float h_rotation;
    //Brise se prethodni sadrzaj 'prozora'
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    
    //Podesavnje viewporta
    glViewport(0,0,window_w,window_h);

    

    //Podesavanje vidne tacke
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //1,2,3 -pozicija tacke od koje gledamo 0,0,0-polozaj referentne tacke i 0,1,0-pravac vektora
    gluLookAt(1,2,3,0,0,0,0,1,0);

    initialize_lights();
    
    h_rotation = 12*hours / 10;
  
    /*scena se ljulja i zeka skakuce gube mu se usi noge*/
    glPushMatrix();
     glTranslatef(
            sin(hours / 50.0f),
            0,
            0
        );
    glRotatef(
            /* ugao */
            sin(hours / 50.0f) * 30.0f,
            /* vektor rotacije */
            0, 0, 1
        );

    draw_bunny();
    glPopMatrix();
    /*sargarepice se rotiraju oko z ose*/
    glPushMatrix();
        glRotatef(h_rotation, 0, 0, 1);     
        draw_carrots();
    glPopMatrix();

    /* Primjenjuje se translacija koja pomera scenu*/
    glTranslatef(-0.5,-3.3,0);
    
      /* Crta se trava. */
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glBegin(GL_QUADS);
    glNormal3f(0, 12, 0);
    glTexCoord2f(0, 0);
    glVertex3f(-12, 0, -12);

    glTexCoord2f(0, 12);
    glVertex3f(12, 0, -12);

    glTexCoord2f(12, 12);
    glVertex3f(12, 0, 12);

    glTexCoord2f(12, 0);
    glVertex3f(-12, 0, 12);
    glEnd();
    
    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);
    glTranslatef(0.5,3.5,0);
 
    //Nova slika se salje na ekran 
    glutSwapBuffers();
    
}
