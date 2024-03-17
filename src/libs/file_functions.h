#include <stdio.h>

#ifndef FILEFUNCTIONS_H
#define FILEFUNCTIONS_H

typedef struct charArray {
        char *array;
        int size;
} charArray;

charArray *getStringFromFile(FILE *stream);
charArray *getCharArrayfromString(char *string);

#endif
