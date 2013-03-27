;; A unit test represents a way to test if a function
;; behaves the way we would like it to behave. To obtain this
;; we define a test to be a tuple of 4 elements 
;; - a string that describes what is the purpose of the test
;; - the expression that needs to be tested
;; - a function which receives as input a structure specifying the outcome of the expression evaluation
;;   and returns a structure specifying whether the test passed or not
;; - a score attributed to the given test

;; Construct which defines a test
(define-syntax make-test
  (lambda (x)
    (syntax-case x ()
      ((_ message expr check score)
       (syntax (list message (delay expr) check score))))))


;; Function which receives as input a suite of tests (which is actually a list of tests)
;; and which runs the tests - printing out the status of each test and the final score achieved
(define (run-test-suite suite)
  (let iter ((tests suite) (score 0))
    (if (null? tests)
        (printf "Final score: ~a/~a~n" score (foldl + 0 (map test-score suite)))
          (let* ((test (car tests))
                 (status (eval-test test)))
            (printf "Running test:[~a]: .......................... " (test-message test))
            (if (is-success? status)
                (begin
                  (printf "PASSED~n")
                  (iter (cdr tests) (+ score (test-score test))))
                (begin (printf "FAILED: ~a~n" (failure-value status))
                       (iter (cdr tests) score)))))))

;; Function which receives a test and returns a structure which indicates whether the test passed or failed
(define (eval-test test)
  (let ((result (with-handlers ([(lambda(x) #t) (lambda (exn) (make-failure exn))]) (make-success (force (test-delayed-expr test))))))
    ((test-check test) result)))

;; Accesor functions for test structure
(define test-message car)
(define test-delayed-expr cadr)
(define test-check caddr)
(define test-score cadddr)


;; Data structure for computations which may fail
(define (make-success x)
  (list #t x))

(define (make-failure x)
  (list #f x))

(define is-success? car)
(define (is-failure? v) (not (is-success? v)))

(define success-value cadr)
(define failure-value cadr)


;; some standard checks

(define (standard-check pred failure-message)
  (lambda (eval-result)
    (if (pred eval-result)
        (make-success (success-value eval-result))
        (make-failure failure-message))))
 
;; Checks that the evaluation process results in a result equal to the provided value
(define (evals-to v)
  (lambda (eval-result)
    (cond
      ((is-failure? eval-result) (make-failure (format "An unexpected error was thrown: \"~a\"" (exn-message (failure-value eval-result)))))
      ((equal? (success-value eval-result) v) (make-success (success-value eval-result)))
      (else (make-failure (format "Expected value [~a], instead the result was [~a]" v (success-value eval-result)))))))

;; Checks that the evaluation process results in a value that meets the given condition
(define (meets-condition pred condition-description)
  (lambda (eval-result)
    (cond
      ((not (is-success? eval-result)) (make-failure "An unexpected error was thrown"))
      ((pred (success-value eval-result)) (make-success (success-value eval-result)))
      (else (make-failure (format "The result [~a] did not meet the condition [~a]" (success-value eval-result) condition-description))))))
  
;; Checks whether the evaluation process throws an error
(define throws-error
  (standard-check is-failure? "An error was supposed to be thrown"))

;; Checks whether the evaluation process doesn;t throw an error
(define no-error-thrown (standard-check is-success? "No error was supposed to be thrown"))
  
