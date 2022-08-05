{$M 8192,0,0}
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
	if x='APPEND ' then writeln('Appended. Use glue to keep it in one piece.');
	if x='ASSIGN ' then writeln('So, you always put new names to old things?');
	if x='ATTRIB ' then writeln('Sorry... The attribs where extinct by the Elders...');
	if x='BACKUP ' then writeln('YOU back up. You forgot I can''t move...');
	if x='BREAK '  then writeln('Broken.');
	if x='CALL ' then writeln('Do you think I am a messenger?');
	if x='CD '     then writeln('With CD''s, the sound technology is much better, isn''t it?');
	if x='CHDIR '  then writeln('No. This directory is still virgin.');
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
	end;
	if x='CLS ' then
	begin
		for i:=1 to 2000 do write(chr(1));
		gotoxy(1,1);
	end;
	if x='COMMAND ' then writeln('SIT!');
	if x='COMP ' then writeln('They look the same to me...');
	if x='CTTY ' then writeln('See, titty, why? Why that?');
	if x='DATE ' then writeln('At eight o''clock in McDonald''s is good for you?');
	if x='DEBUG ' then writeln('To DEBUG, you must BUG first.');
	if x='DEL ' then writeln('Del... Does it mean "FROM" in spanish?');
	if x='DIR ' then writeln('No. That is an insult to my inteligence.');
	if x='DISKCOMP ' then writeln('My disk is much bigger than yours!');
	if x='DISKCOPY ' then writeln('I am unable to copy a virtual disk to a ram disk.');
	if x='DOSKEY ' then writeln('I hate DOSKEY. I prefer MadKey.');
	if x='DOSSHELL ' then writeln('PcTools 1.0 is much better to use.');
	if x='ECHO ' then writeln('I am not a parrot!');
	if x='EDLIN ' then writeln('Why don''t you use Chi Writer? It is better than EDLIN.');
	if x='EMM386 ' then writeln('Buy more memory for me, I just have a little reminding...');
	if x='ERASE ' then
	begin
		write('Enter Password: ');
		readln(z);
		writeln('Password correct. Access denied anyway.');
	end;
	if x='EXE2BIN ' then writeln('EXE3CROUD, EXE4DOME, etc...');
	if x='EXIT ' then writeln('Sorry, but I am unable to move. YOU leave.');
	if x='EXPAND ' then writeln('I am not a balloon!');
	if x='FASTOPEN ' then writeln('Slowclose.');
	if x='FC ' then writeln('FC??? What do you mean with that?');
	if x='FDISK ' then writeln('The Floppy DISKs are A: and B:');
	if x='FIND ' then writeln('The search string was never lost');
	if x='FORMAT ' then writeln('This machine cannot format. Use preformatted disks only.');
	if x='GRAFTABL ' then writeln('This program requires Macrosoft Doors');
	if x='GRAPHICS ' then writeln('If you want graphics, why don''t you buy a Mac?');
	if x='HELP ' then writeln('Call your mom!');
	if x='JOIN ' then writeln('I am not a joinable boy!');
	if x='LH ' then writeln('Use LOADHIGH instead LH.');
	if x='LOADFIX ' then writeln('Do you want me to load or to fix? Both I can''t do!');
	if x='LOADHIGH ' then writeln('Savelow.');
	if x='KEYB ' then writeln('Your keyboard cannot be changed.');
	if x='LABEL ' then writeln('I don''t like labels. They are so nominative!');
	if x='MD ' then
	begin
		writeln('General failure reading drive T:');
		write('Abort, Retry, Ignore, Fail, Give up, Halt, Oops?');
		z[2]:=readkey;
	end;
	if x='MORE ' then writeln('You want more, but I will give you less.');
	if x='MEM ' then writeln('If I have some...');
	if x='MIRROR ' then writeln('Oh... Unfortunately I have none in my warehouse.');
	if x='MKDIR ' then writeln('No... I am tired and build directories is a hard work...');
	if x='MODE ' then writeln('All outputs redirected to joystick port');
	if x='NLSFUNC ' then writeln('Please, say that in english!');
	if x='PATH ' then writeln('All paths leads to rome...');
	if x='PRINT ' then writeln('I must to. I can''t write in cursive.');
	if x='QBASIC ' then writeln('It should be QCOMPLEX.');
	if x='PROMPT ' then writeln('What, do you think I am a dog?');
	if x='RD ' then
	begin
		writeln('Invalid path, not directory, directory not empty,');
		writeln('or directory too important to remove.');
	end;
	if x='RECOVER ' then writeln('Thank you. I''m better now.');
	if x='REN ' then writeln('I prefer the ZEN.');
	if x='RENAME ' then writeln('No. The older name is better.');
	if x='REPLACE ' then writeln('I am not a thief');
	if x='RESTORE ' then writeln('Why? I don''t took anything!');
	if x='RMDIR ' then writeln('Use RD, is much handsome');
	if x='SELECT ' then writeln('Hum... Number 3.');
	if x='SET ' then writeln('No, I don''t have any TV set.');
	if x='SETVER ' then writeln('Don''t you mean SETTER?');
	if x='SHARE ' then writeln('Thanks! I love to share everything!');
	if x='SORT ' then writeln('Using Quicksort, Bubblesort, or what?');
	if x='SUBST ' then writeln('The older is broken? That''s why i need to substitute?');
	if x='SYS ' then writeln('BRO ');
	if x='TIME ' then
	begin
		writeln('Current time is NOW');
		write('Enter new time: ');
		readln(z);
		writeln('Invalid format');
	end;
	if x='TREE ' then writeln('I have a redwood. And You?');
	if x='TYPE ' then writeln('You are the typist here!');
	if x='UNDELETE ' then writeln('Why you deleted it if you wanted it?');
	if x='UNFORMAT ' then writeln('This system can''t unformat. Use pre-unformatted disks only.');
	if x='VER ' then
	begin
		writeln('NUTS-DOS Version 1.0');
		writeln;
	end;
	if x='VERIFY ' then writeln('It is still there and untouched.');
	if x='VOL ' then
	begin
		writeln;
		writeln('Volume in Drive O is Nuts');
	end;
	if x='XCOPY ' then writeln('220 X''s copied.');

	if x='PCSHELL ' then writeln('Type in DOSSHELL. It is much better');
	if x='PCTOOLS ' then writeln('Hammers, alligators, etc...');
	if x='TC ' then writeln('Turbo Basic is easier to use.');
	if x='TCC ' then writeln('TCCC. I won.');
	if x='WIN ' then writeln('Lose.');
	if x='A: ' then chdir('b:');
	if x='B: ' then chdir('a:');
	if x='C: ' then writeln('Invalid drive specification');
	if x='D: ' then writeln('E:, F:, G:...');
	if x='FUCK ' then writeln('My, what a filthy mind you have!');
	if x='SHIT ' then writeln('If I do that, will you clean it up?');
	writeln;
end;
{-----------------------------------------------------------}
begin
	writeln;
	randomize;
	repeat
		getdir(0,prompt);
		write(prompt,'>');
		readln(x);
		x:=upstr(x)+' ';
		setcbreak(false);
		if (x='GO AWAY! ') then
		begin
			writeln('Ok! But I''ll return!...');
			halt(0);
		end;
		message(copy(x,1,pos(' ',x)));
	until false;
end.
