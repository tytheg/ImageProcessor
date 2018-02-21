#ifndef FILTERS_H
#define FILTERS_H
#include "source.h"
#include "sink.h"

class Filter: public Sink, public Source
{
protected:
    virtual void Execute() = 0;
public:
    virtual void Update();
    virtual const char *FilterName()=0;
    virtual const char *SourceName(){return FilterName();};
    virtual const char *SinkName(){return FilterName();};

};

class Shrinker: public Filter
{
protected:
    void Execute();
public:
    virtual const char *FilterName(){return "Shrinker";};
};

class LRConcat: public Filter
{
protected:
    void Execute();
public:
    //void Update();
    virtual const char *FilterName(){return "LRConcat";};
};

class TBConcat: public Filter
{
protected:
    void Execute();
public:
    //void Update();
    virtual const char *FilterName(){return "TBConcat";};
};

class Blender: public Filter
{
protected:
    //Assignment said blender would take an argument in the constructor, but main didn't call it
    float factor;
    void Execute();
public:
    void SetFactor(float f);
    virtual const char *FilterName(){return "Blender";};
    //void Update();
};

class Mirror: public Filter
{
protected:
    void Execute();
public:
    //void Update();
    virtual const char *FilterName(){return "Mirror";};
};

class Rotate: public Filter
{
protected:
    void Execute();
public:
    //void Update();
    virtual const char *FilterName(){return "Rotate";};
};

class Subtract: public Filter
{
protected:
    void Execute();
public:
    //void Update();
    virtual const char *FilterName(){return "Subtract";};
};

class Grayscale: public Filter
{
protected:
    void Execute();
public:
    //void Update();
    virtual const char *FilterName(){return "Grayscale";};
};

class Blur: public Filter
{
protected:
    void Execute();
public:
    //void Update();
    virtual const char *FilterName(){return "Blur";};
};

class Color: public Source
{
private:
    Pixel p;
public:
    	 Color(int w, int h, int red, int green, int blue);
    void Execute();
    void Update();
    virtual const char *SourceName(){return "Color";};
};

class CheckSum: public Sink
{
public:
    void OutputCheckSum(char const* filename);
    virtual const char *SinkName(){return "CheckSum";};
};

#endif

