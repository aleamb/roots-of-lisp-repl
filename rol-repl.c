

#include <stdio.h>
#include <ctype.h>
#include "rol.h"

#define EXPR_SIZE 512

void my_read(char* expr, FILE* stream) {
    char buffer[BUFSIZ];
    S_EXP* expr = NULL;
    while (fgets(buffer, EXPR_SIZE, stream) != NULL) {
        char* buf_ptr = buffer;
        rol_read(buffer, expr)
    }
    return expr;
}

int main(int argc, char** argv) {

    char expr[EXPR_SIZE];

    while (!feof(stdin)) {
        S_EXP* expr = my_read(stdin);
        //eval(expr);
        //print();
    }

    return 0;
}