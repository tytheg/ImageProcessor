#ifndef PNMWRITER_H
#define PNMWRITER_H
#include "sink.h"

class PNMwriter: public Sink
{
public:
    PNMwriter();
    void Write(char* filename);

    const char *SinkName(){return "PNMwriter";};
};
#endif
