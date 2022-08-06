TITLE   ESR Handling routines
.MODEL  SMALL

	; o arquivo macros.asm deve estar no mesmo diretorio que este
	; programa

include macros.asm

MPublicDataDef RequestESRHandler
MExternProcDef RecebePacoteAcessoDados

.DATA
	; The stack segment and pointer will have to be saved as you should
	; setup your own stack.

	stk_seg  dw  0               ; variable to store old stack segment
	stk_ptr  dw  0               ; variable to store old stack pointer
	new_stk  dw  512 dup (0)     ; new stack of a 1000 bytes in length
	stk_end  dw  0               ; The end of the stack

.CODE

MProcDefFar RequestESRHandler
	mov     ax,DGroup
	mov     ds,ax
	mov     stk_seg,ss	     ; Save the stack segment
	mov     stk_ptr,sp           ; Save the stack pointer
	mov     ax,ds
	mov     ss,ax                ; move segment of new_stk into SS
	mov     sp,offset stk_end    ; move offset of new_stk to SP
  IF    @datasize
	push    es
  ENDIF
	push    si
	MCallDef RecebePacoteAcessoDados
	mov     ss,stk_seg           ; Restore old stack segment
	mov     sp,stk_ptr           ; Restore old stack pointer
	retf
MProcEnd RequestESRHandler
END
