Program Oscilation;

Uses Graph,Crt;

var  a,b,c,d,y,g,h,z,x,t : real;
     _dest,_orig         : word;
{-------------------------------------------------------------------}
Procedure MovePagina;
begin
   inline($b9/$80/$3e/               { mov cx,3E80  }
          $33/$ff/                   { xor di,di    }
          $8b/$16/>_dest/            { mov dx,_DEST }
          $8e/$c2/                   { mov es,dx    }
          $33/$f6/                   { xor si,si    }
          $8b/$16/>_orig/            { mov dx,_ORIG }
          $1e/                       { push ds      }
          $8e/$da/                   { mov ds,dx    }
          $d1/$e9/                   { shr cx,1     }
          $f3/$a5/                   { rep movsw    }
          $1f);                      { pop ds       }

end;
{-----------------------------------------------------------------------}
Procedure Inicio;
var Gd,Gm : integer;
begin
   Gd:=Detect;
	InitGraph(gd,gm,'');
	setgraphmode(0);
   ClearDevice;
end;
{-------------------------------------------------------------------}
Procedure Cria;
var Tam : Word;
    zip : integer;
begin
t:=-100;
Zip:=1;
repeat
   b:=-40;
   z:=-100;{*10,+100}
   Repeat
      a:=-40;
      x:=-100;  {*2,99.99}
      Repeat
{	       y:=t*exp(-x*x/1000-z*z/1000); }
{         y:=t*sin(x/100)*sin(z/100); }
         y:=(t/2)*(exp(-2*(x+z)/1000)*sin(2*pi*2*(x+z)/1000));
{         y:=t*((z*x*x*x/100)+(-x*z*z*z/100))/1000000; }
{         y:=t*exp(-0.04*(x*x/1000+z*z/1000))*cos(0.15*(x*x/1000+z*z/1000)); }
{         y:=t*((x*x/55000)+(z*z/55000)); }
{         y:=t*sqrt((x*x/55000)+(z*z/55000)); }
{         y:=(t/10)*((x*x*x/100000)+(z*z*z/100000)-(4*x/100000)-(4*z/100000)+(x*z/100000)); }
{         y:=t*((x/1000)+(z/1000)); }
{         y:=(t/5)*((z*x/10000)+(x*z/10000)); }
{         y:=t*((x/1000)-(z/1000)); }
{         y:=(t/2)*(((x+z)/100)*((x-z)/100)); }
{         y:=t*((x/100000)+(2*x*x/100000)); }
{         y:=t*((2*z/1000)-(x/1000)); }
{         y:=(t/2)*exp(sqrt((x*x/10000)+(z*z/10000)))*cos(sqrt((x*x/10000)+(z*z/10000))); }
{         y:=(t/200)*((3*x*x/1000)+(3*z*z/1000)+(8*x/1000)+(8*z/1000)+9); }
{         y:=(t/16)*(cos(2*((x-z)/50))+cos(2*((x-z)/50))); }
{         y:=(t/12)*((sin(2*x/100)+sin(5*x/100))+(sin(2*z/100)+sin(5*z/100))); }
{         y:=(t/12)*((cos(2*x/100)+cos(5*x/100))+(cos(2*z/100)+cos(5*z/100))); }
         d:=140+x+b;
         g:=140+a+z;
         c:=96-y-b;
         h:=96-y-a;
         PutPixel(Trunc(2*d),Trunc(c),3);
         PutPixel(Trunc(2*g),Trunc(h),3);
         a:=a+0.8;
         x:=x+2;
      until (x >= 99.99);
      b:=b+4;
      z:=z+10;
   until (z > 100);
t:=t+40;
_orig:=$a000;
_dest:=$8800+($400*(zip-1));
MovePagina;
zip:=zip+1;
cleardevice;
until (t>100);
end;
{-------------------------------------------------------------------}
Procedure Anima;
var frame : integer;
begin
   for frame:=2 to 6 do
   begin
      _orig:=$8800+($400*(frame-1));
		_dest:=$a000;
      MovePagina;
      delay(50);
   end;
   for frame:=5 downto 1 do
   begin
      _orig:=$8800+($400*(frame-1));
		_dest:=$a000;
      MovePagina;
      delay(50);
   end;
end;
{-------------------------------------------------------------------}
begin
   Inicio;
   Cria;
   Repeat
      Anima;
   Until KeyPressed;
   CloseGraph;
end.





