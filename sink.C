#include "sink.h"
#include <string.h>

Sink::Sink ()
{
    input = 0;
    input2 = 0;
}

Sink::~Sink()
{
}

void Sink::SetInput(Image *img)
{
    input = img;
}

void Sink::SetInput2(Image *img)
{
    input2 = img;
}
