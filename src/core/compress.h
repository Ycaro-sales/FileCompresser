#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef COMPRESS_H
#define COMPRESS_H

bool compress(FILE *file);
int *create_char_frequency_table(unsigned char *buffer);

#endif
