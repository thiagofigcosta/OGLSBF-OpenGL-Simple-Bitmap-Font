#include "../scr/nTFont.hpp"

nTFont *fnt;

void drawScene(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void setup(void);

void drawScene(void){
    glClear(GL_COLOR_BUFFER_BIT);


    string text="1ª linha.\nSegunda linha.\nCarroça à Tração.";
    fnt->drawCentered_MultilineX_Text(text,nTPoint::get(100,50),nTColor::get(1,0,0));
    fnt->drawCentered_MultilineX_Y_Text(text,nTPoint::get(300,50),nTColor::get(1,0,0));
    fnt->drawCentered_X_Text(text,nTPoint::get(500,50),nTColor::get(1,0,0));
    fnt->drawCentered_X_Y_Text(text,nTPoint::get(100,200),nTColor::get(1,0,0));
    fnt->drawCentered_Y_Text(text,nTPoint::get(300,200),nTColor::get(1,0,0));
    fnt->drawText(text,nTPoint::get(500,200),nTColor::get(1,0,0));
    glPointSize(3);
    glColor3f(0,0,0);
    glBegin(GL_POINTS);
    glVertex2f(100,50);
    glVertex2f(300,50);
    glVertex2f(500,50);
    glVertex2f(100,200);
    glVertex2f(300,200);
    glVertex2f(500,200);
    glEnd();
    fnt->drawCentered_X_Text("github.com/thiagofigcosta",nTPoint::get(400,300),nTColor::get(0,0,0));
    glFlush();
}


int main(int argc, char **argv){
    glutInit(&argc, argv);
    setup();
    fnt=nTFont::loadFont("../fnt/BITMAP_TIMES_ROMAN_24.fnt");
    glutMainLoop();
    return 0;
}

void reshape(int w, int h){
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, 800, 600, 0, -1, 1);
   glViewport(0, 0, w, h);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y){
   switch(key){
      case 27://ESC
         exit(0);
         break;
   }
}

void setup(void){
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OGLSBF-OpenGL Simple Bitmap Font");
    glClearColor(1, 1, 1, 1);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
}
