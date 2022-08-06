/*
   Turbo Prolog 2.0 Chapter 17, Example Program 2
   
   Copyright (c) 1986, 88 by Borland International, Inc
   
*/


include "GRAPDECL.PRO"      % You may need to add : ..\\PROGRAMS\\

constants
   bgi_path = ""            % You may need to add : ..\\BGI

predicates
   set_color(integer)

goal
   initgraph(detect, 0, GraphDriver,GraphMode, bgi_path),
   set_color(GraphDriver),

   /* Draw a 90 degree arc with radius of 50 */
   arc(150, 150, 0, 89, 50),

   /* Get the coordinates of the arc and connect ends */
   getarccoords(X,Y,XStart,YStart,XEnd,YEnd),
   line(XStart, YStart, XEnd, YEnd),

   /* Draw a circle */
   circle(150, 150, 100),

   /* Draw an ellipse inside the circle */
   ellipse(150, 150, 0, 359, 100, 50),

   /* Draw and fill a pieslice */
   pieslice(100, 100, 0, 135, 49),
   pieslice(100, 100, 135, 225, 49),
   pieslice(100, 100, 225, 360, 49),
   readchar(_),
   closegraph.

clauses
   set_color(1):-
      !, setbkcolor(3).  % Set background color if CGA
   set_color(_):-
      setcolor(3).