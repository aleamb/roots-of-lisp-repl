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

#ifndef _SEXP_H_
#define _SEXP_H_

typedef enum {
    ATOM,
    CONS
} S_EXP_TYPE;

typedef struct {
    S_EXP_TYPE type;
    void* expr;
} S_EXP_NODE;

typedef S_EXP_NODE* S_EXP;

typedef struct _atom {
    char *name;
} TATOM;

typedef struct _cons {
  S_EXP car;
  S_EXP cdr;
} TCONS;

int s_exp_atom(S_EXP);

char* s_exp_atom_name(S_EXP);

S_EXP s_exp_create_atom(const char*);

S_EXP s_exp_create_cons(S_EXP, S_EXP);

S_EXP s_exp_get_car(S_EXP);

S_EXP s_exp_get_cdr(S_EXP);

void s_exp_set_cdr(S_EXP, S_EXP);

void s_exp_free(S_EXP s_exp);

#endif /* _SEXP_H_ */
