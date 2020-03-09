#include "rol.h"
#include <stdlib.h>
#include <string.h>

S_EXP* rol_make_atom_from_string(const char* name) {
  TATOM *atom = (TATOM*)malloc(sizeof(TATOM)); 
  S_EXP *s_expr = (S_EXP*)malloc(sizeof(S_EXP));

  atom->name = strdup(name);

  s_expr->type = ATOM;
  s_expr->expr = atom;
  return s_expr;
}

S_EXP* rol_make_cons(S_EXP* car, S_EXP* cdr)  {
  TCONS *cons = (TCONS*)malloc(sizeof(TCONS));
  S_EXP *s_expr = (S_EXP*)malloc(sizeof(S_EXP));
  
  s_expr->type = CONS;
  s_expr->expr = cons;
  cons->car = car;
  cons->cdr = cdr;

  return s_expr;
}


void rol_set_cdr(S_EXP* current, S_EXP* cdr) {
    ((TCONS*)(current->expr))->cdr = cdr;
}
