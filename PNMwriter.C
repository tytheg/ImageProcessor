#include "PNMwriter.h"
#include <string.h>
#include <stdio.h>

PNMwriter::PNMwriter()
{
}
 
void PNMwriter::Write(char *filename)
{
    FILE *f_out = fopen(filename, "wb");
    fprintf(f_out, "P6\n%d %d\n%d\n", input->getWidth(), input->getHeight(), input->getMaxval());
    fwrite(input->getPixel(), sizeof(Pixel), input->getWidth()*input->getHeight(), f_out);
    fclose(f_out);
}

