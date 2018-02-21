#ifndef SINK_H
#define	SINK_H
#include "image.h"

class Sink
{
protected:
	const Image* input;
	const Image* input2;
public:
	// Constructor - Save copy of filename
	Sink();

	// Destructor - Release any allocated memory
	~Sink();

	void SetInput(Image *img);
	void SetInput2(Image *img);

	virtual const char*SinkName()=0;
};
#endif
