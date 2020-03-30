
(defun subst (x y z)
  (cond ((atom z)
         (cond ((eq z y) x)
               (t z)))
        (t (cons (subst x y (car z))
                  (subst x y (cdr z ))))))


(defun firstatom (x)
                    (cond ((atom x) x)
                          (t (firstatom (car x)))))
