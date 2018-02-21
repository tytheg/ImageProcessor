#ifndef PNMreader_H
#define PNMreader_H
#include "source.h"
#include "image.h"

class PNMreader: public Source
{
protected:
	virtual void Execute();
public:
	PNMreader(char* fn);
	~PNMreader();
	virtual void Update();
	virtual const char*SourceName(){return "PNMReader";};
};
#endif
