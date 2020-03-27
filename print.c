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

#include <stdio.h>
#include "rol.h"
#include "print.h"


static void _s_exp_print(S_EXP sexp, int level) {
  if (sexp == NULL) return;
  if (sexp == T) { printf(T_NAME); return; }
  if (sexp == NIL) { printf(NIL_NAME); return; }
  if (s_exp_atom(sexp)) {
    char *symbol = s_exp_atom_name(sexp);
    printf("%s", symbol);
  } else {
      S_EXP c_sexp = sexp;
      S_EXP car = s_exp_get_car(c_sexp);
      S_EXP cdr = s_exp_get_cdr(c_sexp);
      int elements_printed = 0;
      if ((car == NULL && cdr == NULL) || (car == NIL && cdr == NIL)) {
        printf(NIL_NAME);
      } else if (s_exp_atom(car) && s_exp_atom(cdr)) {
        printf("(");
        _s_exp_print(car, level);
        printf (" . ");
        _s_exp_print(cdr, level);
        printf(")");
      } else {
        printf("(");
        while (c_sexp != NULL && c_sexp != NIL) {
          if (elements_printed > 0) 
            printf(" ");
          car = s_exp_get_car(c_sexp);
          _s_exp_print(car, level);
          elements_printed++;
          c_sexp = s_exp_get_cdr(c_sexp);
        }
        printf(")");
      }
    }
}

void print(S_EXP sexp) {
  _s_exp_print(sexp, 0);
}
