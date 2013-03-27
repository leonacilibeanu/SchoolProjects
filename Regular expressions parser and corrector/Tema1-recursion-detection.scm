(include "Reflection.scm")

;am folosit functia flatten din laboratorul 2, pentru a liniariza lista
;functia ajutatoare pentru a afla daca exista numele functiei intr-o lista data

(define (flatten L)
  (if (list? L)
  (flatten-it '() L) L))

(define (flatten-it fL L)
  (cond ((null? L) fL)
        ((list? (car L)) (flatten-it fL (append (car L)(cdr L))))
        (else (flatten-it (append fL (cons (car L) '())) (cdr L)))))

;functia forall?, in acest caz, aplica o functie p asupra elementelor listei L si intoarce numarul de elemente
;din lista care verifica p

(define (forall? p L nr)
  (if (null? L) 
      nr
      (if (p (car L)) (forall? p (cdr L) (+ nr 1))
          (forall? p (cdr L) nr))))

;exist? verifica daca functia se reapeleaza macar o data si intoarce numarul de reapelari <=> nr de aparitii al numelui functiei
;intr-o lista L

(define (exist? L name)
  (forall? (lambda (a) (equal? name a)) L 0))

;ignore returneaza lista L, mai putin primele 2 elemente

(define (ignore L)
  (cdr (cdr L)))

;evaluate-if evaluaza expresiile 1 si 2 din structura if, ignorandu-se partea de conditionare
;evalueaza fiecare conditie in parte si pastreaza maximul valorii recursive
;spre exemplu, daca o functie are recursivitate pe coada pe una din expresii si recursivitate pe stiva pe cealalta,
;se ia in considerare cazul cel mai nefavorabil, in care aceasta ar fi stack-recursive

(define (evaluate-if L name)
  (letrec ((iter1 (lambda (L name s arg)
    (if (= (length (flatten L)) 0) s 
         (if (list? (car L)) 
             (cond ((equal? (caar L) 'if)  (max (iter1 (cdr L) name  s (+ 1 arg)) (iter1 (ignore (car L)) name 0 1)))
                   ((equal? (caar L) 'cond) (max (iter1 (cdr L) name  s (+ 1 arg)) (evaluate-cond (cdr (car L)) name)))
                   ((= arg 1) (iter2 L (exist? (flatten (car L)) name) s arg))
                   ((= arg 2) (iter3 L (exist? (flatten (car L)) name) s arg))
                   (else   (iter1 (cdr L) name s (+ 1 arg))))
              (iter1 (cdr L) name s (+ 1 arg))))))
              
           (iter2 (lambda (L nr s arg)
                                (if (> nr 0)  
                                    (if (equal? (caar L) name) (iter1 (cdr L) name (+ s nr) (+ 1 arg))
                                        (iter1 (cdr L) name (+ s 1 nr) (+ 1 arg)))
                                    (iter1 (cdr L) name s (+ 1 arg)))))
           (iter3 (lambda (L nrr s arg)
                                 (if (> nrr 0) 
                                     (if (>= nrr s)
                                     (if (equal? (caar L) name) (iter1 (cdr L) name nrr (+ 1 arg))
                                         (iter1 (cdr L) name (+ 1 nrr) (+ 1 arg))))
                                     (iter1 (cdr L) name s (+ 1 arg))))))
    (iter1 L name 0 1)))
  
;evaluate-cond, apeleaza evaluate-if pentru a determina valoarea recursivitatii pe acea conditie
;pastreaza maximul valorii gasite, la fel ca evaluate-if

(define (evaluate-cond L name)
    (let iter ((L L) (name name) (s-max 0))
      (if (null? L) s-max
              (iter (cdr L) name (max s-max (evaluate-if (cdr (car L)) name))))))

(define (detect-recursion f)
 (let iter ((name (get-function-name f)) (body (get-function-lambda f)) (nr_recursions 0))
   (if (null? body) 
       (cond ((= nr_recursions 1) 'TAIL-RECURSIVE)
             ((= nr_recursions 2) 'STACK-RECURSIVE)
             ((= nr_recursions 3) 'TREE-RECURSIVE))
       (cond ((equal? (car body) 'lambda) (iter name (ignore body) nr_recursions)) ;ignora lambda si parametrii
             ((= (exist? (flatten body) name) 0) 'NON-RECURSIVE)                   ;verifica de la inceput daca este nerecursiva
             ((list? (car body)) 
              ;verifica daca functia se reapeleaza in structuri de tipul if si cond, la fel si pentru structuri neconditionate
              ; daca se reapeleaza determina tipul recursivitatii 
             (if (equal? (caar body) 'if) 
                                     (iter name (cdr body) (max nr_recursions (evaluate-if (cdr (cdr (car body))) name))) 
             (if (equal? (caar body) 'cond)
                                         (iter name (cdr body) (max nr_recursions (evaluate-cond (cdr (car body)) name)))
                     (iter name (cdr body) (+ nr_recursions (evaluate-if body name))))))))))

  
  
