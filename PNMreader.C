#include "PNMreader.h"
#include <logging.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

PNMreader::PNMreader (char* fn) : Source(fn)
{
}

void PNMreader::Execute()
{
	int height, width, maxval;
	char magicNum[128];
	FILE *fs = fopen(filename, "rb");

	if (fs == 0)
	{	printf("invalid file name");
		return;
	}

	fscanf(fs, "%s\n%d %d\n%d\n", magicNum, &width, &height, &maxval);
	if (strcmp(magicNum, "P6"))
	{	printf("not a valid pnm: maginum = '%s'", magicNum);
		return;
	}
	if (maxval > 255)
	{	printf("maxval must be 255 or less");
		return;
	}

	img.setSize(height, width, maxval);
	// Could error check that img.getPixel returns a buffer
	fread(img.getPixel(), sizeof(Pixel), width*height, fs);
	fclose(fs);
}


void PNMreader::Update()
{
	char msg[1024];
	sprintf(msg, "%s: about to execute", SourceName());
	Logger::LogEvent(msg);
	Execute();
	sprintf(msg,"%s: done executing", SourceName());
	Logger::LogEvent(msg);

}


PNMreader::~PNMreader()
{
	if (filename != 0)
		delete [] filename;
}
