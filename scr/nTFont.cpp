#include "nTFont.hpp"
#include "nTFontChar.hpp"

nTFont::nTFont(bool invertx,bool inverty) {
    bitmapTexture=0;
    imgSizex=0;
    imgSizey=0;
    name="";
    size=0;
    lineHeight=0;
    invertX=invertx;
    invertY=inverty;
    chars.reserve(256);
    for(int i=0;i<256;i++)
        chars.push_back(nullptr);
};

nTFont::nTFont() {
    bitmapTexture=0;
    imgSizex=0;
    imgSizey=0;
    name="";
    size=0;
    lineHeight=0;
    invertX=false;
    invertY=true;
    chars.reserve(256);
    for(int i=0;i<256;i++)
        chars.push_back(nullptr);
};

nTFont::nTFont(const nTFont& orig) {
}

nTFont::~nTFont() {
}

/**
 *  Get an integer from a font body string
 *
 *  @param str the font body string
 *  @param start start position to get the integer
 *  @return the founded integer
**/
int nTFont::getIntFromField(string str, int start){
    string number="";
    for(int i=start+1;i<str.size();i++){
        if((str[i]>='0'&&str[i]<='9')||str[i]=='-')
            number+=str[i];
        else if(str[i]==' '||str[i]=='\n'||str[i]=='\t')
            break;
    }
    int out=0;
    istringstream(number)>>out;
    return out;
}

/**
 *  Get an string from a font body string
 *
 *  @param str the font body string
 *  @param start start position to get the string
 *  @return the founded string
**/
string nTFont::getStrFromField(string str, int start){
    int startstr=-1;
    for(int j=start;j<str.size();j++){
        if(str[j]=='"'){
            if(startstr<0){
                startstr=j;
            }else{
                return str.substr(startstr+1,j-startstr-1);
            }
        }
    }
    return "";
}

/**
 *  Load a bitmap font file
 *
 *  @param path path to the bitmap font
 *  @return the address of the font object loaded
**/
nTFont* nTFont::loadFont(string path){
    string texturepath;
    nTFont *font=new nTFont();
    ifstream fontFILE(path);
    int nChars=0,lastSpace=0;
    if(fontFILE.is_open()){
        string header;
        for(int headersize=0;headersize<4;headersize++){
            getline(fontFILE,header);
            lastSpace=0;
            for(int i=0;i<header.size();i++){
                if(header[i]==' ')lastSpace=i;
                else if(header[i]=='='){
                    string field=header.substr(lastSpace+1,i-lastSpace-1);
                    if(field=="face"){
                        font->name=getStrFromField(header,i);
                    }else if(field=="size"){
                        font->size=getIntFromField(header,i);
                    }else if(field=="lineHeight"){
                        font->lineHeight=getIntFromField(header,i);
                    }else if(field=="scaleW"){
                        font->imgSizex=getIntFromField(header,i);
                    }else if(field=="scaleH"){
                        font->imgSizey=getIntFromField(header,i);
                    }else if(field=="file"){
                        texturepath=getStrFromField(header,i);
                        for(int s=path.size()-1;s>=0;s--)
                            if(path[s]=='\\'||path[s]=='/'){
                                texturepath=path.substr(0,s+1)+texturepath;
                                break;
                            }
                    }else if(field=="count"){
                        nChars=getIntFromField(header,i);
                    }
                }
            }
        }
        string body;
        nTFontChar *readChar;
        for(int c=0;c<nChars;c++)
        if(getline(fontFILE,body)){
            lastSpace=0;
            bool haveID=false;
            int charx=-1,chary=-1,charw=-1,charh=-1;
            for(int i=0;i<body.size();i++){
                if(body[i]==' '||body[i]=='\t')lastSpace=i;
                else if(body[i]=='='){
                    string field=body.substr(lastSpace+1,i-lastSpace-1);
                    if(field=="id"&&!haveID){
                        haveID=true;
                        readChar=new nTFontChar();
                        readChar->Character=(char)getIntFromField(body,i);
                    }else if(field=="x"&&haveID){
                        charx=getIntFromField(body,i);
                    }else if(field=="y"&&haveID){
                        chary=getIntFromField(body,i);
                    }else if(field=="width"&&haveID){
                        charw=getIntFromField(body,i);
                    }else if(field=="height"&&haveID){
                        charh=getIntFromField(body,i);
                    }else if(field=="xoffset"&&haveID){
                        readChar->offsetX=getIntFromField(body,i);
                    }else if(field=="yoffset"&&haveID){
                        readChar->offsetY=getIntFromField(body,i);
                    }else if(field=="xadvance"&&haveID){
                        readChar->advance=getIntFromField(body,i);
                    }
                }
            }
            if(haveID){
                if(charx>=0&&chary>=0&&charw>0&&charh>0)
                    readChar->setTextureCoords(charx,chary,charw,charh,font->imgSizex,font->imgSizey);
                int charValue=(int)readChar->Character;
                if(charValue<=255&&charValue>=0)
                    font->chars[charValue]=readChar;
                else
                    font->chars.push_back(readChar);
            }
        }
        fontFILE.close();
        font->bitmapTexture=SOIL_load_OGL_texture(texturepath.c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
        if(font->bitmapTexture==0){
            cout<<"falha ao carregar fonte bitmap "<<texturepath<<endl;
        }
        return font;
    }else{
        cout<<"Erro ao carregar o arquivo "<<path<<endl;
        return nullptr;
  }
}

/**
 *  Draw a text on the screen using this font
 *
 *  @param text the text to be written
 *  @param pos the start position of the text
 *  @param color the font color of the text
**/
void nTFont::drawText(string text, nTPoint pos, nTColor color){
    float xMargin=pos.x;
    nTFontChar* cchar;
    glColor4f(color.R,color.G,color.B,color.A);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bitmapTexture);
    glBegin(GL_QUADS);
    if(!invertY)
        pos.y+=2*lineHeight;
    for(int i=0;i<text.size();i++){
        if(text[i]=='\n'){
            pos.x=xMargin;
            if(invertY){
                pos.y+=lineHeight;
            }else{
                pos.y-=lineHeight;
            }
        }else{
            cchar=nullptr;
            int charValue=(int)text[i];
            if(charValue>255||charValue<0){
                for(int j=256;j<chars.size();j++){
                    cchar=(nTFontChar*)chars[j];
                    if(cchar->Character==text[i])
                        break;
                    else
                        cchar=nullptr;
                }
            }else
                cchar=(nTFontChar*)chars[charValue];
                if(cchar!=nullptr){
                 if(cchar->width>0&&cchar->height>0&&cchar->width<=6666&&cchar->height<=6666){
                    float p0x=pos.x+cchar->offsetX;
                    float p0y=pos.y+cchar->offsetY-lineHeight;
                    float p1x=p0x+cchar->width;
                    float p1y=p0y+cchar->height;
                    if(invertY){
                        if(invertX){
                            p0x=pos.x-cchar->offsetX;
                            p1x=p0x-cchar->width;
                            glTexCoord2f(cchar->textureCoords[0], cchar->textureCoords[2]); glVertex3f(p0x, p0y, pos.z);
                            glTexCoord2f(cchar->textureCoords[1], cchar->textureCoords[2]); glVertex3f(p1x, p0y, pos.z);
                            glTexCoord2f(cchar->textureCoords[1], cchar->textureCoords[3]); glVertex3f(p1x, p1y, pos.z);
                            glTexCoord2f(cchar->textureCoords[0], cchar->textureCoords[3]); glVertex3f(p0x, p1y, pos.z);
                        }else{
                            glTexCoord2f(cchar->textureCoords[1], cchar->textureCoords[2]); glVertex3f(p1x, p0y, pos.z);
                            glTexCoord2f(cchar->textureCoords[0], cchar->textureCoords[2]); glVertex3f(p0x, p0y, pos.z);
                            glTexCoord2f(cchar->textureCoords[0], cchar->textureCoords[3]); glVertex3f(p0x, p1y, pos.z);
                            glTexCoord2f(cchar->textureCoords[1], cchar->textureCoords[3]); glVertex3f(p1x, p1y, pos.z);
                        }
                    }else{
                        p0y=pos.y-cchar->offsetY-lineHeight;
                        p1y=p0y-cchar->height;
                        if(invertX){
                            p0x=pos.x-cchar->offsetX;
                            p1x=p0x-cchar->width;
                            glTexCoord2f(cchar->textureCoords[0], cchar->textureCoords[3]); glVertex3f(p0x, p1y, pos.z);
                            glTexCoord2f(cchar->textureCoords[1], cchar->textureCoords[3]); glVertex3f(p1x, p1y, pos.z);
                            glTexCoord2f(cchar->textureCoords[1], cchar->textureCoords[2]); glVertex3f(p1x, p0y, pos.z);
                            glTexCoord2f(cchar->textureCoords[0], cchar->textureCoords[2]); glVertex3f(p0x, p0y, pos.z);
                        }else{
                            glTexCoord2f(cchar->textureCoords[1], cchar->textureCoords[2]); glVertex3f(p1x, p0y, pos.z);
                            glTexCoord2f(cchar->textureCoords[0], cchar->textureCoords[2]); glVertex3f(p0x, p0y, pos.z);
                            glTexCoord2f(cchar->textureCoords[0], cchar->textureCoords[3]); glVertex3f(p0x, p1y, pos.z);
                            glTexCoord2f(cchar->textureCoords[1], cchar->textureCoords[3]); glVertex3f(p1x, p1y, pos.z);
                        }
                    }
                    
                }else if(cchar->Character!=' '){
                    cout<<"Cant print "<<*cchar<<"with "<<*this;
                }
                if(invertX)
                    pos.x-=cchar->advance;
                else 
                    pos.x+=cchar->advance;
            }else{
               cout<<"Unknown character \'"<<text[i]<<"\' with "<<*this;
            }
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

/**
 *  Calculates the size of the text using this font
 *
 *  @param text the text to be calculated
 *  @return the size of the text
**/
nTPoint nTFont::calcBoundaries(string text){
    nTPoint pos=nTPoint::Origin();
    float maXX=0;
    if(invertX)
        maXX=65000;
    nTFontChar* cchar;
    for(int i=0;i<text.size();i++){
        if(text[i]=='\n'){
            if((invertX&&pos.x<maXX)||(!invertX&&pos.x>maXX)){
                maXX=pos.x;
                pos.x=0;
            }
            if(invertY){
                pos.y+=lineHeight;
            }else{
                pos.y-=lineHeight;
            }
        }else{
            cchar=nullptr;
            int charValue=(int)text[i];
            if(charValue>255||charValue<0){
                for(int j=256;j<chars.size();j++){
                    cchar=(nTFontChar*)chars[j];
                    if(cchar->Character==text[i])
                        break;
                    else
                        cchar=nullptr;
                }
            }else
                cchar=(nTFontChar*)chars[charValue];
                if(cchar!=nullptr){
                    if(invertX)
                        pos.x-=cchar->advance;
                    else
                        pos.x+=cchar->advance;
            }else{
                cout<<"Unknown character \'"<<text[i]<<"\' with "<<*this;
            }
        }
    }
    if(invertX){
        if(pos.x<maXX)
            maXX=pos.x;
    }else{
        if(pos.x>maXX)
            maXX=pos.x;
    }
   
    if(invertY){
        pos.y-=lineHeight;
    }else{
        pos.y+=lineHeight;
    }
    pos.x=maXX;
    return pos;
}

/**
 *  Draw a text centering its x position on the screen using this font
 *
 *  @param text the text to be written
 *  @param pos the start position of the text
 *  @param color the font color of the text
**/
void nTFont::drawCentered_X_Text(string text, nTPoint pos, nTColor color){
    nTPoint size=calcBoundaries(text);
    pos.x-=size.x/2;
    drawText(text,pos,color);
}

/**
 *  Draw a text centering its y position on the screen using this font
 *
 *  @param text the text to be written
 *  @param pos the start position of the text
 *  @param color the font color of the text
**/
void nTFont::drawCentered_Y_Text(string text, nTPoint pos, nTColor color){
    nTPoint size=calcBoundaries(text);
    pos.y-=size.y/2;
    drawText(text,pos,color);
}

/**
 *  Draw a text centering its x and y position on the screen using this font
 *
 *  @param text the text to be written
 *  @param pos the start position of the text
 *  @param color the font color of the text
**/
void nTFont::drawCentered_X_Y_Text(string text, nTPoint pos, nTColor color){
    nTPoint size=calcBoundaries(text);
    pos.x-=size.x/2;
    pos.y-=size.y/2;
    drawText(text,pos,color);
}

/**
 *  Draw a text centering its x position for each line on the screen using this font
 *
 *  @param text the text to be written
 *  @param pos the start position of the text
 *  @param color the font color of the text
**/
void nTFont::drawCentered_MultilineX_Text(string text, nTPoint pos, nTColor color){
    int lastSlashN=0;
    for(int i=0;i<=text.size();i++){
        if(i==text.size()||text[i]=='\n'){
            drawCentered_X_Text(text.substr(lastSlashN,i-lastSlashN),pos,color);
            if(lastSlashN){
                if(invertY){
                    pos.y+=lineHeight;
                }else{
                    pos.y-=lineHeight;
                }
            }
            lastSlashN=i;
        }
    }
}

/**
 *  Draw a text centering its x position for each line and its y position on the screen using this font
 *
 *  @param text the text to be written
 *  @param pos the start position of the text
 *  @param color the font color of the text
**/
void nTFont::drawCentered_MultilineX_Y_Text(string text, nTPoint pos, nTColor color){
    nTPoint size=calcBoundaries(text);
    pos.y-=size.y/2;
    drawCentered_MultilineX_Text(text,pos,color);
}

/**
 *  Modify the operator << to print this type of objects
 *  The parameters are passed automatically
 *
 *  @param strm current string stream
 *  @param font object address
 *  @return the old stream plus the object toString
**/
ostream& operator<<(ostream &strm, const nTFont &font){
    nTFont *fnt=(nTFont*)&font;
        return strm <<"nTFont:["<<"Name:"<<fnt->name<<", "<<"Size:"<<fnt->size<<", "<<"LineHeight:"<<fnt->lineHeight<<", "<<"TexId:"<<fnt->bitmapTexture<<", TexSize(x:"<<fnt->imgSizex<<" y:"<<fnt->imgSizey<<")]\n";
    return strm;
}
