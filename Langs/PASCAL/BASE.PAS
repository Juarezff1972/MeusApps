uses dos,crt,math;

var a,b      : byte;
    c        : string;
    precisao : byte;
{------------------------------------------------------------------------}
Procedure TrocaBase(var b,bf : byte; var numb : string);
var Neg                  : boolean;
    i,XxA,l              : integer;
    OldNumber,NewNumber  : string;
    xa,n,OldBase,NewBase : EXTENDED;
begin
   OldNumber:=numb;
   OldBase:=B;
   NewBase:=bf;
   begin
      n:=0;
      xa:=0;
      XxA:=0;
      l:=0;
      NewNumber:='';
      Neg:=false;
      if copy(OldNumber,1,1)='-' then
      begin
         delete(OldNumber,1,1);
         Neg:=true;
      end;
      for i:=1 to length(OldNumber) do
      begin
         xa:=ord(OldNumber[i])-48;
         if xa>9 then xa:=xa-7;
         n:=n*OldBase+xa;
      end;
      n:=n+0.5;
      l:=trunc(ln(n)/ln(NewBase));
      for i:=l downto 0 do
      begin
         XxA:=trunc(n/exp(i*ln(NewBase)));
         n:=n-XxA*exp(i*ln(NewBase));
         if XxA>9 then NewNumber:=NewNumber+chr(48+XxA+7);
         if XxA<=9 then NewNumber:=NewNumber+chr(48+XxA);
      end;
      if Neg then NewNumber:='-'+NewNumber;
      if OldNumber='' then NewNumber:='';
      if OldNumber='0' then NewNumber:='0';
   end;
   numb:=NewNumber;
end;
{------------------------------------------------------------------------}
Procedure Dec(var n : string; var base : byte; var res : string);
var intg,fract : string;
    x          : real;
    i          : integer;
    neg        : boolean;
    y,z,w      : byte;
    t          : string;
begin
   val(n,x,i);
   neg:=false;
   if x<0 then
   begin
      x:=-x;
      neg:=true;
   end;
   str(trunc(x),intg);
   y:=10;
   TrocaBase(y,Base,Intg);
   x:=frac(x);
   IF BASE=16 THEN
   BEGIN
      str(x:1:PRECISAO,fract);
      VAL(FRACT,X,I);
   END;
   fract:='';
   REPEAT
      x:=x*base;
      str(trunc(x),t);
      x:=frac(x);
      TrocaBase(y,base,t);
      fract:=fract+t;
   UNTIL (X=0.75)OR(X=0);
   IF BASE=2 THEN FRACT:=COPY(FRACT,1,16);
   IF BASE=8 THEN FRACT:=COPY(FRACT,1,8);
   IF BASE=16 THEN FRACT:=COPY(FRACT,1,4);
   res:=intg+'.'+fract;
   if neg then res:='-'+res;
end;
{------------------------------------------------------------------------}
Procedure DecInvert(var n: string;var base:byte;var res:string);
var intg,fract : string;
    x          : real;
    i          : integer;
    neg        : boolean;
    y,z        : byte;
begin
   y:=10;
   intg:=copy(n,1,pos('.',n)-1);
   fract:=copy(n,1+pos('.',n),length(n)-pos('.',n));
   TrocaBase(base,y,intg);
   z:=length(fract);
   TrocaBase(base,y,fract);
   PRECISAO:=6;
   val(fract,x,i);
   x:=x/pot(base,z);
   str(x:1:precisao,fract);
   delete(fract,1,2);
   IF BASE=10 THEN FRACT:=COPY(FRACT,1,6);
   res:=intg+'.'+fract;
end;
{------------------------------------------------------------------------}
begin
   clrscr;
   repeat
      write('Entre com a base inicial: '); readln(b);
      if b=0 then exit;
      write('Entre com o numero:       '); readln(c);
      writeln;
      write('Base 10: ');
      DecInvert(c,b,c);
      writeln(c);
      b:=2;
      write('Base 2:  ');
      Dec(c,b,c);
      writeln(c);
      DecInvert(c,b,c);
      b:=8;
      write('Base 8:  ');
      Dec(c,b,c);
      writeln(c);
      DecInvert(c,b,c);
      write('Base 10: ');
      writeln(c);
      b:=16;
      write('Base 16: ');
      Dec(c,b,c);
      writeln(c);
      writeln;
      writeln;
   until false;
end.
           
