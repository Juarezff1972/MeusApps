CodeSeg    segment
           assume cs:CodeSeg, ds:CodeSeg, es:CodeSeg
     
           org  100h
start:     mov  ax,0040h
           mov  es,ax
           mov  al,es:[0017h]
           or   al,01000000b
           mov  es:[0017h],al
           int  20h

CodeSeg    ends
           end  start
