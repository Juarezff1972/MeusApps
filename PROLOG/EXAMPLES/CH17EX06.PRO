/*
   Turbo Prolog 2.0 Chapter 17, Example Program 6
   
   Copyright (c) 1986, 88 by Borland International, Inc
   
*/

include "GRAPDECL.PRO"    % You may need to add : ..\\PROGRAMS\\

constants
   bgi_path = ""          % You may need to add : ..\\BGI

predicates
   makeViewPort
   set_color(integer)

goal
   initGraph(detect, 0, New_Driver, New_Mode, bgi_path),
   set_color(New_Driver),
   makeViewPort,
   closegraph().

clauses
   makeViewPort:-
      detectGraph(_, G_Mode),
      getMaxX(MaxX),getMaxY(MaxY),
      setViewPort(0,0,MaxX,MaxY,1),
      clearViewPort,
      writef("Default Mode is % , resolution is % by %",
              G_Mode,MaxX,MaxY),nl,
      write("Press key"),readchar(_),nl,fail.

   makeViewPort:-
      detectGraph(G_Driver, _),
      getModeRange(G_Driver,Low,_),
      setGraphMode(Low),
      getMaxX(MaxX),getMaxY(MaxY),
      setViewPort(0,0,MaxX,MaxY,1),
      clearViewPort,
      writef("Default Mode is % , resolution is % by %",
              Low,MaxX,MaxY),nl,
      write("Press key"),readchar(_), nl, fail.

   set_color(1):-
      !, setbkcolor(3).   % Set background color if CGA
   set_color(_):-
      setcolor(3).