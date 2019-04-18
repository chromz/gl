// Rodrigo Custodio
#ifndef BMP_H
#define BMP_H

#include <stddef.h>

int bmp_write(const char *filename, int *fb, int width, int height);

int bmp_load(const char *filename, int **buffer, long *width, long *height);

#endif
