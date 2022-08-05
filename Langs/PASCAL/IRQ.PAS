{$M 2000,0,0}
{$R-,S-,I-,F-,O-,A-,V+,B-,N-,E+,D-,L-}
uses crt,dos;
const irq=0;
		Controller =$20;
		SpecificEOI=$60;
var End_Of_Int     : Byte;
	 OriginalVector : Pointer;
	 OriginalMask   : Byte;
	 IntNumber      : $08..$0f;

{--------------------------------------------------------------}
Procedure STI;
begin
	inline($fb);
end;
{--------------------------------------------------------------}
Procedure CLI;
begin
	inline($fa);
end;
{--------------------------------------------------------------}
{$F+}
Procedure InterruptHandler;
interrupt;
begin
	Sound(800);delay(200);Sound(1200);delay(300);NoSound;
	Port[Controller]:=End_Of_Int;
end;
{$F-}
{--------------------------------------------------------------}
Procedure Install_InterruptHandler;
var EnablePattern : Byte;
begin
	GetIntVec(IntNumber,OriginalVector);
	CLI;
	SetIntVec(IntNumber,@InterruptHandler);
	STI;
	OriginalMask:=Port[Controller+1];
	EnablePattern:=$01;
	EnablePattern:=EnablePattern shl IRQ;
	EnablePattern:=not(EnablePattern);
	Port[Controller+1]:=(OriginalMask and EnablePattern);
end;
{--------------------------------------------------------------}
Procedure Uninstall_InterruptHandler;
begin
	Port[Controller+1]:=OriginalMask;
	CLI;
	SetIntVec(IntNumber,OriginalVector);
	STI;
end;
{--------------------------------------------------------------}
begin
	IntNumber:=IRQ+8;
	End_Of_Int:=SpecificEOI+IRQ;
	Install_InterruptHandler;

	Repeat Until Keypressed;

   Uninstall_InterruptHandler;
end.