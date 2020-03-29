#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sexp.h"
#include "parser.h"
#include "rol.h"
#include "print.h"



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

   printf("%p\n", environment);

    while (!feof(stdin)) {
        S_EXP sexp = rol_read(stdin);
        print(eval(sexp, environment));
        printf("\n");
    }
    s_exp_free(environment);
    return 0;
}