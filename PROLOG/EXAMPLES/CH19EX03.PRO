/*
   Turbo Prolog 2.0 Chapter 19, Example Program 3
   
   Copyright (c) 1986, 88 by Borland International, Inc
   
*/
   
predicates
   plus(integer, integer, integer)
   num(integer)
   
clauses
  plus(X,Y,Z):- bound(X), bound(Y), Z = X + Y.                     /* (i,i,o) */
  plus(X,Y,Z):- bound(Y), bound(Z), X = Z - Y.                     /* (o,i,i) */
  plus(X,Y,Z):- bound(X), bound(Z), Y = Z - X.                     /* (i,o,i) */
  plus(X,Y,Z):- free(X), free(Y), bound(Z), num(X), Y = Z - X.     /* (o,o,i) */
  plus(X,Y,Z):- free(X), free(Z), bound(Y), num(X), Z = X + Y.     /* (o,i,o) */
  plus(X,Y,Z):- free(Y), free(Z), bound(X), num(Y), Z = X + Y.     /* (i,o,o) */
  plus(X,Y,Z):- free(X), free(Y), free(Z), num(X), num(Y), 
  		Z = X + Y.                                         /* (o,o,o) */

  /* Generator of numbers strting from 0 */
  num(0).
  num(X):- num(A), X = A + 1.
