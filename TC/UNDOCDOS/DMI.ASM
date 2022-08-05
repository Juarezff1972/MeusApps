.model tiny
.external prthex
.code
           mov  ax,0F000h               ; look in bios area
           mov  es,ax                   ; es:di points to 0F000:0000h
           xor  di,di                   ;
           mov  cx,0FFFFh               ; search one segment
KeepGoen:  mov  al,'_'                  ; find '_DMI_'
           repne                        ;
           scasb                        ;
           jcxz short NotFound          ; if cx = 0 then not found
           push cx                      ;
           push di                      ; save search next pos
           mov  si,offset DMIid         ;
           mov  cx,04                   ;
           rep                          ;
           cmpsb                        ;
           pop  di                      ;
           pop  cx                      ;
           jne  short KeepGoen          ;

           mov  dx,offset FndDMIStr     ; print address of DMI
           mov  ah,09                   ;
           int  21h                     ;
           mov  ax,di                   ;
           dec  ax                      ;
           push ax                      ;
           call prthex                  ;
           mov  dx,offset HCRLF         ; print 'h' and CR LF
           mov  ah,09                   ;
           int  21h                     ;

           xor  al,al                   ; No error
           jmp  short AllDone

NotFound:  mov  dx,offset NotFndStr
           mov  ah,09h
           int  21h
           mov  al,0FFh                 ; DMI not found
AllDone:   mov  ah,4Ch
           int  21h

DMIid      db  'DMI_'
FndDMIStr  db  13,10,'_DMI_ found at:  F000:',36
HCRLF      db  'h',13,10,36
NotFndStr  db  13,10,"The string '_DMI_' was not found",36

.end
