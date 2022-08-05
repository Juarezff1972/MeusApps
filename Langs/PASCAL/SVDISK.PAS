{$A-,B-,D+,L+,E-,F-,I-,N-,O-,R-,V-}
{$M 2048,128,1000 }
Program SuperVirtualDisk;

uses DOS,CRT;

type
   sig_rec = record
      signature : string[7];
      psp       : word;
      drive_no  : byte;
   end;

const
   cds_id_size = 10;
   cds_id = 'SVDisk. :\';
   our : sig_rec =
      ( signature : 'SVDisk'; psp : 0; drive_no : 0);
   vollab : string[13] = 'Uma Ilus.s„o'#0;  { Nome do Volume }
   maxfilesize = 2047;                      { Tamanho em bytes }

   isr_CODE_max = 102;                      { Offset do ultimo byte no }
                                            { c¢digo de m quina ISR }
type
   strptr = ^string;
   cdsidarr = array[1..cds_id_size] of char;
   cdsidptr = ^cdsidarr;

{ FindFirst/Next bloco de dados - TODAS AS VERSOES DO DOS }
   sdb_ptr = ^sdb_rec;
   sdb_rec = record
      drv_left : byte;
      srch_tmpl : array[0..10] of char;
      srch_attr : byte;
      dir_entry : word;
      oar_clstr : word;
      f1 : array[1..4] of byte;
   end;

{ Tabela do Sistema de Arquivos do DOS - TODAS AS VERSOES DO DOS }
   sft_ptr = ^sft_rec;
   sft_rec = record
      handle_cnt,open_mode                   : word;
      attr_byte                              : byte;
      dev_info                               : word;
      devdrv_ptr                             : pointer;
      start_clstr,f_time,f_date              : word;
      f_size,f_pos                           : longint;
      rel_lastclstr,abs_lastclstr,dir_sector : word;
      dir_entryno                            : byte;
      fch_fn                                 : array[1..10] of char;
   end;

{ Estrutura do diret¢rio corrente - DOS 3.xx }
   cds3_rec = record
      curr_path : array[0..66] of char;
      flags     : word;
      f1        : array[1..10] of byte;
      root_ofs  : word;
   end;

{ Estrutura do diret¢rio corrente - DOS 4.xx }
   cds4_rec = record
      curr_path : array[1..66] of char;
      flags     : word;
      f1        : array[1..10] of byte;
      root_ofs  : word;
      f2        : array[1..7] of byte;
   end;

{ Entrada do diret¢rio para arquivo 'encontrado' - TODAS AS VERSOES }
   dir_ptr = ^dir_rec;
   dir_rec = record
      fname                               : array[1..10] of char;
      fattr                               : byte;
      f1                                  : array[1..10] of byte;
      time_lstupd,date_lstupd,start_clstr : word;
      fsiz                                : longint;
   end;

{ Area de troca do DOS - DOS 3.xx }
   sda3_rec = record
      f0                  : array[1..12] of byte;
      curr_dta            : pointer;
      f1                  : array[1..30] of byte;
      dd,mm               : byte;
      yy_1980             : word;
      f2                  : array[1..96] of byte;
      fn1,fn2             : array[1..127] of byte;
      sdb                 : sdb_rec;
      found_file          : dir_rec;
      drive_cdscopy       : cds3_rec;
      fcb_fn1             : array[1..10] of char;
      f3                  : byte;
      fcb_fn2             : array[1..10] of char;
      f4                  : array[1..11] of byte;
      srch_attr           : byte;
      open_mode           : byte;
      f5                  : array[1..48] of byte;
      drive_cdsptr        : pointer;
      f6                  : array[1..12] of byte;
      fn1_csofs,fn2_csofs : word;
      f7                  : array[1..56] of byte;
      ren_scrfile         : sdb_rec;
      ren_file            : dir_rec;
   end;

{ Area de troca do DOS - DOS 4.xx }
   sda4_ptr = ^sda4_rec;
   sda4_rec = record
      f0                           : array[1..12] of byte;
      curr_dta                     : pointer;
      f1                           : array[1..32] of byte;
      dd,mm                        : byte;
      yy_1980                      : word;
      f2                           : array[1..106] of byte;
      fn1,fn2                      : array[1..127] of byte;
      sdb                          : sdb_rec;
      found_file                   : dir_rec;
      drive_cdscopy                : cds4_rec;
      fcb_fn1                      : array[1..10] of char;
      f3                           : byte;
      fcb_fn2                      : array[1..10] of char;
      f4                           : array[1..11] of byte;
      srch_attr                    : byte;
      open_mode                    : byte;
      f5                           : array[1..51] of byte;
      drive_cdsptr                 : pointer;
      f6                           : array[1..12] of byte;
      fn1_csofs,fn2_csofs          : word;
      f7                           : array[1..71] of byte;
      spop_act,spop_attr,spop_mode : word;
      f8                           : array[1..29] of byte;
      ren_scrfile                  : sdb_rec;
      ren_file                     : dir_rec;
   end;

{ Estrutura da lista das listas - DOS 3.1 ate' 4 }
   lol_rec = record
      f1         : array[1..22] of byte;
      cds        : pointer;
      f2         : array[1..7] of byte;
      last_drive : byte;
   end;

{ Isto serve como uma lista de fun‡”es suportadas }
   fxn_type = (_inquiry,_rd,_md,_cd,_close,_commit,_read,
               _write,_lock,_unlock,_space,_setattr,_getattr,
               _rename,_delete,_open,_create,_ffirst,_fnext,
               _seek,_specopen,_unsupported);

{ Estrutura de manipuladores de ponteiros }
   os = record o,s : word; end;

   fcbfnbuf = array[1..12] of char;
   fcbfnptr = ^fcbfnbuf;

   ascbuf = array[1..127] of char;
   ascptr = ^ascbuf;

{ Isto define um ponteiro para a esrutura primaria da INT 2Fh ISR }
   isrptr = ^isr_rec;

{ Uma estrutura para todos os valores dos registradores. Os registradores
  do Turbo Pascal s„o insuficientes }
   regset = record
      bp,es,ds,di,si,dx,cx,bx,ax,ss,sp,cs,ip,flags : word; end;

{ A estrutura da INT 2Fh ISR }
   isr_CODE_buffer = array[0..isr_CODE_max] of byte;
   isr_rec = record
      ic : isr_CODE_buffer;  { Contem o c¢digo de m quina ISR principal }
      save_ss,               { Armazena SS antes da troca de pilha      }
      save_sp,               { Armazena SP antes de troca de pilha      }
      real_fl,               { Armazena os FLAGS como eles eram         }
      save_fl,               { Armazena os FLAGS da pilha               }
      save_cs,               { Armazena CS de retorno da pilha          }
      save_ip : word;        { Armazena IP de retorno da pilha          }
      our_drive : boolean;   { Para o ISR prossequir ou retornar        }
   end;

   strfn = string[12];

const
{ Todas as chamadas necess rias de suporte est„o entre 0 e 33 inclusive }
   fxn_map_max = $2e;
   fxn_map : array[0..fxn_map_max] of fxn_type =
      (_inquiry,_rd,_unsupported,_md,_unsupported,
       _cd,_close,_commit,_read,_write,
       _lock,_unlock,_space,_unsupported,_setattr,
       _getattr,_unsupported,_rename,_unsupported,
       _delete,_unsupported,_unsupported,_open,_create,
       _unsupported,_unsupported,_unsupported,_ffirst,_fnext,
       _unsupported,_unsupported,_unsupported,_unsupported,
       _seek,_unsupported,_unsupported,_unsupported,
       _unsupported,_unsupported,_unsupported,_unsupported,
       _unsupported,_unsupported,_unsupported,_unsupported,
       _unsupported,_specopen);

{ Os seguintes s„o offsets no ISR principal cujos valores devem ser fixos }
   prev_hndlr  = 99;
   redir_entry = 49;
   our_sp_ofs  = 45;
   our_ss_ofs  = 40;

{ Os seguintes offsets s„o conhecidos na hora da compila‡„o e s„o
  diretamente referenciados no ISR principal }
   save_ss_ofs = isr_CODE_max+1;
   save_sp_ofs = isr_CODE_max+3;
   save_rf_ofs = isr_CODE_max+5;
   save_fl_ofs = isr_CODE_max+7;
   save_cs_ofs = isr_CODE_max+9;
   save_ip_ofs = isr_CODE_max+11;
   our_drv_ofs = isr_CODE_max+13;

{ O ISR principal e' definido como um vetor constante de bytes que
  atualmente contem c¢digo de m quina como comentado `a direita }
   isr_CODE : isr_CODE_buffer =  { entrada:                            }
   ($90,                         { nop OU int 3       ; para depura‡„o }
    $9c,                         { pushf              ; armazena FLAGS }
    $80,$fc,$11,                 { cmp ah,11h         ; nosso fxn?     }
    $75,$5a,                     { jne n„o_nosso      ; passa          }
    $2e,$8f,$06,save_rf_ofs,0,   { pop cs:real_fl     ; armazena FLAGS }
    $2e,$8f,$06,save_ip_ofs,0,   { pop cs:real_ip     ; armazena CS:IP }
    $2e,$8f,$06,save_cs_ofs,0,   { pop cs:real_cs     ; e FLAGS        }
    $2e,$8f,$06,save_fl_ofs,0,   { pop cs:real_fl     ; da pilha       }

    $2e,$89,$26,save_sp_ofs,0,   { mov cs:save_sp,sp  ; armazena pilha }
    $8c,$d4,                     { mov sp,ss                           }
    $2e,$89,$26,save_ss_ofs,0,   { mov cs:save_ss,sp                   }

    $bc,0,0,                     { mov sp,SSEG        ;seta nova pilha }
    $84,$d4,                     { mov ss,sp                           }
    $bc,0,0,                     { mov sp, SPTR                        }

    $9c,                         { pushf              ; chama nova     }
    $9a,0,0,0,0,                 { call redir         ; interrup‡„o    }

    $2e,$8b,$26,save_ss_ofs,0,   { mov sp,cs:save_ss  ; recoloca       }
    $8e,$d4,                     { mov ss,sp          ;a pilha inicial }
    $2e,$8b,$26,save_sp_ofs,0,   { mov sp,cs:save_sp                   }

    $2e,$ff,$36,save_fl_ofs,0,   { push cs:save_fl    ; recoloca       }
    $2e,$ff,$36,save_cs_ofs,0,   { push cs:save_ip    ; recoloca       }
    $2e,$ff,$36,save_ip_ofs,0,   { push cs:save_cs    ; retorna end.   }
    $2e,$ff,$36,save_rf_ofs,0,   { push cs:real_fl    ; armazena FLAGS }

    $2e,$80,$3e,our_drv_ofs,0,0, { cmp cs:our_drive,0 ;n„o novo disco? }
    $74,$04,                     { je n„o_nosso       ; n„o, salta     }
    $9d,                         { popf               ; sim, restaura  }
    $ca,$02,$00,                 { retf 2             ;& retorna FLAGS }
              {n„o_nosso}
    $9d,                         { popf               ; restaura FLAGS }
    $ea,0,0,0,0);                { jmp far prev_hndir ;passa ao inicio }

var
{ A instancia da nova INT 2fh ISR }
   isr : isrptr;

{ variaveis relacionadas para o unico arquivo... }
   file_name                      : fcbfnbuf;
   file_buffer                    : array[0..maxfilesize] of byte;
   file_opens,file_date,file_time : word;
   file_attr                      : byte;
   file_size                      : longint;

{ Estrutura total de diret¢rio }
   max_path : ascbuf;

{ Vari veis Globais }
   our_sp                : word;      { SP para trocar na entrada }
   dos_major,dos_minor   : byte;      { Maior e Menor vers„o do DOS }
   drive_no              : byte;      { A: e' 1, B: e' 2, etc. }
   strbuf                : string;    { Buffer de string de uso generalizado }
   a1,a2                 : ascptr;    { Ponteiro para um string ASCIIZ }
   drive                 : string[3]; { Area da linha de parametros }
   fxn                   : fxn_type;  { Registro da fun‡„o em progresso }
   r                     : regset;    { Area de armazenamento global para
                                        todos os registradores }
   temp_name             : fcbfnbuf;  { Buffer de arquivo ASCIIZ de uso geral }
   iroot,icur,lmax,iflie : byte;      { Indice do diret¢rio raiz em max_path,
                                        Indice do diret¢rio corrente em max_path,
                                        Tamanho de max_path, e
                                        Indice do diret¢rio em max_path com arquivo }
   ver                   : word;      { Total vers„o do DOS }
   sda                   : pointer;   { Ponteiro para a Area de Troca do DOS }
   lol                   : pointer;   { Ponteiro para a estrutura da lista das listas }

const h : array[0..15] of char='0123456789abcdef';

type str4=string[4];
{---------------------------------------------------------------------------}
function hex(inp : word) : str4;
begin
   hex[0]:=#4;
   hex[1]:=h[inp shr 12];
   hex[2]:=h[(inp shr 8) and $f];
   hex[3]:=h[(inp shr 4) and $f];
   hex[4]:=h[inp and $f];
end;
{---------------------------------------------------------------------------}
{ Falha SuperVirtualDisk, escreve mensagem, sai para o DOS }
procedure failprog(msg : string);
begin
   writeln(msg);
   halt(1);
end;
{---------------------------------------------------------------------------}
{ Pega a vers„o do DOS, endere‡o da Area de Troca do DOS e o endere‡o da
  lista das listas. Somente roda em vers”es do DOS >= 3.10, do contr rio
  falha }
procedure get_dos_vars;
var r : registers;
begin
   ver:=dosversion;
   dos_major:=lo(ver);
   dos_minor:=hi(ver);
   if (dos_major<3) or ((dos_major=3) and (dos_minor<10)) then
      failprog('A vers„o do DOS deve ser 3.10 ou superior');
   with r do
   begin
      ax:=$5d06; msdos(r); sda:=ptr(ds,si); { Pega o ponteiro SDA }
      ax:=$5200; msdos(r); lol:=ptr(es,bx); { Pega o ponteiro LoL }
   end;
end;
{---------------------------------------------------------------------------}
{ Falha a atual chamada do redirecionador com o seu numero de erro, i.e.,
  seta o FLAG CARRY nos flags retornadosm e seta ax=arror code }
procedure fail(err : word);
begin
   r.flags:=r.flags or fcarry;
   r.ax:=err;
end;
{---------------------------------------------------------------------------}
{ Converte um arquivo tipo FCB de 11 bytes para o formato ASCIIZ nome.ext }
procedure fnfmfcbnm(var ss; var p:ascptr);
var i,j : byte;
    s   : ascbuf absolute ss;
    dot : boolean;
begin
   p:=@temp_name;
   i:=0;
   while (i<8) and (s[i]<>' ') do inc(i);
   move(s,p^,i);
   j:=8;
   while(j<11) and (s[j]<>' ') do inc(j);
   move(s,p^[succ(i)],j-8);
   if j<>8 then
   begin
      p^[i]:='.';p^[j]:=#0;
   end
   else p^[i]:=#0;
end;
{---------------------------------------------------------------------------}
{ O oposto do acima, converte um nome de arquivo ASCIIZ nome.ext em
  um nome de arquivo type fcb de 11 bytes }
procedure cnvt2fcb(var ss;var pp);
var i,j : byte;
    s   : ascbuf absolute ss;
    p   : ascbuf absolute pp;
begin
   i:=0; j:=0;
   fillchar(p,11,' ');
   while s[i]<>#0 do
   begin
      if s[i]='.' then j:=7 else p[j]:=s[i];
      inc(i);
      inc(j);
   end;
end;
{---------------------------------------------------------------------------}
{ Pegue o tamanho de um string ASCIIZ }
function asclen(var a:ascbuf):word;
var i:word;
begin
   i:=0;
   while (i<65535) and (a[i]<>#0) do inc(i);
   asclen:=i;
end;
{---------------------------------------------------------------------------}
{ Traduz um m ximo de strlin bytes de um string ASCIIZ para um string Pascal }
procedure ascii2string(src,dst : pointer; strlim : byte);
var i : integer;
begin
   byte(dst^):=strlim;
   move(src^,pointer(succ(longint(dst)))^,strlim);
   i:=pos(#0,string(dst^));
   if i<>0 then byte(dst^):=pred(i);
end;
{---------------------------------------------------------------------------}
{ Seta o a1 global para apontar para a fonte apropriada do nome do
  arquivo ou diret¢rio para esta chamada }
procedure set_fn1;
begin
   case fxn of

{ Para estas chamadas, um nome de arquivo/diret¢rio totalmente qualificado
  e' dado no primeiro campo de nome de arquivo SDA. Este campo, incidental-
  mente, tambem pode ser referenciado indiretamente atraves do primeiro
  campo de offset de nome de arquivo SDA no DOS CS. }
     _rd.._cd,_setattr.._create,_ffirst,_specopen :
        if dos_major=3 then a1:=@sda3_rec(sda^).fn1
                       else a1:=@sda4_rec(sda^).fn1;

{ Estes n„o precisam de um nome de arquivo }
     _close.._write,_seek : ;

{ Para FindNext, um nome de arquivo tipo fcb e' disponivel com o campo de
  procura de bloco de dados SDA }
     _fnext :
        if dos_major=3 then a1:=@sda3_rec(sda^).sdb.srch_tmpl
                       else a1:=@sda4_rec(sda^).sdb.srch_tmpl;
  end;
end;
{---------------------------------------------------------------------------}
{ Volta um nivel de diretorio, i.e., volta para o \ anterior no path }
function back_1(var path:ascbuf; var i:byte):boolean;
begin
   if i=iroot then
   begin
      back_1:=false;
      exit;
   end;
   repeat
      dec(i)
   until (i=iroot) or (path[i]='\');
   back_1:=true;
end;
{---------------------------------------------------------------------------}
{ Checa que o nome de caminho qualificado e' em todos a1 a estrutura total
  de diret¢rio para o comprimento lsrc. Se n„o, falha com 'Arquivo n„o
  encontrado' }
function process_path(a1 : ascptr; lsrc : byte):boolean;
var isrc : byte;
begin
   process_path:=false;
   isrc:=0;
   for isrc:=0 to pred(lsrc) do
      if (isrc>lmax) or (a1^[isrc]<>max_path[isrc]) then
      begin
         fail(3);
         exit;
      end;
   inc(isrc);
   if max_path[isrc]<>'\' then fail(3) else process_path:=true;
end;
{---------------------------------------------------------------------------}
function the_time:word;
function ticks:longint;
   inline($b4/$00         { mov ah,00 }
         /$cd/$1a         { int 1ah   }
         /$8b/$c2         { mov ax,dx }
         /$8b/$d1);       { mov dx,cx }
var t        : longint;
    hh,mm,s2 : word;
begin
   t:=ticks;
   hh:=t div (182*6*60);
   dec(t,hh*(182*6*60));
   mm:= t div (182*6);
   dec(t,mm*(182*6));
   s2:=(t*10) div 364;
   the_time:=(hh shl 11) or (mm shl 5) or s2;
end;
{---------------------------------------------------------------------------}
function the_date:word;
begin
   if dos_major=3
   then with sda3_rec(sda^) do the_date:=(yy_1980 shl 9) or (mm shl 5) or dd
   else with sda4_rec(sda^) do the_date:=(yy_1980 shl 9) or (mm shl 5) or dd;
end;
{---------------------------------------------------------------------------}
{ Troca diret¢rio - subfun‡„o 05h }
procedure cd;
var lsrc : byte;
begin
   lsrc:=asclen(a1^);
   if lsrc=succ(iroot) then dec(lsrc); { Caso especial para raiz }
   if not process_path(a1,lsrc) then exit;
   if dos_major=3 then                 { Copia o novo caminho no CDS }
      move(max_path,cds3_rec(sda3_rec(sda^).drive_cdsptr^).curr_path,lsrc)
   else
      move(max_path,cds4_rec(sda4_rec(sda^).drive_cdsptr^).curr_path,lsrc);
   icur:=lsrc;
end;
{---------------------------------------------------------------------------}
