/*
   Turbo Prolog 2.0 Chapter 17, Example Program 1
   
   Copyright (c) 1986, 88 by Borland International, Inc
   
*/
include "GRAPDECL.PRO"  % You may need to add : ..\\PROGRAMS\\

constants
   bgi_path = ""        % You may need to add : ..\\BGI

predicates
   toText
   tempTextmode
   toGraphic
   set_color(integer)

goal
   toGraphic,
   tempTextmode,
   toText.

clauses
   toGraphic:-
      % Detect graphics hardware
      detectGraph(G_Driver, G_Mode),
      initGraph(G_Driver,G_Mode,_,_,bgi_path),
      set_color(G_Driver),
      outtextxy(10,10,"A look into the BGI"),
      format(Str,"Graph_Driver = %, Graph_Mode = %",G_Driver,G_Mode),
      outtextxy(10,30,Str),
      outtextxy(10,100,"Press any key to return to text mode: "),
      readchar(_).

  TempTextmode:-
      getGraphMode(GraphMode),
      restoreCRTMode,
      write("\nNow you are in text mode.\n\n"),
      write("\nPress any key to go back to graphics ..."),nl,
      readchar(_),
      setGraphMode(GraphMode).

   ToText:-
      detectgraph(Driver,_),
      set_color(Driver),
      outtextxy(50,100,"That's all!"),
      readchar(_),
      closegraph,
      exit.

   set_color(cga):-   % Is it a CGA?
      !,
      setbkcolor(3).
   set_color(_):-
      setcolor(3).