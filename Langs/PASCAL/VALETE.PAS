program Teste;

uses Graph,crt;

var gd,gm : integer;
    linha : array[1..18] of word;
    final : array[1..18] of word;
    i     : integer;

begin
   gd:=cga;
   gm:=cgahi;
   initgraph(gd,gm,'');
   linha[1] :=$ffff;    final[1] :=$1f;
   linha[2] :=$7fff;    final[2] :=$f;
   linha[3] :=$2048;    final[3] :=$2;
   linha[4] :=$1fff;    final[4] :=$c;
   linha[5] :=$1800;    final[5] :=$c;
   linha[6] :=$198c;    final[6] :=$c;
   linha[7] :=$1800;    final[7] :=$c;
   linha[8] :=$1800;    final[8] :=$c;
   linha[9] :=$181;     final[9] :=$c;
   linha[10]:=$1bfd;    final[10]:=$c;
   linha[11]:=$108;     final[11]:=$0;
   linha[12]:=$1f8c;    final[12]:=$c;
   linha[13]:=$1fff;    final[13]:=$c;
   linha[14]:=$1000;    final[14]:=$4;
   linha[15]:=$1000;    final[15]:=$4;
   linha[16]:=$1000;    final[16]:=$4;
   linha[17]:=$1fff;    final[17]:=$c;

   for i:=1 to 17 do
   begin
      setlinestyle(userbitln,linha[i],1);
      line(0,i,16,i);
      setlinestyle(userbitln,final[i],1);
      line(17,i,20,i);
   end;

   repeat until keypressed;

end.