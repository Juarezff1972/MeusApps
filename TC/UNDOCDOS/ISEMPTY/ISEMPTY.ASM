CodeSeg    segment
           assume cs:CodeSeg, ds:CodeSeg, es:CodeSeg
.186
           org  100h
start:     mov  ax,cs                   ; free unused part of Mem Block
           mov  es,ax                   ;   for .COM file format
           mov  bx,4096                 ;
           mov  ah,4Ah                  ;
           int  21h                     ;

           mov  ah,62h                  ; get segment of psp
           int  21h                     ; call interrupt
           mov  es,bx                   ; put bx into es
           mov  ax,es:[002Ch]           ; get offset in segment
           mov  es,ax                   ;  and put in ES:DI
           xor  di,di                   ;

Loop1:     mov  al,es:[di]              ; if null string then exit (with error)
           or   al,al                   ;
           jz   short NotFound          ;
           mov  si,offset IsEmptyS      ; see if our env var.
           mov  cx,08                   ;
           repe cmpsb                   ;
           jz   short IsEpS             ;
FindNull:  mov  al,es:[di]              ; not found so go to next env var.
           inc  di                      ;
           or   al,al                   ;
           jnz  short FindNull          ;
           jmp  short Loop1             ;

IsEpS:     push ds                      ;
           push es                      ;
           pop  ds                      ;
           mov  dx,di                   ; is our env var.
           call GetLOF                  ; get file length in dx:ax
           pop  ds                      ;
           jnc  short NoErrLOF          ;
           mov  al,0FEh                 ; error with length of file
           jmp  short Done              ;
NoErrLOF:  or   dx,dx                   ; is dx empty
           jnz  short NotEmpty          ; no so false
           or   ax,ax                   ; is ax empty
           jnz  short NotEmpty          ; no so false
           mov  al,01                   ; if file length = 0 then TRUE
           jmp  short Done              ;
NotEmpty:  xor  al,al                   ; FALSE
           jmp  short Done              ;
NotFound:  mov  al,0FFh                 ; didn't find 'ISEMPTY='
Done:      mov  ah,4Ch                  ; exit to DOS
           int  21h                     ; with return code in AL


GetLOF     proc near

           mov  ax,03D00h               ; we will attempt to open for read/only
           int  21h
           jc   short len_exit          ; code 0FEh, and exit
           mov  bx,ax                   ; put handle into bx
           mov  al,02h                  ; end of file
           xor  cx,cx                   ; cx = 0
           xor  dx,dx                   ; dx = 0
           mov  ah,42h                  ; move file pointer
           int  21h                     ; call interrupt
           pushf                        ; if error then exit with code (0FEh)
           push ax                      ; length
           push dx                      ; length
           mov  ah,03Eh                 ; close the infile
           int  21h                     ; call interrupt
           pop  dx                      ; rc (length)
           pop  ax                      ; rc (length)
           popf                         ; restore flags
           clc                          ; make sure carry is clear'd
Len_exit:  ret                          ;
GetLOF     endp                         ; end of procedure

IsEmptyS   db   'ISEMPTY='

CodeSeg    ends
           end  start
