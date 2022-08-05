uses dos,drivers,crt;
var regs : registers;
	 i    : byte;
	 cursor : array[0..15,0..1] of word;
begin
	initevents;
	showmouse;
   regs.ah:=$f1;
	regs.dx:=$10;
	regs.bx:=$3;
	intr($10,regs);{Mode_reg em bl...}
	regs.dx:=0;
	regs.ah:=$f0;
	intr($10,regs); {Gr1_Pos em BL}
	cursor[0,0]:=$FFFF;
	cursor[1,0]:=$FFFF;
	cursor[2,0]:=$FFFF;
	cursor[3,0]:=$FFFF;
	cursor[4,0]:=$FFFF;
	cursor[5,0]:=$F00F;
	cursor[6,0]:=$0000;
	cursor[7,0]:=$0000;
	cursor[8,0]:=$0000;
	cursor[9,0]:=$0000;
	cursor[10,0]:=$F00F;
	cursor[11,0]:=$FFFF;
	cursor[12,0]:=$FFFF;
	cursor[13,0]:=$FFFF;
	cursor[14,0]:=$FFFF;
	cursor[15,0]:=$FFFF;
	cursor[0,1]:=$0000;
	cursor[1,1]:=$0000;
	cursor[2,1]:=$0000;
	cursor[3,1]:=$0000;
	cursor[4,1]:=$0000;
	cursor[5,1]:=$0000;
	cursor[6,1]:=$07E0;
	cursor[7,1]:=$7FFE;
	cursor[8,1]:=$7FFE;
	cursor[9,1]:=$07E0;
	cursor[10,1]:=$0000;
	cursor[11,1]:=$0000;
	cursor[12,1]:=$0000;
	cursor[13,1]:=$0000;
	cursor[14,1]:=$0000;
	cursor[15,1]:=$0000;
	regs.ax:=9;
	regs.bx:=6;
	regs.cx:=5;
	regs.dx:=ofs(cursor)+1;
	regs.es:=seg(cursor);
	intr($33,regs);
{	regs.AX:=10;
	regs.BX:=1;
	regs.CX:=$0607;
	regs.DX:=$0607;
	intr($33,regs);}
	repeat until keypressed;
	hidemouse;
	doneevents;
end.
{FUN_F0     EQU     0f0H         ; Function F0
GR_CONTR   EQU     010H         ; Graphics Controller
MODE_REG   EQU     005H         ; Mode Regisiter
GR1_PORT   EQU     030H         ; Graphics 1 Position Register
GR2_PORT   EQU     038H         ; Graphics 2 Position Register
VIDEO      EQU     010H         ; BIOS ROM Video Routine Entry
			  ; Retrieve the Mode Register in Graphics Controller
MODE_REG   DB      00
			  MOV     DX, GR_CONTR
			  MOV     BX, MODE_REG
			  MOV     AH, FUN_F0
			  INT     VIDEO
			  MOV     MODE_REG, BL
			  ; Retrieve Graphics 1 Position Data
GR1_POS    DB      00
			  MOV     DX, GR1_POS
			  MOV     AH, FUN_F0
			  INT     VIDEO
			  MOV     GR1_POS, NL}
