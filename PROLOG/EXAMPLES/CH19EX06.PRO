/*
   Turbo Prolog 2.0 Chapter 19, Example Program 6
   
   Copyright (c) 1986, 88 by Borland International, Inc
   
*/
domains
   tree = reference t(val, tree, tree)
   val  = integer
   list = integer*

predicates
   insert(integer, tree)
   instree(list, tree)
   treemembers(tree, list, list)
   sort(list, list)
   append(list, list, list)

clauses
   insert(Val, t(Val, _, _)):- !.
   insert(Val, t(Val1, Tree,_)):- Val < Val1, !, insert(Val, Tree).
   insert(Val, t(_, _, Tree)):- insert(Val, Tree).

   instree([], _).
   instree([H|T], Tree):-
      insert(H,Tree),
      instree(T, Tree).

   treemembers(T, NewX, NewX):- free(T).
   treemembers(t(Cur_Elem, Lt, Rt), X, NewX):- 
      free(Lt), 
      free(Rt), 
      append(X, [Cur_Elem], NewX), !.
   treemembers(t(Cur_Elem, Lt, Rt), X, NewX):- 
      treemembers(Lt, X, NextX),
      append(NextX, [Cur_Elem], NextX2),
      treemembers(Rt, NextX2, Newx).

   sort(L, L1):-
      instree(L, Tree),
      treemembers(Tree, [], L1).

   append([], L2, L2).
   append([X|L1], L2, [X|L3]):-
      append(L1, L2, L3).

goal
   sort([3, 6, 4, 5], L),
   write(L),nl.
   