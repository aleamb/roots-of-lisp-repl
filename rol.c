#include "rol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


S_EXP* T;
S_EXP* NIL;
char* T_atom_name = "T";

S_EXP* rol_init_environment() {

  S_EXP* env = NULL;
  
  
  TATOM* atom = (TATOM*)malloc(sizeof(TATOM));
  atom->name = T_atom_name;
  T = (S_EXP*)malloc(sizeof(S_EXP));
  T->type = ATOM;
  T->expr = atom;

  NIL = rol_make_cons(NULL, NULL);

  env = rol_make_cons(rol_make_cons(T, rol_make_cons(T, NIL)), NIL);

  return env;
}

S_EXP* rol_make_atom_from_string(const char* name) {

  if (strcmpi(name, "NIL") == 0) return NIL;
  if (strcmpi(name, "T") == 0) return T;

  TATOM *atom = (TATOM*)malloc(sizeof(TATOM)); 
  S_EXP *s_expr = (S_EXP*)malloc(sizeof(S_EXP));

  atom->name = strdup(name);

  s_expr->type = ATOM;
  s_expr->expr = atom;
  return s_expr;
}

S_EXP* rol_make_cons(S_EXP* car, S_EXP* cdr) {
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

char* rol_get_atom_name(S_EXP* atom) {
  if (atom->type == ATOM) {
    return ((TATOM*)atom->expr)->name;
  }
  return NULL;
}

int rol_nil(S_EXP* s_expr) {
  return (s_expr == NULL || s_expr->expr == NULL || s_expr == NIL || rol_empty_list(s_expr) );
}

int rol_t(S_EXP* s_expr) {
  return (s_expr != NULL && s_expr->expr != NULL && s_expr == T || !rol_empty_list(s_expr) );
}

int rol_is_atom(S_EXP* s_exp) {
  return s_exp->type == ATOM;
}

int rol_is_cons(S_EXP* s_exp) {
  return s_exp->type == CONS;
}

int rol_empty_list(S_EXP* s_expr) {
  return ( s_expr->type != ATOM && ((TCONS*)s_expr->expr)->car == NULL && ((TCONS*)s_expr->expr)->cdr == NULL );
}

S_EXP* rol_get_car(S_EXP* s_exp) {
  TCONS* cons = (TCONS*)(s_exp->expr);
  if (!cons) return NIL;
  if (!cons->car) return NIL;
  return ((TCONS*)(s_exp->expr))->car;
}

S_EXP* rol_get_cdr(S_EXP* s_exp) {
  TCONS* cons = (TCONS*)(s_exp->expr);
  if (!cons) return NIL;
  if (!cons->cdr) return NIL;
  return ((TCONS*)(s_exp->expr))->cdr;
}

static void _rol_print(S_EXP* s_exp, int level) {
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
      int elements_printed = 0;
      if ((car == NULL && cdr == NULL) || (car == NIL && cdr == NIL)) {
        printf("NIL");
      } else if (rol_is_atom(car) && rol_is_atom(cdr)) {
        printf("(");
        _rol_print(car, level);
        printf (" . ");
        _rol_print(cdr, level);
        printf(")");
      } else {
        printf("(");
        while (c_s_exp != NULL && c_s_exp != NIL) {
          if (elements_printed > 0) 
            printf(" ");
          car = rol_get_car(c_s_exp);
          _rol_print(car, level);
          elements_printed++;
          c_s_exp = rol_get_cdr(c_s_exp);
        }
        printf(")");
      }
    }
}

void rol_print(S_EXP* s_exp) {
  _rol_print(s_exp, 0);
}

