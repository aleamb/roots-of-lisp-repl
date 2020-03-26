
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
  if (!s_expr || s_expr == NIL || rol_empty_list(s_expr)) return T;
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

S_EXP* caddar(S_EXP* expr) {
  return car((cdr(cdr(car(expr)))));
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
    return cons(rol_make_cons(car(expr1), rol_make_cons(car(expr2), NIL)), pair(cdr(expr1), cdr(expr2)));
  }
  return NIL;
}

S_EXP* assoc(S_EXP* expr1, S_EXP* expr2) {
  if (expr2 == NIL) return NIL;
  if (rol_t(eq(caar(expr2), expr1) )) {
    return cadar(expr2);
  }
  return assoc(expr1, cdr(expr2));
}

// evaluator

S_EXP* evcon(S_EXP* c, S_EXP* a) {
  S_EXP* result = NULL;
  if (rol_t(eval(caar(c), a))) {
    result = eval(cadar(c), a);
  } else {
    result = evcon(cdr(c), a);
  }
  return result;
}

S_EXP* evlis(S_EXP* m, S_EXP* a) {
  S_EXP* result = NULL;
  if (rol_t(_null(m))) {
    result = NIL;
  } else {
    result = cons(eval(car(m), a), evlis(cdr(m), a));
  }
  return result;
}


S_EXP* eval(S_EXP* e, S_EXP* a) {
  S_EXP* result = NIL;
  if (rol_t(atom(e))) {
    result = assoc(e, a);
  } else if (rol_t(atom(car(e)))) {
    if (atom_name_equal(car(e), "quote")) {
      result = cadr(e);
    } else if (atom_name_equal(car(e), "quote")) {
      result = cadr(e);
    } else if (atom_name_equal(car(e), "atom")) {
      result = atom(eval(cadr(e),a));
    } else if (atom_name_equal(car(e), "eq")) {
      result = eq( eval(cadr(e), a), eval(caddr(e), a));
    } else if (atom_name_equal(car(e), "car")) {
      result = car(eval(cadr(e), a));
    } else if (atom_name_equal(car(e), "cdr")) {
      result = cdr(eval(cadr(e), a));
    } else if (atom_name_equal(car(e), "cons")) {
      result = cons( eval(cadr(e), a), eval(caddr(e), a));
    } else if (atom_name_equal(car(e), "cond")) {
      result = evcon(cdr(e), a);
    } else {
      result = eval(cons(assoc(car(e), a), cdr(e)), a);
    }
  } else if (atom_name_equal(caar(e), "label")) {
    result = eval( cons(caddar(e), cdr(e)), 
                  cons(rol_make_cons(cadar(e), rol_make_cons(car(e), NIL)), a)); 
  } else if (atom_name_equal(caar(e), "lambda")) {
    result = eval(caddar(e), append( pair(cadar(e), evlis(cdr(e), a)) , a));
  }
  return result;
}

