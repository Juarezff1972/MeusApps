/*
   Turbo Prolog 2.0 Chapter 19, Example Program 5
   
   Copyright (c) 1986, 88 by Borland International, Inc
   
*/
domains
   tree = reference t(val, tree, tree)
   val  = string

predicates
   insert(val, tree)

clauses
   insert(ID, t(ID, _, _)):- !.
   insert(ID, t(ID1, Tree,_)):- ID < ID1, !, insert(Val, Tree).
   insert(ID, t(_, _, Tree)):- insert(Val, Tree).