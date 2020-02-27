

#include <stdio.h>
#include <ctype.h>
#include "rol.h"

#define EXPR_SIZE 512

void rol_read(char* expr, FILE* stream) {
    S_EXP* s_expr;
    S_EXP* curr_s_exp;
    char buffer[BUFSIZ];
    char c;
    char npar = 0;
    int state = 0;

    s_expr = NULL;
    curr_s_exp = NULL;

    while (fgets(buffer, EXPR_SIZE, stream) != NULL) {
        char* buf_ptr = buffer;

        while ((c = *buf_ptr++)) {
            if (c == '(') {

            }
                /*
            if (c == ' ' || c == '\t') {
                if (state != 0) {
                }
            } else if (isalpha(c)) {
                if (state == 0) {
                    state = 1;
                }
            } else if (c == '\'') {
                if (state == 0) {
                    state = 2;
                }
            } else if (c == ) {

            }*/
        }
    }
}

int main(int argc, char** argv) {

    char expr[EXPR_SIZE];

    while (!feof(stdin)) {
        rol_read(expr, stdin);
        //eval(expr);
        //print();
    }

    return 0;
}