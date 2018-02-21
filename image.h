#ifndef IMAGE_H
#define IMAGE_H
//#include "Source.h"

class Source;

struct Pixel
{
    unsigned char  r, g, b;
};

class Image
{
    // Data members go here
private:
    Pixel* pixel;
    int height, width, maxval;

protected:
    Source* src;

public:
    Image();
    Image(int wid, int hit, int max);
    Image(Image &);

    void setSize(int hit, int wid, int max = 255);
    int getHeight() const;
    void setHeight(int val);
    int getWidth() const;
    void setWidth(int val);
    int getMaxval() const;
    void setMaxval(int val);
    Pixel* getPixel() const;
    void setPixel(Pixel* pix);
    Source *getSource() const;
    void setSource(Source *s);
    virtual void Update() const;
};
#endif
