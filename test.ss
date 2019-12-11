(define (function1 V1 V2)
	(cond 
		((< V1 V2) (+ V1 V2))
		((> V1 V2) (- V1 V2))
		((= V1 V2) (* V1 V2))
		(else (display "Hello"))
	)
)

(define (test1 in)
	(if (< in 0)
		(if (= 4 4)
			(cond 
				((< 5 5) (+ 5 5))
				((> 5 5) (- 5 5))
				((= 5 5) (* 5 5))
				(else (display "Hello"))
			)
		)
		"positive"
	)
)

(define (test2 one two three four)
	(+ 7 (/ 32 2))
)

(define (main)
	(display (function1 3 5))
	(newline)
	(display (function1 13 5))
	(newline)
	(display (test1 1))
	(newline)
	(display (test1 -1))
)

(main)
