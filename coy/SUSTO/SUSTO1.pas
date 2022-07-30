unit SUSTO1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ComCtrls, ExtCtrls, Mask, Buttons, Spin;

type
  TForm1 = class (TForm)
    GroupBox1: TGroupBox;
    label1: TLabel;
    Edit1: TEdit;
    ProgressBar1: TProgressBar;
    label2: TLabel;
    Edit2: TEdit;
    ProgressBar2: TProgressBar;
    ProgressBar3: TProgressBar;
    Edit3: TEdit;
    label3: TLabel;
    GroupBox2: TGroupBox;
    label4: TLabel;
    label5: TLabel;
    label6: TLabel;
    Edit4: TEdit;
    ProgressBar4: TProgressBar;
    Edit5: TEdit;
    ProgressBar5: TProgressBar;
    ProgressBar6: TProgressBar;
    Edit6: TEdit;
    GroupBox3: TGroupBox;
    label7: TLabel;
    label8: TLabel;
    label9: TLabel;
    Edit7: TEdit;
    ProgressBar7: TProgressBar;
    Edit8: TEdit;
    ProgressBar8: TProgressBar;
    ProgressBar9: TProgressBar;
    Edit9: TEdit;
    GroupBox4: TGroupBox;
    label10: TLabel;
    label11: TLabel;
    label12: TLabel;
    Edit10: TEdit;
    ProgressBar10: TProgressBar;
    Edit11: TEdit;
    ProgressBar11: TProgressBar;
    ProgressBar12: TProgressBar;
    Edit12: TEdit;
    Edit13: TEdit;
    label13: TLabel;
    Panel1: TPanel;
    label15: TLabel;
    label16: TLabel;
    Lento: TLabel;
    TrackBar1: TTrackBar;
    label14: TLabel;
    MaskEdit1: TMaskEdit;
    label17: TLabel;
    Edit14: TEdit;
    label18: TLabel;
    Timer1: TTimer;
    label19: TLabel;
    Edit15: TEdit;
    Bevel1: TBevel;
    BitBtn1: TBitBtn;
    SpeedButton1: TSpeedButton;
    label20: TLabel;
    Edit16: TEdit;
    label21: TLabel;
    Edit17: TEdit;
    Bevel2: TBevel;
    Bevel3: TBevel;
    SpinEdit1: TSpinEdit;
    Label22: TLabel;
    Bevel4: TBevel;
    Image1: TImage;
    procedure listafilageral (Sender: TObject);
    procedure Timer1Timer (Sender: TObject);
    procedure FormCreate (Sender: TObject);
    procedure TrackBar1Change (Sender: TObject);
    procedure BitBtn1Click (Sender: TObject);
    procedure SpeedButton1Click (Sender: TObject);
    procedure SpinEdit1Change(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  dia:   byte;
  hora:  byte;
  Min:   byte;
  Seg:   byte;
  dmes:  byte;
  fila:  word;
  morte: word;
  desist:word;
  freq:  byte;
  dsem:  array [1..7] of string;
  atend1, atend2, atend3, atend4, atend5, atend6,
  atend7, atend8, atend9, atend10, atend11, atend12: byte;
  pronto1, pronto2, pronto3, pronto4, pronto5, pronto6,
  pronto7, pronto8, pronto9, pronto10, pronto11, pronto12: boolean;

implementation

uses config, pausa;

{$R *.DFM}
////////////////////////////////////////////////////////////////////////////
procedure TForm1.listafilageral (Sender: TObject);
var aux : string;
begin
  STR (trackbar1.position, aux);
  edit13.text := aux;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm1.Timer1Timer (Sender: TObject);
var code, aux          : integer;
  aux1, aux2, aux3    : string;
  tmp1, tmp2, tmp3    : byte;
  fila1, fila2, fila3 : word;
  fila4, fila5, fila6 : word;
  fila7, fila8, fila9 : word;
  fila10, fila11, fila12 : word;
  incremento: byte;
begin
  incremento := TRUNC (RANDOM (freq) ); // número de pacientes que chegam na
                                        // fila por minuto
  INC (Min, freq); // incremento do tempo
  if Min >= 60 then
  begin
    Min := 0;
    INC (hora);
    if hora >= 24 then
    begin
      hora := 0;
      INC (dia);
      INC (dmes);
      if form2.radiobutton1.checked then //pausa diária
      begin
         timer1.enabled:=false;
         form5.show;
      end;
      if dmes >= 32 then
      begin
         dmes := 1;
         if form2.radiobutton3.checked then //pausa mensal
         begin
            timer1.enabled:=false;
            form5.show;
         end;
      end;
      if dia >= 8 then
      begin
         dia := 1;
         if form2.radiobutton2.checked then //pausa semanal
         begin
            timer1.enabled:=false;
            form5.show;
         end;
      end;
      Edit14.text := dsem [dia];
    end;
  end;
{------------------Visualização do "relógio"----------------------}
  STR (hora, aux1);
  STR (Min, aux2);
  if hora < 10 then aux1 := '0' + aux1;
  if Min < 10 then aux2 := '0' + aux2;
  MaskEdit1.text := aux1 + ':' + aux2;
  STR (fila, aux1);   Edit13.text := aux1;
  STR (dmes, aux1);   Edit15.text := aux1;
  tmp1 := form2.spinedit92.value;
  tmp2 := form2.spinedit93.value;
  if (hora >= tmp1) and (hora <= tmp2) then
  begin
    if TRUNC (RANDOM (100) ) <= Form2.chegam [dia] then
    begin
      INC (fila, incremento);
      STR (TRUNC (RANDOM (4) ), aux1);
    end;
  begin

{------------Aqui é ativada a falta (ou fuga) dos médicos por 24 horas--------}
   if (trunc(random(trunc(form2.spinedit13.value)))=5) and (atend1=0) then
   begin
      label1.font.color:=clRed;
      atend1:=trunc(1440/freq);
      pronto1:=false;
   end;
   if (trunc(random(trunc(form2.spinedit20.value)))=5) and (atend2=0) then
   begin
      label2.font.color:=clRed;
      atend2:=trunc(1440/freq);
      pronto2:=false;
   end;
   if (trunc(random(trunc(form2.spinedit27.value)))=5) and (atend3=0) then
   begin
      label3.font.color:=clRed;
      atend3:=trunc(1440/freq);
      pronto3:=false;
   end;
   if (trunc(random(trunc(form2.spinedit34.value)))=5) and (atend4=0) then
   begin
      label4.font.color:=clRed;
      atend4:=trunc(1440/freq);
      pronto4:=false;
   end;
   if (trunc(random(trunc(form2.spinedit41.value)))=5) and (atend5=0) then
   begin
      label5.font.color:=clRed;
      atend5:=trunc(1440/freq);
      pronto5:=false;
   end;
   if (trunc(random(trunc(form2.spinedit48.value)))=5) and (atend6=0) then
   begin
      label6.font.color:=clRed;
      atend6:=trunc(1440/freq);
      pronto6:=false;
   end;
   if (trunc(random(trunc(form2.spinedit55.value)))=5) and (atend7=0) then
   begin
      label7.font.color:=clRed;
      atend7:=trunc(1440/freq);
      pronto7:=false;
   end;
   if (trunc(random(trunc(form2.spinedit62.value)))=5) and (atend8=0) then
   begin
      label8.font.color:=clRed;
      atend8:=trunc(1440/freq);
      pronto8:=false;
   end;
   if (trunc(random(trunc(form2.spinedit69.value)))=5) and (atend9=0) then
   begin
      label9.font.color:=clRed;
      atend9:=trunc(1440/freq);
      pronto9:=false;
   end;
   if (trunc(random(trunc(form2.spinedit76.value)))=5) and (atend10=0) then
   begin
      label10.font.color:=clRed;
      atend10:=trunc(1440/freq);
      pronto10:=false;
   end;
   if (trunc(random(trunc(form2.spinedit83.value)))=5) and (atend11=0) then
   begin
      label11.font.color:=clRed;
      atend11:=trunc(1440/freq);
      pronto11:=false;
   end;
   if (trunc(random(trunc(form2.spinedit90.value)))=5) and (atend12=0) then
   begin
      label12.font.color:=clRed;
      atend12:=trunc(1440/freq);
      pronto12:=false;
   end;

{-----------------Aqui o paciente diz que especialidade médica necessita-----}
{-----------------e vai para as filas----------------------------------------}
    tmp3 := TRUNC (RANDOM (4) );

{--------Caso vá para a primeira especialidade-------------------------------}
{------todas as outras são análogas a esta-----------------------------------}
    if tmp3 = 0 then
    begin
      aux1 := Edit1.text;         VAL (aux1, fila1, code);
      aux1 := Edit2.text;         VAL (aux1, fila2, code);
      aux1 := Edit3.text;         VAL (aux1, fila3, code);

{------------Caso o médico falte, realoque sua fila em outro médico----------}
      if label1.font.color=clred then // o médico 1 faltou?
      begin
         if fila2<fila3 then          // aloque sua fila em outro médico
         begin
            fila2:=fila2+fila1;
            if fila2>40 then fila3:=40-fila2;
         end
         else
         begin
            fila3:=fila3+fila1;
            if fila3>40 then fila2:=40-fila3;
         end;
         fila1:=0;
      end;
      if label2.font.color=clred then    // o médico 2 faltou?
      begin
         if fila1<fila3 then
         begin
            fila1:=fila1+fila2;
            if fila1>40 then fila3:=40-fila1;
         end
         else
         begin
            fila3:=fila3+fila2;
            if fila3>40 then fila1:=40-fila3;
         end;
         fila2:=0;
      end;
      if label3.font.color=clred then // o médico 3 faltou?
      begin
         if fila1<fila2 then
         begin
            fila1:=fila1+fila3;
            if fila1>40 then fila2:=40-fila1;
         end
         else
         begin
            fila2:=fila2+fila3;
            if fila2>40 then fila1:=40-fila2;
         end;
         fila3:=0;
      end;
      if (fila1 < fila2) and (fila1 < fila3) then // a fila 1 é a menor?
      begin
        if fila1 < 40 then // a fila 1 tem lugares ainda?
        begin
          INC (fila1);
          if fila > 0 then
          begin
            DEC (fila);
          end;
          edit1.text := InttoStr (fila1);
          progressbar1.position := fila1;
        end
      end
      else
        if (fila2 < fila3) then // se a fila 1 não é menor, a 2 é?
        begin
          if fila2 < 40 then // a fila 2 tem lugares ainda?
          begin
            INC (fila2);
            if fila > 0 then
            begin
              DEC (fila);
            end;
            edit2.text := InttoStr (fila2);
            progressbar2.position := fila2;
          end
        end
        else
        begin
          if fila3 < 40 then // a fila 3 tem lugares ainda?
          begin
            INC (fila3);
            if fila > 0 then
            begin
              DEC (fila);
            end;
            edit3.text := InttoStr (fila3);
            progressbar3.position := fila3;
          end
        end
    end;

{-------------------------Segunda especialidade------------------------------}
    if tmp3 = 1 then
    begin
      aux1 := Edit4.text;         VAL (aux1, fila4, code);
      aux1 := Edit5.text;         VAL (aux1, fila5, code);
      aux1 := Edit6.text;         VAL (aux1, fila6, code);
      if label4.font.color=clred then
      begin
         if fila5<fila6 then
         begin
            fila5:=fila5+fila4;
            if fila5>40 then fila6:=40-fila5;
         end
         else
         begin
            fila6:=fila6+fila4;
            if fila6>40 then fila5:=40-fila6;
         end;
         fila4:=0;
      end;
      if label5.font.color=clred then
      begin
         if fila4<fila6 then
         begin
            fila4:=fila4+fila5;
            if fila4>40 then fila6:=40-fila4;
         end
         else
         begin
            fila6:=fila6+fila5;
            if fila6>40 then fila4:=40-fila6;
         end;
         fila5:=0;
      end;
      if label6.font.color=clred then
      begin
         if fila4<fila5 then
         begin
            fila4:=fila4+fila6;
            if fila4>40 then fila5:=40-fila4;
         end
         else
         begin
            fila5:=fila5+fila6;
            if fila5>40 then fila4:=40-fila5;
         end;
         fila6:=0;
      end;
      if (fila4 < fila5) and (fila4 < fila6) then
      begin
        if fila4 < 40 then
        begin
          INC (fila4);
          if fila > 0 then
          begin
            DEC (fila);
          end;
          edit4.text := InttoStr (fila4);
          progressbar4.position := fila4;
        end
      end
      else
        if (fila5 < fila6) then
        begin
          if fila5 < 40 then
          begin
            INC (fila5);
            if fila > 0 then
            begin
              DEC (fila);
            end;
            edit5.text := InttoStr (fila5);
            progressbar5.position := fila5;
          end
        end
        else
        begin
          if fila6 < 40 then
          begin
            INC (fila6);
            if fila > 0 then
            begin
              DEC (fila);
            end;
            edit6.text := InttoStr (fila6);
            progressbar6.position := fila6;
          end
        end
    end;

{------------------------Terceira especialidade------------------------------}
    if tmp3 = 2 then
    begin
      aux1 := edit7.text;         VAL (aux1, fila7, code);
      aux1 := edit8.text;         VAL (aux1, fila8, code);
      aux1 := edit9.text;         VAL (aux1, fila9, code);
      if label7.font.color=clred then
      begin
         if fila8<fila9 then
         begin
            fila8:=fila8+fila7;
            if fila8>40 then fila9:=40-fila8;
         end
         else
         begin
            fila9:=fila9+fila7;
            if fila9>40 then fila8:=40-fila9;
         end;
         fila7:=0;
      end;
      if label8.font.color=clred then
      begin
         if fila7<fila9 then
         begin
            fila7:=fila7+fila8;
            if fila7>40 then fila9:=40-fila7;
         end
         else
         begin
            fila9:=fila9+fila8;
            if fila9>40 then fila7:=40-fila9;
         end;
         fila8:=0;
      end;
      if label9.font.color=clred then
      begin
         if fila7<fila8 then
         begin
            fila7:=fila7+fila9;
            if fila7>40 then fila8:=40-fila7;
         end
         else
         begin
            fila8:=fila8+fila9;
            if fila8>40 then fila7:=40-fila8;
         end;
         fila9:=0;
      end;
      if (fila7 < fila8) and (fila7 < fila9) then
      begin
        if fila7 < 40 then
        begin
          INC (fila7);
          if fila > 0 then
          begin
            DEC (fila);
          end;
          edit7.text := InttoStr (fila7);
          progressbar7.position := fila7;
        end
      end
      else
        if (fila8 < fila9) then
        begin
          if fila8 < 40 then
          begin
            INC (fila8);
            if fila > 0 then
            begin
              DEC (fila);
            end;
            edit8.text := InttoStr (fila8);
            progressbar8.position := fila8;
          end
        end
        else
        begin
          if fila9 < 40 then
          begin
            INC (fila9);
            if fila > 0 then
            begin
              DEC (fila);
            end;
            edit9.text := InttoStr (fila9);
            progressbar9.position := fila9;
          end
        end
    end;

{---------------------------Quarta especialidade----------------------------}
    if tmp3 = 3 then
    begin
      aux1 := edit10.text;         VAL (aux1, fila10, code);
      aux1 := edit11.text;         VAL (aux1, fila11, code);
      aux1 := edit12.text;         VAL (aux1, fila12, code);
      if label10.font.color=clred then
      begin
         if fila11<fila12 then
         begin
            fila11:=fila11+fila10;
            if fila11>40 then fila12:=40-fila11;
         end
         else
         begin
            fila12:=fila12+fila10;
            if fila12>40 then fila11:=40-fila12;
         end;
         fila10:=0;
      end;
      if label11.font.color=clred then
      begin
         if fila10<fila12 then
         begin
            fila10:=fila10+fila11;
            if fila10>40 then fila12:=40-fila10;
         end
         else
         begin
            fila12:=fila12+fila11;
            if fila12>40 then fila10:=40-fila12;
         end;
         fila11:=0;
      end;
      if label12.font.color=clred then
      begin
         if fila10<fila11 then
         begin
            fila10:=fila10+fila12;
            if fila10>40 then fila11:=40-fila10;
         end
         else
         begin
            fila11:=fila11+fila12;
            if fila11>40 then fila10:=40-fila11;
         end;
         fila12:=0;
      end;
      if (fila10 < fila11) and (fila10 < fila12) then
      begin
        if fila10 < 40 then
        begin
          INC (fila10);
          if fila > 0 then
          begin
            DEC (fila);
          end;
          edit10.text := InttoStr (fila10);
          progressbar10.position := fila10;
        end
      end
      else
        if (fila11 < fila12) then
        begin
          if fila11 < 40 then
          begin
            INC (fila11);
            if fila > 0 then
            begin
              DEC (fila);
            end;
            edit11.text := InttoStr (fila11);
            progressbar11.position := fila11;
          end
        end
        else
        begin
          if fila12 < 40 then
          begin
            INC (fila12);
            if fila > 0 then
            begin
              DEC (fila);
            end;
            edit12.text := InttoStr (fila12);
            progressbar12.position := fila12;
          end
        end
    end
  end;
{----------código auxiliar------------------}
  aux1 := Edit1.text;          VAL (aux1, fila1, code);
  aux1 := Edit2.text;          VAL (aux1, fila2, code);
  aux1 := Edit3.text;          VAL (aux1, fila3, code);
  aux1 := Edit4.text;          VAL (aux1, fila4, code);
  aux1 := Edit5.text;          VAL (aux1, fila5, code);
  aux1 := Edit6.text;          VAL (aux1, fila6, code);
  aux1 := edit7.text;          VAL (aux1, fila7, code);
  aux1 := edit8.text;          VAL (aux1, fila8, code);
  aux1 := edit9.text;          VAL (aux1, fila9, code);
  aux1 := edit10.text;         VAL (aux1, fila10, code);
  aux1 := edit11.text;         VAL (aux1, fila11, code);
  aux1 := edit12.text;         VAL (aux1, fila12, code);


  if (fila1 > 0) and // o médico verifica se tem clientes na fila
    ((hora >= form2.SpinEdit9.value) and (hora <= form2.SpinEdit10.value)) then
  begin
    if pronto1 then // pode atender?
    begin
      pronto1 := false; // medico avalia tempo de atend. cliente
      tmp3 := TRUNC (form2.SpinEdit8.Value/ 15) - 1;
      atend1 := TRUNC (RANDOM (tmp3) );
      DEC (fila1);// intervalo2: qdo nao ha paciente, descansa um pouco
      edit1.text := InttoStr (fila1);
      progressbar1.position := fila1;
      label1.font.color:=clBlack;
    end
    else
    begin
      if atend1 > 0 then DEC (atend1); // continua atendendo
      edit1.text := InttoStr (fila1);
      progressbar1.position := fila1;
      if atend1 = 0 then begin pronto1 := true;label1.font.color:=clBlack; end;
    end;
  end;
  if (fila2 > 0) and
    ((hora >= form2.SpinEdit16.value)and(hora <= form2.SpinEdit17.value)) then
  begin
    if pronto2 then
    begin
      pronto2 := false;
      tmp3 := TRUNC (form2.SpinEdit15.Value/ 15) - 1;
      atend2 := TRUNC (RANDOM (tmp3) );
      DEC (fila2);
      edit2.text := InttoStr (fila2);
      progressbar2.position := fila2;
      label2.font.color:=clBlack;
    end
    else
    begin
      if atend2 > 0 then DEC (atend2);
      edit2.text := InttoStr (fila2);
      progressbar2.position := fila2;
      if atend2 = 0 then begin pronto2 := true;label2.font.color:=clBlack; end;
    end;
  end;
  if (fila3 > 0) and
    ((hora >= form2.SpinEdit23.value)and(hora <= form2.SpinEdit24.value)) then
  begin
    if pronto3 then
    begin
      pronto3 := false;
      tmp3 := TRUNC (form2.SpinEdit22.Value/ 15) - 1;
      atend3 := TRUNC (RANDOM (tmp3) );
      DEC (fila3);
      edit3.text := InttoStr (fila3);
      progressbar3.position := fila3;
      label3.font.color:=clBlack;
    end
    else
    begin
      if atend3 > 0 then DEC (atend3);
      edit3.text := InttoStr (fila3);
      progressbar3.position := fila3;
      if atend3 = 0 then begin pronto3 := true;label3.font.color:=clBlack; end;
    end;
  end;
  if (fila4 > 0) and
    ((hora >= form2.SpinEdit30.value)and(hora <= form2.SpinEdit31.value)) then
  begin
    if pronto4 then
    begin
      pronto4 := false;
      tmp3 := TRUNC (form2.SpinEdit29.Value/ 15) - 1;
      atend4 := TRUNC (RANDOM (tmp3) );
      DEC (fila4);
      edit4.text := InttoStr (fila4);
      progressbar4.position := fila4;
      label4.font.color:=clBlack;
    end
    else
    begin
      if atend4 > 0 then DEC (atend4);
      edit4.text := InttoStr (fila4);
      progressbar4.position := fila4;
      if atend4 = 0 then begin pronto4 := true;label4.font.color:=clBlack; end;
    end;
  end;
  if (fila5 > 0) and
    ((hora >= form2.SpinEdit37.value)and(hora <= form2.SpinEdit38.value)) then
  begin
    if pronto5 then
    begin
      pronto5 := false;
      tmp3 := TRUNC (form2.SpinEdit36.Value/ 15) - 1;
      atend5 := TRUNC (RANDOM (tmp3) );
      DEC (fila5);
      edit5.text := InttoStr (fila5);
      progressbar5.position := fila5;
      label5.font.color:=clBlack;
    end
    else
    begin
      if atend5 > 0 then DEC (atend5);
      edit5.text := InttoStr (fila5);
      progressbar5.position := fila5;
      if atend5 = 0 then begin pronto5 := true;label5.font.color:=clBlack; end;
    end;
  end;
    if (fila6 > 0) and
      ((hora >= form2.SpinEdit44.value)and(hora <= form2.SpinEdit45.value))then
  begin
    if pronto6 then
    begin
      pronto6 := false;
      tmp3 := TRUNC (form2.SpinEdit43.Value/ 15) - 1;
      atend6 := TRUNC (RANDOM (tmp3) );
      DEC (fila6);
      edit6.text := InttoStr (fila6);
      progressbar6.position := fila6;
      label6.font.color:=clBlack;
    end
    else
    begin
      if atend6 > 0 then DEC (atend6);
      edit6.text := InttoStr (fila6);
      progressbar6.position := fila6;
      if atend6 = 0 then begin pronto6 := true;label6.font.color:=clBlack; end;
    end;
  end;
    if (fila7 > 0) and
      ((hora >= form2.SpinEdit51.value)and(hora <= form2.SpinEdit52.value))then
  begin
    if pronto7 then
    begin
      pronto7 := false;
      tmp3 := TRUNC (form2.SpinEdit50.Value/ 15) - 1;
      atend7 := TRUNC (RANDOM (tmp3) );
      DEC (fila7);
      edit7.text := InttoStr (fila7);
      progressbar7.position := fila7;
      label7.font.color:=clBlack;
    end
    else
    begin
      if atend7 > 0 then DEC (atend7);
      edit7.text := InttoStr (fila7);
      progressbar7.position := fila7;
      if atend7 = 0 then begin pronto7 := true;label7.font.color:=clBlack; end;
    end;
  end;
    if (fila8 > 0) and
      ((hora >= form2.SpinEdit58.value)and(hora <= form2.SpinEdit59.value))then
  begin
    if pronto8 then
    begin
      pronto8 := false;
      tmp3 := TRUNC (form2.SpinEdit57.Value/ 15) - 1;
      atend8 := TRUNC (RANDOM (tmp3) );
      DEC (fila8);
      edit8.text := InttoStr (fila8);
      progressbar8.position := fila8;
      label8.font.color:=clBlack;
    end
    else
    begin
      if atend8 > 0 then DEC (atend8);
      edit8.text := InttoStr (fila8);
      progressbar8.position := fila8;
      if atend8 = 0 then begin pronto8 := true;label8.font.color:=clBlack; end;
    end;
  end;
    if (fila9 > 0) and
      ((hora >= form2.SpinEdit65.value)and(hora <= form2.SpinEdit66.value))then
  begin
    if pronto9 then
    begin
      pronto9 := false;
      tmp3 := TRUNC (form2.SpinEdit64.Value/ 15) - 1;
      atend9 := TRUNC (RANDOM (tmp3) );
      DEC (fila9);
      edit9.text := InttoStr (fila9);
      progressbar9.position := fila9;
      label9.font.color:=clBlack;
    end
    else
    begin
      if atend9 > 0 then DEC (atend9);
      edit9.text := InttoStr (fila9);
      progressbar9.position := fila9;
      if atend9 = 0 then begin pronto9 := true;label9.font.color:=clBlack; end;
    end;
  end;
    if (fila10 > 0) and
      ((hora >= form2.SpinEdit72.value)and(hora <= form2.SpinEdit73.value))then
  begin
    if pronto10 then
    begin
      pronto10 := false;
      tmp3 := TRUNC (form2.SpinEdit71.Value/ 15) - 1;
      atend10 := TRUNC (RANDOM (tmp3) );
      DEC (fila10);
      edit10.text := InttoStr (fila10);
      progressbar10.position := fila10;
      label10.font.color:=clBlack;
    end
    else
    begin
      if atend10 > 0 then DEC (atend10);
      edit10.text := InttoStr (fila10);
      progressbar10.position := fila10;
      if atend10 = 0 then begin pronto10:=true;label10.font.color:=clBlack;end;
    end;
  end;
    if (fila11 > 0) and
      ((hora >= form2.SpinEdit79.value)and(hora <= form2.SpinEdit80.value))then
  begin
    if pronto11 then
    begin
      pronto11 := false;
      tmp3 := TRUNC (form2.SpinEdit78.Value/ 15) - 1;
      atend11 := TRUNC (RANDOM (tmp3) );
      DEC (fila11);
      edit11.text := InttoStr (fila11);
      progressbar11.position := fila11;
      label11.font.color:=clBlack;
    end
    else
    begin
      if atend11 > 0 then DEC (atend11);
      edit11.text := InttoStr (fila11);
      progressbar11.position := fila11;
      if atend11=0 then begin pronto11 := true;label11.font.color:=clBlack;end;
    end;
  end;
    if (fila12 > 0) and
      ((hora >= form2.SpinEdit86.value)and(hora <= form2.SpinEdit87.value))then
  begin
    if pronto12 then
    begin
      pronto12 := false;
      tmp3 := TRUNC (form2.SpinEdit85.Value/ 15) - 1;
      atend12 := TRUNC (RANDOM (tmp3) );
      DEC (fila12);
      edit12.text := InttoStr (fila12);
      progressbar12.position := fila12;
      label12.font.color:=clBlack;
    end
    else
    begin
      if atend12 > 0 then DEC (atend12);
      edit12.text := InttoStr (fila12);
      progressbar12.position := fila12;
      if atend12=0 then begin pronto12 := true;label12.font.color:=clBlack; end;
    end;
  end;

  if (TRUNC (RANDOM (50) ) = 5) and (fila > 0) then DEC (fila); // paciente
                                                    // abandona fila principal

  if (TRUNC (RANDOM (50) ) = 5) and (fila1 > 0) then // paciente morre ou
  begin                                              // abandona fila interna
    DEC (fila1);
    edit1.text := InttoStr (fila1);
    progressbar1.position := fila1;
    if TRUNC (RANDOM (40) ) = 5 then 
    begin
      INC (morte);  STR (morte, aux1);  edit17.text := aux1; // paciente morreu
    end
    else
    begin
      INC (desist); STR (desist, aux1); edit16.text := aux1; // paciente fugiu
    end;
  end;
  if (TRUNC (RANDOM (50) ) = 5) and (fila2 > 0) then
  begin
    DEC (fila2);
    edit2.text := InttoStr (fila2);
    progressbar2.position := fila2;
    if TRUNC (RANDOM (40) ) = 5 then
    begin
      INC (morte);  STR (morte, aux1);  edit17.text := aux1;
    end
    else
    begin
      INC (desist); STR (desist, aux1); edit16.text := aux1;
    end;
  end;
  if (TRUNC (RANDOM (50) ) = 5) and (fila3 > 0) then
  begin
    DEC (fila3);
    edit3.text := InttoStr (fila3);
    progressbar3.position := fila3;
    if TRUNC (RANDOM (40) ) = 5 then
    begin
      INC (morte);  STR (morte, aux1);  edit17.text := aux1;
    end
    else
    begin
      INC (desist); STR (desist, aux1); edit16.text := aux1;
    end;
  end;
  if (TRUNC (RANDOM (50) ) = 5) and (fila4 > 0) then
  begin
    DEC (fila4);
    edit4.text := InttoStr (fila4);
    progressbar4.position := fila4;
    if TRUNC (RANDOM (40) ) = 5 then
    begin
      INC (morte);  STR (morte, aux1);  edit17.text := aux1;
    end
    else
    begin
      INC (desist); STR (desist, aux1); edit16.text := aux1;
    end;
  end;
  if (TRUNC (RANDOM (50) ) = 5) and (fila5 > 0) then
  begin
    DEC (fila5);
    edit5.text := InttoStr (fila5);
    progressbar5.position := fila5;
    if TRUNC (RANDOM (40) ) = 5 then
    begin
      INC (morte);  STR (morte, aux1);  edit17.text := aux1;
    end
    else
    begin
      INC (desist); STR (desist, aux1); edit16.text := aux1;
    end;
  end;
  if (TRUNC (RANDOM (50) ) = 5) and (fila6 > 0) then
  begin
    DEC (fila6);
    edit6.text := InttoStr (fila6);
    progressbar6.position := fila6;
    if TRUNC (RANDOM (40) ) = 5 then
    begin
      INC (morte);  STR (morte, aux1);  edit17.text := aux1;
    end
    else
    begin
      INC (desist); STR (desist, aux1); edit16.text := aux1;
    end;
  end;
  if (TRUNC (RANDOM (50) ) = 5) and (fila7 > 0) then
  begin
    DEC (fila7);
    edit7.text := InttoStr (fila7);
    progressbar7.position := fila7;
    if TRUNC (RANDOM (40) ) = 5 then
    begin
      INC (morte);  STR (morte, aux1);  edit17.text := aux1;
    end
    else
    begin
      INC (desist); STR (desist, aux1); edit16.text := aux1;
    end;
  end;
  if (TRUNC (RANDOM (50) ) = 5) and (fila8 > 0) then
  begin
    DEC (fila8);
    edit8.text := InttoStr (fila8);
    progressbar8.position := fila8;
    if TRUNC (RANDOM (40) ) = 5 then
    begin
      INC (morte);  STR (morte, aux1);  edit17.text := aux1;
    end
    else
    begin
      INC (desist); STR (desist, aux1); edit16.text := aux1;
    end;
  end;
  if (TRUNC (RANDOM (50) ) = 5) and (fila9 > 0) then
  begin
    DEC (fila9);
    edit9.text := InttoStr (fila9);
    progressbar9.position := fila9;
    if TRUNC (RANDOM (40) ) = 5 then
    begin
      INC (morte);  STR (morte, aux1);  edit17.text := aux1;
    end
    else
    begin
      INC (desist); STR (desist, aux1); edit16.text := aux1;
    end;
  end;
  if (TRUNC (RANDOM (50) ) = 5) and (fila10 > 0) then
  begin
    DEC (fila10);
    edit10.text := InttoStr (fila10);
    progressbar10.position := fila10;
    if TRUNC (RANDOM (40) ) = 5 then
    begin
      INC (morte);  STR (morte, aux1);  edit17.text := aux1;
    end
    else
    begin
      INC (desist); STR (desist, aux1); edit16.text := aux1;
    end;
  end;
  if (TRUNC (RANDOM (50) ) = 5) and (fila11 > 0) then
  begin
    DEC (fila11);
    edit11.text := InttoStr (fila11);
    progressbar11.position := fila11;
    if TRUNC (RANDOM (40) ) = 5 then
    begin
      INC (morte);  STR (morte, aux1);  edit17.text := aux1;
    end
    else
    begin
      INC (desist); STR (desist, aux1); edit16.text := aux1;
    end;
  end;
  if (TRUNC (RANDOM (50) ) = 5) and (fila12 > 0) then
  begin
    DEC (fila12);
    edit12.text := InttoStr (fila12);
    progressbar12.position := fila12;
    if TRUNC (RANDOM (40) ) = 5 then
    begin
      INC (morte);  STR (morte, aux1);  edit17.text := aux1;
    end
    else
    begin
      INC (desist); STR (desist, aux1); edit16.text := aux1;
    end;
  end;
end;

  if hora > tmp2 then fila := 0; // se é hora de fechar, os pacientes que não
                                 // entraram desistem
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm1.FormCreate (Sender: TObject);
begin
  Timer1.Interval := 2001 - TrackBar1.Position;
  hora := 6;
  Min := 0;
  Seg := 0;
  fila := 0;
  dia := 1;
  dmes := 1;
  dsem [1] := 'Segunda';
  dsem [2] := 'Terça';
  dsem [3] := 'Quarta';
  dsem [4] := 'Quinta';
  dsem [5] := 'Sexta';
  dsem [6] := 'Sábado';
  dsem [7] := 'Domingo';
  pronto1 := true; atend1 := 0;
  pronto2 := true; atend2 := 0;
  pronto3 := true; atend3 := 0;
  pronto4 := true; atend4 := 0;
  pronto5 := true; atend5 := 0;
  pronto6 := true; atend6 := 0;
  pronto7 := true; atend7 := 0;
  pronto8 := true; atend8 := 0;
  pronto9 := true; atend9 := 0;
  pronto10 := true; atend10 := 0;
  pronto11 := true; atend11 := 0;
  pronto12 := true; atend12 := 0;
  freq:=15;
  RANDOMIZE;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm1.TrackBar1Change (Sender: TObject);
begin
  Timer1.Interval := 2001 - TrackBar1.Position;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm1.BitBtn1Click (Sender: TObject);
begin
  CLOSE;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm1.SpeedButton1Click (Sender: TObject);
begin
  Timer1.Enabled := SpeedButton1.Down;
  if SpeedButton1.Down then SpeedButton1.Caption := 'Ativado'
     else SpeedButton1.Caption := 'Desativado';
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm1.SpinEdit1Change(Sender: TObject);
begin
   freq:=SpinEdit1.value;
end;

end.

