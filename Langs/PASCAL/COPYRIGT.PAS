Procedure CopyRight;
var dono,dono1,z : string[40];
    serie        : integer;
    serie1       : string[10];
    dono2        : array[1..80] of byte;
    i            : integer;
begin
   serie:= 1991 ;
   dono:='Qualquer pessoa';
   dono1:='';
   dono2[ 1 ]:= 81 ;
   dono2[ 2 ]:= 117 ;
   dono2[ 3 ]:= 97 ;
   dono2[ 4 ]:= 108 ;
   dono2[ 5 ]:= 113 ;
   dono2[ 6 ]:= 117 ;
   dono2[ 7 ]:= 101 ;
   dono2[ 8 ]:= 114 ;
   dono2[ 9 ]:= 32 ;
   dono2[ 10 ]:= 112 ;
   dono2[ 11 ]:= 101 ;
   dono2[ 12 ]:= 115 ;
   dono2[ 13 ]:= 115 ;
   dono2[ 14 ]:= 111 ;
   dono2[ 15 ]:= 97 ;
   for i:=1 to length(dono) do
      dono1:=dono1+chr(dono2[i]);
   serie1:='1991';
   if (dono<>dono1) then
   begin
      writeln('C¢pia n„o autorizada! Pertence a ',dono1,'!');
      halt(1);
   end;
   str(serie,z);
   if (z<>serie1) then
   begin
      writeln('C¢pia n„o autorizada! N£mero de s‚rie: ',z);
      halt(2);
   end;
end;
{-------------------------------------------------------------}
