Dosseg
.model medium, basic
.286
.stack
.code
Even                                    ; make all math even for faster execution
           public    OpenEFile          ; make OpenEFile public
OpenEFile  proc far basic uses bp bx cx, File:word, Attrb:word, Handle:word ; save registers
           mov  bx,offset File
           mov  dx,[bx+2]
           mov  bx,offset Attrb
           mov  ax,[bx]
           mov  ah,3Dh
           int  21h
           jc   CDone
           mov  bx,offset Handle
           mov  [bx],ax
           xor  ax,ax
CDone:     ret
OpenEFile  endp

Even                                    ; make all math even for faster execution
           public    GetFDtTm           ; make GetFDtTm public
GetFDtTm   proc far basic uses bp si, Handle:word, FDate:word,FTime:word ; save registers
           mov  si,offset Handle
           mov  bx,[si]
           xor  al,al
           mov  ah,57h
           int  21h
           jc   DDone
           mov  si,offset FDate
           mov  [si],dx
           mov  si,offset FTime
           mov  [si],cx
           xor  ax,ax
DDone:     ret
GetFDtTm   endp                         ; end of procedure

Even                                    ; make all math even for faster execution
           public    CloseFile          ; make CloseFile public
CloseFile  proc far basic uses bp bx cx, Handle:word ; save registers
           mov  bx,offset Handle
           mov  ax,[bx]
           mov  bx,ax
           mov  ah,3Eh
           int  21h
           jc   CDone
           xor  ax,ax
CDone:     ret
CloseFile  endp
           end                          ; end of assembly code
