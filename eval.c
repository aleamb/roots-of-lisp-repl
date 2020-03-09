#include <stdarg.h>
#include "eval.h"

S_EXP* T;
S_EXP* NIL;


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
  return (a->type == ATOM && b->type == ATOM && strcmpi(rol_get_atom_name(a), rol_get_atom_name(b)) == 0) || (rol_nil(a) && ro_nil(b)) ? T : NIL;
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

  S_EXP* expr = va_arg(argp, S_EXP*);

  
  va_end(argp);
}
