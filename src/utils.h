#include <regex.h>
#include <stdbool.h>
#ifndef UTILS_H
#define SWAP(x, y)                                                             \
        do {                                                                   \
                typeof(x) temp = (x);                                          \
                (x) = (y);                                                     \
                (y) = temp;                                                    \
        } while (0)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

unsigned char set_bit(unsigned char c, int i);

bool regex_match(char *pattern, char *expression);

bool is_bit_i_set(unsigned char c, int i);

char *remove_extension(char *file_name);

#endif // !UTILS_H
