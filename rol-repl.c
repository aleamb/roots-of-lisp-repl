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


void repl(FILE * stream, S_EXP environment) {

    while (!feof(stream)) {
        S_EXP sexp = rol_read(stream);
        if (sexp) {
          print(eval(sexp, environment));
          printf("\n");
        }
    }
}

void try_load_init_file(S_EXP environment) {
  
  FILE* init_file = fopen("init.lisp", "r");

  if (init_file) {
    puts("init.lisp found. Loading...");
    repl(init_file, environment);
    fclose(init_file);
    puts("init.lisp loaded OK");
  }
}



int main(int argc, char** argv) {

  S_EXP environment = create_environment();

  try_load_init_file(environment);
  repl(stdin, environment);
  clean();

  return 0;
}