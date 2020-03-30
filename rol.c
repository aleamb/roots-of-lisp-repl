/* 
  MIT License

  Copyright (c) 2020 Alejandro Ambroa

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE. 
*/

#include <string.h>
#include <stdio.h>
#include "rol.h"

// build T and NIL objects

static TATOM t_atom = { "T" }; 
static S_EXP_NODE t_atom_node = { ATOM, 0, &t_atom, NULL }; 
const S_EXP T = &t_atom_node ;

static TCONS nil_cons= { NULL, NULL }; 
static S_EXP_NODE t_nil_node = { CONS, 0, NULL, &nil_cons }; 
const S_EXP NIL = &t_nil_node;

// utilities

static int empty_list(S_EXP sexp) {
  return !s_exp_atom(sexp) && (s_exp_get_car(sexp) == NULL && s_exp_get_cdr(sexp) == NULL);
}

int atom_name_equal(S_EXP sexp, const char* name) {
  return s_exp_atom(sexp) && strcasecmp(s_exp_atom_name(sexp), name) == 0;
}

/*
 Primitives (chapter 1)
 
 Primitive implementations except cond.
 
 It is no possible implement cond primitve in the same way as paper. 
*/

S_EXP quote(S_EXP a) {
    return a;
}

S_EXP atom(S_EXP sexp) {
  if (s_exp_atom(sexp) || empty_list(sexp)) {
    return T;
  }
  return NIL;
}

S_EXP eq(S_EXP x, S_EXP y) {
  return (s_exp_atom(x) && s_exp_atom(y) && strcasecmp(s_exp_atom_name(x), s_exp_atom_name(y)) == 0) || (empty_list(x) && empty_list(y)) ? T : NIL;
}

S_EXP car(S_EXP x) { 
  S_EXP sexp = s_exp_get_car(x);
  return sexp ? sexp : NIL; 
}

S_EXP cdr(S_EXP x) { 
  S_EXP sexp = s_exp_get_cdr(x);
  return sexp ? sexp : NIL; 
}

S_EXP cons(S_EXP x, S_EXP y) { 
  return s_exp_create_cons(x ? x : NIL, y ? y : NIL);
}


/*

  Abbreviations (CHapter 3)

  Implements abbreviations for eval function.

  In the paper, the use of the 'list' function was reduced to 
  two parameters. So I have omitted the list implementation so 
  I don't have to deal with varargs, making use of conses.

*/

S_EXP cadr(S_EXP sexp) {
  return car(cdr(sexp));
}

S_EXP caddr(S_EXP sexp) {
  return car(cdr(cdr(sexp)));
}

S_EXP cdar(S_EXP sexp) {
  return cdr(car(sexp));
}

S_EXP caar(S_EXP sexp) {
  return car(car(sexp));
}

S_EXP cadar(S_EXP sexp) {
  return car(cdr(car(sexp)));
}

S_EXP caddar(S_EXP sexp) {
  return car(cdr(cdr(car(sexp))));
}

S_EXP _null(S_EXP x) {
  return eq(x, NIL);
}

S_EXP and(S_EXP x, S_EXP y) {
  return (!empty_list(x) && !empty_list(y)) ? T : NIL;
}

S_EXP not(S_EXP x) {
  return empty_list(x) ? T : NIL;
}

S_EXP append(S_EXP x, S_EXP y) {
  if (!empty_list(_null(x))) {
    return y;
  }
  return cons(car(x), append(cdr(x), y));
}

S_EXP pair(S_EXP x, S_EXP y) {
  if (!empty_list( and( _null(x), _null(y)))) {
    return NIL;
  }
  if (!empty_list(and(not(atom(x)), not(atom(y)) ))) {
    return cons(s_exp_create_cons(car(x), s_exp_create_cons(car(y), NIL)), pair(cdr(x), cdr(y)));
  }
  return NIL;
}

S_EXP assoc(S_EXP x, S_EXP y) {
  if (empty_list(y)) return NIL;
  if (!empty_list(eq(caar(y), x) )) {
    return cadar(y);
  }
  return assoc(x, cdr(y));
}

S_EXP defun(S_EXP m, S_EXP a) {
  // create ((label f lambda (params))

  S_EXP label = cons(cons(s_exp_create_atom("label"), cons(cadr(m), cons(cons(
                                      s_exp_create_atom("lambda"),
                                      cons(caddr(m), cdr(cdr(cdr(m))))),
                                  NULL
                                ))) , NULL);
  S_EXP sexp = a;
  while (sexp != NIL) {
    if (!empty_list(eq(caar(sexp),cadr(m)))) {
      s_exp_set_car(sexp, cadr(sexp));
      s_exp_set_cdr(sexp, cdr(cdr(sexp)));
      break;
    }
    sexp = cdr(sexp);
  }
  
  S_EXP car = s_exp_get_car(a);

  s_exp_set_car(a, cons(cadr(m), label));
  s_exp_set_cdr(a, cons(car, s_exp_get_cdr(a)));

 return label;
}


// evaluator (Chapter 4)

S_EXP evcon(S_EXP c, S_EXP a) {
  S_EXP result = NIL;
  if (!empty_list(eval(caar(c), a))) {
    result = eval(cadar(c), a);
  } else {
    result = evcon(cdr(c), a);
  }
  return result;
}

S_EXP evlis(S_EXP m, S_EXP a) {
  S_EXP result = NIL;
  if (!empty_list(_null(m))) {
    result = NIL;
  } else {
    result = cons(eval(car(m), a), evlis(cdr(m), a));
  }
  return result;
}

S_EXP eval(S_EXP e, S_EXP a) {
  S_EXP result = NIL;
  if (!empty_list(atom(e))) {
    result = assoc(e, a);
  } else if (!empty_list(atom(car(e)))) {
    if (atom_name_equal(car(e), "quote")) {
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
    } else if (atom_name_equal(car(e), "defun")) {
      /*
        This extra code added to original eval function the  implementation 
        of defun calls as and internal C function which assocs defun code with 
        defun first parameter (name of function) and adds it to top level 
        environment.
      */
      result = defun(e, a);
    } else {
      result = eval(cons(assoc(car(e), a), cdr(e)), a);
    }
  } else if (atom_name_equal(caar(e), "label")) {
    result = eval(cons(caddar(e), cdr(e)), 
                  cons(s_exp_create_cons(cadar(e), s_exp_create_cons(car(e), NULL)), a)); 
  } else if (atom_name_equal(caar(e), "lambda")) {
    result = eval(caddar(e), append( pair(cadar(e), evlis(cdr(e), a)) , a));
  }
  return result;
}

