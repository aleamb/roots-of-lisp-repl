#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "eval.h"
#include "rol.h"


// utilities
int atom_name_equal(S_EXP* exp, const char* name) {
  return exp != NULL && exp != NIL && strcmpi(((TATOM*)exp->expr)->name, name) == 0;
}

//axioms
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


// abbreviations
S_EXP* cadr(S_EXP* expr) {
  return car(cdr(expr));
}

S_EXP* caddr(S_EXP* expr) {
  return car(cdr(cdr(expr)));
}

S_EXP* cdar(S_EXP* expr) {
  return cdr(car(expr));
}

S_EXP* caar(S_EXP* expr) {
  return car(car(expr));
}

S_EXP* cadar(S_EXP* expr) {
  return car(cdr(car(expr)));
}

S_EXP* list(S_EXP* expr1, ...) {
  va_list argp;
  va_start(argp, expr1);
  S_EXP* l = cons(expr1, NIL);
  S_EXP* expr;
  while ((expr = va_arg(argp, S_EXP*)) != NULL) {
    S_EXP* cs = cons(expr, NIL);
    rol_set_cdr(l, cs);
    l = cs;
  }
  return l;
}


// functions (Chapter 3)

S_EXP* _null(S_EXP* expr) {
  if (expr == NIL || rol_empty_list(expr)) {
    return T;
  }
  return NIL;
}

S_EXP* and(S_EXP* expr1, S_EXP* expr2) {
  if (expr1 == T && expr2 == T) {
    return T;
  }
  return NIL;
}

S_EXP* not(S_EXP* expr1) {
  if (rol_t(expr1)) {
    return NIL;
  }
  return T;
}

S_EXP* append(S_EXP* expr1, S_EXP* expr2) {
  if (rol_t(_null(expr1)) ) {
    return expr2;
  }
  return cons(car(expr1), append(cdr(expr1), expr2));
}

S_EXP* pair(S_EXP* expr1, S_EXP* expr2) {
  if (rol_t( and( _null(expr1), _null(expr2)))) {
    return NIL;
  }
  if (rol_t(and(not(atom(expr1)), not(atom(expr2)) ))) {
    return cons(list(car(expr1), car(expr2)), pair(cdr(expr1), cdr(expr2)));
  }
  return NIL;
}

S_EXP* assoc(S_EXP* expr1, S_EXP* expr2) {
  if (rol_t(eq(caar(expr2), expr1) )) {
    return cadar(expr2);
  }
  return assoc(expr1, cdr(expr2));
}

// evaluator

S_EXP* eval(S_EXP* e, S_EXP* a) {
  S_EXP* result = NULL;
  if (rol_t(atom(e))) {
    result = assoc(e, a);
  } else if (rol_t(atom(car(e)))) {

    if (atom_name_equal(car(e), "quote")) {
      result = cadr(e);
    } else if (atom_name_equal(car(e), "quote")) {
    } else if (atom_name_equal(car(e), "atom")) {
    } else if (atom_name_equal(car(e), "eq")) {
    } else if (atom_name_equal(car(e), "car")) {
    } else if (atom_name_equal(car(e), "cdr")) {
    } else if (atom_name_equal(car(e), "cons")) {
    } else if (atom_name_equal(car(e), "cond")) {
    } else {
      
    }
  }
  return result;

}