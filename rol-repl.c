#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "rol.h"
#include "parser.h"
#include "eval.h"


S_EXP* rol_read(FILE* stream) {
    printf("rol>");
    return parse(stream);
}

void print(S_EXP* s_exp) {
  rol_print(s_exp);
  printf("\n");
}

int main(int argc, char** argv) {

    
    S_EXP* environment = rol_init_environment();

    while (!feof(stdin)) {
        S_EXP* expr = rol_read(stdin);
        //print(expr);
        print(eval(expr, environment));
    }

    return 0;
}