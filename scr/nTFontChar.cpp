#include "nTFontChar.hpp"

nTFontChar::nTFontChar() {
    Character=0;
    for(int i=0;i<4;i++)
        textureCoords[i]=0;
    offsetX=0;
    offsetY=0;
    advance=0;
};

nTFontChar::nTFontChar(const nTFontChar& orig) {
}

nTFontChar::~nTFontChar() {
}

/**
 *	Calculate and define the texture coordinates and define the size of the character
 *
 *	@param x the start x position of the character
 *	@param y the start y position of the character
 *	@param width_ the width of the character
 *	@param height_ the height of the character
 *	@param imgSizeX the total bitmapfont size x
 *	@param imgSizeY the total bitmapfont size y
**/
void nTFontChar::setTextureCoords(int x, int y, int width_, int height_, int imgSizeX, int imgSizeY){
    width=width_;
    height=height_;
    float xStart=(float)x/(float)imgSizeX;
    float yStart=(float)(imgSizeY-y)/(float)imgSizeY;
    float xEnd=((float)(x+width))/(float)imgSizeX;
    float yEnd=((float)(imgSizeY-(y+height)))/(float)imgSizeY;
    textureCoords[0]=xStart;
    textureCoords[1]=xEnd;
    textureCoords[2]=yStart;
    textureCoords[3]=yEnd;
}

/**
 *	Copy the values from a font character to an other
 *
 *	@param origin the font character source of the values
**/
void nTFontChar::copyFrom(nTFontChar *origin){
    Character=origin->Character;
    for(int i=0;i<4;i++)
        textureCoords[i]=origin->textureCoords[i];
    offsetX=origin->offsetX;
    offsetY=origin->offsetY;
    advance=origin->advance;
}

/**
 *	Modify the operator << to print this type of objects
 *	The parameters are passed automatically
 *
 *	@param strm current string stream
 *	@param fontc object address
 *	@return the old stream plus the object toString
**/
ostream& operator<<(ostream &strm, const nTFontChar &fontc){
    nTFontChar *fntc=(nTFontChar*)&fontc;
        return strm <<"nTFontChar:["<<"Char:"<<fntc->Character<<"("<<(int)(fntc->Character)<<"), "<<"Advance:"<<fntc->advance<<", Size(x:"<<fntc->width<<" y:"<<fntc->height<<"), "<<"Offset(x:"<<fntc->offsetX<<" y:"<<fntc->offsetY<<"), "<<"TexCoords(x0:"<<fntc->textureCoords[0]<<" x1:"<<fntc->textureCoords[1]<<" y0:"<<fntc->textureCoords[2]<<" y1:"<<fntc->textureCoords[3]<<")]\n";
    return strm;
}
