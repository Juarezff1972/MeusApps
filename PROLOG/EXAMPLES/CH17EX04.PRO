/*
   Turbo Prolog 2.0 Chapter 17, Example Program 4
   
   Copyright (c) 1986, 88 by Borland International, Inc
   
*/

include "GRAPDECL.PRO"   % You may need to add : ..\\PROGRAMS\\

constants
   bgi_path = ""         % You may need to add :  ..\\BGI
   gray50 = [$aa, $55, $aa, $55, $aa, $55, $aa, $55 ]

predicates
   set_color(integer)

goal
   initgraph(0, 0, GraphDriver, GraphMode, bgi_path),
   set_color(GraphDriver),
   getfillsettings(SavePat,SaveColor), 		/* retrieve current settings */
   getfillpattern(SavePatternList), 		/* save user fill pattern */
   setfillstyle(slash_FILL, blue), 		/* change fill style */
   bar(0, 0, 100, 100), 			/* draw slash-filled blue bar */
   setfillpattern(gray50, yellow), 		/* custom fill pattern */
   bar(100, 100, 200, 200), 			/* draw customized yellow bar */
   setfillpattern(SavePatternList,SaveColor),	/* restore user fill pattern */
   setfillstyle(SavePat, SaveColor),
   readchar(_),
   closegraph.

clauses
   set_color(1):-
      !, setbkcolor(3).  % Set background color if CGA
   set_color(_):-
      setcolor(3).