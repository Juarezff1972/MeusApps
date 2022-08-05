program teste;
type
    proc = procedure;
    ptrproc = ^proc;
    terminais = (TIdentifier,TComma,TPoint,TAssign,TColon,TIntConst,
                TSemiColon,TPlus,TEqual,TMinus,TComentOpen,TComentClose,
                TRParenthesis,TLParenthesis,TRBracket,TLBracket,
                TSlashBar,TPointer,TNEqual,TAsterisk,
                TLEqual,TLess,TGEqual,TGreater,TSetSep,
                TProgram,TBegin,TEnd,TVar,TProcedure,TFunction,TLabel,TFile,TSet,TRecord,
                TArray,TType,TConst,TGoto,TWith,TDownTo,TTo,TFor,TDo,TWhile,
                TUntil,TRepeat,TOf,TCase,TElse,TThen,TIf,TNot,TAnd,TOr,TIn,TNil,
                TMod,TDiv,TInteger,TChar,TReal,TBoolean,TTrue,TFalse,
                TEmpty, TNenhum, TQualquer);

    classe = (Inteiro, PalRes, Identif, Simples, Composto, Constante, Tipo, Nenhuma);

    registro = record
                 tipo:char;
                 simbolo:integer;
                 cl:classe;
                 proximo:integer;
                 alternativa:integer;
               end;

    restype = record
                nome:string;
                valor:terminais;
              end;

const
    PalReserv:array [0..34] of restype = ((nome:'program';  valor:TProgram),
                                          (nome:'begin';    valor:TBegin),
                                          (nome:'end';      valor:TEnd),
                                          (nome:'var';      valor:TVar),
                                          (nome:'procedure';valor:TProcedure),
                                          (nome:'function'; valor:TFunction),
                                          (nome:'label';    valor:TLabel),
                                          (nome:'file';     valor:TFile),
                                          (nome:'set';      valor:TSet),
                                          (nome:'record';   valor:TRecord),
                                          (nome:'array';    valor:TArray),
                                          (nome:'type';     valor:TType),
                                          (nome:'const';    valor:TConst),
                                          (nome:'goto';     valor:TGoto),
                                          (nome:'with';     valor:TWith),
                                          (nome:'begin';    valor:TBegin),
                                          (nome:'downto';   valor:TDownTo),
                                          (nome:'to';       valor:TTo),
                                          (nome:'for';      valor:TFor),
                                          (nome:'do';       valor:TDo),
                                          (nome:'while';    valor:TWhile),
                                          (nome:'until';    valor:TUntil),
                                          (nome:'repeat';   valor:TRepeat),
                                          (nome:'of';       valor:TOf),
                                          (nome:'case';     valor:TCase),
                                          (nome:'else';     valor:TElse),
                                          (nome:'then';     valor:TThen),
                                          (nome:'if';       valor:TIf),
                                          (nome:'not';      valor:TNot),
                                          (nome:'and';      valor:TAnd),
                                          (nome:'or';       valor:TOr),
                                          (nome:'in';       valor:TIn),
                                          (nome:'nil';      valor:TNil),
                                          (nome:'mod';      valor:TMod),
                                          (nome:'div';      valor:TDiv)
    );

    TabTipo:array [0..3] of restype = ((nome:'integer';  valor:TInteger),
                                       (nome:'boolean';  valor:TBoolean),
                                       (nome:'char';     valor:TChar),
                                       (nome:'real';     valor:TReal)
    );

    TabConst:array [0..1] of restype = ((nome:'false'; valor:TFalse),
                                        (nome:'true';  valor:TTrue)
    );


const
    VPrograma = 0;
    VBloco = 6;
    VParam = 32;
    VExpSimp = 83;
    VComSRot = 45;
    VComandos = 41;
    VTermo = 90;
    VFator = 95;
    VExpressao = 74;

{    Tab:array [0..39]  of registro = ((Tipo:'T'; Simbolo:integer (TProgram);    cl:PalRes;  Proximo: 1; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TIdentifier); cl:Identif; Proximo: 2; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TColon);      cl:Simples; Proximo: 3; Alternativa:-1),
                                      (Tipo:'N'; Simbolo:integer (VBloco);      cl:Nenhuma; Proximo: 4; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TPoint);      cl:Simples; Proximo: 5; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TEmpty);      cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                      (Tipo:'T'; Simbolo:integer (TType);       cl:PalRes;  Proximo: 7; Alternativa:12),
                                      (Tipo:'T'; Simbolo:integer (TIdentifier); cl:Identif; Proximo: 8; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TEqual);      cl:Simples; Proximo: 9; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TInteger);    cl:Tipo;    Proximo:11; Alternativa:10),
                                      (Tipo:'T'; Simbolo:integer (TBoolean);    cl:Tipo;    Proximo:11; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TColon);      cl:Simples; Proximo:12; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TIdentifier); cl:Identif; Proximo:13; Alternativa:17),
                                      (Tipo:'T'; Simbolo:integer (TEqual);      cl:Simples; Proximo:14; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TInteger);    cl:Tipo;    Proximo:16; Alternativa:15),
                                      (Tipo:'T'; Simbolo:integer (TBoolean);    cl:Tipo;    Proximo:16; Alternativa:-1),

                                      (Tipo:'T'; Simbolo:integer (TConst);      cl:PalRes;  Proximo:18; Alternativa:28),
                                      (Tipo:'T'; Simbolo:integer (TIdentifier); cl:Identif; Proximo:19; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TEqual);      cl:Simples; Proximo:20; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TTrue);       cl:Constante;Proximo:22; Alternativa:21),
                                      (Tipo:'T'; Simbolo:integer (TFalse);      cl:Constante;Proximo:22; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TColon);      cl:Simples; Proximo:23; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TIdentifier); cl:Identif; Proximo:24; Alternativa:28),
                                      (Tipo:'T'; Simbolo:integer (TEqual);      cl:Simples; Proximo:25; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TTrue);       cl:Constante;Proximo:27; Alternativa:26),
                                      (Tipo:'T'; Simbolo:integer (TFalse);      cl:Constante;Proximo:27; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TColon);      cl:Simples; Proximo:23; Alternativa:-1),

                                      (Tipo:'T'; Simbolo:integer (TVar);        cl:PalRes;  Proximo:29; Alternativa:39),
                                      (Tipo:'T'; Simbolo:integer (TIdentifier); cl:Identif; Proximo:30; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TColon);      cl:Simples; Proximo:31; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TInteger);    cl:Tipo;    Proximo:33; Alternativa:32),
                                      (Tipo:'T'; Simbolo:integer (TBoolean);    cl:Tipo;    Proximo:33; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TColon);      cl:Simples; Proximo:34; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TIdentifier); cl:Identif; Proximo:35; Alternativa:39),
                                      (Tipo:'T'; Simbolo:integer (TEqual);      cl:Simples; Proximo:36; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TInteger);    cl:Tipo;    Proximo:38; Alternativa:37),
                                      (Tipo:'T'; Simbolo:integer (TBoolean);    cl:Tipo;    Proximo:38; Alternativa:-1),
                                      (Tipo:'T'; Simbolo:integer (TColon);      cl:Simples; Proximo:34; Alternativa:-1),

                                      (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1)
    );
}
    Tab:array [0..109]  of registro = ((Tipo:'T'; Simbolo:integer (TProgram);    cl:PalRes;  Proximo: 1; Alternativa:-1),
                                       (Tipo:'T'; Simbolo:integer (TIdentifier); cl:Identif; Proximo: 2; Alternativa:-1),
                                       (Tipo:'T'; Simbolo:integer (TColon);      cl:Simples; Proximo: 3; Alternativa:-1),
                                       (Tipo:'N'; Simbolo:integer (VBloco);      cl:Nenhuma; Proximo: 4; Alternativa:-1),
                                       (Tipo:'T'; Simbolo:integer (TPoint);      cl:Simples; Proximo: 5; Alternativa:-1),
                                       (Tipo:'T'; Simbolo:integer (TEmpty);      cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'T'; Simbolo:integer (TLabel);      cl:PalRes;  Proximo: 7; Alternativa:10),
                                       (Tipo:'T'; Simbolo:integer (TIntConst);     cl:Inteiro; Proximo: 8; Alternativa:-1),
                                       (Tipo:'T'; Simbolo:integer (TComma);      cl:Simples; Proximo: 7; Alternativa: 9),
                                       (Tipo:'T'; Simbolo:integer (TSemiColon);  cl:Simples; Proximo:16; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TVar);        cl:PalRes;  Proximo:11; Alternativa:16),
                                       (Tipo:'T'; Simbolo:integer (TIdentifier); cl:Identif; Proximo:12; Alternativa:15),
                                       (Tipo:'T'; Simbolo:integer (TComma);      cl:Simples; Proximo:11; Alternativa:13),
                                       (Tipo:'T'; Simbolo:integer (TColon);      cl:Simples; Proximo:14; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TType);       cl:Tipo;    Proximo:15; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TSemiColon);  cl:Tipo;    Proximo:11; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TProcedure);  cl:PalRes;  Proximo:17; Alternativa:22),
                                       (Tipo:'T'; Simbolo:integer (TIdentifier); cl:Identif; Proximo:18; Alternativa: 0),
                                       (Tipo:'N'; Simbolo:integer (VParam);      cl:Nenhuma; Proximo:19; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TSemiColon);  cl:Simples; Proximo:20; Alternativa: 0),
                                       (Tipo:'N'; Simbolo:integer (VBloco);      cl:Nenhuma; Proximo:21; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TSemiColon);  cl:Simples; Proximo:16; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TFunction);   cl:PalRes;  Proximo:23; Alternativa:27),
                                       (Tipo:'T'; Simbolo:integer (TIdentifier); cl:Identif; Proximo:24; Alternativa: 0),
                                       (Tipo:'N'; Simbolo:integer (VParam);      cl:Nenhuma; Proximo:25; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TColon);      cl:Simples; Proximo:26; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TType);       cl:Tipo;    Proximo:19; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TBegin);      cl:PalRes;  Proximo:28; Alternativa: 0),
                                       (Tipo:'N'; Simbolo:integer (VComandos);   cl:Nenhuma; Proximo:29; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TSemiColon);  cl:Simples; Proximo:28; Alternativa:30),
                                       (Tipo:'T'; Simbolo:integer (TEnd);        cl:PalRes;  Proximo:31; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'T'; Simbolo:integer (TRParenthesis);    cl:Simples; Proximo:33; Alternativa:40),
                                       (Tipo:'T'; Simbolo:integer (TVar);        cl:PalRes;  Proximo:34; Alternativa:34),
                                       (Tipo:'T'; Simbolo:integer (TIdentifier); cl:Identif; Proximo:35; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TComma);      cl:Simples; Proximo:34; Alternativa:36),
                                       (Tipo:'T'; Simbolo:integer (TColon);      cl:Simples; Proximo:37; Alternativa:40),
                                       (Tipo:'T'; Simbolo:integer (TType);       cl:Tipo;    Proximo:38; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TSemiColon);  cl:Simples; Proximo:33; Alternativa:39),
                                       (Tipo:'T'; Simbolo:integer (TLParenthesis);    cl:Simples; Proximo:40; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'T'; Simbolo:integer (TIntConst);   cl:Inteiro; Proximo:42; Alternativa:43),
                                       (Tipo:'T'; Simbolo:integer (TColon);      cl:Simples; Proximo:43; Alternativa: 0),
                                       (Tipo:'N'; Simbolo:integer (VComSRot);    cl:Nenhuma; Proximo:44; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'T'; Simbolo:integer (TGoto);       cl:PalRes;  Proximo:46; Alternativa:47),
                                       (Tipo:'T'; Simbolo:integer (TIntConst);   cl:Inteiro; Proximo:47; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'T'; Simbolo:integer (TIf);         cl:PalRes;  Proximo:49; Alternativa:55),
                                       (Tipo:'N'; Simbolo:integer (VExpSimp);    cl:Nenhuma; Proximo:50; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TThen);       cl:PalRes;  Proximo:51; Alternativa: 0),
                                       (Tipo:'N'; Simbolo:integer (VComSRot);    cl:Nenhuma; Proximo:52; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TElse);       cl:PalRes;  Proximo:53; Alternativa:54),
                                       (Tipo:'N'; Simbolo:integer (VComSRot);    cl:Nenhuma; Proximo:54; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'T'; Simbolo:integer (TWhile);      cl:PalRes;  Proximo:56; Alternativa:60),
                                       (Tipo:'N'; Simbolo:integer (VExpSimp);    cl:Nenhuma; Proximo:57; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TDo);         cl:PalRes;  Proximo:58; Alternativa: 0),
                                       (Tipo:'N'; Simbolo:integer (VComSRot);    cl:Nenhuma; Proximo:59; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                      (Tipo:'T'; Simbolo:integer (TBegin);      cl:PalRes;  Proximo:61; Alternativa:65),
                                       (Tipo:'N'; Simbolo:integer (VComandos);   cl:Nenhuma; Proximo:62; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TSemiColon);  cl:Simples; Proximo:61; Alternativa:63),
                                       (Tipo:'T'; Simbolo:integer (TEnd);        cl:PalRes;  Proximo:64; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'T'; Simbolo:integer (TIdentifier); cl:Identif; Proximo:66; Alternativa:71),
                                       (Tipo:'T'; Simbolo:integer (TAssign);     cl:Composto;Proximo:67; Alternativa:71),
                                       (Tipo:'T'; Simbolo:integer (TRParenthesis);cl:Simples; Proximo:68; Alternativa:72),
                                       (Tipo:'N'; Simbolo:integer (VExpressao);  cl:Nenhuma; Proximo:69; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TComma);      cl:Simples; Proximo:68; Alternativa:70),
                                       (Tipo:'T'; Simbolo:integer (TLParenthesis);cl:Simples; Proximo:71; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'N'; Simbolo:integer (VExpressao);  cl:Nenhuma; Proximo:73; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'N'; Simbolo:integer (VExpSimp);    cl:Nenhuma; Proximo:75; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TEqual);      cl:Simples; Proximo:81; Alternativa:76),
                                       (Tipo:'T'; Simbolo:integer (TNEqual);     cl:Composto;Proximo:81; Alternativa:77),
                                       (Tipo:'T'; Simbolo:integer (TLess);       cl:Simples; Proximo:81; Alternativa:78),
                                       (Tipo:'T'; Simbolo:integer (TLEqual);     cl:Composto;Proximo:81; Alternativa:79),
                                       (Tipo:'T'; Simbolo:integer (TGEqual);     cl:Composto;Proximo:81; Alternativa:80),
                                       (Tipo:'T'; Simbolo:integer (TGreater);    cl:Simples; Proximo:81; Alternativa:82),
                                       (Tipo:'N'; Simbolo:integer (VExpSimp);    cl:Nenhuma; Proximo:82; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'T'; Simbolo:integer (TPlus);       cl:Simples; Proximo:85; Alternativa:84),
                                       (Tipo:'T'; Simbolo:integer (TMinus);      cl:Simples; Proximo:85; Alternativa:85),
                                       (Tipo:'N'; Simbolo:integer (VTermo);      cl:Nenhuma; Proximo:86; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TPlus);       cl:Simples; Proximo:85; Alternativa:87),
                                       (Tipo:'T'; Simbolo:integer (TMinus);      cl:Simples; Proximo:85; Alternativa:88),
                                       (Tipo:'T'; Simbolo:integer (TOr);         cl:PalRes;  Proximo:85; Alternativa:89),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'N'; Simbolo:integer (VFator);      cl:Nenhuma; Proximo:91; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TAsterisk);    cl:Simples; Proximo:90; Alternativa:92),
                                       (Tipo:'T'; Simbolo:integer (TDiv);        cl:Identif; Proximo:90; Alternativa:93),
                                       (Tipo:'T'; Simbolo:integer (TAnd);        cl:PalRes;  Proximo:90; Alternativa:94),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'T'; Simbolo:integer (TIdentifier); cl:Identif; Proximo:96; Alternativa:101),
                                       (Tipo:'T'; Simbolo:integer (TRParenthesis);cl:Simples; Proximo:97; Alternativa:100),
                                       (Tipo:'N'; Simbolo:integer (VExpressao);  cl:Nenhuma; Proximo:98; Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TComma);      cl:Simples; Proximo:97; Alternativa:99),
                                       (Tipo:'T'; Simbolo:integer (TLParenthesis);cl:Simples; Proximo:100;Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'T'; Simbolo:integer (TIntConst);   cl:Inteiro; Proximo:102;Alternativa:103),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'T'; Simbolo:integer (TRParenthesis);cl:Simples; Proximo:104;Alternativa:107),
                                       (Tipo:'N'; Simbolo:integer (VExpressao);  cl:Nenhuma; Proximo:105;Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TRParenthesis);cl:Simples; Proximo:106;Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1),

                                       (Tipo:'T'; Simbolo:integer (TNot);        cl:PalRes;  Proximo:108;Alternativa: 0),
                                       (Tipo:'N'; Simbolo:integer (VFator);      cl:Nenhuma; Proximo:109;Alternativa: 0),
                                       (Tipo:'T'; Simbolo:integer (TQualquer);   cl:Nenhuma; Proximo:-1; Alternativa:-1)
    );

var
    f:file of char;
    Linha, Coluna:integer;
    lookahead:boolean;
    NextChar:char;
    CurrentPos:longint;

function GetChar (var pos : longint):char;
var
    ch:char;
begin
    seek (f, pos);s    pos := pos + 1;
    read (f, ch);
    if (Eof (f))
      then
        ch := '$';
    GetChar := ch;
end;

function Reserved (s:string):Terminais;
var
    inx:integer;
    ret:Terminais;
    encontrou:boolean;
begin
    inx := 0;
    encontrou := false;
    while (inx < 35) and (not encontrou) do
      begin
        if (s = PalReserv [inx].Nome)
          then
            begin
              ret := PalReserv [inx].Valor;
              encontrou := true;
            end;
        inx := inx + 1;
      end;
    if (not encontrou)
      then
        ret := TIdentifier;
    Reserved := ret;
end;


function FTipo (s:string):Terminais;
var
    inx:integer;
    ret:Terminais;
    encontrou:boolean;
begin
    inx := 0;
    encontrou := false;
    while (inx < 4) and (not encontrou) do
      begin
        if (s = TabTipo [inx].Nome)
          then
            begin
              ret := TabTipo [inx].Valor;
              encontrou := true;
            end;
        inx := inx + 1;
      end;
    if (not encontrou)
      then
        ret := TIdentifier;
    FTipo := ret;
end;

function FConst (s:string):Terminais;
var
    inx:integer;
    ret:Terminais;
    encontrou:boolean;
begin
    inx := 0;
    encontrou := false;
    while (inx < 2) and (not encontrou) do
      begin
        if (s = TabConst [inx].Nome)
          then
            begin
              ret := TabConst [inx].Valor;
              encontrou := true;
            end;
        inx := inx + 1;
      end;
    if (not encontrou)
      then
        ret := TIdentifier;
    FConst := ret;
end;


function ProximoToken (var tk:string; var cl:classe): Terminais;
var
    flag:boolean;
    comp:Terminais;
begin
    flag := false;
    if (not lookahead)
      then
        NextChar := GetChar (CurrentPos)
      else
        lookahead := false;

    tk := '';
    while (not flag) do
      begin
        if (NextChar = '$') then flag:=true;
        if (NextChar <> '$')
          then
            case NextChar of
             #13:begin
                    Linha := Linha + 1;
                    Coluna := 0;
                    NextChar := GetChar (CurrentPos);
                    lookahead := false;
                  end;

             #10:begin
                    NextChar := GetChar (CurrentPos);
                    lookahead := false;
                  end;

             #32:begin
                    Coluna := Coluna + 1;
                    NextChar := GetChar (CurrentPos);
                    lookahead := false;
                  end;

             #08:begin
                    Coluna := Coluna + 8;
                    NextChar := GetChar (CurrentPos);
                    lookahead := false;
                  end;

              '(':begin
                    tk := tk + NextChar;
                    NextChar := GetChar (CurrentPos);
                    if (NextChar = '*')
                      then
                        begin
                          tk := tk + NextChar;
                          cl := composto;
                          comp := TComentOpen;
                          lookahead := false;
                        end
                      else
                        begin
                          cl := Simples;
                          comp := TRParenthesis;
                          lookahead := true;
                        end;
                      flag := true;
                  end;

              ')':begin
                    cl := Simples;
                    tk := tk + NextChar;
                    comp := TLParenthesis;
                    flag := true;
                  end;

              '[':begin
                    cl := Simples;
                    tk := tk + NextChar;
                    comp := TRBracket;
                    flag := true;
                  end;

              ']':begin
                    cl := Simples;
                    tk := tk + NextChar;
                    comp := TLBracket;
                    flag := true;
                  end;

              ',':begin
                    cl := Simples;
                    tk := tk + NextChar;
                    comp := TComma;
                    flag := true;
                  end;

              '''':begin
                     cl := Simples;
                     tk := tk + NextChar;
                     comp := TSlashBar;
                     flag := true;
                   end;

              '=':begin
                    cl := Simples;
                    tk := tk + NextChar;
                    comp := TEqual;
                    flag := true;
                  end;

              '*':begin
                    tk := tk + NextChar;
                    NextChar := GetChar (CurrentPos);
                    if (NextChar = ')')
                      then
                        begin
                          tk := tk + NextChar;
                          cl := composto;
                          comp := TComentClose;
                          lookahead := false;
                        end
                      else
                        begin
                          cl := simples;
                          lookahead := true;
                          comp := TAsterisk;
                        end;
                      flag := true;
                  end;

              '^':begin
                    cl := Simples;
                    tk := tk + NextChar;
                    comp := TPointer;
                    flag := true;
                  end;

              '+':begin
                    cl := Simples;
                    tk := tk + NextChar;
                    comp := TPlus;
                    flag := true;
                  end;

              '-':begin
                    cl := Simples;
                    tk := tk + NextChar;
                    comp := TMinus;
                    flag := true;
                  end;

              ';':begin
                    cl := Simples;
                    tk := tk + NextChar;
                    comp := TColon;
                    flag := true;
                  end;

              ':':begin
                    tk := tk + NextChar;
                    NextChar := GetChar (CurrentPos);
                    if (NextChar = '=')
                      then
                        begin
                          tk := tk + NextChar;
                          cl := Composto;
                          comp := TAssign;
                        end
                      else
                        begin
                          cl := Simples;
                          lookahead := true;
                          comp := TSemiColon;
                        end;
                      flag := true;
                    end;

              '<':begin
                    tk := tk + NextChar;
                    NextChar := GetChar (CurrentPos);
                    if (NextChar = '>')
                      then
                        begin
                          tk := tk + NextChar;
                          comp := TNEqual;
                          cl := Composto;
                          lookahead := false;
                        end
                      else
                        if (NextChar = '=')
                          then
                            begin
                              tk := tk + NextChar;
                              comp := TLEqual;
                              cl := Composto;
                              lookahead := false;
                            end
                          else
                            begin
                              comp := TLess;
                              cl := Simples;
                              lookahead := true;
                              flag := true;
                            end;
              end;

              '>':begin
                    tk := tk + NextChar;
                    NextChar := GetChar (CurrentPos);
                    if (NextChar = '=')
                      then
                        begin
                          tk := tk + NextChar;
                          comp := TGEqual;
                          cl := Composto;
                          lookahead := false;
                        end
                      else
                        begin
                          comp := TGreater;
                          cl := Simples;
                          lookahead := true;
                          flag := true;
                        end;
                  end;

              '.':begin
                    tk := tk + NextChar;
                    NextChar := GetChar (CurrentPos);
                    if (NextChar = '.')
                      then
                        begin
                          tk := tk + NextChar;
                          comp := TSetSep;
                          cl := Composto;
                          lookahead := false;
                        end
                      else
                        begin
                          comp := TPoint;
                          cl := Simples;
                          lookahead := true;
                          flag := true;
                        end;
                  end;

              '0'..'9':begin
                         repeat
                           tk := tk + NextChar;
                           NextChar := GetChar (CurrentPos);
                         until (NextChar < '0') or (NextChar > '9');
                         lookahead := true;
                         cl := Inteiro;
                         comp := TIntConst;
                         flag := true;
                       end;

              'A'..'Z',
              'a'..'z':begin
                         repeat
                           tk := tk + NextChar;
                           NextChar := GetChar (CurrentPos);
                         until ((NextChar < '0') or (NextChar > '9')) and
                               ((NextChar < 'A') or (NextChar > 'Z')) and
                               ((NextChar < 'a') or (NextChar > 'z'));
                         lookahead := true;
                         cl := Identif;
                         comp := TIdentifier;
                         flag := true;
                       end;
              else
                begin
                  { Tratar erros lexicos }
                end
            end;
          end;
    Coluna := Coluna + byte (tk [0]);
    ProximoToken := comp;
end;

function Estrategias (var tk : string; var comp : terminais; var cl : classe; var Ent:integer) : integer;
var
    pos : longint;
    tkaux : string;
    Compaux : terminais;
    claux : classe;
    alt : integer;
    TkEsp : terminais;
    ret : integer;
begin
    ret := 0;

    { Estrategia 1 }
    pos := CurrentPos;
    Compaux := ProximoToken (tkaux, claux);
    alt := Ent;
    while (alt <> -1) and (ret <> 1) do
      if (terminais (Tab [alt].Simbolo) = Compaux)
        then
          begin
            Ent := alt;
            comp := Compaux;
            cl := claux;
            tk := tkaux;
            ret := 1;
            {Ignorou}
          end
        else
          alt := Tab [alt].Alternativa;

    { Estrategia 2 }
    if (ret = 0)
      then
        begin
          alt := Tab [Ent].Proximo;
          while (alt <> -1) and (ret <> 0) do
            if (terminais (Tab [alt].Simbolo) = Comp)
              then
                begin
                  Ent := alt;
                  ret := 2;
                  {Ignorou}
                end
            else
              alt := Tab [alt].Alternativa;
        end;

    { Estrategia 3 }
    if (ret = 0)
      then
        begin
          pos := CurrentPos;
          Compaux := ProximoToken (tkaux, claux);
          alt := Tab [Ent].Proximo;
          while (alt <> -1) and (ret <> 0) do
            if (terminais (Tab [alt].Simbolo) = Compaux)
              then
                begin
                  Ent := alt;
                  comp := Compaux;
                  cl := claux;
                  tk := tkaux;
                  ret := 3;
                  {Ignorou}
                end
              else
                alt := Tab [alt].Alternativa;
        end;

    { Estrategia 4 }
    if (ret = 0)
      then
        begin
        end;
end;

procedure Programa (Ent : integer);
var
    tk:string;
    cl:classe;
    Terminal:Terminais;
    Used : boolean;
    method : integer;
    comp : terminais;

begin
    Used := true;
    while (Tab [Ent].Proximo <> -1) do
      begin
        if (Tab [Ent].Tipo = 'N')
          then
            begin
              Programa (Tab [Ent].Simbolo);
              Ent := Tab [Ent].Proximo;
            end
          else
            begin

              if (Used)
                then
                  Terminal := ProximoToken (tk, cl);

              if (Terminal = TIdentifier)
                then
                  begin
                    Terminal := Reserved (tk);
                    writeln(tk);
                    if (Terminal <> TIdentifier)
                      then
                        if (Tab [Ent].cl = PalRes)
                          then
                            if (Terminais (Tab [Ent].Simbolo) = Terminal)
                              then
                                begin
                                  { Esta Correto }
                                  Ent := Tab [Ent].Proximo;
                                  Used := true;
                                end
                              else
                                begin
                                  if (Tab [Ent].Alternativa = -1)
                                    then
                                      begin
                                        method := Estrategias (tk, comp, cl, Ent);
                                        Used := true;
                                        Ent := Tab [Ent].Proximo;
                                      end
                                    else
                                      begin
                                        Ent := Tab [Ent].Alternativa;
                                        Used := false;
                                      end;
                                end
                          else
                            begin
                              if (Tab [Ent].Alternativa = -1)
                                then
                                  begin
                                    method := Estrategias (tk, comp, cl, Ent);
                                    Used := true;
                                    Ent := Tab [Ent].Proximo;
                                  end
                                else
                                  begin
                                    Ent := Tab [Ent].Alternativa;
                                    Used := false;
                                  end;
                            end
                      else
                        begin
                          Terminal := FTipo (tk);
                          if (Terminal <> TIdentifier)
                            then
                              if (Tab [Ent].cl = Tipo)
                                then
                                  if (Terminais (Tab [Ent].Simbolo) = Terminal)
                                    then
                                      begin
                                        Ent := Tab [Ent].Proximo;
                                        Used := true;
                                      end
                                    else
                                      begin
                                        if (Tab [Ent].Alternativa = -1)
                                          then
                                            begin
                                              method := Estrategias (tk, comp, cl, Ent);
                                              Used := true;
                                              Ent := Tab [Ent].Proximo;
                                            end
                                          else
                                            begin
                                              Ent := Tab [Ent].Alternativa;
                                              Used := false;
                                            end;
                                      end
                                else
                                  begin
                                    if (Tab [Ent].Alternativa = -1)
                                      then
                                        begin
                                          method := Estrategias (tk, comp, cl, Ent);
                                          Used := true;
                                          Ent := Tab [Ent].Proximo;
                                        end
                                      else
                                        begin
                                          Ent := Tab [Ent].Alternativa;
                                          Used := false;
                                        end;
                                  end
                            else
                              begin
                                Terminal := FConst (tk);
                                if (Terminal <> TIdentifier)
                                  then
                                    if (Tab [Ent].cl = Constante)
                                      then
                                        if (Terminais (Tab [Ent].Simbolo) = Terminal)
                                          then
                                            begin
                                              Ent := Tab [Ent].Proximo;
                                              Used := true;
                                            end
                                          else
                                            begin
                                              if (Tab [Ent].Alternativa = -1)
                                                then
                                                  begin
                                                    method := Estrategias (tk, comp, cl, Ent);
                                                    Used := true;
                                                    Ent := Tab [Ent].Proximo;
                                                  end
                                                else
                                                  begin
                                                    Ent := Tab [Ent].Alternativa;
                                                    Used := false;
                                                  end;
                                            end
                                      else
                                        begin
                                          if (Tab [Ent].Alternativa = -1)
                                            then
                                              begin
                                                method := Estrategias (tk, comp, cl, Ent);
                                                Used := true;
                                                Ent := Tab [Ent].Proximo;
                                              end
                                            else
                                              begin
                                                Ent := Tab [Ent].Alternativa;
                                                Used := false;
                                              end;
                                        end
                                else
                                  if (Tab [Ent].cl = Identif)
                                    then
                                      if (Terminais (Tab [Ent].Simbolo) = Terminal)
                                        then
                                          begin
                                            Ent := Tab [Ent].Proximo;
                                            Used := true;
                                          end
                                        else
                                          begin
                                            if (Tab [Ent].Alternativa = -1)
                                              then
                                                begin
                                                  method := Estrategias (tk, comp, cl, Ent);
                                                  Used := true;
                                                  Ent := Tab [Ent].Proximo;
                                                end
                                              else
                                                begin
                                                  Ent := Tab [Ent].Alternativa;
                                                  Used := false;
                                                end;
                                          end
                                    else
                                      begin
                                        if (Tab [Ent].Alternativa = -1)
                                          then
                                            begin
                                              method := Estrategias (tk, comp, cl, Ent);
                                              Used := true;
                                              Ent := Tab [Ent].Proximo;
                                            end
                                          else
                                            begin
                                              Ent := Tab [Ent].Alternativa;
                                              Used := false;
                                            end;
                                      end;
                              end;
                        end;
                    Terminal := TIdentifier;
                  end
                else
                  Ent := Tab [Ent].Proximo;
            end;
      end;
end;

begin
    Assign (f, ParamStr (1));
    Reset (f);
    Linha := 1;
    Coluna := 0;
    lookahead := false;
    CurrentPos := 0;

    Programa (0);

    Close (f);
end.