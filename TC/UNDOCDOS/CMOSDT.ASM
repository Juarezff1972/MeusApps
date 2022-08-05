; this simple (no where near neat) routine gets the current date and
;  time from the CMOS.  Not from DOS.
;
; For more info:  http://www.zekes.com/~blunt/index.htm
; blunt@zekes.com
;

.model tiny
.186
.code

           mov  dx,offset DateStr
           mov  ah,09
           int  21h

           mov  al,08h
           call getcmos
           call prtbcd

           mov  ah,02
           mov  dl,'/'
           int  21h

           mov  al,07h
           call getcmos
           call prtbcd

           mov  ah,02
           mov  dl,'/'
           int  21h

           mov  al,32h
           call getcmos
           call prtbcd
           mov  al,09h
           call getcmos
           call prtbcd

           mov  al,0Bh                  ; get register B status
           call getcmos                 ;
           and  al,11111011b            ; clear bit 2 (we want BCD format)
           or   al,00000010b            ; set bit 1 (we want 24 hour format)
           push ax                      ;
           mov  dx,70h                  ;
           mov  al,0Bh                  ; write our new status back to
           out  dx,al                   ;  register B
           pop  ax                      ;
           inc  dx                      ;
           out  dx,al                   ;

           mov  dx,offset TimeStr
           mov  ah,09
           int  21h

           mov  al,04h
           call getcmos
           call prtbcd

           mov  ah,02
           mov  dl,':'
           int  21h

           mov  al,02h
           call getcmos
           call prtbcd

           mov  ah,02
           mov  dl,':'
           int  21h

           mov  al,00h
           call getcmos
           call prtbcd

           mov  ah,02
           mov  dl,13
           int  21h
           mov  dl,10
           int  21h

           ret



; *** print a BCD value to stdout
;  input:  al = BCD value to print
; return:  nothing
prtbcd     proc near uses ax dx
           push ax
           xchg dx,ax
           shr  dl,04
           add  dl,'0'
           mov  ah,02
           int  21h
           pop  dx
           and  dl,0Fh
           add  dl,'0'
           mov  ah,02
           int  21h
           ret
prtbcd     endp

; *** get a value from a specified CMOS port
;  input:  al = port to get
; return:  al = value of port
getcmos    proc near uses dx
           mov  dx,0070h
           out  dx,al
           inc  dx
           in   al,dx
           ret
getcmos    endp

DateStr    db  13,10,'Current CMOS Date: (mm/dd/yyyy) $'
TimeStr    db  13,10,'Current CMOS Time:  (hh:mm:ss)   $'

.end
