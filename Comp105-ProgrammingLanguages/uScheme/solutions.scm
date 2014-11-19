;Ian Cross

;---------------------------------------------------------
;#2 - SCHEME FUNCTIONS
;---------------------------------------------------------

;(a) - count 
;determine how many times a top level element in a list is equal to x
; 	example: (count '1 '(1 (1 2) 1)) = 2
(define count (x xs)
	(if (null? xs)
		0
	(if (equal? x (car xs))
		(+ 1 (count x (cdr xs)))
	(count x (cdr xs)))
	)
)

;(b) - countall
;returns number of times an atom x appears anywhere in xs (not just top level)
;	example: (countall '2 '(1 2 3 (1 2 (1 2)))) = 3
(define countall (x xs)
	(if (null? xs)
		0
	(if (atom? (car xs))
		(if (equal? x (car xs))
			(+ 1 (countall x (cdr xs)))
		(countall x (cdr xs)))
	(countall x (car xs))))
)

;(c) - mirror
;returns a list in which every list in xs recursively mirrored and 
;the resulting lists are in reverse order. (works with nested lists)
; 	example: (mirror '(1 2 (3 4 (5 6 (7 8))))) = ((((8 7) 6 5) 4 3) 2 1)
(define  mirror (xs)
	(if (null? xs)
		xs
	(if (atom? (car xs))
		(append (mirror (cdr xs)) (cons (car xs) '()))
	(cons (mirror (car xs)) '()))
	)
)

;(d) - flatten
;takes a 
(define  flatten (xs)
	(if (null? xs)
		xs
	(if (atom? (car xs))
		(cons (car xs)(flatten (cdr xs)))
	(mirror (car xs)))
	)
)

;(e) - contig sublist
;helper function for contig-sublist. from-beginning takes a list
; and determines if there is a contiguous list in it. This is necessary
; to determine if we get the following case '(1 2 3) '(1 2 4 1 2 3)
;Basically, it starts from the beginning of xs and returns once it is
;exhausted
(define from-beginning (xs ys)
	(if (null? xs)
		1
		(if (null? ys)
			0
			(if (equal? (car xs) (car ys))
				(from-beginning (cdr xs) (cdr ys))
			0)
		)
	)
)
;returns 1 if xs is a contiguous sublist in ys
;
(define contig-sublist (xs ys)
	(if (null? ys)
		0
		(if (= 1 (from-beginning xs ys))
		1
		(contig-sublist xs (cdr ys)))
	)
)


;(f)
(define sublist? (xs ys)
	(if (null? xs)
		1
		(if (null? ys)
			0
			(if (equal? (car xs) (car ys))
				(sublist? (cdr xs) (cdr ys))
			(sublist? xs (cdr ys)))
		)
	)
)


;------------------------------------------------------------
; #9 - MAP, CURRY, FOLDL, FOLDR
;---------------------------------------------------------

;a)takes the cdrs of each element of a list of lists
(define cdr* (xs)
	(map cdr xs))

;b) finds the max of a non-empty list of ints
(define max* (xs)
	(foldr max (car xs) xs))

;g)appends two lists
(define append (xs ys)
	(foldr cons ys xs))

;i)reverses a list
(define reverse (xs)
	(foldl cons '() xs))

;---------------------------------------------------------
;A) TAKE AND DROP
;used together, the exact semantics are as follows
; (append (take n xs) (drop n xs)) == xs
;---------------------------------------------------------


; -take
;expects a natural number and a list, returns the longest prefix of 
;xs that contains at most n elements.
(define take (n xs)
	(if (or (< n 2) (null? (cdr xs)))
		(cons (car xs) '())
	(cons (car xs) (take (- n 1) (cdr xs))))
)

; -drop
;expects a natural number and a list. it removes n elements from the 
;front of the list
(define drop (n xs)
	(if (or (< n 2) (null? (cdr xs)))
		(cdr xs)
	(drop (- n 1) (cdr xs)))
)

;-----------------------------------------------------------
;B) ZIP AND UNZIP
;used together, they satisfy this algebraic laws
;   (zip (car (unzip pairs)) (cadr (unzip pairs))) == pairs
;   (unzip ( zip xs ys))  ==  (list2 xs ys)
;---------------------------------------------------------


; -zip_map
; helper function for zip, does the actual mapping over one list
(define zip_map (xs)  
 	(if (null? (car xs))
 		'()
    (cons (map car xs) (zip_map (map cdr xs))))
)
; -zip
; returns an association list from a pair of lists
; it calls zip_map with a single list containing xs and ys
(define zip (xs ys)
	(zip_map (list2 xs ys))
)

; -unzip
;converts an association list to a pair of lists
(define unzip (xs)
	(list2 (map car xs) (map car (map cdr xs)))
)

;---------------------------------------------------------
;C) ARG MAX
;---------------------------------------------------------

; -max-val
; helper function for arg-max finds and returns the maximum 
; of a list after applying function f on each element
(define max_val (f xs)
	(if (null? (cdr xs))
		(f (car xs))
	(max (f (car xs)) (max_val f (cdr xs))))
)

; -arg-max
; returns the maximum arg that yields the biggest value after 
; applying a function to it
(define arg-max (f xs)
	(if (= (max_val f xs) (f (car xs)))
		(car xs)
	(arg-max f (cdr xs)))
)

;---------------------------------------------------------
;D MERGING SORTED LISTS
;---------------------------------------------------------

;checks whether the lists are null and if they aren't,
; it compares the values. helper function of merge
(define check_val (xs ys)
	(if (null? xs)
		(cons (car ys) (merge xs (cdr ys)))
		(if (null? ys)
			(cons (car xs) (merge (cdr xs) ys))
			(if (< (car xs) (car ys))
				(cons (car xs) (merge (cdr xs) ys))
			(cons (car ys) (merge xs (cdr ys))))
		)
	)
)
;merge
; returns a the merged sorted list of xs and ys
(define merge (xs ys)
	(if (and (null? xs) (null? ys))
		'()
		(check_val xs ys))
)

;---------------------------------------------------------
;E INTERLEAVING LISTS
;---------------------------------------------------------

;maps through one list and interleaves
(define interleave_map (xs)
	(if (null? (car xs))
		(car (cdr xs))
	(if (null? (car (cdr xs)))
		(car xs)
	(append (map car xs) (interleave_map (map cdr xs)))))	
)
; -interleave
;returns the interleaved list of xs and ys. calls interleave_map helper function
(define interleave (xs ys)
	(interleave_map (list2 xs ys))
)


