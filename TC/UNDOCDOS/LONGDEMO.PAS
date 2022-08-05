{  this is an example on viewing long file names in Pascal
   make sure that you have a 'dummy' file named: 'tempfile.tmp'
   in the current directory be for running this program. }

Program longdemo;

{$g+}

{ This was created with Turbo Pascal 6.x }

var
  buffer                 : string[255];
  OrgFileName            : string[128];
  NewFileName            : string[128];
  OurError               : byte;

begin

  OrgFileName := 'tempfile.tmp' + chr(0);
  NewFileName := 'this is a temp file with a temp name' + chr(0);

  { check to see if long file names are supported }
  asm
    mov  ax,$71A0
    int  $21
    mov  OurError,al
  end;

  if (OurError = 0) then begin
    writeln('Long file names are not supported');
    halt(255);
  end;

  { get current directory }
  OurError := $00;              { clear error flag var. }
  asm
    pusha
    push ds                   
    pop  es
    mov  ax,$7147
    mov  dl,$00
    mov  si,offset buffer
    inc  si
    int  $21

    { since we have changed the length of the string, we now }
    { have to change the discriptor byte to match our change }
    mov  di,offset buffer     { undocumented get string length }
    push di
    inc  di
    mov  ax,$1212
    int  $2F
    pop  di
    mov  [di],cl              { put it in the descriptor byte }
    popa
    jnc  @GetCDone
    mov  OurError,al
@GetCDone:
  end;

  if (OurError = 0) then
    writeln('The current Directory is:  ', buffer)
  else begin
    writeln('Error getting current directory name');
    halt(255);
  end;

  { rename a file name }
  OurError := $00;              { clear error flag var. }
  asm
    pusha
    push ds
    pop  es
    mov  ax,$7156
    mov  di,offset NewFileName
    inc  di
    mov  dx,offset OrgFileName
    inc  dx
    int  $21
    popa
    jnc  @GetCDone1
    mov  OurError,al
@GetCDone1:
  end;

  if (OurError = 0) then begin
    writeln('The filename, ', OrgFileName);
    writeln(' was correctly changed to:  ', NewFileName);
  end
  else begin
    writeln('Error renaming file.');
    halt(255);
  end;
end.
