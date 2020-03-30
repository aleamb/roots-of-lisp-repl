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
#include <stdio.h>
#include "sexp.h"
#include "rol.h"

typedef struct _list {

  void *ptr;
  struct _list* next;
} SEXPLIST;

SEXPLIST sexplist = { 0 };
SEXPLIST* sexplist_ptr = &sexplist;

void* s_exp_alloc(int size) {
  void* ptr = malloc(size);

  sexplist_ptr->next = (SEXPLIST*)malloc(sizeof(SEXPLIST));
  sexplist_ptr->next->ptr = ptr;
  sexplist_ptr->next->next = NULL;
  sexplist_ptr = sexplist_ptr->next;

  return ptr;
}

void clean() {

  SEXPLIST* curr = sexplist.next;

  while (curr) {
    SEXPLIST* tmp = curr;
    curr = curr->next;

    if (s_exp_atom((S_EXP)tmp->ptr)) {
      TATOM* atom = ((S_EXP)tmp->ptr)->atom;
      if ((S_EXP)atom != T && atom && atom->name) {
        free(atom->name);
      }
    }
    
    free(tmp->ptr);
    free(tmp);
  }
}
