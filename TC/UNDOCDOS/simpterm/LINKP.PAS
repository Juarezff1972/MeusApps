PROGRAM LINKP;

uses Dos, Crt;

{$F+}
{ this stuff is for the communications part }
const
    { registers in UART }
    SER_RBF =  0;   { the read buffer }
    SER_THR =  0;   { the write buffer }
    SER_IER =  1;   { the int. enable register }
    SER_IIR =  2;   { the int. identification register }
    SER_LCR =  3;   { control data config. and divisor latch }
    SER_MCR =  4;   { modem control reg. }
    SER_LSR =  5;   { line status reg. }
    SER_MSR =  6;   { modem status of cts, ring etc. }
    SER_DLL =  0;   { the low byte of baud rate divisor }
    SER_DLH =  1;   { the hi byte of divisor latch }

    COM2    = $2F8; { COM2 port number }
 
type
  AnArray   = array[0..1024] of byte;

var
  old_isr : pointer;                { holds old com port interrupt handler }
  ser_buffer : AnArray;             { the receive buffer}
  ser_end,ser_start : integer;      { indexes into receive buffer }
  old_int_mask : byte;              { current character }
  serial_lock : byte;
  out_buf : string[24];             { isn't altered while it is being written }
  chb : char;
  ch1 : word;


function inport(portnum:integer): byte;
  var
    alreg : byte;
  begin
    asm
      cli
      mov  dx,portnum
      in   al,dx
      mov  alreg,al
      sti
    end;
    inport := alreg;
  end;

procedure outport(portnum:word; val:byte);
  begin
    asm
      cli
      mov  dx,portnum
      mov  al,val
      out  dx,al
      sti
    end;
  end;

procedure serial_isr; interrupt;
  var
    ser_ch : byte;
  begin
    serial_lock := 1;    { lock out other functions so buffer doesn't get corrupted }
    ser_ch := inport(COM2 + SER_RBF);   { place character into next position }
    ser_end := ser_end + 1;
    if (ser_end > 1023) then            { wrap buffer index around }
      ser_end := 0;
    ser_buffer[ser_end] := ser_ch;      { move character into buffer }
    outport($20, $20);                  { restore PIC }
    serial_lock := 0;                   { undo lock }
  end;

{ this function reads a character from the circulating buffer }
{ and returns it to the caller }
function serial_read : word;
  var
    ch : byte;
  begin
    repeat
    until serial_lock = 0;     { wait for isr to end }

{ is there a character(s) ready in buffer? }
    if (ser_end <> ser_start) then
      begin
        ser_start := ser_start + 1;
        if (ser_start > 1023) then { wrap buffer index if needed }
          ser_start := 0;
        ch := ser_buffer[ser_start];  { get the character out of buffer }
        serial_read := word(ch);   { send data back to caller }
      end
    else
      serial_read := $FFFF;  { buffer was empty return -1 }
  end;


{ this function writes a character to the transmit buffer, but first it }
{ waits for the transmit buffer to be empty. }
procedure serial_write(ch:byte);
  begin
{ wait for transmit buffer to be empty }
    repeat
    until (inport(COM2 + SER_LSR) AND $20) <> 0;

    outport(COM2 + SER_THR, byte(ch));    { send the character }
  end;

procedure open_serial;
  var
    ch : byte;
  begin
    asm
      cli
    end;

    outport(COM2 + SER_LCR, $80);  { set the baud rate }
    outport(COM2 + SER_DLL, 12);
    outport(COM2 + SER_DLH, 0);
    outport(COM2 + SER_LCR, $03);  { set the configuration for the port }
    ch := inport(COM2 + SER_RBF);  { do a read just incase one already there }

{ enable the interrupt when char is ready to be received }
    outport(COM2 + SER_MCR, $0B);
    outport(COM2 + SER_IER, 1);

    GetIntVec($0B,old_isr);
    SetIntVec($0B,addr(serial_isr));
 
    old_int_mask := inport($21);    { enable interrupt on PIC }
    outport($21, old_int_mask AND $F7);

    asm
      sti
    end;
  end;


{ this function closes the port, turning off interrupts and }
{ restoring the old interrupt vector }
procedure close_serial;
  begin
    asm
      cli
    end;
    outport(COM2 + SER_MCR, 0); { disable the interrupts }
    outport(COM2 + SER_IER, 0);
    outport($21, old_int_mask);
    asm
      sti
    end;

    { reset old isr handler }
    SetIntVec($0B,old_isr);
  end;

{ Send the buffer }
procedure send_command;
  var
    i : integer;
    ch : char;
  begin
    i := 1;
    ch := out_buf[i];
    while (ch <> #00) do
      begin
        serial_write(byte(ch));
        i := i + 1;
        ch := out_buf[i];
      end;
  end;


{ *** main *** }
begin
  serial_lock := 0;
  ser_end := -1;
  ser_start := -1;
  
{ Default baud rate = 9600 }
{ Initialize port handlers (open com) }
  open_serial;

{ open modem for commands }
  out_buf := '+++' + #00;
  send_command;

{ ready modem for number }
  out_buf := 'atdt' + #00;
  send_command;

{ dial number }
  out_buf := '5555555' + #13 + #00;
  send_command;

  writeln('Connecting... (press <esc> to quit)');

  chb := ' ';
  while (chb<>chr(27)) do
    begin
      if KeyPressed then   { try and get a character from local machine }
        begin
          chb := ReadKey;
{if the modem/host doesn't return the char then you will need the next line }
          { write(chb); }
          serial_write(byte(chb));   { send the character to other machine }

          if chb=chr(13) then            { test for CR, if so add an line feed }
            begin
{if the modem/host doesn't return the char then you will need the next line }
              { writeln; }
              serial_write(10);
            end;
        end;

      ch1 := serial_read;  { try and get a character from remote }
      if (ch1 <= $FF) AND (ch1 >= 0)
        then write(char(ch1));
    end;
 

  { close modem }
  out_buf := 'at&f' + #00;
  send_command;

  { close serial port }
  close_serial;        { close the connection }
end.
