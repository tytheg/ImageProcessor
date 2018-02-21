#include "image.h"
#include "source.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

    // Constructors
    Image::Image()
    {
        pixel = 0;
        setSize(0, 0);
    }

    Image::Image(int hit, int wid, int max)
    {
        pixel = 0;
        setSize(hit, wid, max);
    }

    Image::Image(Image &im)
    {
        pixel = 0;
        setSize(im.getHeight(), im.getWidth(), im.getMaxval());
    }

    //Functions
    void Image::setSize(int hit, int wid, int max)
    {
        width = wid;
        height = hit;
        maxval = max;

        if (width != 0 && height != 0)
        {
            setPixel(new Pixel[width*height]);
	}

    }

    int Image::getHeight() const
    {
        return height;
    }

    void Image::setHeight(int val)
    {
        height = val;
    }

    int Image::getWidth() const
    {
        return width;
    }

    void Image::setWidth(int val)
    {
        width = val;
    }

    int Image::getMaxval() const
    {
        return maxval;
    }

    void Image::setMaxval(int val)
    {
        maxval = val;
    }

    Pixel* Image::getPixel() const
    {
        return pixel;
    }

    void Image::setPixel(Pixel* pix)
    {
        if (pixel != 0)
        {
            delete [] pixel;
        }

        pixel = pix;
    }

    Source* Image::getSource() const
    {
	return src;
    }

    void Image::setSource(Source* s)
    {
	src = s;
    }

    void Image::Update() const
    {
	// Call Update for our Source class (will either be PNMReader or Filter)
	if (src)
	    src->Update();
    }

