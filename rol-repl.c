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
  if (s_exp == NULL) { printf(")"); return; }
  if (s_exp->type == ATOM) {
    char *symbol = ((TATOM*)(s_exp->expr))->name;
    printf("%s", symbol);
  } else {
    S_EXP* car = ((TCONS*)(s_exp->expr))->car;
    S_EXP* cdr = ((TCONS*)(s_exp->expr))->cdr;
    if (!car && !cdr) {
      printf("NIL");
    } else {
      if (level == 0 || car->type == CONS)
        printf("(");
      _print(car, level + 1 );
      if (cdr != NULL)
        printf(" ");
      _print(cdr,  level + 1 );
     
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