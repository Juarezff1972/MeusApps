{$A+,B-,D+,E+,F-,G+,I-,L+,N-,O-,P-,Q-,R-,S-,T-,V-,X+}
{$M 16384,0,655360}
Uses Crt,Dos,SpecMem;

const
  IWAD_SIG = Ord('I')+(Ord('W')+(Ord('A')+Ord('D') shl 8) shl 8) shl 8;
  PWAD_SIG = Ord('P')+(Ord('W')+(Ord('A')+Ord('D') shl 8) shl 8) shl 8;
  DOOM_WAD = 'DOOM.WAD';
  DOOM2_WAD = 'DOOM2.WAD';
  HERETIC_WAD = 'HERETIC.WAD';
  PNAMES = 'PNAMES'#0#0;
  TEXTURE1 = 'TEXTURE1';
  TEXTURE2 = 'TEXTURE2';
  PLAYPAL = 'PLAYPAL'#0;
  P_START = 'P_START'#0;
  P_END   = 'P_END'#0#0#0;
  P1_START= 'P1_START';
  P1_END  = 'P1_END'#0#0;
  F_START = 'F_START'#0;
  F_END   = 'F_END'#0#0#0;
  F1_START= 'F1_START';
  F1_END  = 'F1_END'#0#0;
  OK = '[Ok]';

  DUMMY_TEXTURE : array[1..20] of word = (1,0,12,0,95,0,0,0,0,0,64,64,0,0,1,0,0,0,0,0);

  heretic_mode : boolean = False;
  BUFFSIZE = 65528;

type
  errors = (ERR_NONE,ERR_USER_ESCAPE,ERR_NO_MEM,ERR_OPEN,ERR_READ,
            ERR_WRITE,ERR_NOWAD,ERR_NOTEX,ERR_NOPALETTE,ERR_USER);
  header= record    {header of a wadfile}
    Sig   : Longint;  {signature}
    Num   : Longint;  {numbers of resources}
    Start : Longint;  {offset of dirlist}
  end;
  char8 = array[1..8] of Char;
  p_entry=^entry;
  entry = record    {each single entry in the dirlist}
    Start : Longint;  {offset of resource}
    case integer of
      1: (Size  : Longint;  {length in bytes}
          Name  : char8;    {resource's name});
      2: (dummy : array[1..3] of byte;
          fnum  : byte;     {file number});
  end;
  p_txinfo = ^txinfo;
  txinfo = record   {texture info}
    Name : char8;     {name of the texture}
    dummy: array[1..6] of word;
    Num  : integer;   {number of patches}
  end;
  p_ptinfo = ^ptinfo;
  ptinfo = record   {patch info}
    dummy: longint;
    index: word;      {index of patch name inside PNAMES}
    dumm2: longint;
  end;
  entry_array = array[1..65528 div sizeof(entry)] of entry;
  p_entry_array = ^entry_array;

  color_remap = array[0..255] of byte;
  rgb_triplet = record
    red   : byte;
    green : byte;
    blue  : byte;
  end;
  color_map   = array[0..255] of rgb_triplet;

var
  path   : array[1..4] of string;        {wad path}
  number : array[1..4] of integer;       {number of resources}
  dirlist: array[1..4] of p_entry_array; {pointers to dirlist}
  wadfile: array[1..4] of file;          {file handle}

  pnarray: array[1..1024] of char8;
  numpn  : integer;                      {number of patches in pnarray}
  pconv  : array[0..512] of integer;
  textptr: array[1..1024] of longint;    {texture pointer inside texture}
  texture: array[0..48000] of byte;      {texture data}
  numtx  : integer;                      {number of textures}
  txsize : word;                         {size of texture}
  colors : array[1..2] of color_remap;
  convert_patches : boolean;  {resource is a patch (TRUE) or a floor (FALSE)}

  why    : string;
  incheck: boolean;
  finalsize : longint;

procedure myhalt(err:errors);
  begin
    halt(ord(err));
  end;

function PtrAdd(p:pointer;n:word):pointer; assembler;
  asm
    les ax, p
    mov dx, es
    add ax, n
  end;

procedure checkabort;
  begin
    if keypressed then case readkey of
      #0: readkey;
      #27: myhalt(ERR_USER_ESCAPE);
    end;
  end;

procedure input(x,y:integer;var a:string;n:integer);
  var
   i,p : integer;
   c : char;
   done : boolean;

  procedure del;
    begin
      dec(p);
      delete(a,p,1);
      gotoxy(x+p,y);
      write(copy(a,p,n),#32);
      gotoxy(x+p,y)
    end;

  begin
    textattr:=red*16+yellow;
    gotoxy(x,y);
    write(#32:n+2);
    gotoxy(x+1,y);
    write(a);
    p:=length(a)+1;
    gotoxy(x+p,y);
    done:=FALSE;
    repeat
      c:=upcase(readkey);
      case c of
        #0 :
          begin
            c:=readkey;
            case c of
              #75 : if p>1 then dec(p);
              #77 : if p<=length(a) then inc(p);
              #71 : p:=1;
              #79 : p:=length(a)+1;
              #83 :
                if p<=length(a) then
                  begin
                    inc(p);
                    del
                  end
              end;
            gotoxy(x+p,y)
          end;
        #32..#96 :
          if length(a)<n then
            begin
              insert(c,a,p);
              gotoxy(x+p,y);
              write(copy(a,p,n));
              inc(p);
              gotoxy(x+p,y)
            end;
        #8 : if p>1 then del;
        #27 :
          begin
            p:=1;
            gotoxy(x+p,y);
            write(#32:length(a));
            a:='';
            gotoxy(x+p,y);
            done:=true;
          end;
        #13 : done:=true
        end
    until done;
    gotoxy(x,y);
    writeln(#32,a,#32:n-length(a)+1)
  end;

function isdir(name:string):boolean;
  var trovato:boolean;
      s:searchrec;
  begin
    trovato:=false;
    findfirst(name,directory,s);
    if (doserror=0) and (ioresult=0) then
      if (s.attr and directory)=directory then trovato:=true;
    isdir:=trovato
  end;

procedure askmerge;
  var i:integer;
  begin
    textattr:=lightgreen;
    write('Choose the target game:');
    textattr:=green;
    writeln(' (ESC quits, ENTER choose, any other key to toggle)');
    repeat
      textattr:=white;
      if heretic_mode then begin
        write('HERETIC');
        textattr:=lightgray;
        write(' - DM2CONV will contain all the textures and floors from DOOM/DOOM II');
      end
      else begin
        write('DOOM II');
        textattr:=lightgray;
        write(' - DM2CONV will contain all the textures missing in DOOM II');
      end;
      clreol;
      gotoxy(1,wherey);
      case readkey of
        #27: myhalt(ERR_USER_ESCAPE);
        #13: exit;
        #0: readkey;
      end;
      heretic_mode:=not heretic_mode;
    until false;
  end;

procedure gamedir(previous,name:string;var s:string);
  var t:string;
  begin
    if previous='' then previous:='C:\GAMES\X';
    FSplit(previous,s,t,t);
    FSplit(name,previous,t,previous);
    s:=s+t;
  end;

procedure askpath;
  var
    y:integer;
    b:Boolean;
    blank:boolean;
  procedure ask(a:string;var s:String);
    begin
      gotoxy(1,y);
      textattr:=lightcyan;
      write(a);
      b:=False;
      repeat
        gotoxy(14,y+1);
        textattr:=White;
        if b then begin
          write('The path specified does not exist!');
          clreol;
          while not keypressed do ;
          gotoxy(14,y+1);
        end;
        write(why);
        input(13,y,s,60);
        b:=True;
        if (s='') and blank then myhalt(ERR_USER_ESCAPE);
      until (s='') or isdir(s) ;
      if s='' then begin
        gotoxy(13,y);
        textattr:=white;
        write(' *** NOT INCLUDED ***');
        clreol;
      end;
      gotoxy(14,y+1);
      textattr:=White;
      clreol;
    end;
  begin
    path[1]:='';
    path[2]:='';
    path[3]:='';
    path[4]:='';
    gotoxy(1,1);
    textattr:=lightmagenta;
    writeln('This program creates a patch wad file named DM2CONV.WAD containing');
    writeln('all the textures missing in DOOM II or HERETIC.');
    writeln;
    writeln('You can choose to merge textures from DOOM, DOOM II or HERETIC.');
    writeln('Registered versions of the selected games are required.');
    writeln;
    writeln('This wad will enable DOOM II/HERETIC to use levels designed for DOOM');
    writeln('and converted by DM2CONV with no /TEXTURE argument.');
    writeln;
    askmerge;
    writeln;
    writeln;
    y:=wherey;
    gamedir('',DOOM_WAD,path[1]);
    gotoxy(1,y);
    textattr:=LightGreen;
    Writeln('Please insert the full path for the following sources:');
    inc(y);
    why:='';
    { *** SEE BELOW ***
    blank:=not heretic_mode;
    if heretic_mode then why:='Leave this field blank if you plan to convert only DOOM II''s wads';
    }
    ask(DOOM_WAD,path[1]);
    inc(y);
    if heretic_mode then begin
      { +------------------------------------------------------------+
        | DOOM II IS NOT SUPPORTED NOW. HERETIC WILL HANG IF DOOM II |
        | TEXTURES ARE INCLUDED INTO DM2CONV.WAD                     |
        +------------------------------------------------------------+
      gamedir(path[1],DOOM2_WAD,path[2]);
      blank:=path[1]='';
      if blank then why:='No DOOM support, only DOOM II''s wads will be supported.'
      else why:='Leave this field blank if you plan to convert only DOOM''s wads';
      ask(DOOM2_WAD,path[2]);
      inc(y);
      }
      if path[2]='' then gamedir(path[1],HERETIC_WAD,path[3])
      else gamedir(path[2],HERETIC_WAD,path[3]);
      why:='';
      blank:=true;
      ask(HERETIC_WAD,path[3]);
    end
    else begin
      why:='';
      gamedir(path[1],DOOM2_WAD,path[3]);
      blank:=true;
      ask('DOOM2.WAD',path[3]);
    end;
    inc(y);
    gotoxy(1,y);
    textattr:=LightGreen;
    clreol;
    inc(y);
    gotoxy(1,y);
    Writeln('Please insert the full path for the destination:');
    inc(y);
    path[4]:=path[3];
    why:='You will need about ';
    if heretic_mode then
      if path[1]='' then why:=why+'5.2 M'
      else if path[2]='' then why:=why+'3.4 M'
      else why:=why+'5.6 M'
    else why:=why+'600 K';
    why:=why+'bytes free in this directory.';
    ask('DM2CONV.WAD',path[4]);
  end;

var OldExitProc:Pointer;

procedure SExitProc; far;
  const xxx=':'#13#10;
  var i:integer;
  begin
    ExitProc:=OldExitProc;
    if incheck then begin
      textattr:=LightRed;
      gotoxy(2,wherey-1);
      writeln('x');
    end;
    textattr:=white;
    clreol;
    writeln;
    if Exitcode=0 then begin
      writeln('DM2CONV.WAD succesfully created (',finalsize,' bytes).');
      textattr:=lightgray;
      writeln;
      writeln('Now, to play any DOOM level simply include DM2CONV.WAD after -FILE.');
      if heretic_mode then write('example: HERETIC')
      else write('example: DOOM2');
      writeln(' -FILE DM2CONV.WAD anywad.WAD');
      writeln;
      textattr:=yellow;
      write('Remember to convert the wads with DM2CONV ');
      if heretic_mode then begin
        writeln('using the HERETIC response file');
        writeln('(you must remove the /TEXTURE and /FLOOR switches).');
        textattr:=lightgray;
        writeln('example: DM2CONV anywad @HERETIC.RSP');
      end
      else begin
        writeln('without the /TEXTURE switch');
        textattr:=lightgray;
        writeln('example: DM2CONV anywad');
      end;
      textattr:=lightgray;
    end
    else begin
      write('Operation aborted');
      case errors(exitcode) of
        ERR_USER_ESCAPE: writeln(' by user request!');
        ERR_NO_MEM     : writeln(': not enough memory!');
        ERR_OPEN       : writeln(xxx,'Cannot open ',why);
        ERR_READ       : writeln(xxx,'Cannot read ',why);
        ERR_WRITE      : writeln(xxx,'Cannot write ',why);
        ERR_NOTEX      : writeln(xxx,'Missing texture info in ',why);
        ERR_NOPALETTE  : writeln(xxx,'Missing palette in ',why);
        else writeln(xxx,why);
      end;
    end;
    i:=wherey;
    window(1,1,80,25);
    textattr:=lightgray;
    gotoxy(1,25);
    clreol;
    gotoxy(1,i+2);
  end;

procedure initialize;
  var i:integer;
  begin
    OldExitProc:=ExitProc;
    ExitProc:=@SExitProc;
    new(dirlist[4]);
    if dirlist[4]=nil then myhalt(ERR_NO_MEM);
    textmode(CO80);
    textattr:=blue*16+white;
    gotoxy(1,1);
    clreol;
    write('Welcome to DM2CONV.WAD''s maker v2.0':53);
    textattr:=lightgray*16+black;
    gotoxy(1,25);
    clreol;
    write(' Press ESC to abort the creation process.');
    window(1,3,80,24);
  end;

procedure checkmark;
  var i:byte;
  begin
    i:=textattr;
    textattr:=white;
    gotoxy(2,wherey-1);
    writeln('û');
    textattr:=i;
    incheck:=false;
  end;

procedure putcheckmark;
  begin
    textattr:=lightgray;
    write('[ ] ');
    incheck:=true;
  end;

procedure fseek(start:longint;index:integer);
  var i:word;
  begin
    why:=path[index];
    if start>0 then begin
      seek(wadfile[index],start);
      if ioresult<>0 then myhalt(ERR_READ);
      checkabort;
    end;
  end;

procedure blockw(var p;size:word);
  var i:word;
  begin
    why:=path[4];
    blockwrite(wadfile[4],p,size,i);
    if (ioresult<>0) or (size<>i) then myhalt(ERR_WRITE);
    checkabort;
  end;

procedure blockr(start:longint;index:integer;var p;size:word);
  var i:word;
  begin
    fseek(start,index);
    blockread(wadfile[index],p,size,i);
    if (ioresult<>0) or (size<>i) then myhalt(ERR_READ);
    checkabort;
  end;

procedure openread(index:integer;name:string);
  var h:header;
      i:word;
  begin
    why:=path[index]+'\'+name;
    path[index]:=why;
    putcheckmark;
    writeln('Opening ',why);
    assign(wadfile[index],why);
    reset(wadfile[index],1);
    if ioresult<>0 then myhalt(ERR_OPEN);
    blockread(wadfile[index],h,sizeof(h),i);
    if (ioresult<>0) or (i<>sizeof(h)) then myhalt(ERR_READ);
    if h.Sig<>IWAD_SIG then myhalt(ERR_NOWAD);
    checkabort;
    seek(wadfile[index],h.start);
    number[index]:=h.num;
    if ioresult<>0 then myhalt(ERR_OPEN);
    GetMem(dirlist[index],h.num*sizeof(entry));
    if dirlist[index]=nil then myhalt(ERR_NO_MEM);
    Blockread(wadfile[index],dirlist[index]^,h.num*sizeof(entry),i);
    if (ioresult<>0) or (i<>h.num*sizeof(entry)) then myhalt(ERR_READ);
    checkabort;
    checkmark;
  end;

function searchentry(index:integer;name:char8):integer;
  var i:integer;
  begin
    i:=number[index];
    while (i>0) and (dirlist[index]^[i].Name<>name) do dec(i);
    searchentry:=i;
  end;

procedure readpalette(i:integer;var cmap:color_map);
  var j:integer;
      l:longint;
  begin
    why:=path[i];
    j:=searchentry(i,PLAYPAL);
    if j=0 then myhalt(ERR_NOPALETTE);
    blockr(dirlist[i]^[j].Start,i,cmap,sizeof(color_map));
  end;

procedure makeremaptables;
  var cmap1,cmap2:color_map;
      i,j,k,x:integer;
      r,g,b:longint;
      l,min:longint;
  begin
    putcheckmark;
    writeln('Reading palette information for colour remapping');
    readpalette(3,cmap2);
    for x:=1 to 2 do if path[x]<>'' then begin
      readpalette(x,cmap1);
      for i:=0 to 255 do begin
        min:=MAXLONGINT;
        r:=cmap1[i].red;
        g:=cmap1[i].green;
        b:=cmap1[i].blue;
        for j:=0 to 255 do begin
          l:=sqr(r-cmap2[j].red)+sqr(g-cmap2[j].green)+sqr(b-cmap2[j].blue);
          if l<min then begin
            min:=l;
            k:=j;
            if min=0 then break;
          end;
        end;
        colors[x][i]:=k;
      end;
    end;
    checkmark;
  end;

procedure remap(p:P_Memory;n:integer);
  var columns:integer;
      i,j,k:integer;
      line:array[0..255] of byte;
      offs:longint;
  begin
    if convert_patches then begin
      CopyToMem(p,columns,1,2);
      while columns>0 do begin
        dec(columns);
        CopyToMem(p,offs,columns*4+9,4);
        inc(offs);
        CopyToMem(p,line,offs,sizeof(line));
        i:=0;
        while line[i]<255 do begin
          j:=line[i+1]+2;
          inc(i,2);
          while j>0 do begin
            line[i]:=colors[n][line[i]];
            inc(i);
            dec(j);
          end;
        end;
        CopyFromMem(p,line,offs,i);
      end;
    end
    else begin
      offs:=1;
      columns:=4096;
      while columns>0 do begin
        if sizeof(line)>columns then i:=columns
        else i:=sizeof(line);
        CopyToMem(p,line,offs,i);
        for j:=0 to i-1 do line[j]:=colors[1][line[j]];
        CopyFromMem(p,line,offs,i);
        inc(offs,i);
        dec(columns,i);
      end;
    end;
  end;

procedure ReadResource(var d:entry);
  var offs,len,size:Longint;
      filenum:integer;
      p:P_Memory;
  begin
    filenum:=d.fnum;
    d.fnum:=0;
    offs:=d.Start;
    len:=d.Size;
    d.Start:=FilePos(wadfile[4]);
    if len>0 then begin
      p:=GetMemory(len,1);
      if p=nil then
       myhalt(ERR_NO_MEM);
      fseek(offs,filenum);
      if not FileReadBlock(wadfile[filenum],p,0,len) then myhalt(ERR_READ);
      if heretic_mode and (filenum<>3) then remap(p,filenum);
      checkabort;
      why:=path[4];
      if not FileWriteBlock(wadfile[4],p,0,len) then myhalt(ERR_WRITE);
      checkabort;
      FreeMemory(p);
    end;
  end;

procedure writewad;
  var h   : header;
      i,j : integer;
      l   : longint;
      a,b : integer;
      num : integer;
      onum: integer;
  procedure addentry(na:char8;st,si:longint);
    begin
      inc(num);
      with dirlist[4]^[num] do begin
        Name:=na;
        Size:=si;
        Start:=st;
      end;
    end;
  procedure copyresources(index,initial,final:integer);
    var i,j:integer;
        d:char8;
    begin
      for i:=initial to final do with dirlist[index]^[i] do begin
        d:=name;
        if size>0 then begin
          j:=a;
          while (j<=b) and (dirlist[3]^[j].name<>d) do inc(j);
          if j>b then begin
            j:=onum;
            while (j<=num) and (dirlist[4]^[j].name<>d) do inc(j);
            if j>num then begin
              inc(num);
              dirlist[4]^[num]:=dirlist[index]^[i];
              dirlist[4]^[num].fnum:=index;
            end;
          end;
        end;
      end;
    end;
  procedure saveresources;
    var m : longint;
        i : integer;
    begin
      l:=0;
      for i:=onum to num do inc(l,(dirlist[4]^[i].Size and $FFFFFF)+1);
      m:=0;
      for i:=onum to num do begin
        with dirlist[4]^[i] do begin
          inc(m,(Size and $FFFFFF)+1);
          gotoxy(5,wherey);
          write(Name,m*100 div l:6,'%');
        end;
        ReadResource(dirlist[4]^[i]);
      end;
      gotoxy(1,wherey);
      clreol;
      checkmark;
    end;
  begin
    why:=path[4]+'\DM2CONV.WAD';
    path[4]:=why;
    putcheckmark;
    writeln('Creating ',why);
    assign(wadfile[4],why);
    rewrite(wadfile[4],1);
    if ioresult<>0 then myhalt(ERR_WRITE);
    h.sig:=PWAD_SIG;
    blockw(h,sizeof(h));

    num:=0;
    addentry(PNAMES,FilePos(wadfile[4]),4+numpn*8);
    l:=numpn;
    blockw(l,4);
    blockw(pnarray,numpn*8);

    j:=numtx*4+4;
    for i:=1 to numtx do inc(textptr[i],j);
    addentry(TEXTURE1,FilePos(wadfile[4]),4+numtx*4+txsize);
    l:=numtx;
    blockw(l,4);
    blockw(textptr,numtx*4);
    blockw(texture,txsize);

    if heretic_mode then begin {DUMMY TEXTURE2}
      addentry(TEXTURE2,FilePos(wadfile[4]),sizeof(DUMMY_TEXTURE));
      blockw(DUMMY_TEXTURE,sizeof(DUMMY_TEXTURE));
    end;
    checkmark;

    onum:=num+1;
    convert_patches:=True;
    putcheckmark;
    if heretic_mode then writeln('Converting and adding patches')
    else writeln('Adding DOOM patches');
    a:=searchentry(3,P_START)+1;
    b:=searchentry(3,P_END)-1;
    addentry(P_START,0,0);
    addentry(P1_START,0,0);
    if path[1]<>'' then copyresources(1,searchentry(1,P_START),searchentry(1,P_END));
    if heretic_mode and (path[2]<>'') then copyresources(2,searchentry(2,P_START),searchentry(2,P_END));
    addentry(P1_END,0,0);
    addentry(P_END,0,0);
    saveresources;

    if heretic_mode then begin
      onum:=num+1;
      convert_patches:=False;
      putcheckmark;
      writeln('Converting and adding floors');
      a:=1;
      b:=0;
      addentry(F_START,0,0);
      addentry(F1_START,0,0);
      copyresources(3,searchentry(3,F_START),searchentry(3,F_END));
      if path[1]<>'' then copyresources(1,searchentry(1,F_START),searchentry(1,F_END));
      if heretic_mode and (path[2]<>'') then copyresources(2,searchentry(2,F_START),searchentry(2,F_END));
      addentry(F1_END,0,0);
      addentry(F_END,0,0);
      saveresources;
    end;

    putcheckmark;
    writeln('Writing directory structure');
    why:=path[4];
    h.Start:=FilePos(wadfile[4]);
    h.Num:=num;
    blockw(dirlist[4]^,num*sizeof(entry));
    finalsize:=FilePos(wadfile[4]);
    seek(wadfile[4],0);
    if ioresult<>0 then myhalt(ERR_WRITE);
    blockw(h,sizeof(h));
    checkmark;
  end;

procedure mergetexture(optn,otxn,otxs:integer);
{optn=old patch number,otxn=old texture number,otxs=old texture size}
  var i,j,k: integer;
      offs : longint;
      t    : p_txinfo;
      q    : pointer;
      p    : p_ptinfo;
  begin
    {PATCH NAMES MERGING}
    k:=optn;
    for i:=optn+1 to numpn do begin
      j:=optn;
      while (j>0) and (pnarray[j]<>pnarray[i]) do dec(j);
      if j=0 then begin
        inc(k);
        pnarray[k]:=pnarray[i];
        j:=k;
      end;
      pconv[i-optn-1]:=j-1;
    end;
    numpn:=k;
    {TEXTURE POINTER SORT}
    j:=numtx;
    while j>1 do begin
      k:=0;
      for i:=1 to j-1 do if textptr[i]>textptr[i+1] then begin
        k:=i;
        offs:=textptr[i];
        textptr[i]:=textptr[i+1];
        textptr[i+1]:=offs;
      end;
      j:=k;
    end;
    {TEXTURE INFO MERGING}
    txsize:=otxs;
    k:=otxn;
    for i:=otxn+1 to numtx do begin
      t:=addr(texture[textptr[i]]);
      j:=otxn;
      while (j>0) and (p_txinfo(addr(texture[textptr[j]]))^.Name<>t^.Name) do dec(j);
      if j=0 then begin
        inc(k);
        textptr[k]:=txsize;
        q:=addr(texture[txsize]);
        Move(t^,q^,sizeof(txinfo));
        inc(txsize,sizeof(txinfo));
        p:=PtrAdd(t,sizeof(txinfo));
        for j:=1 to t^.num do begin
          q:=addr(texture[txsize]);
          p^.Index:=pconv[p^.Index]; {convert PNAMES entries}
          Move(p^,q^,sizeof(ptinfo));
          p:=PtrAdd(p,sizeof(ptinfo));
          inc(txsize,sizeof(ptinfo));
        end;
      end;
    end;
    numtx:=k;
  end;

procedure readpnames(i:integer);
  var j:integer;
      l:longint;
      optn,otxn,otxs:integer;
  procedure readtx(txname:char8);
    var k:integer;
        m:longint;
    begin
      j:=searchentry(i,txname);
      if j=0 then myhalt(ERR_NOTEX);
      blockr(dirlist[i]^[j].Start,i,l,4);
      blockr(0,i,textptr[numtx+1],l*4);
      m:=txsize-(l+1)*4;
      for k:=numtx+1 to numtx+l do inc(textptr[k],m);
      m:=dirlist[i]^[j].Size-(l+1)*4;
      blockr(0,i,texture[txsize],m);
      inc(txsize,m);
      inc(numtx,l);
    end;
  begin
    otxs:=txsize;
    otxn:=numtx;
    optn:=numpn;
    putcheckmark;
    why:=path[i];
    write('Reading ');
    if i<>3 then write('and merging ');
    writeln('textures from ',path[i]);
    j:=searchentry(i,PNAMES);
    if j=0 then myhalt(ERR_NOTEX);
    blockr(dirlist[i]^[j].Start,i,l,4);
    blockr(0,i,pnarray[numpn+1],l*8);
    inc(numpn,l);
    readtx(TEXTURE1);
    if (i=1) or ((i=3) and heretic_mode) then readtx(TEXTURE2);
    if i<>3 then mergetexture(optn,otxn,otxs);
    checkmark;
  end;

procedure install;
  var i,j:integer;
  begin
    textattr:=lightgray;
    clrscr;
    if path[1]<>'' then openread(1,DOOM_WAD);
    if heretic_mode then begin
      if path[2]<>'' then openread(2,DOOM2_WAD);
      openread(3,HERETIC_WAD);
      makeremaptables;
    end
    else openread(3,DOOM2_WAD);
    numtx:=0;
    txsize:=0;
    numpn:=0;
    readpnames(3);
    if path[1]<>'' then readpnames(1);
    if heretic_mode and (path[2]<>'') then readpnames(2);
    writewad;
    putcheckmark;
    writeln('Closing files');
    if path[1]<>'' then close(wadfile[1]);
    if path[2]<>'' then close(wadfile[2]);
    close(wadfile[3]);
    close(wadfile[4]);
    checkmark;
  end;

begin
  initialize;
  gotoxy(1,6);
  askpath;
  install;
end.