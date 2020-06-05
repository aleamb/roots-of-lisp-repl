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

#ifndef _PARSER_H_ 
#define _PARSER_H_

#include <stdio.h>
#include "sexp.h"

#define EXPR_SIZE 512
#define ATOM_SIZE 64

typedef enum {
  TOKEN_LIST_OPEN,
  TOKEN_LIST_CLOSE,
  TOKEN_QUOTE,
  TOKEN_ATOM,
  TOKEN_INVALID,
  TOKEN_EOF
} TOKEN;

typedef struct {
  FILE* stream;
  char buffer[EXPR_SIZE];
  char token_value[ATOM_SIZE];
  int buffer_index;
  int line;
  int position;
  int status;
  TOKEN token;
} S_EXP_PARSER_CONTEXT;

S_EXP parse(S_EXP_PARSER_CONTEXT*);
S_EXP_PARSER_CONTEXT* init_parser(FILE* stream);

#endif /* _PARSER_H_ */
