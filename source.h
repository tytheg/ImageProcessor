#ifndef SOURCE_H
#define	SOURCE_H
#include "image.h"	

class Source
{
protected:
	Image img;
	char* filename;
	virtual void Execute(void) = 0;

public:
	Source();			// Default constructor
	Source(char* fn);	// Constructor - Save copy of filename
	~Source();			// Destructor - Release any allocated memory
	Image* GetOutput();;

	virtual const char *SourceName()=0;

	virtual void Update() = 0;
};
#endif
