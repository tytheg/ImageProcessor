#include "source.h"
#include <string.h>
#include <stdio.h>
#include <logging.h>
	
Source::Source()
{
	img.setSource(this);
}

Source::Source(char* fn)
{
	if (fn == 0)
	{	printf("No input specified");
		return;
	}
	filename = new char[strlen(fn) + 1];
	strcpy(filename, fn);
	img.setSource(this);
}

Source::~Source()
{
}

Image* Source::GetOutput()
{
	return &img;
}

void Source::Update()
{
    char msg[128];
    sprintf(msg, "%s: about to execute", SourceName());
    Logger::LogEvent(msg);
    Execute();
    sprintf(msg, "%s: done executing", SourceName());
    Logger::LogEvent(msg);
}
