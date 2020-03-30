#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sexp.h"
#include "parser.h"
#include "rol.h"
#include "print.h"

extern void clean();

S_EXP create_environment() {

  S_EXP env = NULL;
  env = s_exp_create_cons(s_exp_create_cons(T, s_exp_create_cons(T, NULL)), NULL);
  return env;
}

S_EXP rol_read(FILE* stream) {
    printf("rol>");
    return parse(stream);
}

int main(int argc, char** argv) {

   S_EXP environment = create_environment();
  
    while (!feof(stdin)) {
        S_EXP sexp = rol_read(stdin);
        if (sexp) {
          print(eval(sexp, environment));
          printf("\n");
        }
    }
    clean();
    return 0;
}