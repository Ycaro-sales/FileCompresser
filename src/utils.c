#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char set_bit(unsigned char c, int i) {
        unsigned char mask = 1 << i;
        return mask | c;
}

bool is_bit_i_set(unsigned char c, int i) {
        unsigned char mask = 1 << i;
        return mask & c;
}

char *remove_extension(char *file_name) {
        char *tmp = malloc(sizeof(file_name));
        for (int i = 0; i < strlen(file_name); i++) {
                tmp[i] = file_name[i];
                if (file_name[i] == '.') {
                        tmp[i] = '\0';
                        break;
                }
        }

        return tmp;
}

char *concat(const char *dest, const char *src) {
        char *buffer = malloc(strlen(dest) + strlen(src) + 1);
        strcpy(buffer, dest);
        strcat(buffer, src);
        return buffer;
}

bool regex_match(char *pattern, char *expression) {
        regex_t *regex = malloc(sizeof(regex_t));
        /* compila a ER passada em argv[1]
         * em caso de erro, a função retorna diferente de zero */
        if (regcomp(regex, pattern, REG_EXTENDED | REG_NOSUB) != 0) {
                fprintf(stderr, "erro regcomp\n");
                exit(1);
        }

        /* tenta casar a ER compilada (&reg) com a entrada (argv[2])
         * se a função regexec retornar 0 casou, caso contrário não */
        if ((regexec(regex, expression, 0, (regmatch_t *)NULL, 0)) == 0)
                return true;
        else
                return false;
}
