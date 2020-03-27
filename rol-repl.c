#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sexp.h"
#include "parser.h"
#include "rol.h"
#include "print.h"


S_EXP rol_read(FILE* stream) {
    printf("rol>");
    return parse(stream);
}

/*
void print(S_EXP* s_exp) {
  rol_print(s_exp);
  printf("\n");
}

S_EXP* init_environment() {

  S_EXP* env = NULL;

  TATOM* atom = (TATOM*)malloc(sizeof(TATOM));
  atom->name = ATOM_NAME_T;
  T = (S_EXP*)malloc(sizeof(S_EXP));
  T->type = ATOM;
  T->expr = atom;

  NIL = rol_make_cons(NULL, NULL);
  env = rol_make_cons(rol_make_cons(T, rol_make_cons(T, NULL)), NULL);

  return env;
}
*/

int main(int argc, char** argv) {

   S_EXP environment = rol_create_environment();

    while (!feof(stdin)) {
        S_EXP sexp = rol_read(stdin);
        
        print(sexp);
        printf("\n");
        print(eval(sexp, environment));
        s_exp_free(sexp);
    }
    s_exp_free(environment);
    return 0;
}