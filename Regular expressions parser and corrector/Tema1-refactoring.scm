(include "Reflection.scm")

(define (refactor-code fun refactoring-pack)
  "Do your magic here")



;; Misc

;; Testeaza daca un simbol incepe cu un anumit caracter
;; Ex (symbol-starts-with? #\? '?variable) => #t
(define (symbol-starts-with? first-char x)
    (and (symbol? x)
         (let ((s (symbol->string x)))
           (and (>= (string-length s) 1)
                (equal? first-char (string-ref s 0))))))


;; Testeaza ca argumentul este o entitate primitiva (care nu mai poate fi descompusa)
(define (atom? x)
  (or (symbol? x)
      (number? x)
      (char? x)
      (string? x)
      (boolean? x)))
