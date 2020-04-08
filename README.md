# roots-of-lisp-repl

Lisp REPL implementation based on Paul Graham's article "The Roots of Lisp" http://www.paulgraham.com/rootsoflisp.html

It has tried to be as precise as possible to the Paul Graham's paper.

As original paper, no arithmetic functions available.

Any pull requests are welcome.


- Additional features:

1. It has been added ```defun``` feature. Functions will be defined in top level scope.
2. REPL search for init.lisp file on same directory. If available, it loads its contents. This can be useful for extending interpreter.
3. Memory management for dynamic sexp objects created during evaluation. This manager is very simple, only storages sexp objects in a linked list and
  transverse into it calling ```free``` libc function in each sexp object pointer. This occurs closing REPL with Ctrl-D (Ctrl-Z on Windows)


## Build

CMake >= 3.5 required

On Windows, use CMake with MSBuild tools or MinGW/MSYS

1. create directory named build in the project's root directory

```
mkdir build

```

2. Execute CMake

```
cmake ..

```

3. Execute Make if system is Linux/Unix/Mac/MinGW

On Unix/Linux/MinGW based systems:

```
make

```

If the system is Windows with MsBuild tools:

```
msbuild rol.sln

```

## REPL Session Example

- Run ```rol``` executable in build directory (If using MSBuilt tools, excutable can be locate on Debug directory). You will see the ROL (Roots of Lisp) REPL prompt

```
rol>

```

- Define a function:

```
rol> (defun subst (x y z)
      (cond ((atom z)
         (cond ((eq z y) x)
               (t z)))
          (t (cons (subst x y (car z))
                  (subst x y (cdr z ))))))
Output:
((label subst (lambda (x y z) ...
rol>

```

- Call it:

```
rol>(subst 'm 'b '(a b b c b))
(a m m c m)
rol>

```

End REPL session with EOF caharacter (Ctrl-D or Ctrl-Z on Windows)
