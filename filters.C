#include "filters.h"
#include "source.h"
#include "sink.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <logging.h>
#include <exception>

/*
	Filter::Update()
	Use: Copy input image to our image, scaling to 1/2 height and 1/2 width
*/
void	Filter::Update()
{
	const char* name = FilterName();
	char msg[1024];
	if (input != NULL && input != this->GetOutput())
	{
		sprintf(msg, "%s: about to update input1", name);
		Logger::LogEvent(msg);
		input->Update();
		sprintf(msg, "%s: done updating input1", name);
		Logger::LogEvent(msg);
	}
	else
	{
		char msg[1024];
		sprintf(msg, "%s: Error no input1", SinkName());
        	DataFlowException e(SinkName(), msg);
		throw e;
			
	}
	if (input2 != NULL && input != this->GetOutput())
	{
		if(input != input2)
		{
			sprintf(msg, "%s: about to update input2",  name);
			Logger::LogEvent(msg);
			input2->Update();
			sprintf(msg, "%s: about to update input2",  name);
			Logger::LogEvent(msg);
		}
	}
	else if((name != "Shrinker") && (name != "Mirror") && (name != "Rotate") && (name != "Grayscale") && (name != "Blur"))
	{
		char msg[1024];
		sprintf(msg, "%s: Error no input2", SinkName());
        	DataFlowException e(SinkName(), msg);
		throw e;		
	}
	sprintf(msg, "%s: about to Execute", FilterName());
	Logger::LogEvent(msg);

	Execute();

	sprintf(msg, "%s: done Executing", FilterName());
	Logger::LogEvent(msg);

}


/*
	Shrinker::Execute()
	Use: Copy input image to our image, scaling to 1/2 height and 1/2 width
*/
void Shrinker::Execute()
{
	
    int width = (input->getWidth()) / 2;
    int height = (input->getHeight()) / 2;
    int maxval = input->getMaxval();

    Pixel *pixin = input->getPixel(),
          *pixout = new Pixel[height*width];

    img.setHeight(height);
    img.setWidth(width);
    img.setMaxval(maxval);
    img.setPixel(pixout);

    int ogwidth = input->getWidth();
    int ogheight = input->getHeight();

    // Copy every other column and row to the new buffer
    for (int i = 0; i < ogheight; i += 2)
    {
        for (int j = 0; j < ogwidth; j += 2)
        {
            *pixout++ = pixin[ogwidth* i + j];
        }
    }

}


/*
	LRConcat::Execute()
	Use: Concatenate 2 input images to our image, image1 on the left and image2 on the right
*/
void LRConcat::Execute()
{
    // Make sure both input images are the same height
    if (input->getHeight() != input2->getHeight())
    {   
	char msg[1024];
	sprintf(msg, "%s: heights must match: %d, %d", SinkName(), input->getHeight(), input2->getHeight());
        DataFlowException e(SinkName(), msg);
	throw e;
    }

    // Get and adjust image properties
    int width =  input->getWidth() + input2->getWidth();
    int height = input->getHeight();
    int maxval = input->getMaxval();

    Pixel *pixout = new Pixel[(height)*(width)],
          *pixl = input->getPixel(),
          *pixr = input2->getPixel();

    img.setHeight(height);
    img.setWidth(width);
    img.setMaxval(maxval);
    img.setPixel(pixout);

    // For each row, copy left image column and then append right image columns
    for (int i = 0; i < input->getHeight(); ++i)
    {
        for (int j = 0; j < input->getWidth(); ++j)
        {
            *pixout++ = *pixl++;
        }
        for (int k = 0; k< input2->getWidth(); ++k)
        {
            *pixout++ = *pixr++;
        }
    } 
}
/*
	TBConcat::Execute()
	Use: Concatenate 2 input images to our image, image1 on the top and image2 on the bottom
*/
void TBConcat::Execute()
{
    // Make sure both input images are the same width
    if (input->getWidth() != input2->getWidth())
    {   
	char msg[124];
	sprintf(msg, "%s: widths must match: %d, %d", SinkName(), input->getWidth(), input2->getWidth());
        DataFlowException e(SinkName(), msg);
	throw e;
    }

    int width =  input->getWidth();
    int height = input->getHeight() + input2->getHeight();
    int maxval = input->getMaxval();
 
    Pixel *pixout = new Pixel[(height)*(width)];

    img.setHeight(height);
    img.setWidth(width);
    img.setMaxval(maxval);
    img.setPixel(pixout);

    memcpy(pixout, input->getPixel(), sizeof(Pixel) * input->getHeight() * input->getWidth());
    memcpy(pixout + (input->getHeight() * input->getWidth()), input2->getPixel(), sizeof(Pixel) * input2->getHeight() * input2->getWidth());
}

/*
	Blender::Execute()
	Use: Blend 2 input images into our image with a ratio as per SetFactor
*/
void Blender::Execute()
{
    //make sure both images have same height and width
    if (input->getHeight() != input2->getHeight() || input->getWidth() != input2->getWidth())
    {
	char msg[1024];
	sprintf(msg, "%s: dimensions must match: (%d, %d) x (%d,%d)", SinkName(), input->getWidth(), input->getHeight(), input2->getWidth(), input2->getHeight());
        DataFlowException e(SinkName(), msg);
	throw e;
    }
    if (factor < 0.0 || factor > 1.0)
    {
	char msg[1024];
	sprintf(msg, "%s: factor must be >= 0 and <= 1: %f", SinkName(), factor);
        DataFlowException e(SinkName(), msg);
	throw e; 
    }


    int width =  input->getWidth();
    int height = input->getHeight();
    int maxval = input->getMaxval();

    Pixel *pixout = new Pixel[input->getHeight() * input->getWidth()],
          *pix1 = input->getPixel(),
          *pix2 = input2->getPixel();

    img.setHeight(height);
    img.setWidth(width);
    img.setMaxval(maxval);
    img.setPixel(pixout);

    // For all pixels in each row and column, blend the r/g/b values based on the factor property
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            pixout[width * i + j].r = (pix1[width * i + j].r * factor) + ((1 - factor) * pix2[width * i + j].r);
            pixout[width * i + j].g = (pix1[width * i + j].g * factor) + ((1 - factor) * pix2[width * i + j].g);
            pixout[width * i + j].b = (pix1[width * i + j].b * factor) + ((1 - factor) * pix2[width * i + j].b);
        }
    }
}

/*
	Blender::SetFactor(f)
	Use: Set the factor property which will be used to determine the portion of each image's pixel values to use when blending

*/
void Blender::SetFactor (float f)
{
    factor = f;
}
/*
	Miror::Execute
	Use: Reverse an image so the left-most pixels are on the right and right most are on the left, etc.
*/

void Mirror::Execute()
{
    // Get and adjust image properties
    int width =  input->getWidth();
    int height = input->getHeight();
    int maxval = input->getMaxval();

    Pixel *pixout = new Pixel[(height)*(width)],
          *pixin = input->getPixel();

    img.setHeight(height);
    img.setWidth(width);
    img.setMaxval(maxval);
    img.setPixel(pixout);

    // For each row, copy left image column and then append right image columns
    for (int i = 0; i < height; ++i)
    {
        for (int j = width-1; j>= 0; --j)
	{
	    *pixout++ = pixin[i*width + j];
	}
    } 
}



void Rotate::Execute()
{
    // Get and adjust image properties
    int width =  input->getWidth();
    int height = input->getHeight();
    int maxval = input->getMaxval();

    Pixel *pixout = new Pixel[(height)*(width)],
          *pixin = input->getPixel();

    img.setHeight(width);
    img.setWidth(height);
    img.setMaxval(maxval);
    img.setPixel(pixout);

    int count = 0;
    // For each row, copy left image column and then append right image columns
    for (int i = 0; i < width; ++i)
    {
        for (int j = height -1; j>= 0 ; --j)
	{
	    *pixout++  = pixin[j*width + i];
	}
    } 
}

void Subtract::Execute()
{
    // Get and adjust image properties
    //make sure both images have same height and width
    if (input->getHeight() != input2->getHeight() || input->getWidth() != input2->getWidth())
    {
	char msg[1024];
	sprintf(msg, "%s: dimensions must match: (%d, %d) x (%d,%d)", SinkName(), input->getWidth(), input->getHeight(), input2->getWidth(), input2->getHeight());
        DataFlowException e(SinkName(), msg);
	throw e;
    }
 
    int width =  input->getWidth();
    int height = input->getHeight();
    int maxval = input->getMaxval();

    Pixel *pixout = new Pixel[(height)*(width)],
          *pix1 = input->getPixel(),
	  *pix2 = input2->getPixel();


    img.setHeight(height);
    img.setWidth(width);
    img.setMaxval(maxval);
    img.setPixel(pixout);

    int count = 0;
    // For each row, copy left image column and then append right image columns
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
	{
	    int place = i * width + j;
	    if (pix1[place].r > pix2[place].r)
	    {
		pixout[place].r = pix1[place].r - pix2[place].r;
	    }
	    else
		pixout[place].r = 0;

	    if (pix1[place].g > pix2[place].g)
	    {
		pixout[place].g = pix1[place].g - pix2[place].g;
	    }
	    else
		pixout[place].g = 0;

	    if (pix1[place].b > pix2[place].b)
	    {
		pixout[place].b = pix1[place].b - pix2[place].b;
	    }
	    else
		pixout[place].b = 0;

	}
    } 

}

void Grayscale::Execute()
{
    int width =  input->getWidth();
    int height = input->getHeight();
    int maxval = input->getMaxval();

    Pixel *pixout = new Pixel[(height)*(width)],
          *pixin = input->getPixel();


    img.setHeight(height);
    img.setWidth(width);
    img.setMaxval(maxval);
    img.setPixel(pixout);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
	{
	    int place = i * width + j;
	    int p =  pixin[place].r/5 + pixin[place].g/2 + pixin[place].b/4;
	    pixout[place].r = p;
	    pixout[place].g = p;
	    pixout[place].b = p;
	}
    }
    
}

void Blur::Execute()
{
    int w =  input->getWidth();
    int h = input->getHeight();
    int maxval = input->getMaxval();

    Pixel *pixout = new Pixel[h*w],
          *pixin = input->getPixel();


    img.setHeight(h);
    img.setWidth(w);
    img.setMaxval(maxval);
    img.setPixel(pixout);

    for (int i = 0; i < h; ++i)
    {
	for (int j = 0; j < w; ++j)
        {
	    int p = i * w + j;
	    if (i == 0 || i == h - 1 || j == 0 || j == w -1)
	    {
		pixout[p] = pixin[p];
	    }
	    else
	    {
		pixout[p].r =  (pixin[p - w - 1].r/8 +  pixin[p - w].r/8 +  pixin[p - w + 1].r/8 +  pixin[p - 1].r/8 +  pixin[p + 1].r/8 +  pixin[p + w -1].r/8 +  pixin[p + w].r/8 +  pixin[p + w + 1].r/8);
		pixout[p].g =  (pixin[p - w - 1].g/8 +  pixin[p - w].g/8 +  pixin[p - w + 1].g/8 +  pixin[p - 1].g/8 +  pixin[p + 1].g/8 +  pixin[p + w -1].g/8 +  pixin[p + w].g/8 +  pixin[p + w + 1].g/8);
		pixout[p].b =  (pixin[p - w - 1].b/8 +  pixin[p - w].b/8 +  pixin[p - w + 1].b/8 +  pixin[p - 1].b/8 +  pixin[p + 1].b/8 +  pixin[p + w -1].b/8 +  pixin[p + w].b/8 +  pixin[p + w + 1].b/8);
	    }
	}
    }
    
    
}

Color::Color(int w, int h, int red, int green, int blue)
{
    img.setSize(h,w);
    p.r = red;
    p.g = green;
    p.b = blue;
}

void Color::Execute()
{
    Pixel* pix = img.getPixel();
    int size = img.getHeight() * img.getWidth();
    for (int i = 0; i < size; ++i)
    {
	*pix++ = p;
    }
}

void Color::Update()
{
    char msg[1024]; 
    sprintf(msg, "%s: about to Execute", SourceName());
    Logger::LogEvent(msg);

    Execute();

    sprintf(msg, "%s: done Executing", SourceName());
    Logger::LogEvent(msg);

}

void CheckSum::OutputCheckSum(char const* filename)
{
    int rval=0, gval=0, bval=0;
    int size = input->getHeight() * input->getWidth();
    Pixel* pixin = input->getPixel();

    for (int i = 0; i < size; ++i)
    {
	rval += pixin->r;
	gval += pixin->g;
	bval += pixin++->b;
    }

    FILE *f_out = fopen(filename, "w");
    fprintf(f_out, "CHECKSUM: %d, %d, %d\n", rval%256, gval%256, bval%256);
    fclose(f_out);

}
