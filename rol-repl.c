

#include <stdio.h>
#include <ctype.h>
#include "rol.h"

#define EXPR_SIZE 512

void rol_read(char *buffer, S_EXP* s_expr) {


}

S_EXP* my_read(FILE* stream) {
    char buffer[BUFSIZ];
    S_EXP* s_expr = NULL;
    while (fgets(buffer, EXPR_SIZE, stream) != NULL) {
        char* buf_ptr = buffer;
        rol_read(buffer, s_expr);
    }
    return s_expr;
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