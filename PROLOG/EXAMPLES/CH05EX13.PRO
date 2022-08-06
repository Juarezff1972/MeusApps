/*
   Turbo Prolog 2.0 Chapter 5, Example Program 13
   
   Copyright (c) 1986, 88 by Borland International, Inc
   
*/

predicates
   action(integer)

clauses
   action(1):-
     write("You type 1.").
   action(2):-
     write("You typed two.").
   action(3):-
     write("Three is what you typed.").
   action(X):-
     X<>1, X<>2, X<>3,
     write("I don't know that number!").

goal
   write("Type a number from 1 to 3: "),
   readreal(Choice),              /* readreal (integer) reads only an integer */
   action(Choice).                       /* binds Choice to the integer typed */