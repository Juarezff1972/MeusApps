program teste;

uses crt,janela;

var a : integer;

{-----------------------------------------------------------------------}
begin
   textcolor(7);
   textbackground(0);
   randomize;
   clrscr;
   for a:=1 to 2000 do
   begin
      textcolor(14);
      textbackground(8);
      write(chr(178));   { 176,177,178,219 }
   end;
   gotoxy(1,1);
   readln;
   textcolor(15);
   textbackground(2);
   sombra(10,10,40,15);
   retangulo(10,10,40,15,dupla);
   readln;
   textcolor(0);
   textbackground(15);
   sombra(20,7,60,12);
   retangulo(20,7,60,12,simples);
   readln;
   textcolor(14);
   textbackground(8);
   sombra(15,3,21,20);
   retangulo(15,3,21,20,asterisco);
   readln;
   textcolor(15);
   textbackground(12);
   sombra(5,5,30,10);
   retangulo(5,5,30,10,mais);
   readln;
   textcolor(0);
   textbackground(7);
   sombra(60,18,75,23);
   retangulo(60,18,75,23,numero);
   readln;
   textcolor(15);
   textbackground(10);
   sombra(55,10,70,19);
   retangulo(55,10,70,19,grossa);
   readln;
   textcolor(0);
   textbackground(6);
   sombra(50,8,65,17);
   retangulo(50,8,65,17,quadrado);
   gotoxy(1,1);
   readln;
end.