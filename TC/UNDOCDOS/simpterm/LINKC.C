#include <dos.h>
#include <memory.h>
#include "linkc.h"

void (_interrupt _far *Old_Isr)();  // holds old com port interrupt handler
char ser_buffer[SERIAL_BUFF_SIZE];  // the receive buffer
int ser_end = -1, ser_start=-1;     // indexes into receive buffer
int ser_ch;                         // current character
int old_int_mask;                   // the old interrupt mask on the PIC
int open_port;                      // the currently open port
int serial_lock = 0;                // serial ISR semaphore so the buffer
																		// isn't altered while it is being written
                                    // to by the ISR

void _interrupt _far serial_isr(void)
{
	serial_lock = 1; // lock out other functions so buffer doesn't get corrupted
	ser_ch = inp(open_port + SER_RBF);  // place character into next position
	if (++ser_end > SERIAL_BUFF_SIZE-1) // wrap buffer index around
    ser_end = 0;
	ser_buffer[ser_end] = ser_ch; // move character into buffer
	outp(PIC_ICR,0x20);  // restore PIC
	serial_lock = 0;  // undo lock
}

// this function reads a character from the circulating buffer
// and returns it to the caller
int serial_read()
{
	int ch;

	while(serial_lock){}   // wait for isr to end

// is there a character(s) ready in buffer?
	if (ser_end != ser_start) {
		if (++ser_start > SERIAL_BUFF_SIZE-1) // wrap buffer index if needed
			ser_start = 0;
		ch = ser_buffer[ser_start]; // get the character out of buffer
		return(ch); // send data back to caller
	} else {
		return(-1); // buffer was empty return -1
	}
}

// this function writes a character to the transmit buffer, but first it
// waits for the transmit buffer to be empty.
serial_write(unsigned char ch)
{
	while(!(inp(open_port + SER_LSR) & 0x20)){} // wait for transmit buffer to be empty

	_asm cli;
	outp(open_port + SER_THR, ch); // send the character
	_asm sti;
}

open_serial(int port_base, int baud, int configuration)
{
	char ch;

	_asm cli;

	open_port = port_base; // save the port for other functions

	outp(port_base + SER_LCR, SER_DIV_LATCH_ON);  // set the baud rate
	outp(port_base + SER_DLL, baud);
	outp(port_base + SER_DLH, 0);
	outp(port_base + SER_LCR, configuration); // set the configuration for the port

	ch = inp(port_base + SER_RBF);  // do a read just incase one already there

// enable the interrupt when char is ready to be received
	outp(port_base + SER_MCR, 0x0B);
	outp(port_base + SER_IER, 1);

	if (port_base == COM_1) {
		Old_Isr = _dos_getvect(INT_SER_PORT_0);
		_dos_setvect(INT_SER_PORT_0, serial_isr);
	} else {
		Old_Isr = _dos_getvect(INT_SER_PORT_1);
		_dos_setvect(INT_SER_PORT_1, serial_isr);
	}

	old_int_mask = inp(PIC_IMR); // enable interrupt on PIC
	outp(PIC_IMR,(port_base==COM_1)?(old_int_mask & 0xEF):(old_int_mask & 0xF7));

	_asm sti;
}

// this function closes the port, turning off interrupts and
// restoring the old interrupt vector
close_serial(int port_base)
{
	_asm cli;
	outp(port_base + SER_MCR, 0); // disable the interrupts
	outp(port_base + SER_IER, 0);
	outp(PIC_IMR, old_int_mask );
	_asm sti;

	// reset old isr handler
	if (open_port == COM_1)
		_dos_setvect(INT_SER_PORT_0, Old_Isr);
	else
   _dos_setvect(INT_SER_PORT_1, Old_Isr);
}

void send_command(unsigned char *buffer, int length)
{
  int	i;

/*	Send the buffer	*/
	for (i=0; i < length; i++)
		serial_write( (unsigned char) buffer[i]);
}

void main(void)
{
  char ch, out_buf[24];
  int done=0, ch1;

/* Default baud rate = 9600 */
/* Initialize port handlers (open com)  */
  open_serial(COM_2,SER_BAUD_9600,SER_PARITY_NONE|SER_BITS_8|SER_STOP_1);

/* open modem for commands */
	memcpy(out_buf,"+++",3);
  send_command(out_buf,3);

/* ready modem for number */
	memcpy(out_buf,"atdt",4);
  send_command(out_buf,4);

/* dial number */
  memcpy(out_buf,"555-5555",8);
  out_buf[8] = 0x0D;
  send_command(out_buf,9);

	printf("\nConnecting... (press <esc> to quit)");

  while(!done) {
    if (kbhit()) {  // try and get a character from local machine
      ch = getch();
//if the modem/host doesn't return the char then you will need the next line
			//printf("%c", ch);
      serial_write(ch);   // send the character to other machine

      if (ch==27) done=1; // has user pressed ESC
      if (ch==13) {       // test for CR, if so add an line feed
//if the modem/host doesn't return the char then you will need the next line
				//printf("\n");
        serial_write(10);
      }
    }

    ch1 = serial_read();  // try and get a character from remote
		if (ch1 <= 0xFF && ch1 >= 0x00)
			printf("%c", ch1);
  }

	/* close modem */
	memcpy(out_buf,"at&f",4);
  send_command(out_buf,4);

	/* close serial port */
  close_serial(COM_2);  // close the connection
}
