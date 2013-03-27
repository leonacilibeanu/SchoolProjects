(include "UnitTesting.scm")

(include "Tema1-recursion-detection.scm")
(include "Tema1-refactoring.scm")

(require net/url)

(define known-update-date #"25.03.2012")


;; RECURSION DETECTION =================================================================================================

;; Functions for recursion detection

(define-r (add x y)
  (+ x y))

(define-r (my-length L)
  (if (null? L)
      0
      (+ 1 (my-length (cdr L)))))

(define-r (my-length-t L acum)
  (if (null? L)
      acum
      (my-length-t (cdr L) (add1 acum))))

(define-r my-reverse
  (lambda (L)
    (cond
      ((null? L) L)
      (else (append (my-reverse (cdr L)) (list (car L)))))))

(define-r (fibonacci-t n new old)
  (if (= n 1)
      new
      (fibonacci-t (sub1 n) (+ new old) new)))

(define-r (my-foldl f acum L)
  (if (null? L)
      acum
      (my-foldl f (f (car L) acum) (cdr L))))

(define-r (my-foldr f acum L)
  (if (null? L)
      acum
      (f (car L) (my-foldr f acum (cdr L)))))

(define-r (my-filter p L)
  (cond
    ((null? L) L)
    ((p (car L)) (cons (car L) (my-filter p (cdr L))))
    (else (my-filter p (cdr L)))))

(define-r (filter-odd-elements L acum)
  (cond
    ((null? L) (reverse acum))
    (else (if (odd? (car L))
              (filter-odd-elements (cdr L) (cons (car L) acum))
              (filter-odd-elements (cdr L) acum)))))

(define-r (faulty-identity L)
  (cons (car L) (faulty-identity (cdr L)))) ;;will crash but we don't care.

(define-r (fibo n)
  (cond
    ((zero? n) 0)
    ((= n 1) 1)
    (else (+ (fibo (- n 1)) (fibo (- n 2))))))

(define-r (compute-expression ex)
  (if (number? ex)
      ex
      (if (list? ex)
          (+ (compute-expression (cadr ex)) (compute-expression (caddr ex)))
          (error "Unexpected value"))))

(define-r (weird-function x)
  (if x
      (and x (weird-function (weird-function x)))
      42))

(define recursion-detection-test-suite
  (list
   (make-test "Non recursive function" (detect-recursion add) (evals-to 'NON-RECURSIVE) 3)
   (make-test "Stack-recursive function" (detect-recursion my-length) (evals-to 'STACK-RECURSIVE) 3)
   (make-test "Tail-recursive function" (detect-recursion my-length-t) (evals-to 'TAIL-RECURSIVE) 3)
   (make-test "Stack Recursive list reverse" (detect-recursion my-reverse) (evals-to 'STACK-RECURSIVE) 3)
   (make-test "Tail Recursive Fibonacci" (detect-recursion fibonacci-t) (evals-to 'TAIL-RECURSIVE) 3)
   (make-test "Fold left" (detect-recursion my-foldl) (evals-to 'TAIL-RECURSIVE) 3)
   (make-test "Fold right" (detect-recursion my-foldr) (evals-to 'STACK-RECURSIVE) 3)
   (make-test "Stack recursive filter" (detect-recursion my-filter) (evals-to 'STACK-RECURSIVE) 3)
   (make-test "Tail recursive filter variant" (detect-recursion filter-odd-elements) (evals-to 'TAIL-RECURSIVE) 3)
   (make-test "Stack recursive with no conditional" (detect-recursion faulty-identity) (evals-to 'STACK-RECURSIVE) 3)
   (make-test "*BONUS* Fibonacci is tree recursive" (detect-recursion fibo) (evals-to 'TREE-RECURSIVE) 3)
   (make-test "*BONUS* Expression computation" (detect-recursion compute-expression) (evals-to 'TREE-RECURSIVE) 3)
   (make-test "*BONUS* Weird function is tree recursive" (detect-recursion weird-function) (evals-to 'TREE-RECURSIVE) 4)))

;; REFACTORING =====================================================================================================================

;; Refactoring rules

;; Removing boilerplate

(define redundant-lambda
  '((WHEN (lambda (?x) (?f ?x)))
    (THEN ?f)))

(define redundant-lambda-2
  '((WHEN (lambda (?x ?y) (?f ?x ?y)))
    (THEN ?f)))

(define redundant-if
  '((WHEN (if ?cond #t #f))
    (THEN ?cond)))

(define redundant-if-not
  '((WHEN (if ?cond #f #t))
    (THEN (not ?cond))))

(define use-cons-not-append
  '((WHEN (append (list ?x) ?L))
    (THEN (cons ?x ?L))))

(define use-cond-for-multibranch-if
  '((WHEN (if ?cond1 ?result1 (if ?cond2 ?result2 ?result3)))
    (THEN (cond (?cond1 ?result1) (?cond2 ?result2) (else ?result3)))))

;; quick optimizations

(define constant-folding-arithmetic-binary-case
  '((WHEN ((:conditional ?operator (lambda (op) (member op '(+ - * =))))
           (:conditional ?left-op number?)
           (:conditional ?right-op number?)))
    (THEN (:evaluation (?operator ?left-op ?right-op)))))

(define constant-folding-if-true
  '((WHEN (if #t ?then-branch ?else-branch))
    (THEN ?then-branch)))

(define constant-folding-if-false
  '((WHEN (if #f ?then-branch ?else-branch))
    (THEN ?else-branch)))

(define constant-folding-logical-binary-case
  '((WHEN ((:conditional ?operator (lambda (op) (member op '(and or))))
           (:conditional ?left-op boolean?)
           (:conditional ?right-op boolean?)))
    (THEN (:evaluation (?operator ?left-op ?right-op)))))

(define double-negation
  '((WHEN (not (not ?x)))
    (THEN ?x)))

(define redundant-multiplication-1
  '((WHEN (* 1 ?x))
    (THEN ?x)))

(define redundant-multiplication-2
  '((WHEN (* ?x 1))
    (THEN ?x)))

(define (quoted-expression? ex)
  (and (list? ex) (not (null? ex)) (equal? (car ex) 'quote)))

(define (quoted? predicate)
  (lambda (x)
    (and (quoted-expression? x)
         (predicate x))))

;; True if x evaluates to itself
(define (evaluation-invariant? x)
  (or (quoted-expression? x)
      (and (atom? x) (not (symbol? x)))))

(define (all-constant x)
  (or (evaluation-invariant? x)
      (and (list? x)
           (not (null? x))
           (member (car x) '(car cdr cons))
           (andmap all-constant (cdr x)))))

(define base-case-should-be-first
  '((WHEN (if ?cond
              (:conditional ?then-branch (compose not all-constant))
              (:conditional ?else-branch all-constant)))
    (THEN (if (not ?cond)
              ?else-branch
              ?then-branch))))
  
(define use-quotation-for-clarity
  '((WHEN (cons (:conditional ?c evaluation-invariant?) (:conditional ?l (quoted? list?))))
    (THEN (:evaluation (quasiquote (quote (unquote (cons (if (quoted-expression? ?c) (cadr ?c) ?c) ?l))))))))

;; Achieve DRY with higher-order functions

(define rewrite-as-map
  '((WHEN (lambda (?L) (if (null? ?L) '() (cons (:abstraction ?f ((car ?L) x)) (?this (cdr ?L))))))
    (THEN (lambda (?L) (map ?f ?L)))))

(define rewrite-as-filter
  '((WHEN (lambda (?L)
            (cond
              ((null? ?L) '())
              ((:abstraction ?predicate ((car L) x)) (cons (car ?L) (?this (cdr ?L))))
              (else (?this (cdr ?L))))))
    (THEN (lambda (?L) (filter ?predicate ?L)))))

(define rewrite-named-let-as-foldl
  '((WHEN (let ?iter ((?List ?Initial-List) (?acum ?initial-value))
            (if (null? ?List)
                ?acum
                (?iter (cdr ?List) (:abstraction ?f ((car ?List) x) (?acum acum))))))
    (THEN (foldl ?f ?initial-value ?Initial-List))))

(define rewrite-as-foldr
  '((WHEN (lambda (?L)
            (if (null? ?L)
                ?base-value
                (:abstraction ?f ((car ?L) x) ((?this (cdr ?L)) y)))))
    (THEN (lambda (?L) (foldr ?f ?base-value ?L)))))


;; Recursion morphing

(define use-the-Y-combinator
  '((WHEN (lambda (?x) (:abstraction ?lambda-ex (?this f))))
    (THEN (Y (lambda (f) (lambda (?x) (:evaluation (caddr (quote ?lambda-ex)))))))))

;; Refactoring packs ===================================================================================================

(define stage1-pack
  (list redundant-lambda
        redundant-lambda-2
        redundant-if
        redundant-if-not
        use-cons-not-append
        use-cond-for-multibranch-if))

(define stage2-pack
  (append stage1-pack
          (list constant-folding-arithmetic-binary-case
                redundant-multiplication-1
                redundant-multiplication-2
                constant-folding-if-true
                constant-folding-if-false
                constant-folding-logical-binary-case
                double-negation
                base-case-should-be-first
                use-quotation-for-clarity)))

(define stage3-pack
  (append stage2-pack
          (list rewrite-as-map
                rewrite-as-filter
                rewrite-named-let-as-foldl
                rewrite-as-foldr)))


;; Functions to refactor ===============================================================================================

(define-r (stack-factorial n)
  (if (zero? n)
      1
      (* n (stack-factorial (sub1 n)))))

(define-r (positive-odd? n)
  (if (and (odd? n) (positive? n)) #t #f))

(define-r (not-positive-odd? n)
  (if (and (odd? n) (positive? n)) #f #t))

(define-r (map-odd? L)
  (map (lambda (x) (if (odd? x) #t #f)) L))


(define-r (filter-odd-numbers L)
  (if (null? L)
      '()
      (if (odd? (car L))
          (append (list (car L)) (filter-odd-numbers (cdr L)))
          (filter-odd-numbers (cdr L)))))

(define-r (multiply-all L)
  (if (null? L)
      '()
      (append (list (* (+ 1 2) (* (- 7 6) (car L)))) (multiply-all (cdr L)))))


(define-r (the-answer-is-42)
  (if (= (+ 74 1) (- 76 1))
      (if (and (= 6 (* 3 2)) (= (- 200000000000000000000000 (+ 200000000000000000000000 2))  1))
          (cons 10001 '(1 2 3 1000))
          (if (or (and #t (= 1 (* 1 1))) (and #f #t))
              (- (+ 1 (* 42 1)) 1)
              (cons cons (cons cons (cons cons (cons cons (cons cons '())))))))
      'SOME-SYMBOL))

(define-r (flipped-map f L)
  (if (not (null? L))
      (cons (f (car L)) (flipped-map f (cdr L)))
      '()))

(define-r (create-a-funny-list)
  (if (not (not (and #t (= 3 3))))
      (cons 'a (cons 3 (cons 4 (cons 2.3 (cons #f (cons 'symbol (cons '(x1 x2) (cons 4 '(the end)))))))))
      'something-on-the-else-branch))

(define-r (get-the-sum-of-the-reversed-list X)
  (let iter ((L (let iter2 ((l X) (rev '()))
                  (if (null? l)
                      rev
                      (iter2 (cdr l) (cons (car l) rev)))))
             (s 0))
    (if (null? L)
        s
        (iter (cdr L) (+ (car L) s)))))

(define-r (list-to-tree L)
  (if (null? L)
      empty-tree
      (insert-in-tree (car L) (list-to-tree (cdr L)))))

(define-r (sum-matrix-lines LL)
  (if (null? LL)
      '()
      (cons (let sum-list ((L (car LL)) (result 0))
              (if (null? L)
                  result
                  (sum-list (cdr L) (+ (car L) result))))
            (sum-matrix-lines (cdr LL)))))

;; Tests ===============================================================================================================

;; Stage 1 (20p)

(define stage1-test-suite
  (list
   (make-test "remove append and rewrite with cond in filter-odd-numbers"
              (refactor-code filter-odd-numbers stage1-pack)
              (evals-to '(lambda (L) (cond
                                       ((null? L) '())
                                       ((odd? (car L)) (cons (car L) (filter-odd-numbers (cdr L))))
                                       (else (filter-odd-numbers (cdr L))))))
              5)
   
   (make-test "remove redundant if from positive-odd?"
              (refactor-code positive-odd? stage1-pack)
              (evals-to '(lambda (n) (and (odd? n) (positive? n))))
              5)
   
   (make-test "remove redundant if from not-positive-odd?"
              (refactor-code not-positive-odd? stage1-pack)
              (evals-to '(lambda (n) (not (and (odd? n) (positive? n)))))
              5)
   
   (make-test "remove redundant lambda and if from map-odd?"
              (refactor-code map-odd? stage1-pack)
              (evals-to '(lambda (L) (map odd? L)))
              5)))

;; Stage 2 (20p)

(define stage2-test-suite
  (list
   (make-test "massive constant folding"
              (refactor-code the-answer-is-42 (remove use-cond-for-multibranch-if stage2-pack))
              (evals-to '(lambda () 42))
              5)
   (make-test "constant folding in lambda and append removing"
              (refactor-code multiply-all stage2-pack)
              (evals-to '(lambda (L) (if (null? L)
                                         '()
                                         (cons (* 3 (car L)) (multiply-all (cdr L))))))
              5)
   (make-test "unflip de flipped map"
              (refactor-code flipped-map stage2-pack)
              (evals-to '(lambda (f L) (if (null? L) '() (cons (f (car L)) (flipped-map f (cdr L))))))
              5)
   (make-test "use quoted list instead of repeated cons"
              (refactor-code create-a-funny-list stage2-pack)
              (evals-to '(lambda () '(a 3 4 2.3 #f symbol (x1 x2) 4 the end)))
              5)))

;; Stage 3 (30p)

(define stage3-test-suite
  (list
   (make-test "rewrite as map"
              (refactor-code multiply-all stage3-pack)
              (evals-to '(lambda (L) (map (lambda (x) (* 3 x)) L)))
              5)
   (make-test "rewrite as filter"
              (refactor-code filter-odd-numbers stage3-pack)
              (evals-to '(lambda (L) (filter odd? L)))
              5)
   (make-test "fold and fold again"
              (refactor-code get-the-sum-of-the-reversed-list stage3-pack)
              (evals-to '(lambda (X) (foldl + 0 (foldl cons '() X))))
              5)
   (make-test "rewrite as foldr"
              (refactor-code list-to-tree stage3-pack)
              (evals-to '(lambda (L) (foldr insert-in-tree empty-tree L)))
              5)
   (make-test "rewrite foldl and map"
              (refactor-code sum-matrix-lines (remove rewrite-as-foldr stage3-pack))
              (evals-to '(lambda (LL) (map (lambda (x) (foldl + 0 x)) LL)))
              5)
   (make-test "recursion with the Y combinator"
              (refactor-code stack-factorial (list use-the-Y-combinator))
              (evals-to '(Y (lambda (f) (lambda (n) (if (zero? n) 1 (* n (f (sub1 n))))))))
              5)))
              
                           
(define refactoring-test-suite
  (append stage1-test-suite stage2-test-suite stage3-test-suite))
   
(define complete-test-suite
  (append recursion-detection-test-suite refactoring-test-suite))

(run-test-suite complete-test-suite)

;; Version testing =====================================================================================================

(define (check-if-last-version)
  (let ((last-version (get-last-modification)))
    (cond
      ((not last-version) (printf "~nCould not obtain latest tester version~n"))
      ((not (equal? last-version known-update-date))
       (printf "~nWARNING! The tester is not up to date. Your version is [~a] while the last version is [~a]~n"
               known-update-date
               last-version)))))
               

(define (get-last-modification)
  (let ((adr "https://docs.google.com/document/pub?id=1NJ8PVba4BHVKtNEedx6LqnBReqt8vtMO04FKraBv_A8&amp;embedded=true"))
    (let ((m (regexp-match 
              #px"Versiune tester:.*\\[(.*)\\]"
              (get-pure-port (string->url adr)))))
      (if m
          (cadr m)
          #f))))

(check-if-last-version)

