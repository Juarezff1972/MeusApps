Dosseg
.model medium, basic
.286
.stack
.code
           public    DiskData           ; make DiskData public
DiskData   proc far basic uses bp bx cx dx, DRV:word,SPC:word,NAC:word,BPS:word,CPD:word

           mov  bx,DRV
           mov  dx,[bx]                 ; DL = Drive code (0=default,1=A:,etc)
           mov  ah,36h
           int  21h
           cmp  ax,0FFFFh               ; If error, RETURN with ax = -1
           je   short ExitD

           push dx                      ; Store CPD for later use
           mov  dx,bx                   ; Put NAC into dx
           mov  bx,NAC                  ; Point bx at location of NAC variable
           mov  [bx],dx                 ; Store # avail. clusters into NAC
           mov  bx,SPC
           mov  [bx],ax                 ; Store Sectors Per Cluster in SPC
           mov  bx,BPS
           mov  [bx],cx                 ; Store Bytes per Sector in BPS
           pop  dx
           mov  bx,CPD
           mov  [bx],dx                 ; Store Clusters per drive in CPD
           xor  ax,ax                   ; Indicate RETURN = 0 = no problems

ExitD:     ret
DiskData   endp                         ; end of procedure
           end                          ; end of assembly code
