{
  TITLE  : Special-memory unit V1.0
  DESC.  : This units handles large array in memory.
  AUTHOR : Vincenzo Alcamo (alcamo@arci01.bo.cnr.it)

  The memory array is a virtual array composed of chained blocks of memory.
  You allocate it with GetMemory(dimension,element_size).
  To use any element, you must first get a pointer to the element with
  the GetUnitPtr function.
  This is because the array can be larger than 64K: the maximum size
  is limited by the available DOS memory.
}
Unit SpecMem;

interface

type
  P_Memory = ^Memory;
  Memory = record
    Units    : Word;
    UnitSize : Word;
    Next     : P_Memory;
    Data     : array [0..65519] of Byte;
  end;

procedure SetHighMem;
procedure RestoreHighMem;
function MyMaxAvail:Longint;
procedure MyGetMem(var p:Pointer;size:Word);
procedure MyFreeMem(p:Pointer;size:Word);

procedure FreeMemory(var p:P_Memory);
function GetMemory(Units:Longint;UnitSize:Word):Pointer;
function GetUnitPtr(p:P_Memory;n:Longint):Pointer;
procedure CopyToMem(p:P_Memory;var dest;first,num:Longint);
procedure CopyFromMem(p:P_Memory;var source;first,num:Longint);
function FileWriteBlock(var f:File;p:P_Memory;offs:Longint;size:Longint):Boolean;
function FileReadBlock(var f:File;p:P_Memory;offs:Longint;size:Longint):Boolean;

implementation

{$UNDEF DOS}
{
 When DOS is defined the unit uses all available DOS memory including
 upper memory blocks (UMB), otherwise it uses the available heap memory.
 If you define DOS you should use a small heap and issue the SetHighMem
 command to allow UMB. If the program must shell to DOS you must invoke
 RestoreHighMem before shelling, and SetHighMem on return.
 UMB are provided by expanded memory managers such as EMM386.
}

function MyMaxAvail:Longint;
{$IFDEF DOS}
assembler;
  asm
    mov ah, $48
    mov bx, -1
    int $21
    xor dx, dx
    mov ax, bx
    shl ax, 1
    rcl dx, 1
    shl ax, 1
    rcl dx, 1
    shl ax, 1
    rcl dx, 1
    shl ax, 1
    rcl dx, 1
  end;
{$ELSE}
  begin
    MyMaxAvail:=MaxAvail;
  end;
{$ENDIF}

procedure MyGetMem(var p:Pointer;size:Word);
{$IFDEF DOS}
assembler;
  asm
    les di, p
    xor ax, ax
    cld
    stosw
    stosw
    mov bx, size
    shr bx, 1
    shr bx, 1
    shr bx, 1
    shr bx, 1
    test size, 15
    jz  @@OK
    inc bx
@@OK:
    mov ah, $48
    int $21
    jc  @@ERROR
    mov es:[di-2], ax
@@ERROR:
  end;
{$ELSE}
  begin
    GetMem(p,size);
  end;
{$ENDIF}

procedure MyFreeMem(p:Pointer;size:Word);
{$IFDEF DOS}
assembler;
  asm
    cmp word ptr p, 0
    jne @@ERROR
    les di, p
    mov ah, $49
    int $21
@@ERROR:
  end;
{$ELSE}
  begin
    FreeMem(p,size);
  end;
{$ENDIF}

{Free the memory array P}
procedure FreeMemory(var p:P_Memory);
  var t:P_Memory;
  begin
    while p<>nil do begin
      t:=p^.Next;
      MyFreeMem(p,p^.Units*p^.UnitSize+8);
      p:=t;
    end;
  end;

var TotalUnits:Longint;
function GetMemoryRaw(Units:Longint;UnitSize:Word):Pointer;
  var f,l,p:P_Memory;
      unit2:Longint;
      size:Longint;
  begin
    TotalUnits:=0;
    unit2:=Units;
    f:=nil;
    while (Units>0) and (unit2>0) do begin
      size:=MyMaxAvail;
      if size>65528 then size:=65528;
      unit2:=(size-8) div UnitSize;
      if unit2>Units then unit2:=Units;
      size:=unit2*UnitSize+8;
      if unit2>0 then begin
        MyGetMem(Pointer(p),size);
        if p<>nil then begin
          FillChar(p^,size,0);
          p^.Units:=unit2;
          inc(TotalUnits,unit2);
          p^.Unitsize:=Unitsize;
          p^.Next:=nil;
          if f=nil then f:=p
          else l^.Next:=p;
          l:=p;
          dec(Units,unit2);
          if Units<unit2 then unit2:=Units;
        end
      end;
    end;
    GetMemoryRaw:=f;
  end;

{Allocate a memory array of UNITS elements, each long UNITSIZE bytes}
function GetMemory(Units:Longint;UnitSize:Word):Pointer;
  var p:P_Memory;
  begin
    p:=GetMemoryRaw(Units,UnitSize);
    if TotalUnits=Units then GetMemory:=p
    else begin
      FreeMemory(p);
      GetMemory:=nil;
    end;
  end;

{Allocate the largest possible memory array with elements of UNITSIZE size}
function AllocateMax(var s:Longint;UnitSize:Word):Pointer;
  begin
    AllocateMax:=GetMemoryRaw(1024*1024,UnitSize);
    s:=TotalUnits;
  end;

{Calculate the block for the requested element inside the memory array}
procedure SeekUnit(var p:P_Memory;var n:Longint);
  begin
    dec(n);
    while (p<>nil) and (p^.Units<=n) do begin
      dec(n,p^.Units);
      p:=p^.Next;
    end;
  end;

{Get address of Nth element in memory array P}
function GetUnitPtr(p:P_Memory;n:Longint):Pointer;
  begin
    SeekUnit(p,n);
    if p<>nil then GetUnitPtr:=addr(p^.Data[n*p^.UnitSize])
    else GetUnitPtr:=nil;
  end;

{Copy NUM elements from memory array P[FIRST] to physical memory}
procedure CopyToMem(p:P_Memory;var dest;first,num:Longint);
  var d : ^Byte;
      n : Word;
  begin
    d:=Addr(dest);
    SeekUnit(p,first);
    while (p<>nil) and (num>0) do begin
      n:=p^.Units-first;
      if n>num then n:=num;
      Move(p^.Data[first*p^.UnitSize],d^,num*p^.UnitSize);
      d:=Pointer(Longint(d)+n);
      dec(num,n);
      p:=p^.Next;
      first:=0;
    end;
  end;

{Copy NUM elements from physical memory to memory array P[FIRST]}
procedure CopyFromMem(p:P_Memory;var source;first,num:Longint);
  var d : ^Byte;
      n : Word;
  begin
    d:=Addr(source);
    SeekUnit(p,first);
    while (p<>nil) and (num>0) do begin
      n:=p^.Units-first;
      if n>num then n:=num;
      Move(d^,p^.Data[first*p^.UnitSize],num*p^.UnitSize);
      d:=Pointer(Longint(d)+n);
      dec(num,n);
      p:=p^.Next;
      first:=0;
    end;
  end;

{Read SIZE bytes from file F inside memory array P<OFFS>}
function FileReadBlock(var f:File;p:P_Memory;offs:Longint;size:Longint):Boolean;
  var l:Longint;
      i:Word;
  begin
    while p<>nil do begin
      l:=Longint(p^.Units)*p^.Unitsize;
      if offs<l then break;
      dec(offs,l);
      p:=p^.Next;
    end;
    while (size>0) and (p<>nil) do begin
      l:=Longint(p^.Units)*p^.Unitsize-offs;
      if l>size then l:=size;
      BlockRead(f,p^.Data[offs],l,i);
      if (IOResult=0) and (l=i) then begin
        dec(size,l);
        offs:=0;
        p:=p^.Next;
      end
      else break;
    end;
    FileReadBlock:=size=0;
  end;

{Write SIZE bytes from memory array P<OFFS> to file F}
function FileWriteBlock(var f:File;p:P_Memory;offs:Longint;size:Longint):Boolean;
  var l:Longint;
      i:Word;
  begin
    while p<>nil do begin
      l:=Longint(p^.Units)*p^.Unitsize;
      if offs<l then break;
      dec(offs,l);
      p:=p^.Next;
    end;
    while (size>0) and (p<>nil) do begin
      l:=Longint(p^.Units)*p^.Unitsize-offs;
      if l>size then l:=size;
      BlockWrite(f,p^.Data[offs],l,i);
      if (IOResult=0) and (l=i) then begin
        dec(size,l);
        p:=p^.Next;
      end
      else break;
    end;
    FileWriteBlock:=size=0;
  end;


{$IFDEF DOS}
var OldExitProc:Pointer;
    OldStrategy:Word;
    OldLinkStatus:Word;

procedure SetHighMem; assembler;
  asm
    mov ax, $5800
    int $21
    mov OldStrategy, ax
    mov ax, $5802
    int $21
    mov ah, 0
    mov OldLinkStatus, ax
    mov ax, $5803
    mov bx, 1
    int $21
    mov ax, $5801
    mov bx, $80
    int $21
  end;

procedure RestoreHighMem; assembler;
  asm
    mov ax, $5803
    mov bx, OldLinkStatus
    int $21
    mov ax, $5801
    mov bx, OldStrategy
    int $21
  end;

procedure SExitProc; far;
  begin
    ExitProc:=OldExitProc;
    RestoreHighMem;
  end;

begin
  OldExitProc:=ExitProc;
  ExitProc:=@SExitProc;
  SetHighMem;
{$ELSE}
procedure SetHighMem; begin end;
procedure RestoreHighMem; begin end;

function HeapCheck(size:Word):Integer; far;
  begin
    HeapCheck:=1;
  end;

begin
  HeapError:=@HeapCheck;
{$ENDIF}
end.
