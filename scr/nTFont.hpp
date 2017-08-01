#ifndef NTCOLOR
#define NTCOLOR
typedef struct color{
        float R,G,B,A;
        void set(float R_,float G_, float B_, float A_){
            R=R_;G=G_;B=B_;A=A_;
        };
        void set(float R_,float G_, float B_){
            R=R_;G=G_;B=B_;A=1;
        };
        static struct color White(){
            struct color colo;
            colo.set(1,1,1);
            return colo;
        };
        static struct color Black(){
            struct color colo;
            colo.set(0,0,0);
            return colo;
        };
        static struct color get(float R_,float G_, float B_, float A_){
            struct color clor;
            clor.set(R_,G_,B_,A_);
            return clor;
        };
        static struct color get(float R_,float G_, float B_){
            struct color clor;
            clor.set(R_,G_,B_,1);
            return clor;
        };
}nTColor;
#endif // NTCOLOR
#ifndef NTPOINT
#define NTPOINT
typedef struct point{
        float x,y,z;
        void set(float x_,float y_,float z_){
            x=x_;y=y_;z=z_;
        };
        void set(float x_,float y_){
            x=x_;y=y_;z=0;
        };
        static struct point Origin(){
            struct point ori;
            ori.set(0,0,0);
            return ori;
        };
        static struct point get(float x_,float y_,float z_){
            struct point pnt;
            pnt.set(x_,y_,z_);
            return pnt;
        };
        static struct point get(float x_,float y_){
            struct point pnt;
            pnt.set(x_,y_);
            return pnt;
        };
}nTPoint;
#endif // NTPOINT

#ifndef NTFONT_H
#define NTFONT_H
#ifdef __EMSCRIPTEN__
    #include <GL/glut.h>
#else
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/freeglut.h>
#endif
#endif
#include "soil/SOIL.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class nTFont {
public:
    nTFont();
    nTFont(bool invertx,bool inverty);
    nTFont(const nTFont& orig);
    virtual ~nTFont();

    static nTFont* loadFont(string path);
    void drawText(string text, nTPoint pos, nTColor color);
    void drawCentered_X_Text(string text, nTPoint pos, nTColor color);
    void drawCentered_Y_Text(string text, nTPoint pos, nTColor color);
    void drawCentered_X_Y_Text(string text, nTPoint pos, nTColor color);
    void drawCentered_MultilineX_Text(string text, nTPoint pos, nTColor color);
    void drawCentered_MultilineX_Y_Text(string text, nTPoint pos, nTColor color);
    nTPoint calcBoundaries(string text);

    bool invertX;
    bool invertY;
    string name;
    int size;
private:
    static int getIntFromField(string str, int start);
    static string getStrFromField(string str, int start);

    int lineHeight;
    GLuint bitmapTexture;
    int imgSizex,imgSizey;
    vector<void*> chars;

    friend ostream& operator<<(ostream &strm, const nTFont &font);
};

#endif /* NTFONT_H */
