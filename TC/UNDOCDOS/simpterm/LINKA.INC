; this stuff is for the communications part */

; registers in UART
SER_RBF  equ  0    ; the read buffer
SER_THR  equ  0    ; the write buffer
SER_IER  equ  1    ; the int. enable register
SER_IIR  equ  2    ; the int. identification register
SER_LCR  equ  3    ; control data config. and divisor latch
SER_MCR  equ  4    ; modem control reg.
SER_LSR  equ  5    ; line status reg.
SER_MSR  equ  6    ; modem status of cts, ring etc.
SER_DLL  equ  0    ; the low byte of baud rate divisor
SER_DLH  equ  1    ; the hi byte of divisor latch

; bit patterns for control registers
SER_BAUD_1200  equ  96   ; baud rate divisors for 1200 baud - 19200
SER_BAUD_2400  equ  48
SER_BAUD_9600  equ  12
SER_BAUD_19200 equ  6

SER_GP02  equ  8   ; enable interrupt

COM_1  equ  03F8h  ; base port address of port 0
COM_2  equ  02F8h  ; base port address of port 1

SER_STOP_1  equ  0 ; 1 stop bit per character
SER_STOP_2  equ  4 ; 2 stop bits per character
SER_BITS_5  equ  0 ; send 5 bit characters
SER_BITS_6  equ  1 ; send 6 bit characters
SER_BITS_7  equ  2 ; send 7 bit characters
SER_BITS_8  equ  3 ; send 8 bit characters

SER_PARITY_NONE  equ  0     ; no parity
SER_PARITY_ODD   equ  8     ; odd parity
SER_PARITY_EVEN  equ  24    ; even parity

SER_DIV_LATCH_ON  equ  80h ; used to turn reg 0,1 into divisor latch

PIC_IMR  equ  21h ; pic's interrupt mask reg.
PIC_ICR  equ  20h ; pic's interupt control reg.

INT_SER_PORT_0  equ  0Ch  ; port 0 interrupt com 1 & 3
INT_SER_PORT_1  equ  0Bh  ; port 0 interrupt com 2 & 4

SERIAL_BUFF_SIZE  equ  1024    ; current size of circulating receive buffer
