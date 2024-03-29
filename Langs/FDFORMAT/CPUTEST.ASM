MachineCheck proc   near                        ;Teste do tipo da CPU (Metodo oficial da INTEL...
                                                ;...mas imbecil)
             xor       ax,ax                    ;Joga 0...
             push      ax                       ;sobre a Pilha (Stack)
             popf                               ;Move os Flags para baixo
             pushf                              ;Joga-o de novo em seguida
             pop       ax                       ;para o AX
             and       ax,0F000h                ;Nimm somente os Bits 11-8
             cmp       ax,0F000h                ;Wenn alle gesetzt....
             je        short MC_8086            ;...dann ist es ein 8086

             mov       ax,0F000h                ;Pushe f00...
             push      ax                       ;...auf den Stack
             popf                               ;Ziehe die Flags runter
             pushf                              ;und wieder auf den Stack
             pop       ax                       ;Als AX vom Stack
             and       ax,0F000h                ;Nimm nur Bits 11-8
             jz        short MC_80286           ;Wenn Null, dann 80286

             mov       ax,3                     ;Setze AX=3 f�r 80386/80386SX/80486
             ret                                ;Ende

mc_80286:    mov       ax,2                     ;Setze AX=2 f�r 80286
             ret                                ;Ende

mc_8086:     mov       ax,1                     ;Setze AX=1 f�r 8086/8088
             ret                                ;Ende
MachineCheck endp

;Man kann von den Jungs bei Intel zwar nicht verlangen, da� die gleich eine Routine
;ver�ffentlichen, die auch V20/V30/... erkennen, aber den 80188/80186 h�tten die ja
;mal einbauen k�nnen, da die Dinger alle Nicht-Priviligierten 286er Befehle k�nnen.
