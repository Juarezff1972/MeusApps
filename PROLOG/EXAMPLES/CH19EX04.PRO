/*
   Turbo Prolog 2.0 Chapter 19, Example Program 4
   
   Copyright (c) 1986, 88 by Borland International, Inc
   
*/

domains
   refinteger = integer
   reflist = reference refinteger*

predicates
   member(refinteger, reflist)
   append(reflist, reflist, reflist)

clauses
   member(X, [X|_]).
   member(X,[_|L]):- member(X, L).
   
   append([], L, L).
   append([X|L1], L2, [X|L3]):- append(L1, L2, L3).
   