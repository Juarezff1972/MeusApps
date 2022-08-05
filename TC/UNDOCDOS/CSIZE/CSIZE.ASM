CodeSeg    segment
           assume cs:CodeSeg, ds:CodeSeg, es:CodeSeg

           org  100h                    ; we know that on entry ds=es=cs=ss
start:     mov  al,es:[0080h]           ; get length of commandline
           or   al,al                   ; if no command line then print
           jz   short Help              ; help screen
           mov  al,es:[0082h]           ; get first char ('0' - '7') ????
           sub  al,'0'                  ; make value from ascii
           mov  ah,al                   ; put in ah
           mov  al,es:[0083h]           ; get second char ('0' - '7') ????
           sub  al,'0'                  ; make value from ascii
           or   ax,ax                   ; if 00 then hide cursor
           jz   short CurOff            ;
           mov  cx,ax                   ; ch=(start), cl=(finish)
           and  ch,1101111b             ; make sure display cursor bit is clr
           mov  ah,01h                  ;  so it will display the cursor
           int  10h                     ;
           jmp  short Done              ; we're all done
CurOff:    mov  ah,01h                  ; turn off cursor
           mov  ch,00100000b            ; bit number 5
           int  10h                     ;
           jmp  short Done              ; we're all done
Help:      mov  dx,offset HelpS         ; print help screen
           mov  ah,09                   ; DOS print string service
           int  21h                     ;
Done:      int  20h                     ; exit to DOS

HelpS      db  13,10,'Set Cursor size.  Usage CSIZE sf <enter>'
           db  13,10,' where s = Start (0-7) and f = Finish (0-7) scan line'
           db  13,10,' 00 turns cursor off (67 is DOS default)',13,10,36

    ; DOS's print service (09h) must have the string end in '$' (ascii 36)

CodeSeg    ends
           end  start
