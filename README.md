# roots-of-lisp-repl

Lisp REPL implementation based on Paul Graham's article "The Roots of Lisp" http://www.paulgraham.com/rootsoflisp.html

This a project for educationa purposes. It has tried to be as precise as possible to the Paul Graham's Paper.

As original paper, no arithmetic functions available. Any improvements are welcome.


- Additional features:

1. It has been added ```defun``` feature. Functions will be defined in top level scope.
2. REPL search for init.lisp file on same directory. If available, it loads its contents. This can be useful for extending interpreter.
3. Memory managmenet for dynamic sexp objects created during evaluation. This manager is very simple, only storgaes sexp objects in a linked list and
  transverse into it to calling ```free```` libc function in each sexp object pointer. This ocurrs closing REPL with Ctrl-D (Ctrl-Z on Windows)


## Build

Needs CMake >= 3.5

1. create directory named build in thh project root directory

```
mkdir build
```

2. Execute Cmake

```
cmake ..

```

3. Execute make


## REPL Session Example

- Run ```rol``` executable. You will see the ROL (Roots of Lisp) REPL prompt

```
rol>

```

- Define a function:

```
rol> (defun subst (x y z)
      (cond ((atom z)
         (cond ((eq z y) x)
               ('t z)))
          (t (cons (subst x y (car z))
                  (subst x y (cdr z ))))))
Output:
((label subst (lambda (x y z) (cond ((atom z) (cond ((eq z y) x) ((quote t) z))) (t (cons (subst x y (car z)) (subst x y (cdr z))))))))
rol>

```

- Call it:

```
rol>(subst 'm 'b '(a b b c b))
(a m m c m)
rol>

```

End REPL session with EOF caharacter (Ctrl-D or Ctrl-Z on Windows)

