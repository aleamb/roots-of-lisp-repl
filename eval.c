#include <stdarg.h>
#include <string.h>
#include "eval.h"



S_EXP* eval(S_EXP*, S_EXP*);

S_EXP* quote(S_EXP* s_expr) {
    return s_expr;
}

S_EXP* atom(S_EXP* s_expr) {
  if (s_expr->type == ATOM) {
    return T;
  }
  return NIL;
}

S_EXP* eq(S_EXP* a, S_EXP* b) {
  return (a->type == ATOM && b->type == ATOM && strcmpi(rol_get_atom_name(a), rol_get_atom_name(b)) == 0) || (rol_nil(a) && rol_nil(b)) ? T : NIL;
}

S_EXP* car(S_EXP* expr) { 
  return rol_get_car(expr);
}

S_EXP* cdr(S_EXP* expr) { 
  return rol_get_cdr(expr);
}

S_EXP* cons(S_EXP* expr1, S_EXP* expr2) { 
  return rol_make_cons(expr1, expr2);
}

S_EXP* cond(S_EXP* expr1, ...) { 
  va_list argp;
  va_start(argp, expr1);
  S_EXP* expr = NULL;
  if (expr1->type == ATOM) {
    printf("Arguments must be a list");
    return NIL;
  }

  S_EXP* car_result = eval(rol_get_car(expr1), env);
  if (!rol_nil(car_result)) {
    return eval(rol_get_cdr(expr), env);
  }

  while ((expr = va_arg(argp, S_EXP*)) != NULL) {
    S_EXP* car_result = eval(rol_get_car(expr), env);
    if (!rol_nil(car_result)) {
      return eval(rol_get_cdr(expr), env);
    }
  }
  va_end(argp);
  return NIL;
}
