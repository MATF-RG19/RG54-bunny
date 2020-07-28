#include <GL/glut.h>


//Dimenzije prozora
static int window_w, window_h;



//Deklaracije callback funkcija
static void on_display(void);
static void on_reshape(int w,int h);

int main(int argc,char **argv){
    
    //Inicijalizuje se GLUT
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB  | GLUT_DOUBLE);
    
    
    //Kreira se prozor
    glutInitWindowSize(1000,800);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Bunny");
    
    //Registruju se callback funkcije 
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    
    //Obavlja se OpenGL inicijalizacija
    
    glClearColor(0.8,0.8,0.8,0);
    
    //Program ulazi u glavnu petlju
    
    glutMainLoop();
    
    return 0;
}






static void on_reshape(int w,int h)
{
    //pamti se sirina i visina prozora
    window_w = w;
    window_h = h;

}



static void on_display(void)
{
    //Brise se prethodni sadrzaj 'prozora'
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    
    //Podesavnje viewporta
    glViewport(0,0,window_w,window_h);

    /* Podesavanje projekcije*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //prvi argument naredne fje-odredjuje ugao vidnog polja u stepenima u smeru y
    //drugi-odnos koji odredjuje vidno polje,treci i cetvrti odredjuju rastojanje izmedju view i clipping plane 
    gluPerspective(120,window_w/(float)window_h,1,6);

    //Podesavanje vidne tacke
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //1,2,3 -pozicija tacke od koje gledamo 0,0,0-polozaj referentne tacke i 0,1,0-pravac vektora
    gluLookAt(1,2,3,0,0,0,0,1,0);



    //Nova slika se salje na ekran 
    glutSwapBuffers();
    
}
