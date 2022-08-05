{$A+,B-,D+,E+,F-,G+,I-,L+,N-,O-,P-,Q-,R-,S-,T-,V-,X+}
{$M 16384,0,655360}
{ DM2CONV v2.0 by Vincenzo Alcamo }
{ This program is Public Domain   }
type
  shortname = array[1..3] of char;
  dname = array[1..8] of char;
  p_string = ^string;
  obj = record
    id : integer;
    sname : shortname;
    game : byte; {1=DOOM,2=DOOM II,4=HERETIC}
    name : p_string
  end;
  errors = (ERR_OPENS,ERR_READS,ERR_OPEND,ERR_WRITED,ERR_PWAD,
            ERR_TOOENTRY,ERR_TOOMAPS,ERR_TOOMUS,ERR_NOMAPS,ERR_NOEQ,ERR_BADEND,
            ERR_BADNUM,ERR_NOMEM,ERR_OPEN,ERR_READ);
  header= record
    Sig   : Longint;
    Num   : Longint;
    Start : Longint;
  end;
  entry = record
    Start : Longint;
    RSize : Longint;
    Name  : dname;
  end;
  thing = record
    xpos : integer;
    ypos : integer;
    angle: integer;
    code : integer;
    flags: integer;
  end;
  sidedef = record
    x,y  : integer;
    a,b,c: dname;
    sect : integer;
  end;
  sector = record
    y1,y2: integer;
    a,b  : dname;
    lum,action,tag: integer;
  end;
  linedef = record
    v1,v2:integer;
    attr:integer;
    action:integer;
    tag,rside,lside:integer;
  end;
  repname = record
    before : dname;
    after  : dname;
  end;
  repname_array = array[1..1024] of repname;
  p_repname_array = ^repname_array;

const
  show_list : boolean = false;
  show_example: boolean = false;
  show_help : boolean = false;
  show_note : boolean = false;
  nocheck   : boolean = false;
  debug     : boolean = false;
  ignore    : boolean = false;
  do_texture: boolean = false; {remap wall textures}
  do_floor  : boolean = false; {remap floor textures}
  remapping : boolean = false; {remap levels}
  heretic   : boolean = false; {heretic mode}
  savedir   : boolean = false; {save directory entries}
  no_conv   : boolean = false; {no conversion}
  remap_lev : integer = 1;
  remap_mus : integer = 0;
  replaces  : integer = 0;
  BUFFSIZE = 65528;
  MAXENTRY = BUFFSIZE div sizeof(entry);
  MAXTHING = BUFFSIZE div sizeof(thing);
  MAXSIDES = BUFFSIZE div sizeof(sidedef);
  MAXSECS  = BUFFSIZE div sizeof(sector);
  MAXLINES = BUFFSIZE div sizeof(linedef);

  IWAD_SIG = Ord('I')+(Ord('W')+(Ord('A')+Ord('D') shl 8) shl 8) shl 8;
  PWAD_SIG = Ord('P')+(Ord('W')+(Ord('A')+Ord('D') shl 8) shl 8) shl 8;
  N_THINGS = 'THINGS'#0#0;
  N_SECTORS= 'SECTORS'#0;
  N_SIDEDEFS='SIDEDEFS';
  N_LINEDEFS='LINEDEFS';
  NULL_NAME= #0#0#0#0#0#0#0#0;

  REP_PERCENT=16384;
  MAXREP=4096;

  mnames : array[1..32] of dname =  (
    'D_RUNNIN',
    'D_STALKS',
    'D_COUNTD',
    'D_BETWEE',
    'D_DOOM'#0#0,
    'D_THE_DA',
    'D_SHAWN'#0,
    'D_DDTBLU',
    'D_IN_CIT',
    'D_DEAD'#0#0,
    'D_STLKS2',
    'D_THEDA2',
    'D_DOOM2'#0,
    'D_DDTBL2',
    'D_RUNNI2',
    'D_DEAD2'#0,
    'D_STLKS3',
    'D_ROMERO',
    'D_SHAWN2',
    'D_MESSAG',
    'D_COUNT2',
    'D_DDTBL3',
    'D_AMPIE'#0,
    'D_THEDA3',
    'D_ADRIAN',
    'D_MESSG2',
    'D_ROMER2',
    'D_TENSE'#0,
    'D_SHAWN3',
    'D_OPENIN',
    'D_EVIL'#0#0,
    'D_ULTIMA');

  remap_action : array[99..141] of integer =
    (026,106,101,102,103,104,100,086,075,004,002,003,029,103,050,061,
     103,042,001,031,095,088,010,021,062,105,000,000,107,095,095,022,
     020,068,032,028,033,027,034,000,000,000,000);

type
  a_buffer = array[1..BUFFSIZE] of byte;
  a_dirlist= array[1..MAXENTRY] of entry;
  a_things = array[1..MAXTHING] of thing;
  a_sidedefs=array[1..MAXSIDES] of sidedef;
  a_sectors= array[1..MAXSECS] of sector;
  a_linedefs=array[1..MAXLINES] of linedef;
  a_replace= array[1..MAXREP] of word;

var
  objects    : array[1..120] of obj;
  replace    : a_replace;
  replace2   : a_replace;
  numobjects : integer;
  source     : string;
  dest       : string;
  datafile   : string;
  buffer     : ^a_buffer;
  dirlist    : ^a_dirlist;
  things     : ^a_things;
  sidedefs   : ^a_sidedefs;
  sectors    : ^a_sectors;
  linedefs   : ^a_linedefs;
  numentry   : integer;
  maxside    : integer;

  reptexture : p_repname_array;
  nreptexture: integer;
  repfloor   : p_repname_array;
  nrepfloor  : integer;
  repdirs    : p_repname_array;
  nrepdirs   : integer;

  repside    : word;
  repfloo    : word;
  repthing   : word;
  replev     : word;

procedure adjust_name(var name:dname); assembler;
  asm
    cld
    les di, name
    mov cx, 8
    mov al, 32
    repne scasb
    jnz @@FINE
    xor ax, ax
    dec di
    inc cx
    rep stosb
@@FINE:
  end;

procedure CopyTable(table:p_repname_array;source:p_repname_array;var num:integer);
  var i,j,k:integer;
      name:dname;
  begin
    i:=1;
    j:=num;
    while source^[i].before[1]<>#0 do begin
      name:=source^[i].before;
      adjust_name(name);
      k:=1;
      while (k<=j) and (table^[k].before<>name) do inc(k);
      if (k>j) and (num<1024) then begin
        inc(num);
        table^[num].before:=name;
        table^[num].after:=source^[num].after;
        adjust_name(table^[num].after);
      end;
      inc(i);
    end;
  end;

function remap_name(table:p_repname_array;var name:dname;num:integer):integer; assembler;
  asm
    cld
    les di, name
    mov cx, 8
@@LOOP:
    mov al, es:[di]
    cmp al, 0
    je  @@FILLZERO
    cmp al, 'a'
    jb  @@STORE
    cmp al, 'z'
    ja  @@STORE
    sub al, 32
@@STORE:
    stosb
    loop @@LOOP
@@FILLZERO:
    rep stosb
@@OK:
    push ds
    lds si, name
    les di, table
    mov cx, num
    cld
    lodsw
    mov bx, [si]
    mov dx, [si+2]
    mov si, [si+4]
@@CICLO:
    scasw
    jnz @@NEXT
    cmp bx, es:[di]
    jnz @@NEXT
    cmp dx, es:[di+2]
    jnz @@NEXT
    cmp si, es:[di+4]
    jnz @@NEXT
    mov ax, es
    mov ds, ax
    mov si, di
    add si, 6
    les di, name
    mov cx, 8
    rep movsb
    mov ax, 1
    jmp @@FINE
@@NEXT:
    add di, 14
    loop @@CICLO
    xor ax, ax
@@FINE:
    pop ds
  end;

procedure texture_table; assembler;
  asm
    {TABLE OF TEXTURE REPLACEMENTS FOR DOOM II}
    DB 'AASTINKYDOORSTOP'
    DB 'ASHWALL ASHWALL2'
    DB 'BLODGR1 PIPE6   '
    DB 'BLODGR2 PIPE6   '
    DB 'BLODGR3 PIPE6   '
    DB 'BLODGR4 PIPE6   '
    DB 'BRNBIGC MIDGRATE'
    DB 'BRNBIGL MIDGRATE'
    DB 'BRNBIGR MIDGRATE'
    DB 'BRNPOIS2BROWN96 '
    DB 'BROVINE BROWN1  '
    DB 'BROWNWELBROWNHUG'
    DB 'CEMPOIS CEMENT1 '
    DB 'COMP2   COMPTALL'
    DB 'COMPOHSOCOMPWERD'
    DB 'COMPTILECOMPWERD'
    DB 'COMPUTE1COMPSTA1'
    DB 'COMPUTE2COMPTALL'
    DB 'COMPUTE3COMPTALL'
    DB 'DOORHI  TEKBRON2'
    DB 'GRAYDANGGRAY5   '
    DB 'ICKDOOR1DOOR1   '
    DB 'ICKWALL6ICKWALL5'
    DB 'LITE2   BROWN1  '
    DB 'LITE4   LITE5   '
    DB 'LITE96  BROWN96 '
    DB 'LITEBLU2LITEBLU1'
    DB 'LITEBLU3LITEBLU1'
    DB 'LITEMET METAL1  '
    DB 'LITERED DOORRED '
    DB 'LITESTONSTONE2  '
    DB 'MIDVINE1MIDGRATE'
    DB 'MIDVINE2MIDGRATE'
    DB 'NUKESLADSLADWALL'
    DB 'PLANET1 COMPSTA2'
    DB 'REDWALL1REDWALL '
    DB 'SKINBORDSKINMET1'
    DB 'SKINTEK1SKINMET2'
    DB 'SKINTEK2SKINMET2'
    DB 'SKULWAL3SKSPINE1'
    DB 'SKULWALLSKSPINE1'
    DB 'SLADRIP1SLADSKUL'
    DB 'SLADRIP2SLADSKUL'
    DB 'SLADRIP3SLADSKUL'
    DB 'SP_DUDE3SP_DUDE4'
    DB 'SP_DUDE6SP_DUDE4'
    DB 'SP_ROCK2SP_ROCK1'
    DB 'STARTAN1STARTAN2'
    DB 'STONGARGSTONE3  '
    DB 'STONPOISSTONE   '
    DB 'TEKWALL2TEKWALL1'
    DB 'TEKWALL3TEKWALL1'
    DB 'TEKWALL5TEKWALL1'
    DB 'WOODSKULWOODGARG'
    DB 0
  end;

procedure htexture_table; assembler;
  asm
    {TABLE OF TEXTURE REPLACEMENTS FOR HERETIC}
    DB 'AASHITTYCELTIC  '
    DB 'AASTINKYREDWALL '
    DB 'ASHWALL LOOSERCK'
    DB 'ASHWALL2LOOSERCK'
    DB 'ASHWALL3SQPEB2  '
    DB 'ASHWALL4SQPEB2  '
    DB 'ASHWALL6GRNBLOK1'
    DB 'ASHWALL7BRWNRCKS'
    DB 'BFALL1  LAVAFL1 '
    DB 'BFALL2  LAVAFL1 '
    DB 'BFALL3  LAVAFL1 '
    DB 'BFALL4  LAVAFL1 '
    DB 'BIGBRIK1SNDBLCKS'
    DB 'BIGBRIK2CSTLRCK '
    DB 'BIGBRIK3SNDBLCKS'
    DB 'BIGDOOR1DOORSTON'
    DB 'BIGDOOR2GRSKULL2'
    DB 'BIGDOOR3GRSKULL3'
    DB 'BIGDOOR4SKULLSB2'
    DB 'BIGDOOR5DOORWOOD'
    DB 'BIGDOOR6DOORWOOD'
    DB 'BIGDOOR7SKULLSB2'
    DB 'BLAKWAL1METL1   '
    DB 'BLAKWAL2METL1   '
    DB 'BLODGR1 SPINE2  '
    DB 'BLODGR2 SPINE2  '
    DB 'BLODGR3 SPINE2  '
    DB 'BLODGR4 SPINE2  '
    DB 'BLODRIP1SPINE2  '
    DB 'BLODRIP2SPINE2  '
    DB 'BLODRIP3SPINE2  '
    DB 'BLODRIP4SPINE2  '
    DB 'BRICK1  SNDCHNKS'
    DB 'BRICK10 GRNBLOK1'
    DB 'BRICK11 RCKSNMUD'
    DB 'BRICK12 SNDCHNKS'
    DB 'BRICK2  SNDCHNKS'
    DB 'BRICK3  SNDCHNKS'
    DB 'BRICK4  SNDCHNKS'
    DB 'BRICK5  SPINE2  '
    DB 'BRICK6  SNDCHNKS'
    DB 'BRICK7  SNDCHNKS'
    DB 'BRICK8  SNDCHNKS'
    DB 'BRICK9  SNDCHNKS'
    DB 'BRICKLITSPINE2  '
    DB 'BRNBIGC GATMETL5'
    DB 'BRNBIGL GATMETL5'
    DB 'BRNBIGR GATMETL5'
    DB 'BRNPOIS GRNBLOK1'
    DB 'BRNPOIS2SNDCHNKS'
    DB 'BRNSMAL1GATMETL4'
    DB 'BRNSMAL2GATMETL4'
    DB 'BRNSMALCGATMETL4'
    DB 'BRNSMALLGATMETL4'
    DB 'BRNSMALRGATMETL4'
    DB 'BRONZE1 SPINE2  '
    DB 'BRONZE2 SPINE1  '
    DB 'BRONZE3 SPINE2  '
    DB 'BRONZE4 SPINE2  '
    DB 'BROVINE SNDCHNKS'
    DB 'BROVINE2GRNBLOK1'
    DB 'BROWN1  SNDCHNKS'
    DB 'BROWN144SNDPLAIN'
    DB 'BROWN96 SPINE1  '
    DB 'BROWNGRNGRNBLOK1'
    DB 'BROWNHUGSNDPLAIN'
    DB 'BROWNPIPSINE2   '
    DB 'BROWNWELSNDPLAIN'
    DB 'BRWINDOWSNDCHNKS'
    DB 'BSTONE1 BRWNRCKS'
    DB 'BSTONE2 BRWNRCKS'
    DB 'BSTONE3 BRWNRCKS'
    DB 'CEMENT1 GRSKULL1'
    DB 'CEMENT2 GRSKULL1'
    DB 'CEMENT3 GRSKULL1'
    DB 'CEMENT4 GRSKULL1'
    DB 'CEMENT5 GRSKULL1'
    DB 'CEMENT6 GRSKULL1'
    DB 'CEMENT7 GRSKULL1'
    DB 'CEMENT8 GRSKULL1'
    DB 'CEMENT9 GRSKULL1'
    DB 'CEMPOIS GRSKULL1'
    DB 'COMP2   TRISTON1'
    DB 'COMPBLUEBLUEFRAG'
    DB 'COMPOHSOSANDSQ2 '
    DB 'COMPSPANSKULLSB1'
    DB 'COMPSTA1SKULLSB1'
    DB 'COMPSTA2SKULLSB1'
    DB 'COMPTALLTRISTON1'
    DB 'COMPTILETRISTON1'
    DB 'COMPUTE1TRISTON1'
    DB 'COMPUTE2TRISTON1'
    DB 'COMPUTE3TRISTON1'
    DB 'COMPWERDTRISTON1'
    DB 'CRACKLE2RCKSNMUD'
    DB 'CRACKLE4RCKSNMUD'
    DB 'CRATE1  WOODWL  '
    DB 'CRATE2  WOODWL  '
    DB 'CRATE3  WOODWL  '
    DB 'CRATELITWOODWL  '
    DB 'CRATINY WOODWL  '
    DB 'CRATWIDEWOODWL  '
    DB 'DBRAIN1 LAVAFL1 '
    DB 'DBRAIN2 LAVAFL1 '
    DB 'DBRAIN3 LAVAFL1 '
    DB 'DBRAIN4 LAVAFL1 '
    DB 'DOOR1   DOOREXIT'
    DB 'DOOR3   DOOREXIT'
    DB 'DOORBLU BLUEFRAG'
    DB 'DOORBLU2BLUEFRAG'
    DB 'DOORHI  DOORWOOD'
    DB 'DOORRED REDWALL '
    DB 'DOORRED2REDWALL '
    DB 'DOORSTOPMETL2   '
    DB 'DOORTRAKMETL2   '
    DB 'DOORYEL DRIPWALL'
    DB 'DOORYEL2DRIPWALL'
    DB 'EXITDOORDOOREXIT'
    DB 'EXITSIGNSNDCHNKS'
    DB 'EXITSTONGRSTNPB '
    DB 'FIREBLU1RCKSNMUD'
    DB 'FIREBLU2RCKSNMUD'
    DB 'FIRELAV2LAVAFL1 '
    DB 'FIRELAV3LAVAFL1 '
    DB 'FIRELAVALAVAFL1 '
    DB 'FIREMAG1LAVAFL1 '
    DB 'FIREMAG2LAVAFL1 '
    DB 'FIREMAG3LAVAFL1 '
    DB 'FIREWALALAVAFL1 '
    DB 'FIREWALBLAVAFL1 '
    DB 'FIREWALLLAVAFL1 '
    DB 'GRAY1   SQPEB1  '
    DB 'GRAY2   SQPEB1  '
    DB 'GRAY4   SQPEB1  '
    DB 'GRAY5   SQPEB1  '
    DB 'GRAY7   SQPEB1  '
    DB 'GRAYBIG SQPEB1  '
    DB 'GRAYDANGSQPEB1  '
    DB 'GRAYPOISSQPEB1  '
    DB 'GRAYTALLSQPEB1  '
    DB 'GRAYVINESQPEB1  '
    DB 'GSTFONT1GRNBLOK1'
    DB 'GSTFONT2GRNBLOK1'
    DB 'GSTFONT3GRNBLOK1'
    DB 'GSTGARG GRNBLOK1'
    DB 'GSTLION GRNBLOK1'
    DB 'GSTONE1 GRNBLOK1'
    DB 'GSTONE2 GRNBLOK1'
    DB 'GSTSATYRGRNBLOK1'
    DB 'GSTVINE1GRNBLOK1'
    DB 'GSTVINE2GRNBLOK1'
    DB 'ICKDOOR1DOORSTON'
    DB 'ICKWALL1CSTLRCK '
    DB 'ICKWALL2CSTLRCK '
    DB 'ICKWALL3CSTLRCK '
    DB 'ICKWALL4CSTLRCK '
    DB 'ICKWALL5CSTLRCK '
    DB 'ICKWALL6CSTLRCK '
    DB 'ICKWALL7CSTLRCK '
    DB 'LITE2   SNDCHNKS'
    DB 'LITE3   DRIPWALL'
    DB 'LITE4   DRIPWALL'
    DB 'LITE5   DRIPWALL'
    DB 'LITE96  SPINE1  '
    DB 'LITEBLU1DRIPWALL'
    DB 'LITEBLU2DRIPWALL'
    DB 'LITEBLU3DRIPWALL'
    DB 'LITEBLU4DRIPWALL'
    DB 'LITEMET SKULLSB1'
    DB 'LITERED REDWALL '
    DB 'LITESTONSQPEB1  '
    DB 'MARBFAC2STNGLS1 '
    DB 'MARBFAC3STNGLS2 '
    DB 'MARBFAC4STNGLS1 '
    DB 'MARBFACESTNGLS1 '
    DB 'MARBGRAYSTNGLS1 '
    DB 'MARBLE1 GRNBLOK1'
    DB 'MARBLE2 GRNBLOK1'
    DB 'MARBLE3 GRNBLOK1'
    DB 'MARBLOD1GRNBLOK1'
    DB 'METAL   RCKSNMUD'
    DB 'METAL1  SKULLSB1'
    DB 'METAL2  GRSTNPB '
    DB 'METAL3  GRSTNPB '
    DB 'METAL4  GRSTNPB '
    DB 'METAL5  GRSTNPB '
    DB 'METAL6  GRSTNPB '
    DB 'METAL7  GRSTNPB '
    DB 'MIDBARS1GATMETL5'
    DB 'MIDBARS3GATMETL5'
    DB 'MIDBRN1 GATMETL5'
    DB 'MIDBRONZGATMETL5'
    DB 'MIDGRATEGATMETL5'
    DB 'MIDSPACEGATMETL5'
    DB 'MIDVINE1GATMETL5'
    DB 'MIDVINE2GATMETL5'
    DB 'MODWALL1METL1   '
    DB 'MODWALL2METL1   '
    DB 'MODWALL3METL1   '
    DB 'MODWALL4METL1   '
    DB 'NUKE24  SNDPLAIN'
    DB 'NUKEDGE1SNDPLAIN'
    DB 'NUKEPOISSNDPLAIN'
    DB 'NUKESLADSNDPLAIN'
    DB 'PANBLACKBANNER5 '
    DB 'PANBLUE BANNER6 '
    DB 'PANBOOK SPINE2  '
    DB 'PANBORD1SPINE2  '
    DB 'PANBORD2SPINE2  '
    DB 'PANCASE1SPINE2  '
    DB 'PANCASE2SPINE2  '
    DB 'PANEL1  BANNER5 '
    DB 'PANEL2  BANNER6 '
    DB 'PANEL3  BANNER5 '
    DB 'PANEL4  SPINE2  '
    DB 'PANEL5  SPINE2  '
    DB 'PANEL6  SPINE2  '
    DB 'PANEL7  SPINE2  '
    DB 'PANEL8  SPINE2  '
    DB 'PANEL9  SPINE2  '
    DB 'PANRED  BANNER5 '
    DB 'PIPE1   SPINE2  '
    DB 'PIPE2   SPINE2  '
    DB 'PIPE4   SPINE2  '
    DB 'PIPE6   SPINE2  '
    DB 'PIPES   CHAINSD '
    DB 'PIPEWAL1GRSKULL1'
    DB 'PIPEWAL2GRSKULL1'
    DB 'PLANET1 METL1   '
    DB 'PLAT1   GRSKULL1'
    DB 'REDWALL LAVAFL1 '
    DB 'REDWALL1REDWALL '
    DB 'ROCK1   LOOSERCK'
    DB 'ROCK2   LOOSERCK'
    DB 'ROCK3   LOOSERCK'
    DB 'ROCK4   DRIPWALL'
    DB 'ROCK5   DRIPWALL'
    DB 'ROCKRED1LAVAFL1 '
    DB 'ROCKRED2LAVAFL2 '
    DB 'ROCKRED3LAVAFL3 '
    DB 'SFALL1  WATRWAL1'
    DB 'SFALL2  WATRWAL1'
    DB 'SFALL3  WATRWAL1'
    DB 'SFALL4  WATRWAL1'
    DB 'SHAWN1  SQPEB1  '
    DB 'SHAWN2  SQPEB1  '
    DB 'SHAWN3  SQPEB1  '
    DB 'SILVER1 SQPEB1  '
    DB 'SILVER2 SQPEB1  '
    DB 'SILVER3 SQPEB1  '
    DB 'SKIN2   REDWALL '
    DB 'SKINBORDREDWALL '
    DB 'SKINCUT CTYSTCI1'
    DB 'SKINEDGEREDWALL '
    DB 'SKINFACEREDWALL '
    DB 'SKINLOW CTYSTCI2'
    DB 'SKINMET1CTYSTCI4'
    DB 'SKINMET2CTYSTCI1'
    DB 'SKINSCABCTYSTCI2'
    DB 'SKINSYMBCTYSTCI4'
    DB 'SKINTEK1CTYSTCI1'
    DB 'SKINTEK2CTYSTCI2'
    DB 'SKSNAKE1RCKSNMUD'
    DB 'SKSNAKE2RCKSNMUD'
    DB 'SKSPINE1RCKSNMUD'
    DB 'SKSPINE2RCKSNMUD'
    DB 'SKULWAL3RCKSNMUD'
    DB 'SKULWALLRCKSNMUD'
    DB 'SKY1    SKY1    '
    DB 'SKY2    SKY1    '
    DB 'SKY3    SKY1    '
    DB 'SK_LEFT RCKSNMUD'
    DB 'SK_RIGHTRCKSNMUD'
    DB 'SLADPOISGRSTNPB '
    DB 'SLADRIP1GRSTNPB '
    DB 'SLADRIP2GRSTNPB '
    DB 'SLADRIP3GRSTNPB '
    DB 'SLADSKULGRSTNPB '
    DB 'SLADWALLGRSTNPB '
    DB 'SLOPPY1 RCKSNMUD'
    DB 'SLOPPY2 RCKSNMUD'
    DB 'SPACEW2 SANDSQ2 '
    DB 'SPACEW3 SANDSQ2 '
    DB 'SPACEW4 SANDSQ2 '
    DB 'SPCDOOR1DOORSTON'
    DB 'SPCDOOR2DOORSTON'
    DB 'SPCDOOR3DOORSTON'
    DB 'SPCDOOR4DOORSTON'
    DB 'SP_DUDE1SAINT1  '
    DB 'SP_DUDE2SAINT1  '
    DB 'SP_DUDE3SAINT1  '
    DB 'SP_DUDE4SAINT1  '
    DB 'SP_DUDE5SAINT1  '
    DB 'SP_DUDE6SAINT1  '
    DB 'SP_DUDE7SPINE2  '
    DB 'SP_DUDE8SPINE2  '
    DB 'SP_FACE1GRSKULL1'
    DB 'SP_FACE2GRSKULL1'
    DB 'SP_HOT1 REDWALL '
    DB 'SP_ROCK1METL1   '
    DB 'SP_ROCK2METL1   '
    DB 'STARBR2 CTYSTUC1'
    DB 'STARG1  CTYSTUC2'
    DB 'STARG2  CTYSTUC3'
    DB 'STARG3  CTYSTUC4'
    DB 'STARGR1 CTYSTUC5'
    DB 'STARGR2 CTYSTUC1'
    DB 'STARTAN1CTYSTUC2'
    DB 'STARTAN2CTYSTUC3'
    DB 'STARTAN3CTYSTUC4'
    DB 'STEP1   TMBSTON2'
    DB 'STEP2   TMBSTON2'
    DB 'STEP3   TMBSTON2'
    DB 'STEP4   TMBSTON2'
    DB 'STEP5   TMBSTON2'
    DB 'STEP6   TMBSTON2'
    DB 'STEPLAD1TMBSTON2'
    DB 'STEPTOP TMBSTON2'
    DB 'STONE   TRISTON1'
    DB 'STONE2  TRISTON1'
    DB 'STONE3  TRISTON1'
    DB 'STONE4  SQPEB1  '
    DB 'STONE5  SQPEB1  '
    DB 'STONE6  SQPEB2  '
    DB 'STONE7  SQPEB2  '
    DB 'STONGARGTRISTON1'
    DB 'STONPOISTRISTON1'
    DB 'STUCCO  SANDSQ2 '
    DB 'STUCCO1 SANDSQ2 '
    DB 'STUCCO2 SANDSQ2 '
    DB 'STUCCO3 SANDSQ2 '
    DB 'SUPPORT2DRIPWALL'
    DB 'SUPPORT3DRIPWALL'
    DB 'SW1BLUE SW1OFF  '
    DB 'SW1BRCOMSW1OFF  '
    DB 'SW1BRIK SW1OFF  '
    DB 'SW1BRN1 SW1OFF  '
    DB 'SW1BRN2 SW1OFF  '
    DB 'SW1BRNGNSW1OFF  '
    DB 'SW1BROWNSW1OFF  '
    DB 'SW1CMT  SW1OFF  '
    DB 'SW1COMM SW1OFF  '
    DB 'SW1COMP SW1OFF  '
    DB 'SW1DIRT SW1OFF  '
    DB 'SW1EXIT SW1OFF  '
    DB 'SW1GARG SW1OFF  '
    DB 'SW1GRAY SW1OFF  '
    DB 'SW1GRAY1SW1OFF  '
    DB 'SW1GSTONSW1OFF  '
    DB 'SW1HOT  SW1OFF  '
    DB 'SW1LION SW1OFF  '
    DB 'SW1MARB SW1OFF  '
    DB 'SW1MET2 SW1OFF  '
    DB 'SW1METALSW1OFF  '
    DB 'SW1MOD1 SW1OFF  '
    DB 'SW1PANELSW1OFF  '
    DB 'SW1PIPE SW1OFF  '
    DB 'SW1ROCK SW1OFF  '
    DB 'SW1SATYRSW1OFF  '
    DB 'SW1SKIN SW1OFF  '
    DB 'SW1SKULLSW1OFF  '
    DB 'SW1SLAD SW1OFF  '
    DB 'SW1STARGSW1OFF  '
    DB 'SW1STON1SW1OFF  '
    DB 'SW1STON2SW1OFF  '
    DB 'SW1STON6SW1OFF  '
    DB 'SW1STONESW1OFF  '
    DB 'SW1STRTNSW1OFF  '
    DB 'SW1TEK  SW1OFF  '
    DB 'SW1VINE SW1OFF  '
    DB 'SW1WDMETSW1OFF  '
    DB 'SW1WOOD SW1OFF  '
    DB 'SW1ZIM  SW1OFF  '
    DB 'SW2BLUE SW1ON   '
    DB 'SW2BRCOMSW1ON   '
    DB 'SW2BRIK SW1ON   '
    DB 'SW2BRN1 SW1ON   '
    DB 'SW2BRN2 SW1ON   '
    DB 'SW2BRNGNSW1ON   '
    DB 'SW2BROWNSW1ON   '
    DB 'SW2CMT  SW1ON   '
    DB 'SW2COMM SW1ON   '
    DB 'SW2COMP SW1ON   '
    DB 'SW2DIRT SW1ON   '
    DB 'SW2EXIT SW1ON   '
    DB 'SW2GARG SW1ON   '
    DB 'SW2GRAY SW1ON   '
    DB 'SW2GRAY1SW1ON   '
    DB 'SW2GSTONSW1ON   '
    DB 'SW2HOT  SW1ON   '
    DB 'SW2LION SW1ON   '
    DB 'SW2MARB SW1ON   '
    DB 'SW2MET2 SW1ON   '
    DB 'SW2METALSW1ON   '
    DB 'SW2MOD1 SW1ON   '
    DB 'SW2PANELSW1ON   '
    DB 'SW2PIPE SW1ON   '
    DB 'SW2ROCK SW1ON   '
    DB 'SW2SATYRSW1ON   '
    DB 'SW2SKIN SW1ON   '
    DB 'SW2SLAD SW1ON   '
    DB 'SW2STARGSW1ON   '
    DB 'SW2STON1SW1ON   '
    DB 'SW2STON2SW1ON   '
    DB 'SW2STON6SW1ON   '
    DB 'SW2STONESW1ON   '
    DB 'SW2STRTNSW1ON   '
    DB 'SW2TEK  SW1ON   '
    DB 'SW2VINE SW1ON   '
    DB 'SW2WDMETSW1ON   '
    DB 'SW2WOOD SW1ON   '
    DB 'SW2ZIM  SW1ON   '
    DB 'TANROCK2SNDCHNKS'
    DB 'TANROCK3SNDCHNKS'
    DB 'TANROCK4SNDCHNKS'
    DB 'TANROCK5SNDCHNKS'
    DB 'TANROCK7SNDCHNKS'
    DB 'TANROCK8SNDCHNKS'
    DB 'TEKBRON1DOORWOOD'
    DB 'TEKBRON2DOORWOOD'
    DB 'TEKGREN1GRNBLOK1'
    DB 'TEKGREN2GRNBLOK1'
    DB 'TEKGREN3GRNBLOK1'
    DB 'TEKGREN4GRNBLOK1'
    DB 'TEKGREN5GRNBLOK1'
    DB 'TEKLITE SQPEB2  '
    DB 'TEKLITE2SQPEB2  '
    DB 'TEKWALL1CELTIC  '
    DB 'TEKWALL1CELTIC  '
    DB 'TEKWALL2CELTIC  '
    DB 'TEKWALL3CELTIC  '
    DB 'TEKWALL4CELTIC  '
    DB 'TEKWALL5CELTIC  '
    DB 'TEKWALL6CELTIC  '
    DB 'WOOD1   WOODWL  '
    DB 'WOOD10  WOODWL  '
    DB 'WOOD12  WOODWL  '
    DB 'WOOD3   WOODWL  '
    DB 'WOOD4   WOODWL  '
    DB 'WOOD5   WOODWL  '
    DB 'WOOD6   WOODWL  '
    DB 'WOOD7   WOODWL  '
    DB 'WOOD8   WOODWL  '
    DB 'WOOD9   WOODWL  '
    DB 'WOODGARGWOODWL  '
    DB 'WOODMET1WOODWL  '
    DB 'WOODMET2WOODWL  '
    DB 'WOODMET3WOODWL  '
    DB 'WOODMET4WOODWL  '
    DB 'WOODSKULWOODWL  '
    DB 'WOODVERTWOODWL  '
    DB 'ZDOORB1 DMNMSK  '
    DB 'ZDOORF1 DMNMSK  '
    DB 'ZELDOOR DMNMSK  '
    DB 'ZIMMER1 GRNBLOK1'
    DB 'ZIMMER2 GRNBLOK1'
    DB 'ZIMMER3 GRNBLOK1'
    DB 'ZIMMER4 GRNBLOK1'
    DB 'ZIMMER5 GRNBLOK1'
    DB 'ZIMMER7 GRNBLOK1'
    DB 'ZIMMER8 GRNBLOK1'
    DB 'ZZWOLF1 GRSKULL1'
    DB 'ZZWOLF10GRSKULL1'
    DB 'ZZWOLF11GRSKULL1'
    DB 'ZZWOLF12GRSKULL3'
    DB 'ZZWOLF13GRSKULL2'
    DB 'ZZWOLF2 GRSKULL3'
    DB 'ZZWOLF3 GRSKULL2'
    DB 'ZZWOLF4 GRSKULL3'
    DB 'ZZWOLF5 GRSKULL1'
    DB 'ZZWOLF6 GRSKULL2'
    DB 'ZZWOLF7 GRSKULL3'
    DB 'ZZWOLF9 GRSKULL1'
    DB 'ZZZFACE1HORSES1 '
    DB 'ZZZFACE2HORSES1 '
    DB 'ZZZFACE3HORSES1 '
    DB 'ZZZFACE4HORSES1 '
    DB 'ZZZFACE5HORSES1 '
    DB 'ZZZFACE6HORSES1 '
    DB 'ZZZFACE7HORSES1 '
    DB 'ZZZFACE8HORSES1 '
    DB 'ZZZFACE9HORSES1 '
    DB 0
  end;

procedure hfloor_table; assembler;
  asm
    DB 'BLOOD1  FLTLAVA1'
    DB 'BLOOD2  FLTLAVA2'
    DB 'BLOOD3  FLTLAVA3'
    DB 'CEIL1_1 FLOOR20 '
    DB 'CEIL1_2 FLOOR11 '
    DB 'CEIL1_3 FLOOR11 '
    DB 'CEIL3_1 FLOOR17 '
    DB 'CEIL3_2 FLOOR27 '
    DB 'CEIL3_3 FLAT522 '
    DB 'CEIL3_4 FLAT522 '
    DB 'CEIL3_5 FLAT521 '
    DB 'CEIL3_6 FLAT521 '
    DB 'CEIL4_1 FLOOR16 '
    DB 'CEIL4_2 FLOOR16 '
    DB 'CEIL4_3 FLOOR16 '
    DB 'CEIL5_1 FLOOR30 '
    DB 'CEIL5_2 FLAT512 '
    DB 'COMP01  FLOOR04 '
    DB 'CONS1_1 FLOOR07 '
    DB 'CONS1_5 FLOOR07 '
    DB 'CONS1_7 FLOOR07 '
    DB 'CRATOP1 FLAT504 '
    DB 'CRATOP2 FLOOR27 '
    DB 'DEM1_1  FLOOR19 '
    DB 'DEM1_2  FLOOR19 '
    DB 'DEM1_3  FLOOR19 '
    DB 'DEM1_4  FLOOR19 '
    DB 'DEM1_5  FLOOR19 '
    DB 'DEM1_6  FLOOR19 '
    DB 'FLAT1   FLOOR00 '
    DB 'FLAT10  FLOOR01 '
    DB 'FLAT14  FLAT502 '
    DB 'FLAT17  FLAT503 '
    DB 'FLAT18  FLAT503 '
    DB 'FLAT19  FLAT503 '
    DB 'FLAT1_1 FLOOR03 '
    DB 'FLAT1_2 FLOOR03 '
    DB 'FLAT1_3 FLOOR08 '
    DB 'FLAT2   FLOOR11 '
    DB 'FLAT20  FLAT504 '
    DB 'FLAT22  FLOOR26 '
    DB 'FLAT23  FLOOR23 '
    DB 'FLAT3   FLOOR04 '
    DB 'FLAT4   FLOOR08 '
    DB 'FLAT5   FLOOR06 '
    DB 'FLAT5_1 FLOOR10 '
    DB 'FLAT5_2 FLOOR25 '
    DB 'FLAT5_3 FLOOR09 '
    DB 'FLAT5_4 FLOOR04 '
    DB 'FLAT5_5 FLOOR27 '
    DB 'FLAT5_6 FLOOR06 '
    DB 'FLAT5_7 FLOOR03 '
    DB 'FLAT5_8 FLOOR03 '
    DB 'FLAT8   FLOOR25 '
    DB 'FLAT9   FLOOR04 '
    DB 'FLOOR0_1FLOOR17 '
    DB 'FLOOR0_2FLOOR27 '
    DB 'FLOOR0_3FLOOR18 '
    DB 'FLOOR0_5FLOOR04 '
    DB 'FLOOR0_6FLOOR04 '
    DB 'FLOOR0_7FLOOR04 '
    DB 'FLOOR1_1FLOOR16 '
    DB 'FLOOR1_6FLOOR09 '
    DB 'FLOOR1_7FLOOR09 '
    DB 'FLOOR3_3FLOOR18 '
    DB 'FLOOR4_1FLOOR25 '
    DB 'FLOOR4_5FLOOR25 '
    DB 'FLOOR4_6FLOOR25 '
    DB 'FLOOR4_8FLOOR00 '
    DB 'FLOOR5_1FLOOR01 '
    DB 'FLOOR5_2FLOOR17 '
    DB 'FLOOR5_3FLOOR17 '
    DB 'FLOOR5_4FLOOR10 '
    DB 'FLOOR6_1FLOOR09 '
    DB 'FLOOR6_2FLOOR03 '
    DB 'FLOOR7_1FLOOR27 '
    DB 'FLOOR7_2FLOOR19 '
    DB 'FWATER1 FLTWAWA1'
    DB 'FWATER2 FLTWAWA1'
    DB 'FWATER3 FLTWAWA1'
    DB 'FWATER4 FLTWAWA1'
    DB 'F_SKY1  F_SKY1  '
    DB 'GATE1   FLTTELE1'
    DB 'GATE2   FLTTELE1'
    DB 'GATE3   FLTTELE1'
    DB 'GATE4   FLTTELE1'
    DB 'GRASS1  FLAT512 '
    DB 'GRASS2  FLAT512 '
    DB 'GRNLITE1FLAT513 '
    DB 'GRNROCK FLOOR19 '
    DB 'LAVA1   FLATHUH1'
    DB 'LAVA2   FLATHUH1'
    DB 'LAVA3   FLATHUH1'
    DB 'LAVA4   FLATHUH1'
    DB 'MFLR8_1 FLOOR03 '
    DB 'MFLR8_2 FLOOR17 '
    DB 'MFLR8_3 FLOOR04 '
    DB 'MFLR8_4 FLOOR05 '
    DB 'NUKAGE1 FLTSLUD1'
    DB 'NUKAGE2 FLTSLUD1'
    DB 'NUKAGE3 FLTSLUD1'
    DB 'RROCK01 FLOOR09 '
    DB 'RROCK02 FLAT506 '
    DB 'RROCK03 FLOOR17 '
    DB 'RROCK04 FLOOR01 '
    DB 'RROCK05 FLOOR01 '
    DB 'RROCK06 FLOOR01 '
    DB 'RROCK07 FLOOR01 '
    DB 'RROCK08 FLOOR01 '
    DB 'RROCK09 FLAT510 '
    DB 'RROCK10 FLOOR25 '
    DB 'RROCK11 FLOOR27 '
    DB 'RROCK12 FLOOR27 '
    DB 'RROCK13 FLAT503 '
    DB 'RROCK14 FLOOR10 '
    DB 'RROCK15 FLOOR17 '
    DB 'RROCK16 FLAT516 '
    DB 'RROCK17 FLAT516 '
    DB 'RROCK18 FLAT516 '
    DB 'RROCK19 FLAT513 '
    DB 'RROCK20 FLAT513 '
    DB 'SFLR6_1 FLAT506 '
    DB 'SFLR6_4 FLAT506 '
    DB 'SFLR7_1 FLAT506 '
    DB 'SFLR7_4 FLAT506 '
    DB 'SLIME01 FLTSLUD1'
    DB 'SLIME02 FLTSLUD1'
    DB 'SLIME03 FLTSLUD1'
    DB 'SLIME04 FLTSLUD1'
    DB 'SLIME05 FLTSLUD1'
    DB 'SLIME06 FLTSLUD1'
    DB 'SLIME07 FLTSLUD1'
    DB 'SLIME08 FLTSLUD1'
    DB 'SLIME09 FLTLAVA1'
    DB 'SLIME10 FLTLAVA1'
    DB 'SLIME11 FLTLAVA1'
    DB 'SLIME12 FLTLAVA1'
    DB 'SLIME13 FLOOR05 '
    DB 'SLIME14 FLOOR25 '
    DB 'SLIME15 FLAT500 '
    DB 'SLIME16 FLAT500 '
    DB 'STEP1   FLOOR19 '
    DB 'STEP2   FLOOR19 '
    DB 'TLITE6_1FLOOR06 '
    DB 'TLITE6_4FLOOR06 '
    DB 'TLITE6_5FLOOR06 '
    DB 'TLITE6_6FLOOR06 '
    DB 0
  end;

procedure CreateTable; assembler;
  asm
    push ds
    mov ax, SEG objects
    mov es, ax
    lea di, objects
    lea si, @@TABLE
    mov ax, cs
    mov ds, ax
    xor cx, cx
    cld
@@CICLO:
    lodsb
    cmp al, 0
    je  @@STOP
    xor dx, dx
@@SPACE:
    cmp al, 32
    jne @@NUM
    lodsb
    jmp @@SPACE
@@NUM:
    mov bx, dx
    add dx, dx
    add dx, dx
    add dx, bx
    add dx, dx
    and ax, 15
    add dx, ax
    lodsb
    cmp al, 32
    jne @@NUM
    push ax
    mov ax, dx
    stosw
    pop ax

@@SPACES:
    cmp al, 32
    jne @@SHORT
    lodsb
    jmp @@SPACES
@@SHORT:
    stosb
    movsb
    movsb
    mov bx, si
    lodsb
    and al, 7
    stosb
@@ZERO:
    lodsb
    cmp al, 0
    jne @@ZERO
    mov ax, si
    sub ax, bx
    dec ax
    dec ax
    mov ds:[bx], al
    mov ax, bx
    stosw
    mov ax, ds
    stosw
    inc cx
    jmp @@CICLO
@@STOP:
    pop ds
    mov numobjects, cx
    jmp @@FINE
@@TABLE:
    {      OBJECT NUMBER
           |   SHORTNAME (3 CHARS)
           |   |
           |   |OBJECT TYPE
           |   ||
           |   ||DESCRIPTION (MAX 15 CHARS)
           |   |||}
    DB '2007 AMM3Ammo Clip',0
    DB '  68 ARA2Arachnotron',0
    DB '  64 ARC2Archvile',0
    DB '2015 ARM3Armor Helmet',0
    DB '  18 ARR4Ethereal arrows',0
    DB '   8 BAC3Backpack',0
    DB '   8 BAG4Bag of Holding',0
    DB '2048 BAM3Box of Ammo',0
    DB '2035 BAR3Barrel',0
    DB '2023 BER3Berserk',0
    DB '2006 BFG3BFG9000',0
    DB '2024 BLR3Blur Sphere',0
    DB '2019 BLU3Blue Armor',0
    DB '3003 BOH3Baron of Hell',0
    DB '2046 BRO3Box of Rockets',0
    DB '2049 BSH3Box of Shells',0
    DB '  70 BUR2Burning Barrel',0
    DB '3005 CAC3Cacodemon',0
    DB '2002 CHA3Chaingun',0
    DB '  36 CHA4Chaos Device',0
    DB '  65 CHD2Chaingun Dude',0
    DB '2005 CHS3Chainsaw',0
    DB '  54 CLO4Claw orb',0
    DB '2026 COM3Computer Map',0
    DB '2001 CRO4Crossbow',0
    DB '  16 CYB3Cyberdemon',0
    DB '3002 DEM3Demon',0
    DB '  15 DIS4Disciple',0
    DB '  11 DMS7DM start',0
    DB '  53 DRA4Dragon claw',0
    DB '   7 DSP4Dsparil',0
    DB '  56 DSS4Dsparil spot',0
    DB '  52 EGL4Exit Glitter',0
    DB '2047 ENC3Energy Cell',0
    DB '  55 ENO4Energy orb',0
    DB '  17 ENP3Energy Pack',0
    DB '  31 ESH4EnchantedShield',0
    DB '  82 FLA4Quartz flask',0
    DB '  22 FOR4Flame Orb',0
    DB '   5 GAL4Gargoyle Leader',0
    DB '  66 GAR4Flying gargoyle',0
    DB '2005 GAU4Gauntlets',0
    DB '  12 GEO4Crystal Geode',0
    DB '  46 GLG4GolemLeadGhost',0
    DB '  74 GLI4Glitter',0
    DB '  69 GOG4Golem Ghost',0
    DB '  68 GOL4Golem',0
    DB '  45 GLL4Golem Leader',0
    DB '2018 GRE3Green Armor',0
    DB '  21 GRU4Greater Runes',0
    DB '2014 HEA3Health Potion',0
    DB '  69 HEL2Hell Knight',0
    DB '2004 HST4Hell Staff',0
    DB '3001 IMP3Imp',0
    DB '2022 INV3Invulnerability',0
    DB '  23 IOR4Inferno Orb',0
    DB '  72 KEN2Commander Keen',0
    DB '   6 LIC4Iron Lich',0
    DB '2045 LIG3Light Goggles',0
    DB '3006 LOS3Lost Soul',0
    DB '  20 LRU4Lesser Runes',0
    DB '2002 MAC4FireMace',0
    DB '  67 MAN2Mancubus',0
    DB '  35 MAP4Map scroll',0
    DB '   9 MAU4Maulotaur',0
    DB '2012 MED3Medikit',0
    DB '  83 MEG2Megasphere',0
    DB '  30 MOR4Morph Ovum',0
    DB '  13 MSP4Mace Spheres',0
    DB '  92 OPH4Ophidian',0
    DB '  71 PAI2Pain Elemental',0
    DB '   1 PL17Player 1 start',0
    DB '   2 PL27Player 2 start',0
    DB '   3 PL37Player 3 start',0
    DB '   4 PL47Player 4 start',0
    DB '2004 PLA3Plasma Gun',0
    DB '  16 PMS4Pile macesphere',0
    DB '2003 PRO4Phoenix Rod',0
    DB '  19 QUI4Quiver',0
    DB '2025 RAD3Radiation Suit',0
    DB '2010 RCK3Rocket',0
    DB '  66 REV2Revenant',0
    DB '  84 RIN4Ring',0
    DB '2003 ROC3Rocket Launcher',0
    DB '  90 SCL4Sabreclaw',0
    DB '   9 SER3Sergeant',0
    DB '  75 SHA4Shadowsphere',0
    DB '2008 SHE3Shells',0
    DB '2001 SHO3Shotgun',0
    DB '  85 SIL4Silver shield',0
    DB '2013 SOU3Soul Sphere',0
    DB '  58 SPE3Spectre',0
    DB '   7 SPI3Spiderdemon',0
    DB '2035 SPO4Spore',0
    DB '  82 SSH2Super Shotgun',0
    DB '  84 SSN2SS Nazi',0
    DB '  43 SSS4Spore spaw spot',0
    DB '2011 STI3Stimpack',0
    DB '  14 TEL7Teleport exit',0
    DB '  34 TIM4Time bomb',0
    DB '  86 TOM4Tome of power',0
    DB '  33 TOR4Torch',0
    DB '3004 TRO3Trooper',0
    DB '  32 URN4Mystic Urn',0
    DB '  64 UWA4Undead Warrior',0
    DB '  65 UWG4UndeadWar.Ghost',0
    DB '  81 VIA4Crystal vial',0
    DB '  10 WAN4Wand Crystal',0
    DB '  44 WBR4Wooden Barrel',0
    DB '  70 WER4Weredragon',0
    DB '  83 WIN4Wings of wrath',0
    DB 0
@@FINE:
  end;

{Return a right-padded string of N characters from a string}
function StringN(s:String;n:Integer):String;
  var i:Integer;
  begin
    StringN:=Copy(s,1,n);
    StringN[0]:=Char(n);
    for i:=Length(s)+1 to n do StringN[i]:=' ';
  end;

{Converts string to uppercase}
function Upper(s:String):String;
  var i:Integer;
  begin
    Upper[0]:=s[0];
    for i:=1 to Length(s) do Upper[i]:=UpCase(s[i]);
  end;

{Add a suffix(extension) to a filename (only if the filename hasn't one)}
function AddSuffix(s,n:String):String;
  var i:Integer;
  begin
    i:=Length(s);
    while i>0 do
      if s[i]='.' then break
      else dec(i);
    if i>0 then AddSuffix:=s
    else AddSuffix:=s+'.'+n;
  end;

procedure Title;
  begin
    writeln('DM2CONV v2.0 by Vincenzo Alcamo (alcamo@arci01.bo.cnr.it)');
  end;

procedure List;
  const charmap:array[0..7] of char='  . ,  ;';
  var i,j:integer;
  begin
    Title;
    writeln;
    writeln('LIST OF KNOWN OBJECTS (.=DOOM II ,=HERETIC ;=DOOM/DOOM II/HERETIC)');
    for i:=1 to numobjects do begin
      if i mod 3=1 then writeln
      else write('  ');
      with objects[1+((i-1)div 3)+((i-1)mod 3)*((numobjects+2) div 3)] do
        write(id:4,#32,sname,charmap[game],StringN(name^,15));
    end;
    writeln;
    writeln;
    writeln('You can specify an object by its number, its shortname, its name');
    writeln('or even an initial fragment of its name.');
  end;

procedure More;
  begin
    Title;
    writeln;
    writeln('REPLACEMENT is an expression specifying object substitution:');
    writeln('  {source[:lev]}={dest[@num][:lev]}');
    writeln('source is the initial object, dest is the final object,');
    writeln('num is the number of substitutions (absolute or percentual)');
    writeln('lev specifies the difficulty-level flags of the object.');
    writeln('You can specify more than one replacement.');
    writeln;
    writeln('Replacement expression examples:');
    writeln;
    writeln('DEM=IMP             all Demons become Imps');
    writeln('DEM,IMP=LOS         all Demons and Imps become Lost Souls');
    writeln('DEM=IMP@5           5 Demons become Imps');
    writeln('DEM=IMP@50%         50% of Demons become Imps');
    writeln('DEM=IMP@5,SER       5 Demons become Imps, the rest are Sergeants');
    writeln('DEM=IMP DEM=TRO     No Demons remain for the second expression');
    writeln('DEM:1=IMP           All demons that appers in level 1 become Imps');
    writeln('DEM=IMP:123         All demons become Imps that appear in all levels');
    writeln;
    writeln('Requests greater than available objects are adjusted proportionally:');
    writeln('DEM=IMP@5,TRO@15    If Demons are 9 -> IMP@25%,TRO@75%');
    writeln;
    writeln('You can substitute the % sign with #,$,& whichever you prefer.');
  end;

procedure Help;
  begin
    Title;
    writeln('Converts DOOM maps for use with DOOM II/HERETIC.');
    writeln;
    writeln('DM2CONV <input> [output] [/mapnum] [/M[=num]] [/DEBUG] [/IGNORE] [/HERETIC]');
    writeln('        [/TEXTURE[{=file|:name1=name2}]] [/FLOOR[{=file|:name1=name2}]]');
    writeln('        [/NO{CONV|CHECK|TEXTURE|FLOOR}]... [/SEED[=num]] [replacements]...');
    writeln('        [/R{=file|:name1=name2}] [/LIST] [/EXAMPLES] [/NOTES] [@response]...');
    writeln;
    writeln('input        name of DOOM wad file to convert ** REQUIRED **');
    writeln('output       name of output file (if omitted, the input file is overwritten)');
    writeln('/mapnum      number for the first level remapped (default: 1)');
    writeln('/M[=num]     music remapping (num is the level for the first music)');
    writeln('/DEBUG       display debug information');
    writeln('/IGNORE      make replacements even if no level is remapped');
    writeln('/HERETIC     DOOM[2]->HERETIC conversion (used by HERETIC.RSP, see /NOTES)');
    writeln('/TEXTURE     convert texture names  *** SEE DM2CONV.DOC ***');
    writeln('/FLOOR       convert floor names (/HERETIC only)');
    writeln('/SEED[=num]  random generator seed (default: 0, randomize if num is omitted)');
    writeln('/NOCHECK     allow the use of object numbers not in list');
    writeln('/NOCONV      ignore conversion: useful for /R or object substitution');
    writeln('/R           renames directory entries');
    writeln('@response    response file (text file with additional arguments)');
    writeln('Use /LIST, /EXAMPLES, /NOTES to get further information (use MORE).');
  end;

procedure Notes;
  begin
    Title;
    writeln;
    writeln('Notes about level remapping:');
    writeln('- Level remapping is performed regardless of level name:');
    writeln('  the first level found becomes MAP01 (and so on)');
    writeln('- No other resources are remapped (eg: M_EPI?, etc...)');
    writeln('- DM2CONV acts only in one way: keep a backup of your wads.');
    writeln('- Secret levels are not remapped to the proper level: don''t use wads');
    writeln('  with secret levels  or, at least, avoid entering a secret level.');
    writeln;
    writeln('Music remapping has 3 settings (none, /M, /M=num):');
    writeln('1) no music is remapped.');
    writeln('2) remap musics accordingly to remapped levels');
    writeln('   D_E1M1 becomes D_RUNNIN only if E1M1 was remapped');
    writeln('3) the first music found becomes the music for MAP num,');
    writeln('   the second becomes the music for MAP num+1, and so on.');
    writeln('For 2) and 3): the end-of-level music is also remapped.');
    writeln;
    writeln('DOOM II CONVERSION:  DM2CONV input.wad output.wad @DOOM2.RSP');
    writeln('HERETIC CONVERSION:  DM2CONV input.wad output.wad @HERETIC.RSP');
    writeln('(both DOOM wads and DOOM II wads can be converted to HERETIC)');
  end;

function GetWord(var s:string):string;
  var i:integer;
  begin
    s:=s+#0;
    i:=1;
    while ((s[i]>='0') and (s[i]<='9')) or ((s[i]>='A') and (s[i]<='Z')) do inc(i);
    GetWord:=Copy(s,1,i-1);
    s:=Copy(s,i,length(s)-i);
  end;

function GetNum(var s:string):integer;
  var i,j,k:integer;
  begin
    val(s,j,k);
    if k=0 then begin
      if nocheck and (j>0) and (j<16384) then begin
        GetNum:=j;
        exit;
      end;
      for i:=1 to numobjects do
        if objects[i].id=j then begin
          GetNum:=j;
          exit;
        end;
    end
    else begin
      for i:=1 to numobjects do
        if s=objects[i].sname then begin
          GetNum:=objects[i].id;
          exit;
        end;
      for i:=1 to numobjects do with objects[i] do begin
        j:=1;
        k:=1;
        repeat
          if name^[k]=' ' then inc(k)
          else if s[j]<>UpCase(name^[k]) then break
          else begin
            inc(j);
            inc(k);
          end;
        until (j>length(s)) or (k>length(name^));
        if j>length(s) then begin
          GetNum:=id;
          exit;
        end;
      end;
    end;
    GetNum:=0;
  end;

procedure noname(s:string);
  begin
    writeln('No object found for ',s);
    halt;
  end;

procedure myhalt(code:errors);
  begin
    case code of
      ERR_OPENS: writeln('Error opening: ',source);
      ERR_OPEND: writeln('Error opening: ',dest);
      ERR_READS: writeln('Error reading: ',source);
      ERR_WRITED:writeln('Error writing: ',dest);
      ERR_PWAD:  writeln('File is not a valid WAD: ',source);
      ERR_TOOENTRY:writeln('Too many entries in file: ',source);
      ERR_TOOMAPS,ERR_TOOMUS: begin
        write('Cannot remap ');
        if code=ERR_TOOMAPS then write('level') else write('music');
        write(' after level ');
        if heretic then writeln(27)
        else writeln(32);
      end;
      ERR_NOMAPS:writeln('No maps found in file: ',source);
      ERR_NOEQ:  writeln('Missing ''='' after list of source objects');
      ERR_BADEND:writeln('Expression incorrectly terminated');
      ERR_BADNUM:writeln('Bad number in expression');
      ERR_NOMEM: writeln('Not enough memory');
      ERR_OPEN:  writeln('Error opening: ',datafile);
      ERR_READ:  writeln('Error reading: ',datafile);
    end;
    halt(0);
  end;

function checkdatafile(table:p_repname_array;var num:integer;s:string):boolean;
  var f      :text;
      i      :integer;
      bef,aft:dname;
  function getname(var dest:dname):boolean;
    var j:integer;
        c:char;
    begin
      getname:=false;
      while (i<length(s)) and ((s[i]=' ') or (s[i]=#9)) do inc(i);
      if i<length(s) then
        case s[1] of
          '''',';','#','%','[':;
          else begin
            dest:=NULL_NAME;
            j:=8;
            c:=upcase(s[i]);
            while (j>0) and (i<=length(s)) and (
             ((c>='0') and (c<='9')) or (c='_') or
             ((c>='A') and (c<='Z')) ) do begin
              dec(j);
              dest[8-j]:=c;
              inc(i);
              c:=upcase(s[i]);
            end;
            if c='=' then inc(i);
            getname:=j<8;
          end
        end
    end;
  procedure insertname;
    var bef,aft:dname;
    begin
      if getname(bef) and getname(aft) then begin
        i:=1;
        while i<=num do
          if table^[i].before=bef then break
          else inc(i);
        if (i>num) and (num<1024) then inc(num);
        table^[i].before:=bef;
        table^[i].after:=aft;
      end;
    end;
  begin
    i:=1;
    while (i<=length(s)) and (s[i]<>':') and (s[i]<>'=') do inc(i);
    if i>=length(s) then checkdatafile:=True {use the embedded remap table}
    else begin
      checkdatafile:=False; {do not activate remap}
      inc(i);
      if s[i-1]=':' then insertname
      else begin
        s:=copy(s,i,255);
        datafile:=s;
        writeln('Reading data file: ',s);
        assign(f,s);
        reset(f);
        if ioresult<>0 then myhalt(ERR_OPEN);
        while not eof(f) do begin
          readln(f,s);
          if ioresult<>0 then myhalt(ERR_READ);
          i:=1;
          insertname;
        end;
        close(f);
      end;
    end;
  end;

procedure Swappa(var h,k:integer);
  var i,l:integer;
  begin
    for i:=1 to 3 do begin
      l:=replace[k];
      replace[k]:=replace[h];
      replace[h]:=l;
      inc(k);
      inc(h);
    end;
  end;

function checklevel(var s:string):integer;
  var i,j:integer;
      t:string;
  begin
    j:=0;
    if (length(s)>1) and (s[1]=':') then begin
      s:=Copy(s,2,255);
      t:=GetWord(s);
      for i:=1 to length(t) do case t[i] of
        '1': j:=j or 1;  {skill level 1-2}
        '2': j:=j or 2;  {skill level 3}
        '3': j:=j or 4;  {skill level 4-5}
        'D': j:=j or 8;  {deaf flag}
        'M': j:=j or 16; {multiplayer}
      end;
    end;
    checklevel:=j;
  end;

procedure printlevel(i:integer);
  begin
    if i>0 then write(':');
    if (i and 1)=1 then write('1');
    if (i and 2)=2 then write('2');
    if (i and 4)=4 then write('3');
    if (i and 8)=8 then write('D');
    if (i and 16)=16 then write('M');
  end;

procedure Parse;
  type response = record
         rfile : text;
         rtext : string;
       end;
  const MAX_RESP = 8;
  var
    i,j,k,h : integer;
    s,t     : string;
    l       : longint;
    f       : boolean;
    repn    : integer;
    ri,rc,rs: integer;
    inresp  : integer;
    resp    : array[1..MAX_RESP] of response;
  function GetArgument:string;
    var i,j:integer;
        s  :string;
    begin
      if resp[inresp].rtext='' then begin
        if eof(resp[inresp].rfile) then begin
          resp[inresp].rtext:='';
          close(resp[inresp].rfile);
          dec(inresp);
        end
        else begin
          Readln(resp[inresp].rfile,s);
          if ioresult<>0 then begin
            writeln('Error reading from response file');
            resp[inresp].rtext:='';
            close(resp[inresp].rfile);
            dec(inresp);
          end;
          j:=1;
          for i:=1 to length(s) do
            case s[i] of
              #32,#9: if j>1 then begin
                        s[j]:=#32;
                        inc(j);
                      end;
              else begin
                s[j]:=s[i];
                inc(j);
              end;
            end;
          s[0]:=chr(j-1);
          resp[inresp].rtext:=s;
        end;
      end;
      case resp[inresp].rtext[1] of
        '''',';','#','%','[': resp[inresp].rtext:='';
      end;
      s:=resp[inresp].rtext;
      i:=1;
      while (i<=length(s)) and (s[i]<>#32) do inc(i);
      GetArgument:=Upper(Copy(s,1,i-1));
      resp[inresp].rtext:=Copy(s,i+1,255);
    end;
  begin
    source:='';
    dest:='';
    RandSeed:=0;
    repn:=1;
    inresp:=0;
    i:=1;
    while i<=ParamCount do begin
      f:=not (show_help or show_example or show_list or show_note);
      if inresp>0 then s:=GetArgument
      else s:=Upper(ParamStr(i));
      if s='' then {DO NOTHING}
      else if s[1]='@' then begin
        if inresp=MAX_RESP then writeln('Too many nested response files!')
        else begin
          s:=Copy(s,2,255);
          inc(inresp);
          resp[inresp].rtext:='';
          assign(resp[inresp].rfile,s);
          reset(resp[inresp].rfile);
          if ioresult<>0 then begin
            s:=AddSuffix(s,'RSP');
            assign(resp[inresp].rfile,s);
            reset(resp[inresp].rfile);
          end;
          if ioresult<>0 then begin
            writeln('Error opening response file.');
            dec(inresp);
          end;
        end;
      end
      else if (s[1]='/') or (s[1]='-') then begin
        s:=Copy(s,2,255);
        if (s='HELP') or (s='?') or (s='H') then show_help:=f
        else if s='NOTEXTURE' then do_texture:=false
        else if s='NOFLOOR' then do_floor:=false
        else if (s='NOCHECK') or (s='N') then nocheck:=True
        else if s='NOCONV' then no_conv:=True
        else if (s='LIST') or (s='L') then show_list:=f
        else if (Copy(s,1,7)='EXAMPLE') or (s='E') then show_example:=f
        else if Copy(s,1,4)='NOTE' then show_note:=f
        else if (s='DEBUG') or (s='D') then debug:=True
        else if (s='IGNORE') or (s='I') then ignore:=True
        else if s[1]='R' then checkdatafile(repdirs,nrepdirs,s)
        else if (copy(s,1,7)='TEXTURE') or (s[1]='T') then begin
          do_texture:=do_texture or checkdatafile(reptexture,nreptexture,s);
        end
        else if (copy(s,1,5)='FLOOR') or (s[1]='F') then begin
          do_floor:=do_floor or checkdatafile(repfloor,nrepfloor,s);
        end
        else if s='HERETIC' then heretic:=True
        else if Copy(s,1,4)='SEED' then begin
          s:=Copy(s,5,255);
          j:=0;
          if s[1]='=' then begin
            s:=Copy(s,2,255);
            Val(s,l,j);
            if j<>0 then writeln('Bad number for seed: ',s)
            else RandSeed:=l;
          end
          else Randomize;
          if j=0 then writeln('Seed for random generator is: ',RandSeed);
        end
        else if s[1]='M' then begin
          s:=Copy(s,2,255);
          if s[1]='=' then s:=Copy(s,2,255);
          if Length(s)>0 then begin
            Val(s,j,k);
            if (k<>0) or (j<1) or (j>32) then writeln('Bad number for music: ',s)
            else remap_mus:=j;
          end
          else remap_mus:=-1; {remap level&music}
        end
        else begin
          Val(s,j,k);
          if (k<>0) or (j<1) or (j>32) then writeln('Bad number for remap: ',s)
          else begin
            remap_lev:=j;
            remapping:=true;
            writeln('Remapping from level ',j);
          end;
        end
      end
      else begin
        k:=0;
        for j:=1 to length(s) do if s[j]='=' then k:=1;
        if k=0 then begin
          if source='' then source:=s
          else if dest='' then dest:=s
          else writeln('Extra parameter ignored: ',s);
        end
        else begin
          inc(replaces);
          if debug then writeln('Replacement ',replaces,': ',s);
          rs:=repn;
          s:=','+s+'';         {''=#21 is a sentinel}
          while s[1]=',' do begin
            s:=Copy(s,2,255);
            t:=GetWord(s);
            j:=GetNum(t);
            if j=0 then noname(t);
            replace[repn]:=j;
            inc(repn);
            replace[repn]:=checklevel(s);
            inc(repn);
          end;
          if s[1]<>'=' then myhalt(ERR_NOEQ);
          ri:=repn;
          inc(repn);
          rc:=0;
          s[1]:=',';
          while s[1]=',' do begin
            s:=Copy(s,2,255);
            t:=GetWord(s);
            j:=GetNum(t);
            if j=0 then noname(t);
            replace[repn]:=j;
            inc(repn);
            replace[repn]:=0;
            if s[1]='@' then begin
              s:=Copy(s,2,255);
              t:=GetWord(s);
              val(t,j,k);
              if (k<>0) or (j>=REP_PERCENT) or (j<=0) then myhalt(ERR_BADNUM);
              if (s[1]>='#') and (s[1]<='&') then begin
                inc(j,REP_PERCENT);
                s:=Copy(s,2,255);
              end;
              replace[repn]:=j;
            end;
            inc(repn);
            replace[repn]:=checklevel(s);
            inc(repn);
            inc(rc);
          end;
          if (s[1]<>'') or (rc=0) then myhalt(ERR_BADEND);
          replace[ri]:=REP_PERCENT+rc;
          k:=ri+1;
          h:=k;
          for j:=1 to rc do begin
            if (replace[h+1]>0) and (replace[h+1]<REP_PERCENT) then Swappa(h,k);
            inc(h,3);
          end;
          h:=k;
          for j:=1 to rc do begin
            if replace[h+1]>=REP_PERCENT then Swappa(h,k);
            inc(h,3);
          end;
          if debug then begin
            write('REPLACE');
            j:=rs;
            while j<ri do begin
              write(' ',replace[j]);
              printlevel(replace[j+1]);
              inc(j,2);
            end;
            write(' WITH');
            k:=ri+1;
            for j:=1 to rc do begin
              write(' ',replace[k]);
              if replace[k+1]>0 then
                if replace[k+1]>=REP_PERCENT then write('@',replace[k+1]-REP_PERCENT,'%')
                else write('@',replace[k+1]);
              printlevel(replace[k+2]);
              inc(k,3);
            end;
            writeln;
          end;
        end;
      end;

      if inresp=0 then inc(i);
    end;
    if not (show_example or show_list or show_note) and (source='') then show_help:=true;
    source:=AddSuffix(source,'WAD');
    if dest<>'' then dest:=AddSuffix(dest,'WAD');
  end;

procedure blockr(var f:file;var dest;size:word;var count:word);
  begin
    BlockRead(f,dest,size,count);
    if (ioresult<>0) or (size<>count) then myhalt(ERR_READS);
  end;

procedure blockw(var f:file;var dest;size:word;var count:word);
  begin
    BlockWrite(f,dest,size,count);
    if (ioresult<>0) or (size<>count) then myhalt(ERR_WRITED);
  end;

procedure CopyDest;
  var a,b     : file;
      l       : Longint;
      size,len: Word;
  begin
    writeln('Copying source to destination...');
    Assign(a,source);
    FileMode:=0;  {open for read only}
    Reset(a,1);
    FileMode:=2;  {open for read/write}
    if ioresult<>0 then myhalt(ERR_OPENS);
    Assign(b,dest);
    Rewrite(b,1);
    if ioresult<>0 then myhalt(ERR_OPEND);
    l:=FileSize(a);
    while l>0 do begin
      if l>BUFFSIZE then size:=BUFFSIZE
      else size:=l;
      BlockR(a,buffer^,size,len);
      BlockW(b,buffer^,size,len);
      dec(l,size);
    end;
    Close(a);
    Close(b);
  end;

procedure ReplaceThings(totobj:Integer);
  var index  : array[1..4000] of integer;
      numobj : integer;
      i,j,k,l: integer;
      repn,h : integer;
      numabs : integer;
      nabs   : integer;
      nrel   : integer;
      level  : integer;
      multi  : boolean;
      s      : string;
  procedure Choose(var max:integer;n,c,lev:integer);
    var i,j:integer;
    begin
      if n<max then begin
        for i:=1 to n do begin
          j:=Random(max)+1;
          with things^[index[j]] do begin
            inc(repthing);
            code:=c;
            if lev<>0 then flags:=lev;
          end;
          index[j]:=index[max];
          dec(max);
        end;
      end
      else begin
        for i:=1 to max do with things^[index[i]] do begin
          inc(repthing);
          code:=c;
          if lev<>0 then flags:=lev;
        end;
        max:=0;
      end;
    end;
  begin
    replace:=replace2;
    repn:=1;
    for i:=1 to replaces do begin
      if debug then write('REPLACEMENT=',i);
      numobj:=0;
      while replace[repn]<REP_PERCENT do begin
        j:=replace[repn];
        level:=replace[repn+1] and 7;  {level 1 or 2 or 3}
        if level=0 then level:=7;
        multi:=replace[repn+1]>=16;    {multiplayer flag}
        for k:=1 to totobj do with things^[k] do
          if (code=j) and (flags and level>0) and
             (not multi or (flags and 16=16)) then begin
            inc(numobj);
            index[numobj]:=k;
          end;
        inc(repn,2);
      end;
      if debug then write('  TOTAL OBJECTS=',numobj);
      nabs:=0;
      nrel:=replace[repn]-REP_PERCENT;
      inc(repn);
      if numobj=0 then begin
        if debug then writeln('   SKIPPED');
        inc(repn,nrel*3);
        continue;
      end;
      numabs:=0;
      j:=nrel;
      l:=repn+1;
      k:=1;
      while (k<=j) do begin
        if replace[l]=0 then replace[l]:=REP_PERCENT
        else begin
          if replace[l]>=REP_PERCENT then
            replace[l]:=(longint(numobj)*(replace[l]-REP_PERCENT)+50)div 100;
          inc(numabs,replace[l]);
          inc(nabs);
          dec(nrel);
        end;
        inc(k);
        inc(l,3);
      end;
      if numabs>numobj then begin
        l:=repn+1;
        k:=numobj;
        for j:=1 to nabs do begin
          h:=replace[l];
          if numabs=0 then replace[l]:=0
          else replace[l]:=(longint(h)*k+numabs div 2)div numabs;
          dec(numabs,h);
          dec(k,replace[l]);
          inc(l,3);
        end;
        numabs:=numobj;
      end;
      l:=repn+nabs*3+1;
      numabs:=numobj-numabs;
      while nrel>0 do begin
        j:=(numabs+nrel div 2) div nrel;
        replace[l]:=j;
        dec(numabs,j);
        inc(l,3);
        dec(nrel);
        inc(nabs);
      end;
      for j:=1 to nabs do begin
        if debug then begin
          if j mod 4=1 then writeln
          else write(#32);
          k:=numobjects;
          h:=replace[repn];
          l:=2;
          if heretic then l:=4;
          while (k>0) and ((objects[k].id<>h) or (objects[k].game and l=0)) do dec(k);
          if k<>0 then s:=objects[k].name^
          else begin
            Str(h,s);
            s:='Unknown #'+s;
          end;
          write(s:15,'=');
          Str(replace[repn+1],s);
          write(StringN(s,3));
        end;
        Choose(numobj,replace[repn+1],replace[repn],replace[repn+2]);
        inc(repn,3);
      end;
      if debug then writeln;
    end;
  end;

procedure Plural(n:integer;s:string);
  begin
    write(' ',n,' ',s);
    if n<>1 then write('s');
  end;

procedure Process;
  var f    : file;
      head : header;
      size : word;
      i,j,k: integer;
      l    : integer;
      numt : integer;
      llev : integer;
      fpos : longint;
      rlev : array[1..32] of integer;
      repmus:integer;
  begin
    replace2:=replace;
    repside:=0;
    repfloo:=0;
    repthing:=0;
    replev:=0;
    repmus:=0;
    for i:=1 to 32 do rlev[i]:=0;
    if dest<>'' then CopyDest
    else dest:=source;
    source:=dest;
    Assign(f,dest);
    Reset(f,1);
    if ioresult<>0 then myhalt(ERR_OPEND);
    BlockR(f,head,sizeof(header),size);
    if (head.sig<>PWAD_SIG) and (head.sig<>IWAD_SIG) then myhalt(ERR_PWAD);
    numentry:=head.num;
    if numentry>MAXENTRY then myhalt(ERR_TOOENTRY);
    Seek(f,head.start);
    if ioresult<>0 then myhalt(ERR_READS);
    BlockR(f,dirlist^,numentry*sizeof(entry),size);

    numt:=MAXENTRY+1;
    llev:=0;
    if not no_conv then begin
      for i:=1 to numentry do with dirlist^[i] do begin
        {remap sky resources}
        if not heretic and (Copy(name,1,3)='SKY') and
           (name[4]>='1') and (name[4]<='3') and (name[5]=#0) then begin
          j:=ord(name[4]);
          name:='RSKYx'#0#0#0;
          name[5]:=chr(j);
          savedir:=true;
        end;
        { THE SKY RESOURCE OF DOOM II COULD BE CONVERTED, BUT THEY
          WILL APPEAR WRONG SINCE HERETIC HAS A DIFFERENT PALETTE.
        if heretic and (Copy(name,1,4)='RSKY') and
           (name[5]>='1') and (name[5]<='3') and (name[6]=#0) then begin
          j:=ord(name[5]);
          name:='SKYx'#0#0#0#0;
          name[4]:=chr(j);
          savedir:=true;
        end;
        }
        {remap levels}

        if Copy(name,1,3)='MAP' then begin
          llev:=2; {this signals a DOOM II level}
          if heretic then begin
            j:=(ord(name[4])-48)*10+ord(name[5])-48;
            k:=j;
            if remapping then begin
              k:=remap_lev;
              inc(remap_lev);
            end;
            if k>27 then begin
              if not nocheck then myhalt(ERR_TOOMAPS);
            end
            else begin
              rlev[j]:=k;
              name:='ExMy'#0#0#0#0;
              name[2]:=chr((k-1) div 9+49);
              name[4]:=chr((k-1) mod 9+49);
              savedir:=true;
              inc(replev);
            end;
          end
        end
        else if (name[1]='E') and (name[3]='M') then begin
          llev:=1; {this signals a DOOM level}
          if heretic then begin
            j:=(ord(name[2])-49)*9+ord(name[4])-48;
            if remapping then begin
              if remap_lev>27 then begin
                if not nocheck then myhalt(ERR_TOOMAPS);
              end
              else begin
                rlev[j]:=remap_lev;
                name[2]:=chr((remap_lev-1) div 9+49);
                name[4]:=chr((remap_lev-1) mod 9+49);
                inc(remap_lev);
                savedir:=true;
              end;
            end
            else rlev[j]:=j;
            inc(replev);
          end
          else begin
            if remap_lev>32 then begin
              if not nocheck then myhalt(ERR_TOOMAPS);
            end
            else begin
              rlev[(ord(name[2])-49)*9+ord(name[4])-48]:=remap_lev;
              name:='MAPxx'#0#0#0;
              name[4]:=chr(remap_lev div 10+48);
              name[5]:=chr(remap_lev mod 10+48);
              inc(remap_lev);
              inc(replev);
              savedir:=true;
            end;
          end;
        end;

        if heretic and (llev=2) then {special DOOM II remapping}
           if (Name=N_SECTORS) or (Name=N_LINEDEFS) then begin
             dec(numt);
             dirlist^[numt]:=dirlist^[i];
           end;

      end;
      {remap musics}
      if remap_mus<>0 then
        for i:=1 to numentry do with dirlist^[i] do
          if (name[1]='D') and (name[2]='_') then
            if (name='D_INTER'#0) or (heretic and (name='D_DM2INT')) then begin
              if heretic then name:='MUS_INTR'
              else name:='D_DM2INT';
              savedir:=true;
              inc(repmus);
            end
            else begin
              j:=0;
              if (name[3]='E') and (name[5]='M') then j:=(ord(name[4])-49)*9+ord(name[6])-48
              else if heretic then begin
                j:=32;
                while (j>0) and (mnames[j]<>name) do dec(j);
              end;
              if j>0 then begin
                if remap_mus>0 then begin
                  k:=remap_mus;
                  inc(remap_mus);
                end
                else k:=rlev[j];
                if k>0 then begin
                  if heretic then begin
                    if k>27 then begin
                      if not nocheck then myhalt(ERR_TOOMUS);
                    end
                    else begin
                      name:='MUS_ExMy';
                      dec(k);
                      name[6]:=chr(k div 9+49);
                      name[8]:=chr(k mod 9+49);
                    end;
                  end
                  else begin
                    if k>32 then begin
                      if not nocheck then myhalt(ERR_TOOMUS);
                    end
                    else name:=mnames[k];
                  end;
                  savedir:=True;
                  inc(repmus);
                end;
              end;
            end;
    end; {no_conv}

    if nrepdirs>0 then
      for i:=1 to numentry do with dirlist^[i] do
        savedir:=remap_name(repdirs,name,nrepdirs)>0;

    if savedir then begin
      Seek(f,head.start);
      if ioresult<>0 then myhalt(ERR_WRITED);
      BlockW(f,dirlist^,numentry*sizeof(entry),size);
    end;
    if (replev=0) and (repmus=0) and not ignore then myhalt(ERR_NOMAPS);
    for i:=numentry downto 1 do
      if ((replaces>0) and (dirlist^[i].Name=N_THINGS)) or
         ((nreptexture>0) and (dirlist^[i].Name=N_SIDEDEFS)) or
         ((nrepfloor>0) and heretic and (dirlist^[i].Name=N_SECTORS)) then begin
        j:=numt;
        fpos:=dirlist^[i].Start;
        while (j<=MAXENTRY) and (dirlist^[j].Start<>fpos) do inc(j);
        if j>MAXENTRY then begin {this avoid duplications}
          dec(numt);
          dirlist^[numt]:=dirlist^[i];
        end;
      end;
    if numt<=MAXENTRY then begin
      writeln('Processing REPLACEMENTS...');
      maxside:=(longint(numt-1)*sizeof(entry))div sizeof(sidedef);
      for i:=numt to MAXENTRY do with dirlist^[i] do begin
        Seek(f,start);
        if ioresult<>0 then myhalt(ERR_READS);
        if name=N_SIDEDEFS then begin
          k:=rsize div sizeof(sidedef);
          while k>0 do begin
            j:=maxside;
            if j>k then j:=k;
            fpos:=FilePos(f);
            BlockR(f,sidedefs^,j*sizeof(sidedef),size);
            for l:=1 to j do with sidedefs^[l] do
              inc(repside,remap_name(reptexture,a,nreptexture)+
                          remap_name(reptexture,b,nreptexture)+
                          remap_name(reptexture,c,nreptexture));
            Seek(f,fpos);
            if ioresult<>0 then myhalt(ERR_WRITED);
            BlockW(f,sidedefs^,j*sizeof(sidedef),size);
            dec(k,j);
          end;
        end
        else if name=N_THINGS then begin
          BlockR(f,things^,rsize,size);
          ReplaceThings(rsize div sizeof(thing));
          Seek(f,start);
          if ioresult<>0 then myhalt(ERR_WRITED);
          BlockW(f,things^,rsize,size);
        end
        else if name=N_SECTORS then begin
          BlockR(f,sectors^,rsize,size);
          if (nrepfloor>0) and heretic then
            for j:=1 to rsize div sizeof(sector) do with sectors^[j] do
              inc(repfloo,remap_name(repfloor,a,nrepfloor)+
                          remap_name(repfloor,b,nrepfloor));
          if heretic then
            for j:=1 to rsize div sizeof(sector) do with sectors^[j] do
              if action=17 then action:=1;
          Seek(f,start);
          if ioresult<>0 then myhalt(ERR_WRITED);
          BlockW(f,sectors^,rsize,size);
        end
        else if name=N_LINEDEFS then begin
          BlockR(f,linedefs^,rsize,size);
            for j:=1 to rsize div sizeof(linedef) do with linedefs^[j] do
              if (action>=99) and (action<=141) then
                action:=remap_action[action];
          Seek(f,start);
          if ioresult<>0 then myhalt(ERR_WRITED);
          BlockW(f,linedefs^,rsize,size);
        end;
      end;
    end;
    Close(f);
    write('OK, Remapped:');
    Plural(replev,'level');
    write(',');
    Plural(repmus,'music');
    write(',');
    Plural(repside,'texture');
    write(',');
    if heretic then begin
      Plural(repfloo,'floor');
      write(',');
    end;
    Plural(repthing,'object');
    writeln('.');
  end;

function HeapCheck(size:Word):Integer; far;
  begin
    HeapCheck:=1;
  end;

begin
  HeapError:=@HeapCheck;
  new(reptexture);
  new(repfloor);
  new(repdirs);
  new(buffer);
  if (reptexture=nil) or (repfloor=nil) or (repdirs=nil) or
     (buffer=nil) then myhalt(ERR_NOMEM);
  dirlist:=pointer(buffer);
  sidedefs:=pointer(buffer);
  sectors:=pointer(buffer);
  linedefs:=pointer(buffer);
  things:=pointer(buffer);

  nreptexture:=0;
  nrepfloor:=0;
  nrepdirs:=0;
  CreateTable;
  Parse;
  if heretic then begin
    if do_texture then CopyTable(reptexture,@htexture_table,nreptexture);
    if do_floor then CopyTable(repfloor,@hfloor_table,nrepfloor);
  end
  else begin
    if do_texture then CopyTable(reptexture,@texture_table,nreptexture);
  end;
  if show_help then Help
  else if show_list then List
  else if show_example then More
  else if show_note then Notes
  else Process;
end.
