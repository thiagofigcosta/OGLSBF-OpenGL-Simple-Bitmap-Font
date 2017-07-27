#ifndef NTFONTCHAR_H
#define NTFONTCHAR_H
#include "nTFont.hpp"

using namespace std;

class nTFontChar {
public:
    nTFontChar();
    nTFontChar(const nTFontChar& orig);
    virtual ~nTFontChar();

    void copyFrom(nTFontChar *origin);

    char Character;
private:
    friend class nTFont;

    void setTextureCoords(int x, int y, int width_, int height_, int imgSizeX, int imgSizeY);

    float textureCoords[4];
    int offsetX,offsetY;
    int advance;
    int width, height;
    friend ostream& operator<<(ostream &strm, const nTFontChar &fontc);
};

#endif /* NTFONTCHAR_H */
