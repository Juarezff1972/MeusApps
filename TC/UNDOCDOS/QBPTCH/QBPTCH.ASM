; QBPTCH.COM
; Assembled with MASM 5.1x
; Forever Young Software
; Copyright 1984-1998
; All rights reserved
; 04 June 1998
;
; Credit goes to Kurt Kuzba for information
; on how to write this patch.
;
; *** Make a backup of QB.EXE before you use this patch ***
;
; If this patch destroys your QB.EXE and you didn't make a backup
;  of the original, don't come complaining to me.  I warned you.
;  (Don't you just hate the legal stuff)

CodeSeg    segment
           assume cs:CodeSeg, ds:CodeSeg, es:CodeSeg
.186
           org  100h
start:     mov  ax,cs                   ; free unused part of Mem Block
           mov  es,ax                   ;   for .COM file format
           mov  bx,4096                 ;
           mov  ah,4Ah                  ;
           int  21h                     ;

           mov  dx,offset msg1          ; print start string
           mov  ah,09h                  ;
           int  21h                     ;

           xor  ah,ah                   ; get key from keyboard
           int  16h                     ;
           cmp  al,'Y'                  ; if it is a 'Y' then continue
           je   short contnu            ; else abort

aborted:   mov  dx,offset aborts        ; aborted
           mov  ah,09h                  ;
           int  21h                     ;
           jmp  Done                    ;

contnu:    int  29h                     ; print char entered

           mov  ax,3D02h                ; open existing file (read\write)
           mov  dx,offset qbfiles       ;
           int  21h                     ;
           jnc  short filethere         ;

           mov  dx,offset notthrs       ; print 'not there' string
           mov  ah,09h                  ;
           int  21h                     ;
           jmp  short aborted           ; jmp to 'aborted' and exit

filethere: mov  bx,ax                   ; put file handle in bx
           mov  ax,4200h                ; move file pointer (from start)
           xor  cx,cx                   ;
           mov  dx,0E9C5h               ; offset 0E9C5h
           int  21h
           jnc  short fileok            ; if not eof reached then continue

eofreachd: mov  dx,offset eofrchd       ; print 'eof reached' string
           mov  ah,09h                  ;
           int  21h                     ;
           jmp  short aborted           ; jmp to 'aborted' and exit

fileok:    mov  ah,3Fh                  ; Read from file
           mov  cx,04                   ; read 4 bytes
           mov  dx,offset buff          ;
           int  21h
           jnc  short filerok

           mov  dx,offset filererr      ; print 'file read error' string
           mov  ah,09h                  ;
           int  21h                     ;
           jmp  short aborted           ; jmp to 'aborted' and exit

filerok:   push ds                      ; make sure es=ds
           pop  es
           mov  cx,04                   ; cmp four bytes
           mov  si,offset orgbytes
           mov  di,offset buff
           repe cmpsb
           jz   short wasequal

           mov  dx,offset notorgs       ; print 'not org file' string
           mov  ah,09h                  ;
           int  21h                     ;
           jmp  short aborted           ; jmp to 'aborted' and exit

wasequal:  mov  ax,4200h                ; move file pointer (from start)
           xor  cx,cx                   ;
           mov  dx,0E9C5h               ; offset 0E9C5h
           int  21h                     ;
           jc   short eofreachd         ; if error, abort

           mov  ah,40h                  ; write to file
           mov  dx,offset newbytes      ; new code
           mov  cx,4                    ; 4 bytes
           int  21h                     ;
           jnc  short filewok           ; were bytes written ok?

           mov  dx,offset filewerr      ; print 'file write error' string
           mov  ah,09h                  ;
           int  21h                     ;
           jmp  aborted                 ; jmp to 'aborted' and exit

filewok:   mov  ah,3Eh                  ; close the file
           int  21h                     ;

           mov  dx,offset alldone       ; print 'all done' string
           mov  ah,09h                  ;
           int  21h                     ;
           xor  al,al                   ; RC (errorlevel) = 0
                                        ; else RC (errorlevel) != 0

Done:      mov  ah,4Ch                  ; exit to DOS
           int  21h

msg1       db   13,10,'Quick Basic 4.5 "Fast Save" patch'
           db   13,10,' This patch makes QB save all source files as text files'
           db   13,10,' no matter what the "Save-as" radio button denotes.'
           db   13,10,' Make sure QB.EXE is in the current directory.'
           db   13,10,' ** Making a backup of original is recomended **'
           db   13,10
           db   13,10,'Continue with patch? (Y,N):  ',36
aborts     db   13,10,10,' Program aborted.  QB.EXE not changed',36
notthrs    db   13,10,10,' Missing or bad QB.EXE file.',36
eofrchd    db   13,10,10,' End of File reached.',36
filererr   db   13,10,10,' Error Reading from File.',36
filewerr   db   13,10,10,' Error Writing to File.',36
notorgs    db   13,10,10,' File already modified or different version of QB.',36
alldone    db   13,10,10,' QB.EXE modified correctly.',36

qbfiles    db   'QB.EXE',0
buff       db   00,00,00,00
orgbytes   db   75h,07h,0E8h,0D2h
newbytes   db   0EBh,07h,00h,00h

CodeSeg    ends
           end  start
