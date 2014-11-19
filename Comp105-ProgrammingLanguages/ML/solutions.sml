(* 
solutions.sml 
Ian Cross 11/14/2013
*)

(* EXCEPTIONS *)
exception Not_Enough_Elements
exception Negative_N
exception Not_Found

(*--------------------------------------------------------------------- 
part A - compound
takes a binary operator, and integer N, and a value x, and it applies 
the operator to x N times 
--------------------------------------------------------------------- *)
fun compound binop 0 x = x
	| compound binop n x = binop (x, compound (op binop) (n-1) x);

(* exp: takes in x and y and returns x^y *)
fun exp x 0 = 1
	| exp x y = compound op* (y-1) x;

(*--------------------------------------------------------------------- 
part B - Fibbonacci 
takes in an integer and returns the nth fibonacci number
--------------------------------------------------------------------- *)
fun fib 0 = 0
	| fib 1 = 1
	| fib n = fib(n-1) + fib(n-2);

(*--------------------------------------------------------------------- 
part C - foldl and foldr 
--------------------------------------------------------------------- *)
fun rev list = foldl (op ::) [] list;
fun minlist [] = 0 
	| minlist (x::xs) = 
		foldl (fn (x,y) => if (x < y) then x else y) x (x::xs);

(*--------------------------------------------------------------------- 
part D - myfoldl and myfoldr 
--------------------------------------------------------------------- *)
fun myfoldr operator basis [] = basis
	| myfoldr operator basis (x::xs) = 
			operator (x, myfoldr operator basis xs);
fun myfoldl operator basis [] = basis
	| myfoldl operator basis (x::xs) = 
		myfoldl operator (operator (x, basis)) xs;

(*---------------------------------------------------------------------  
part E - Zip 
takes in 2 lists and produces a list of pairs
--------------------------------------------------------------------- *)
fun zip ([], ys) = []
	| zip (xs, []) = []
	| zip (x::xs, y::ys) = (x,y) :: zip (xs, ys);

(*--------------------------------------------------------------------- 
part F - Unzip 
takes a list of pairs and produces a pair of lists
--------------------------------------------------------------------- *)
fun unzip xs = (map (fn (x,y) => x) xs, map (fn (x,y) => y) xs);

(*--------------------------------------------------------------------- 
part G - flatten 
takes a list of lists and produces a single list containing all the 
elements in the correct order from "the second level"
--------------------------------------------------------------------- *)
fun flatten [] = []
	| flatten (x::xs) = x @ flatten xs;

(* --------------------------------------------------------------------- part H - exception 
mynth: takes in an integer and a list and returns nth element of the list
--------------------------------------------------------------------- *)
fun mynth n [] = raise Not_Enough_Elements
	| mynth 0 (x::xs) = x 
	| mynth n (x::xs) = if n<0 then raise Negative_N else mynth (n-1) xs;

(*--------------------------------------------------------------------- part I - implement an environment 
--------------------------------------------------------------------- *)
(*===== an 'a env is a list of string and 'a duples *)
type 'a env = (string * 'a) list
val emptyEnv = []: 'a env;

(*===== bind a name or key to a value in a provided env (aka cons) *)
fun bindVar (name:string, value, E:'a env) = (name,value)::E; 

(*===== given a name/key, it returns whether the name has been bound *)
fun isBound (name, []) = false
	| isBound (name, ((x:string,y)::xs)) = 
		if name=x then true else isBound (name, xs); 
(*===== given key and env, return the value the key is mapped to *)
fun lookup (name, []) = raise Not_Found
	| lookup (name, ((x:string,y)::xs))= 
		if name=x then y else lookup (name, xs);

(*--------------------------------------------------------------------- 
part J - extendEnv 
extends an environment with a list of mappings
---------------------------------------------------------------------*)
fun extendEnv (keys, values, E) =
	E @ zip (keys, values); 

(*---------------------------------------------------------------------  
part K - implement env as a function
--------------------------------------------------------------------- *)
(*===== this is the base case function, the element has not been found *)
fun emptyEnv2 key = raise Not_Found;

(*===== binds a value to a key. returns a function that returns the value 
of queried key *)
fun bindVar2 (key:string, value, x) = 
	(fn (query) => if key = query then value else x(query));

(*===== given a key, return whether that key is bound in the environment *)
fun isBound2 (key, x) = (x key;true) handle Not_Found => false; 

(*===== given a key, it returns the value mapped to that key or if it isn't
mapped an exception is raised *)
fun lookup2 (key, x) = x key;

(*===== extends an environment with a list of mappings *)
fun extendEnv2 ([],_,x) = x
	| extendEnv2 (_,[],x) = x
	| extendEnv2 (k::keys,v::values,x) = 
		extendEnv2(keys,values,bindVar2(k,v,x));

(*---------------------------------------------------------------------  
part L - sequence 
--------------------------------------------------------------------- *)
(*===== algebraic datatype for 'a seq *)
datatype 'a seq = SEQ of 'a seq * 'a seq
	| SINGLE of 'a 
	| EMPTY;

(*===== in constant time adds a single element to the front of a seq *)
fun scons (elem, sequence) = SEQ(SINGLE(elem),sequence);

(*===== in constant time adds a single element to the back of a seq *)
fun ssnoc (elem, sequence) = SEQ(sequence,SINGLE(elem));

(*===== in constant time, appends 2 sequences *)
fun sappend (seq1, seq2) = SEQ(SEQ(seq2),SEQ(seq1));

(*===== takes in 'a seq and returns a 'a list of thos same elements *)
fun listOfSeq EMPTY = []
	| listOfSeq (SINGLE(x)) = [x]
	| listOfSeq (SEQ(xs,ys)) = listOfSeq xs @ listOfSeq ys;

(*===== takes in 'a list and returns 'a seq of the same elements *)
fun seqOfList [] = EMPTY
	| seqOfList xs = foldr scons EMPTY xs;

(* --------------------------------------------------------------------
part M - alternative to linked lists 
A flist is a pair of a left list and a right list. The finger is the 
first element of the right list. The left list will be stored in reverse 
order which means the head of left list is the element to the left of the 
finger. Ex. [1,2,3,4,5] with finger at 4 would be represented by
([3,2,1],[4,5]. This way, everything at the finger is constant time 
--------------------------------------------------------------------- *)
type 'a flist = 'a list * 'a list;

(*====== returns a sequence containing x whose finger points at that x *)
fun singletonOf x:'a flist = ([],[x]);

(*====== which returns the value that the finger points at *)
fun atFinger (_,[]) = raise Not_Found
	| atFinger (_,y::ys)= y;

(*====== returns 'a flist in which the finger is moved one elem left*)
fun goLeft ([],_) = raise Subscript
	| goLeft (head::left,right) = (left,head::right); 

(*====== returns 'a flist in which the finger is moved one elem left*)
fun goRight (_,[]) = raise Subscript
	| goRight (left, head::right) = (head::left, right);

(*====== deletes the elem left of the finger and returns 'a flist *)
fun deleteLeft ([],_):'a flist  = raise Subscript
	| deleteLeft (head::left,right) = (left,right);

(*====== deletes the elem right of the finger and returns 'a flist *)
fun deleteRight (left, head::(to_delete::right)) = (left,head::right)
	|deleteRight (_,_):'a flist = raise Subscript;

(*====== inserts an elem x left of the finger and returns 'a flist *)
fun insertLeft (x,(left,right):'a flist) = (x::left, right);

(*====== inserts an elem x right of the finger and returns 'a flist *)
fun insertRight (x,(left,r::right):'a flist) = (left, r::(x::right))
	| insertRight (x, (left, [])) : 'a flist = (left, [x]); 

(*====== these are the traditional fold functions except modified to
work with 'a flist *)
fun ffoldl operator basis (xs,ys) = foldl operator basis (List.rev xs @ ys);
fun ffoldr operator basis (xs,ys) = foldr operator basis (List.rev xs @ ys);