#include <GL/glut.h>


//Deklaracije callback funkcija

static void on_display(void);


int main(int argc,char **argv){
    
    //Inicijalizuje se GLUT
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB  | GLUT_DOUBLE);
    
    
    //Kreira se prozor
    glutInitWindowSize(1000,800);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);
    
    //Registruju se callback funkcije 
    glutDisplayFunc(on_display);
    
    //Obavlja se OpenGL inicijalizacija
    
    glClearColor(0.8,0.8,0.8,0);
    
    //Program ulazi u glavnu petlju
    
    glutMainLoop();
    
    return 0;
}

static void on_display(void)
{
    //Brise se prethodni sadrzaj 'prozora'
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    
    //Nova slika se salje na ekran 
    glutSwapBuffers();
    
}
