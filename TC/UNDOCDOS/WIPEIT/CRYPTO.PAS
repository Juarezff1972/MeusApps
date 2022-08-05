{  CRYPTO   is a file encryption routine.  It XORs each byte in the file
            with the next byte in the 'key' given.
            For example:
              crypto filename.txt this is a test
            will encrypt the file 'filename.txt' with 'this is a test'
            This is a simple routine and would be easy (very easy, see
            below) to break the code, but we are programmers learning
            to program, not to encrypt files.
            The reason that this technique is so easy to break is that
            if there are any null bytes in the file, these null bytes
            do not get changed.  For instance.
                00000000b
            xor 00000000b
             =  00000000b
            Also, all spaces get changed to the 'other' case of the key.
            For instance.  If the key is 'THIS IS A TEST' then a string
            of spaces in the encrypted file would be 'this is a test'.
            And visa-versa for 'this is a test' would be 'THIS IS A TEST'
            in the encrypted file.  That would be easy to break.
            Let's see why:
              T = 01010100b while
              t = 01110100b
            Notice that only bit 5 is different.  The binary representation
            of the space is: (ascii 32, hex 20h) 0010000h.
            So when you XOR with 0010000h the only thing that you do is
            change the case of the letter.

            This routine could be easily changed to produce something else
            when a space in encountered.  Maybe we will leave that for a
            later version.

            To use CRYPTO,
              CRYPTO <filename> <key>
            where <filename> can be any (DOS) legal filename
            and <key> can be any length key below 255 (DOS wont allow
            a command line larger) and include spaces.

            To decrypt the file back to its original form, just do the
            EXACT command line.

            To encrypt:
              CRYPTO temp.txt THIS is A TesT
            To decrypt:
              CRYPTO temp.txt THIS is A TesT
 
  CRYPTO.PAS  v1.00b
  Forever Young Software
  (C)opyright 1997
  Benjamin David Lunt
  All Rights Reserved
  07 Dec 1997
  Turbo Pascal 6.x
    tpc crypto;
}

program crypto;

var
  buffer                : array[1..32768] of byte;
  i, keylen, actualread : integer;
  inputfile             : FILE;
  inputfilename, key    : string[255];

begin
  if paramcount < 2 then
    begin
      writeln('USAGE:  CRYPTO <filename> <keyword>');
      halt;
    end;

  inputfilename := paramstr(1);
  assign(inputfile, inputfilename);
  {$I-}
  reset(inputfile, 1);
  {$I+}
  if ioresult <> 0 then
    begin
      writeln('Error:  ', inputfilename, ' doesn''t exist!');
      halt;
    end;

  key := '';
  for i := 2 to paramcount do
    key := key + Paramstr(i);
  keylen := length(key);

  repeat
    blockread(inputfile, buffer, 32768, actualread);
    for i := 1 to actualread do
      buffer[i] := buffer[i] xor ord(key[succ(pred(i) mod keylen)]);
    seek(inputfile, filepos(inputfile) - actualread);
    blockwrite(inputfile, buffer, actualread);
  until eof(inputfile);
  close(inputfile);
end.
