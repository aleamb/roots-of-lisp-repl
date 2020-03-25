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
  if (s_exp == NULL) return;
  if (s_exp == T) { printf("T"); return; }
  if (s_exp == NIL) { printf("NIL"); return; }
  if (rol_is_atom(s_exp)) {
    char *symbol = rol_get_atom_name(s_exp);
    printf("%s", symbol);
  } else {
      S_EXP* c_s_exp = s_exp;
      S_EXP* car = rol_get_car(c_s_exp);
      S_EXP* cdr = rol_get_cdr(c_s_exp);
      if (car == NULL && cdr == NULL) {
        printf("NIL");
      } else {
        printf("(");
        while (c_s_exp != NULL && c_s_exp != NIL) {
          car = rol_get_car(c_s_exp);
          if (rol_is_atom(car)) {
            _print(car, level);
          } else {
            _print(car, level + 1);
          }
          c_s_exp = rol_get_cdr(c_s_exp);
          printf(" ");
        }
        printf(")");
      }
    }
}

void print(S_EXP* s_exp) {
  _print(s_exp, 0);
  printf("\n");
}

int main(int argc, char** argv) {

    rol_init();

    while (!feof(stdin)) {
        S_EXP* expr = rol_read(stdin);
        print(expr);
        //print(eval(expr));
    }

    return 0;
}