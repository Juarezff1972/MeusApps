This is some short test code that I put together to drive the
Soundblaster's DAC in both DMA and direct mode. There are two programs:

DACDMA.EXE			Outputs a raw 8 bit sample file to the DAC in
					DMA mode.

DACDIR.EXE			Outputs a raw 8 bit sample file to the DAC in
					direct mode.

Unfortunately DACDIR has seemed to have stopped working for some reason.
If someone can tell me what has gone wrong, please send me some mail.

The usage of bot programs is quite simple. DACDMA can take up to two
arguments. The first is the name of the sample file. The second is an
optional sample rate to play the sample back. The sample rate will
default to 11KHz. DACDIR only takes one arguement, the name of the raw
sample to play.

The IO address and IRQ for the Soundblaster is set in the file SBP.ASM.
It is fairly important that these are set correctly. I have it set up
for port 0x220 and IRQ 5 (I sort of found a conflict with IRQ 7 under
OS/2 2.0 8-(). Note also, that the samples are only played in mono on
the SB Pro. I haven't yet worked out how to use it in stereo.

As a bonus, I have included a program that will read the Soundblaster
Pro's mixer chip and report what the current volume settings are. I have
a little more documentation for it, but I haven't got around to writing
it up properly yet. It is just some random scribblings.

Any comments or suggestions (or especially more programming information)
can be directed to cejjb@marlin.jcu.edu.au.


					Jeff Bird	  9 Feb 92

