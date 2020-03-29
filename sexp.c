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

#include <stdlib.h>
#include <string.h>
#include "sexp.h"

int mallocs = 0;


static S_EXP create_sexp(S_EXP_TYPE type, void* data) {
  S_EXP sexp = (S_EXP_NODE*)malloc(sizeof(S_EXP_NODE));
  mallocs++;
  sexp->type = type;
  sexp->disposable = 1;
  if (type == ATOM) {
    sexp->atom = data;
    sexp->cons = NULL;
  } else {
    sexp->cons = data;
    sexp->atom = NULL;
  }
  return sexp;
}

static S_EXP create_sexp_atom() {
  S_EXP sexp = create_sexp(ATOM, NULL);
  return sexp;
}

static S_EXP create_sexp_cons() {
  S_EXP sexp = create_sexp(CONS, NULL);
  return sexp;
}

static int sexp_type(S_EXP sexp) {
  return sexp->type;
}

static void sexp_set_type(S_EXP sexp, S_EXP_TYPE type) {
  sexp->type = type;
}

static int is_atom(S_EXP sexp) {
  return sexp == NULL || sexp_type(sexp) == ATOM;
}

static char* atom_name(S_EXP sexp) {
  if (is_atom(sexp)) {
    return sexp->atom->name;
  }
  return NULL;
}

static S_EXP set_atom_name(TATOM* sexp, const char* name) {
  sexp->name = strdup(name);
  mallocs++;
}

static TATOM* create_atom_node(const char* name) {
  TATOM* atom = (TATOM*)malloc(sizeof(TATOM));
  mallocs++;
  set_atom_name(atom, name);
  return atom;
}

static TCONS* create_cons_node() {
  TCONS *cons = (TCONS*)malloc(sizeof(TCONS));
  mallocs++;
  cons->car = NULL;
  cons->cdr = NULL;
  return cons;
}

static void set_car(S_EXP sexp, S_EXP car) {
  if (!s_exp_atom(sexp)) {
    sexp->cons->car = car;
  }
}

static void set_cdr(S_EXP sexp, S_EXP cdr) {
  if (!s_exp_atom(sexp)) {
    sexp->cons->cdr = cdr;
  }
}

static S_EXP get_car(S_EXP sexp) {
  if (!s_exp_atom(sexp) && sexp->cons) {
    return sexp->cons->car;
  }
  return NULL;
}

static S_EXP get_cdr(S_EXP sexp) {
  if (!s_exp_atom(sexp) && sexp->cons) {
    return sexp->cons->cdr;
  }
  return NULL;
}


static void free_atom(S_EXP sexp) {
  if (sexp != NULL) {
    char* name = atom_name(sexp);
    if (name) {
      free(name);
      mallocs--;
    }
    free(sexp);
    mallocs--;
  }
}

/////

int s_exp_atom(S_EXP sexp) {
  return is_atom(sexp);
}

char* s_exp_atom_name(S_EXP sexp) {
  return atom_name(sexp);
}

S_EXP s_exp_create_atom(const char* name) {
  TATOM *atom = create_atom_node(name);
  return create_sexp(ATOM, atom);
}

S_EXP s_exp_create_cons(S_EXP car, S_EXP cdr) {
  TCONS *cons = create_cons_node();
  S_EXP sexp = create_sexp(CONS, cons);
  set_car(sexp, car);
  set_cdr(sexp, cdr);
  return sexp;
}

S_EXP s_exp_get_car(S_EXP sexp) {
  return get_car(sexp);
}

S_EXP s_exp_get_cdr(S_EXP sexp) {
  return get_cdr(sexp);
}

void s_exp_set_car(S_EXP sexp, S_EXP car) {
  set_car(sexp, car);
}

void s_exp_set_cdr(S_EXP sexp, S_EXP cdr) {
  set_cdr(sexp, cdr);
}

void s_exp_free(S_EXP sexp) {
  print(sexp);
  if (sexp == NULL) return;
  if (is_atom(sexp)) { 
    if (sexp->disposable)
      free_atom(sexp);
  } else {
  
    s_exp_free(get_cdr(sexp));
    s_exp_free(get_car(sexp));
    free(sexp);
    
  }
}
