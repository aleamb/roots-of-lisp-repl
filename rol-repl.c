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

void _print(S_EXP* s_exp, int level) {
  if (s_exp == NULL || s_exp == NIL) { return; }
  if (rol_is_atom(s_exp)) {
    char *symbol = rol_get_atom_name(s_exp);
    printf("%s", symbol);
  } else {
    S_EXP* car = rol_get_car(s_exp);
    S_EXP* cdr = rol_get_cdr(s_exp); 
    if (!car && !cdr) {
      printf("NIL");
    } else {
      if (level == 0 || rol_is_cons(car))
        printf("(");
      _print(car, level + 1 );
      if (cdr != NULL)
        printf(" ");
      _print(cdr,  level + 1 ); 
      if (level == 0 || rol_is_cons(car)) printf(")");
    }
  }
}

void print(S_EXP* s_exp) {
  _print(s_exp, 0);
  printf("\n");
}

int main(int argc, char** argv) {

    while (!feof(stdin)) {
        S_EXP* expr = rol_read(stdin);
        print(expr);
        //print(eval(expr));
    }

    return 0;
}