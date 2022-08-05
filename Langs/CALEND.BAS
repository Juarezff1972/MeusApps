begin
   write('Qual o ano (aaaa)');
   readln(Y);
   clrscr;
   M$:='jan31fev28mar31abr30mai31jun30jul31ago31set30out31nov30dez31'
   Z:=Y-1;
   A%:=Z/100;
   P:=(1721427!-A%+INT(A%/4)+INT(265.25*Z))/7
   C%:=7*(P-INT(P))+.5
   FOR M:=1 TO 12;
   V:=5
   gotoxy( 1,17);
   COLOR 0,7;
   write MID$(M$,5*M-4,3);'/';Y
   COLOR 7,0
   writeln;
   gotoxy( 3,12);
   write'D  S  T  Q  Q  S  S'
   FOR D:=1 TO VAL(MID$(M$,5*M-1,2))+((-(M=2))AND(-(Y:=400*INT(Y/400)))OR(-(Y=4*INT(Y/4)))AND(-(Y<>100*INT(Y/100))))
   C%:=C%+1-7*(-(C%=7));
   V:=V+2*(-(C%=1))
   gotoxy( V,3*C%+8);
   writeln(SPC(-(D<10)-1),D);
   NEXT
   gotoxy( 23,11);
   write'Aperte qualquer tecla ';
   repeat until keypressed;
   clrscr;
   NEXT
end.