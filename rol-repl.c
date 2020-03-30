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
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sexp.h"
#include "parser.h"
#include "rol.h"
#include "print.h"

extern void clean();

S_EXP create_environment() {

  S_EXP env = NULL;
  env = s_exp_create_cons(s_exp_create_cons(T, s_exp_create_cons(T, NULL)), NULL);
  return env;
}

S_EXP rol_read(S_EXP_PARSER_CONTEXT* context) {
    printf("rol>");
    return parse(context);
}


void repl(FILE * stream, S_EXP environment) {

    S_EXP_PARSER_CONTEXT* context = init_parser(stream); 
    while (!feof(stream)) {
        S_EXP sexp = rol_read(context);
        if (sexp) {
          print(eval(sexp, environment));
          printf("\n");
        }
    }
    free(context);
}

void try_load_init_file(S_EXP environment) {

  FILE* init_file = fopen("init.lisp", "r");

  if (init_file) {
    puts("init.lisp found. Loading...");
    repl(init_file, environment);
    fclose(init_file);
    puts("init.lisp loaded OK");
  }
}

int main(int argc, char** argv) {

  S_EXP environment = create_environment();

  try_load_init_file(environment);
  repl(stdin, environment);
  clean();

  return 0;
}