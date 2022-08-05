Dosseg
.model medium, basic
.286
.stack
.data
       NewDTABuffer   DB   128 dup(0)
       dtabuff        equ  offset    newdtabuffer
       dtafilename    equ  dtabuff + 30
       olddtaseg      dw  00h
       olddtaoff      dw  00h
       filectr        dw  00h
       dimoff         dw  00h
       Attrbt         dw  00h
       dironly        dw  00h

.code
Even                                    ; make all math even for faster execution
           public    filedir            ; make filedir public
Filedir    proc far basic uses bp bx cx dx ds es si di, pathname:word, varseg:word, varoff:word, FAttr:word

           mov  dironly,00h
           mov  bx,offset FAttr         ; Attr sent by caller
           mov  ax,[bx]                 ; .
           mov  Attrbt,ax               ; .
           cmp  ax,00010000b            ; dir's only ?
           jne  short Notdironly        ;
           inc  dironly                 ; if so, set flag
Notdironly: mov  filectr,00h            ; initialize counter for files found
           mov  ah,02Fh                 ; "get dta addr" service of int 21h
           int  21h
           mov  olddtaseg,es            ; save the entry value for the dta
           mov  olddtaoff,bx
           mov  bx,offset varseg        ; save the segment portion of the
           mov  ax,[bx]                 ;       address of myarray() in the
           mov  es,ax                   ;       es segment.
           mov  bx,offset varoff        ; save the offset portion of the
           mov  ax,[bx]                 ;       address of myarray() in the
           mov  dimoff,ax               ;       dimoff variable.

           mov  dx,dtabuff              ; prepare to create a new dta temporarily
           mov  ah,01Ah                 ; "set dta addr" service of int 21h
           int  21h
           clc                          ; clear carry flag to start

           mov  bx,offset pathname      ; bx = the addr. of the passed pathname
           mov  dx,[bx+2]               ; note that ds:dx = offset of asciiz pathname
           mov  cx,Attrbt               ; Find_first_match
           mov  ah,4Eh                  ; "find first match" service of int 21h
           int  21h
           jc   short ret_to_caller     ; if carry, disk is 100% empty
Found_a_file: cmp  dironly,00h          ; only want dir's ?
           je   short Skipdirchk        ;
           mov  si,dtabuff              ; 
           mov  al,[si+21]              ; Get found files attrb
           cmp  al,00010000b            ; if not dir then skip to next
           jne  short Getnmtch          ;
Skipdirchk: mov  si,dtafilename         ; si points at start of filenamedir in the newdtabuffer
           mov  di,dimoff
           mov  cx,12
PutFile:   lodsb
           or   al,al
           jz   short DoneFileN
           stosb
           loop PutFile
DoneFileN: mov  al,20h
           rep  stosb
           add  dimoff,12
           inc  filectr                 ; keep count of files recorded
Getnmtch:  mov  ah,4Fh                  ; "find next match" service of int 21h
           int  21h
           jnc  short found_a_file
Ret_to_caller:
           push ds                      ; save ds temporarily
           mov  ax,olddtaseg            ; here we restore the entry dta addr
           mov  ds,ax
           mov  dx,olddtaoff
           mov  ah,01Ah                 ; "set dta addr" service of int 21h
           int  21h
           pop  ds                      ; now restore ds from bx
           mov  ax,filectr              ; return number of files (not including directories)
           ret                          ; r2
Filedir    endp                         ; end of procedure
           end                          ; end of assembly code
