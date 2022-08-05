unit zzz111;
interface
uses crt,dos,drivers;
var _dest,_orig: word;
	t : array[1..4096] of byte;
	fundo                      : byte; { Plano de fundo }
	fundomenu,frentemenu       : byte; { Menu principal }
	menutitulo                 : byte; { Titulo do menu }
	fundomenusel,frentemenusel : byte; { Menu principal - selecionado }
	fundobarra,frentebarra     : byte; { Barra superior e inferior }
	SombraAtrib                : word; { Cor da sombra }

	regs       : registers;   event        : tevent;
	segm       : pointer;     tela         : array[1..2] of word;

Procedure MovePagina;
Procedure Retangulo(x1,y1,x2,y2:byte);
Procedure Sombra(x1,y1,x2,y2:byte;sombraatrib:word);
Procedure Cursor(o:boolean;cursoroff:word);
Procedure Cores(cor_fundo,cor_texto:byte);
Function  Caracter(x,y : byte) : char;
Procedure Relogio;
Procedure Espera(tempo : word);
Procedure EscreveBarra(x,y : byte; texto : string; fundo,frente : byte);
Procedure Trata_char(x,y:byte;var nome:string;tam : byte);
Procedure LerTabelaASCII;
Procedure Sair(texto:string;var saida1,saida2:byte);
Procedure Sobre;
{-------------------------------------------------------------------------}
implementation
{-------------------------------------------------------------------------}
Procedure MovePagina;
Begin
   InLine($b9/$80/$3e/               { mov cx,3E80  }
          $33/$ff/                   { xor di,di    }
          $8b/$16/>_Dest/            { mov dx,_Dest }
          $8e/$c2/                   { mov es,dx    }
          $33/$f6/                   { xor si,si    }
          $8b/$16/>_Orig/            { mov dx,_Orig }
          $1e/                       { push ds      }
          $8e/$da/                   { mov ds,dx    }
          $d1/$e9/                   { shr cx,1     }
          $f3/$a5/                   { rep movsw    }
          $1f);                      { pop ds       }
End;
{---------------------------------------------------------------------------}
procedure retangulo;
var x,y : integer;
begin
   window(x1,y1,x2,y2);
   gotoxy(1,1);             writeln('É');
   gotoxy(x2-x1,y2-y1);     writeln('¼');
   gotoxy(x2-x1,1);         writeln('»');
   gotoxy(1,y2-y1);         writeln('È');
   for y:=2 to y2-(y1+1) do
   begin
      gotoxy(1,y);         writeln('º');
		gotoxy(x2-x1,y);     writeln('³');
	end;
	for x:=2 to x2-(x1+1) do
	begin
		gotoxy(x,1);         writeln('Í');
		gotoxy(x,y2-y1);     writeln('Ä');
   end;
   window(x1+1,y1+1,x2-2,y2-2);
   clrscr;
   window(1,1,80,25);
end;
{---------------------------------------------------------------------------}
procedure sombra;
var x,y : integer;
    z,t : byte;
begin
   for x:=x1 to x2-3 do
   begin
		gotoxy(x,y2);
      regs.ah:=8;
      intr($10,regs);
      z:=regs.ah;
      t:=regs.al;
      if (z=sombraatrib)or(z=0) then z:=0 else z:=sombraatrib;
      if t=219 then z:=0;
      regs.cx:=1;
      regs.bx:=z;
      regs.ah:=9;
      intr($10,regs);
   end;
   for x:=x1-2 to x1-1 do
      for y:=y1+1 to y2 do
      begin
         gotoxy(x,y);
         regs.ah:=8;
         intr($10,regs);
         z:=regs.ah;
         t:=regs.al;
         if (z=sombraatrib)or(z=0) then z:=0 else z:=sombraatrib;
         if t=219 then z:=0;
         regs.cx:=1;
         regs.bx:=z;
         regs.ah:=9;
         intr($10,regs);
      end;
end;
{---------------------------------------------------------------------------}
Procedure Cursor;
begin
   if o=true then
   begin
      regs.ax:=$168;
      regs.cx:=cursoroff; { Desativa cursor }
      intr($10,regs);
   end;
   if o=false then
   begin
      regs.ax:=$168;
      regs.cx:=$607;      { Ativa cursor }
      intr($10,regs);
   end;
end;
{---------------------------------------------------------------------------}
procedure cores;
begin
  textattr:=cor_fundo*16+cor_texto;
end;
{---------------------------------------------------------------------------}
Function Caracter;
begin
	gotoxy(x,y);
	regs.ah:=8;
	regs.bh:=0;
	intr($10,regs);
	caracter:=chr(regs.al);
end;
{-------------------------------------------------------------------------}
Procedure Relogio;
var h,m,s,c : word;
	 x,y     : word;
	 texto   : string[5];
	 aux     : string[2];
	 v,u,w   : byte;
begin
	x:=wherex;y:=wherey;
	texto:='';
	gettime(h,m,s,c);
	str(h,aux);	if h<10 then texto:='0'+aux else texto:=aux;
	if (s div 2)=(s/2) then texto:=texto+':' else texto:=texto+' ';
	str(m,aux);	if m<10 then texto:=texto+'0'+aux else texto:=texto+aux;
	for s:=1 to 5 do
	begin
		v:=73+lo(s);
		u:=ord(texto[s]);
		w:=fundobarra*16+frentebarra;
		asm
			mov ah,2;mov bh,0;mov dh,0;mov dl,v;int $10
			mov ah,9;mov al,u;xor bh,bh;mov bl,w;mov cx,1;int $10
		end;
	end;
	gotoxy(x,y);
end;
{-------------------------------------------------------------------------}
Procedure Espera;
var c : char;
	 event : tevent;
begin
	showmouse;
	while keypressed do c:=readkey;
	repeat
		relogio;
		dec(tempo);
		getmouseevent(event);
	until (tempo=0)or(keypressed)or(event.what=evmousedown);
	while keypressed do c:=readkey;
	hidemouse;
end;
{-------------------------------------------------------------------------}
Procedure EscreveBarra;
var i : byte;
	 spc : string;
begin
	spc:='';
	for i:=1 to ((80-length(texto))div 2) do
	begin
		spc:=' '+spc;
	end;
	texto:=spc+texto+spc;
	delete(texto,length(texto),1);
	cores(fundo,frente);
	gotoxy(x,y);
	write(texto);
	gotoxy(x,length(texto)+1);
	regs.ah:=$9;
	regs.al:=32;
	regs.cx:=1;
	regs.dl:=x-1;
	regs.dh:=y-1;
	regs.bh:=0;
	regs.bl:=fundo*16+frente;
	intr($10,regs);
end;
{-------------------------------------------------------------------------}
Procedure Trata_char;
var b : char;
	 event : tevent;
	 h,m,s,ss : word;
	 c : byte;
begin
	nome:='';
	c:=95;
	gotoxy(x,y);
	write(chr(c));
	b:=#2;
	repeat
	  if keypressed then b:=readkey;
	  gettime(h,m,s,ss);
	  if (ss div 2)=(ss / 2) then c:=95 else c:=32;
	  gotoxy(x,y); write(nome+chr(c));
	  relogio;
	  getmouseevent(event);
	  if ((caracter(event.where.x+1,event.where.y+1)='Ã')or
		  (caracter(event.where.x+1,event.where.y+1)='Â')or
		  (caracter(event.where.x+1,event.where.y+1)='Á'))
		  and(event.buttons=mbleftbutton)then b:=#13;
	  cores(fundomenu,frentemenu);
	  if (b<>#13)and(b<>#27)and(b<>#8)and(b<>#2) then
	  begin
		  nome:=nome+b;
		  if length(nome)>=(tam) then delete(nome,length(nome),1);
		  b:=#2;
	  end;
	  if (b=#27) then
	  begin
		  nome:='';
		  gotoxy(x,y); write(' ':tam);
		  b:=#2;
	  end;
	  if b=#0 then
	  begin
		  b:=readkey;
		  b:=#8;
	  end;
	  if (b=#8) then
	  begin
		  delete(nome,length(nome),1);
		  gotoxy(x,y); write(' ':tam);
		  b:=#2;
	  end;
  until b=#13;
end;
{-------------------------------------------------------------------------}
Procedure LerTabelaASCII;
var regs : registers;
	 i,y  : word;
	 asci : file of byte;
	 carac: array[1..16] of string[8];
{------------------------------}
Procedure Desenha177;
var i : byte;
Begin
	t[2832]:=192;t[2833]:=192;t[2834]:=192;t[2835]:=192;t[2836]:=192;
	t[2837]:=192;t[2838]:=192;t[2839]:=192;t[2840]:=192;t[2841]:=192;
	t[2842]:=192;t[2843]:=192;t[2844]:=192;t[2845]:=192;t[2846]:=192;
	t[2847]:=192;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha179;
var i : byte;
Begin
	t[2864]:=7;t[2865]:=7;t[2866]:=7;t[2867]:=7;t[2868]:=7;t[2869]:=7;
	t[2870]:=7;t[2871]:=7;t[2872]:=7;t[2873]:=7;t[2874]:=7;t[2875]:=7;
	t[2876]:=7;t[2877]:=7;t[2878]:=7;t[2879]:=7;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha186;
var i : byte;
Begin
	t[2976]:=224;t[2977]:=224;t[2978]:=224;t[2979]:=224;t[2980]:=224;
	t[2981]:=224;t[2982]:=224;t[2983]:=224;t[2984]:=224;t[2985]:=224;
	t[2986]:=224;t[2987]:=224;t[2988]:=224;t[2989]:=224;t[2990]:=224;
	t[2991]:=224;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha187;
var i : byte;
Begin
	t[2992]:=255;t[2993]:=255;t[2994]:=7;t[2995]:=7;t[2996]:=7;t[2997]:=7;
	t[2998]:=7;t[2999]:=7;t[3000]:=7;t[3001]:=7;t[3002]:=7;t[3003]:=7;
	t[3004]:=7;t[3005]:=7;t[3006]:=7;t[3007]:=7;
   regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
   regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha176;
var i : byte;
Begin
	t[2816]:=3;t[2817]:=3;t[2818]:=3;t[2819]:=3;t[2820]:=3;t[2821]:=3;
	t[2822]:=3;t[2823]:=3;t[2824]:=3;t[2825]:=3;t[2826]:=3;t[2827]:=3;
	t[2828]:=3;t[2829]:=3;t[2830]:=3;t[2831]:=3;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha188;
var i : byte;
Begin
	t[3008]:=7;t[3009]:=7;t[3010]:=7;t[3011]:=7;t[3012]:=7;t[3013]:=7;
	t[3014]:=7;t[3015]:=7;t[3016]:=7;t[3017]:=7;t[3018]:=7;t[3019]:=7;
	t[3020]:=7;t[3021]:=7;t[3022]:=255;t[3023]:=255;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha192;
var i : byte;
Begin
	t[3072]:=255;t[3073]:=192;t[3074]:=193;t[3075]:=193;t[3076]:=195;
	t[3077]:=195;t[3078]:=199;t[3079]:=199;t[3080]:=207;t[3081]:=193;
	t[3082]:=193;t[3083]:=193;t[3084]:=193;t[3085]:=192;t[3086]:=192;
	t[3087]:=255;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha196;
var i : byte;
Begin
	t[3136]:=0;t[3137]:=0;t[3138]:=0;t[3139]:=0;t[3140]:=0;t[3141]:=0;
	t[3142]:=0;t[3143]:=0;t[3144]:=0;t[3145]:=0;t[3146]:=0;t[3147]:=0;
	t[3148]:=0;t[3149]:=0;t[3150]:=255;t[3151]:=255;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha197;
var i : byte;
Begin
	t[3152]:=223;t[3153]:=223;t[3154]:=223;t[3155]:=223;t[3156]:=223;
	t[3157]:=223;t[3158]:=223;t[3159]:=223;t[3160]:=223;t[3161]:=223;
	t[3162]:=223;t[3163]:=223;t[3164]:=223;t[3165]:=223;t[3166]:=223;
	t[3167]:=223;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha202;
var i : byte;
Begin
	t[3232]:=255;t[3233]:=192;t[3234]:=192;t[3235]:=193;t[3236]:=193;
	t[3237]:=193;t[3238]:=193;t[3239]:=207;t[3240]:=199;t[3241]:=199;
	t[3242]:=195;t[3243]:=195;t[3244]:=193;t[3245]:=193;t[3246]:=192;
	t[3247]:=255;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha205;
var i : byte;
Begin
	t[3280]:=255;t[3281]:=255;t[3282]:=0;t[3283]:=0;t[3284]:=0;t[3285]:=0;
	t[3286]:=0;t[3287]:=0;t[3288]:=0;t[3289]:=0;t[3290]:=0;t[3291]:=0;
	t[3292]:=0;t[3293]:=0;t[3294]:=0;t[3295]:=0;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha254;
var i : byte;
Begin
	t[4064]:=251;t[4065]:=251;t[4066]:=251;t[4067]:=251;t[4068]:=251;
	t[4069]:=251;t[4070]:=251;t[4071]:=251;t[4072]:=251;t[4073]:=251;
	t[4074]:=251;t[4075]:=251;t[4076]:=251;t[4077]:=251;t[4078]:=251;
	t[4079]:=251;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha30;
var i : byte;
Begin
	t[480]:=255;t[481]:=3;t[482]:=131;t[483]:=131;t[484]:=195;t[485]:=195;
	t[486]:=227;t[487]:=227;t[488]:=243;t[489]:=131;t[490]:=131;t[491]:=131;
	t[492]:=131;t[493]:=3;t[494]:=3;t[495]:=255;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha193;
var i : byte;
Begin
	t[3088]:=255;t[3089]:=255;t[3090]:=6;t[3091]:=6;t[3092]:=6;t[3093]:=6;
	t[3094]:=6;t[3095]:=6;t[3096]:=6;t[3097]:=6;t[3098]:=6;t[3099]:=6;
	t[3100]:=6;t[3101]:=6;t[3102]:=254;t[3103]:=0;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha194;
var i : byte;
Begin
	t[3104]:=255;t[3105]:=255;t[3106]:=0;t[3107]:=0;t[3108]:=0;t[3109]:=0;
	t[3110]:=0;t[3111]:=0;t[3112]:=255;t[3113]:=0;t[3114]:=0;t[3115]:=0;
	t[3116]:=0;t[3117]:=0;t[3118]:=255;t[3119]:=0;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha195;
var i : byte;
Begin
	t[3120]:=255;t[3121]:=255;t[3122]:=224;t[3123]:=224;t[3124]:=224;
	t[3125]:=224;t[3126]:=224;t[3127]:=224;t[3128]:=224;t[3129]:=224;
	t[3130]:=224;t[3131]:=224;t[3132]:=224;t[3133]:=224;t[3134]:=255;
	t[3135]:=224;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha200;
var i : byte;
Begin
	t[3200]:=224;t[3201]:=224;t[3202]:=224;t[3203]:=224;t[3204]:=224;
	t[3205]:=224;t[3206]:=224;t[3207]:=224;t[3208]:=224;t[3209]:=224;
	t[3210]:=224;t[3211]:=224;t[3212]:=224;t[3213]:=224;t[3214]:=255;
	t[3215]:=255;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha201;
var i : byte;
Begin
	t[3216]:=255;t[3217]:=255;t[3218]:=224;t[3219]:=224;t[3220]:=224;
	t[3221]:=224;t[3222]:=224;t[3223]:=224;t[3224]:=224;t[3225]:=224;
	t[3226]:=224;t[3227]:=224;t[3228]:=224;t[3229]:=224;t[3230]:=224;
	t[3231]:=224;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
Procedure Desenha31;
var i : byte;
Begin
	t[496]:=255;t[497]:=3;t[498]:=3;t[499]:=131;t[500]:=131;t[501]:=131;
	t[502]:=131;t[503]:=243;t[504]:=227;t[505]:=227;t[506]:=195;t[507]:=195;
	t[508]:=131;t[509]:=131;t[510]:=3;t[511]:=255;
	regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{------------------------------}
begin
	assign(asci,'CHAR850');
	reset(asci);
	regs.ah:=$11;regs.al:=$4;regs.bl:=0;intr($10,regs);
	regs.ah:=$11;regs.al:=$30;regs.bh:=6;intr($10,regs);
	for i:=0 to 255*16+15 do read(asci,t[i]);
	close(asci);
	Desenha187;Desenha176;Desenha177;Desenha179;Desenha186;Desenha188;
	Desenha192;Desenha196;Desenha197;Desenha202;Desenha205;Desenha254;
	Desenha30;Desenha31;Desenha193;Desenha194;Desenha195;Desenha201;
	Desenha200;
	regs.ah:=$11;regs.al:=$0;regs.bh:=16;regs.cx:=256;regs.dx:=0;
	regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
end;
{----------------------------------------------------------------}
Procedure Sair;
var esc : array[1..3] of string[6];
	 i,y : byte;
	 c   : char;
    itens : array[1..5] of string[80];
	 esco,ii: byte;
	 xC,yC : array[1..5] of byte;
	 event : tevent;
{---------------------------------}
Procedure Desenhar;
var y : byte;
begin
   cores(FundoMenu,FrenteMenu);
	retangulo(42,2,49,6);
	sombra(42,2,49,6,8);
	cores(FundoMenu,FrenteMenu);
	for y:=1 to 2 do
	begin
		gotoxy(43,2+y);
		write(esc[y]);
	end;
	cores(FundoMenuSel,FrenteMenuSel);
	gotoxy(43,2+i);
	write(esc[i]);
end;
{---------------------------------}
begin
	if length(texto)>79 then texto:=copy(texto,1,79)+#255;
	esco:=1;
	yC[esco]:=1;
	xC[esco]:=1;
	repeat
		itens[esco]:=copy(texto,1,pos(#255,texto)-1);
		delete(texto,1,pos(#255,texto));
		inc(esco);
		yC[esco]:=1;
		xC[esco]:=xC[esco-1]+length(itens[esco-1]);
	until length(texto)=0;
	dec(esco);
	while keypressed do c:=readkey;
	c:=#2;
	esc[1]:= ' Sim ';
	esc[2]:= ' NÆo ';
	i:=1;
	Desenhar;
	showmouse;
	repeat
      getmouseevent(event);
		for ii:=1 to esco do
		if (((event.where.x>=(xC[ii]))and(event.where.x<(xC[ii+1])))
		and (event.where.y=(yC[ii]-1)))and(event.buttons=mbleftbutton) then
		begin
			hidemouse;
			textcolor(FrenteBarra);textbackground(FundoBarra);
			gotoxy(1,1);for i:=1 to esco do write(itens[i]);
			textcolor(FrenteMenuSel);textbackground(FundoMenuSel);
			gotoxy(xC[ii],yC[ii]);write(itens[ii]);
			showmouse;
			saida1:=ii;saida2:=ii;
			asm
				mov ah,5; mov ch,0; mov cl,27; int $16
			end;
		end;
		if ((event.where.x>41)and(event.where.x<48))and
			((event.where.y>1)and(event.where.y<4))and
			(event.buttons=mbleftbutton) then
		begin
			hidemouse;
			cores(FundoMenu,FrenteMenu);
			for y:=1 to 2 do
			begin
				gotoxy(43,2+y);
				write(esc[y]);
			end;
			i:=event.where.y-1;
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(43,2+i);
			write(esc[i]);
			showmouse;
			if event.double then
			asm
				mov ah,5; mov ch,0; mov cl,13; int $16
			end;
		end;
		relogio;
		if keypressed then c:=readkey;
		if c=#75 then
		begin
			asm
				mov ah,5; mov ch,0; mov cl,27; int $16
				mov ah,5; mov ch,75; mov cl,0; int $16
				mov ah,5; mov ch,0; mov cl,13; int $16
			end;
			c:=#2;
		end;
		if c=#77 then
		begin
			asm
				mov ah,5; mov ch,0; mov cl,27; int $16
				mov ah,5; mov ch,77; mov cl,0; int $16
				mov ah,5; mov ch,0; mov cl,13; int $16
			end;
         c:=#2;
		end;
		if c=#72 then
		begin
			hidemouse;
			cores(FundoMenu,FrenteMenu);
			for y:=1 to 2 do
			begin
				gotoxy(43,2+y);
				write(esc[y]);
			end;
			dec(i);
			if i=0 then i:=2;
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(43,2+i);
			write(esc[i]);
			showmouse;
			c:=#2;
		end;
		if c=#80 then
		begin
			hidemouse;
			cores(FundoMenu,FrenteMenu);
			for y:=1 to 2 do
			begin
				gotoxy(43,2+y);
				write(esc[y]);
			end;
			inc(i);
			if i=3 then i:=1;
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(43,2+i);
			write(esc[i]);
			showmouse;
			c:=#2;
		end;
		if (c=#13)and(i=1) then
		begin
			Saida1:=0;
			Saida2:=0;
			c:=#27;
		end;
		if (c=#13)and(i=2) then
		begin
			Saida1:=8;
			Saida2:=8;
			c:=#27;
		end;
	until (c=#27);
	hidemouse;
end;
{----------------------------------------------------------------}
Procedure Sobre;
var c 	 : char;
	 i 	 : byte;
	 event : tevent;
	 go    : boolean;
begin
	hidemouse;
	go:=false;
	cores(fundomenu,frentemenu);
	Sombra(23,6,59,19,SombraAtrib);
	Retangulo(23,6,59,19);
	gotoxy(28,8); write('Super Menu - VersÆo Dedicada');
	gotoxy(33,10);write('Copyright ¸ 1994');
	gotoxy(30,12);write('Juarez Fioravanti Filho');
   gotoxy(32,14);write('Aderson Cleber Pifer');
	cores(FundoMenuSel,FrenteMenuSel);
	gotoxy(38,16);
	write('  Ok  ');
	cores(FundoMenu,Black);
	for i:=37 to 42 do begin gotoxy(i,17);write(chr(223));end;
	gotoxy(37,16);write(chr(220));
	ShowMouse;
	repeat
		if keypressed then c:=readkey;
		relogio;
		GetMouseEvent(Event);
		if ((event.where.x>=37)and(event.where.x<=43)
		 and(event.where.y=15)and(event.buttons=mbLeftButton)) then
		 begin
			c:=#13;
			go:=true;
		 end;
		 if c=#75 then
		begin
			asm
				mov ah,5; mov ch,0; mov cl,27; int $16
				mov ah,5; mov ch,75; mov cl,0; int $16
				mov ah,5; mov ch,0; mov cl,13; int $16
			end;
			c:=#2;
		end;
		if c=#77 then
		begin
			asm
				mov ah,5; mov ch,0; mov cl,27; int $16
				mov ah,5; mov ch,77; mov cl,0; int $16
				mov ah,5; mov ch,0; mov cl,13; int $16
			end;
			c:=#2;
		end;
	until (c=#13)or(c=#27);
	if c=#13 then
	begin
      HideMouse;
		cores(FundoMenuSel,FrenteMenuSel);
		gotoxy(37,16);
		write('  Ok  ');
		cores(FundoMenu,FrenteMenu);
		for i:=37 to 42 do begin gotoxy(i,17); write(chr(32)); end;
		gotoxy(43,16); write(' ');
		showmouse;
		if go then
		repeat
			GetMouseEvent(Event);
		until event.what=evmouseup;
	end;
end;
{----------------------------------------------------------------}
begin
end.
