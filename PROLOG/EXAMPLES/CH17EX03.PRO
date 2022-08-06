/*
   Turbo Prolog 2.0 Chapter 17, Example Program 3
   
   Copyright (c) 1986, 88 by Borland International, Inc
   
*/


include "GRAPDECL.PRO"    % You may need to add : ..\\PROGRAMS\\

constants

   bgi_path = ""          % You may need to add :  ..\\BGI

predicates
   set_color(integer)

goal
   initgraph(detect, 0, GraphDriver, GraphMode, bgi_path),
   set_color(GraphDriver),
   rectangle(0, 0, 100, 100),
   bar3d(100, 10, 200, 100, 5, 1),
   bar(30, 30, 80, 80),
   drawpoly([50, 100, 100, 100, 150, 150, 50, 100]),
   fillpoly([50, 10, 90, 50, 50, 90, 10, 50]),
   readchar(_),
   closegraph.

clauses
   set_color(1):-
      !, setbkcolor(3).   % Set background color if CGA
   set_color(_):-
      setcolor(3).