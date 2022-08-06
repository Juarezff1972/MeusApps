;============================================================================
;
; file:  macros.asm
;
; Macros definitions for Turbo Assembler (masm compatible).
;
;
;
;============================================================================



MPreserveRegisters MACRO
        push si
        push di
        push ds
        push es
        push ss
ENDM



MRestoreRegisters MACRO
        pop  ss
        pop  es
        pop  ds
        pop  di
        pop  si
ENDM


;if using the Lattice Compiler, we must NOT have an underscore in front of
;the symbol name.

IFDEF LATTICE
   MProcDef MACRO ProcName
        ProcName proc
        public ProcName
   ENDM

   MProcEnd MACRO ProcName
        ProcName  endp
   ENDM

   MPublicDataDef MACRO Name
        public Name   
   ENDM

   MCallDef MACRO Name
        call Name
   ENDM

   MExternProcDef MACRO Name
        extrn Name : proc
   ENDM

   MProcDefFar MACRO ProcName
        ProcName proc far
        public ProcName
   ENDM

   MProcDefNear MACRO ProcName
        ProcName proc near
        public ProcName
   ENDM
ELSE
   MProcDef MACRO ProcName
        _&ProcName proc
        public _&ProcName
   ENDM

   MProcEnd MACRO ProcName
        _&ProcName  endp
   ENDM

   MPublicDataDef MACRO Name
        public _&Name   
   ENDM

   MCallDef MACRO Name
        call _&Name
   ENDM

   MExternProcDef MACRO Name
        extrn _&Name : proc
   ENDM

   MProcDefFar MACRO ProcName
        _&ProcName proc far
        public _&ProcName
   ENDM

   MProcDefNear MACRO ProcName
        _&ProcName proc near
        public _&ProcName
   ENDM
ENDIF


