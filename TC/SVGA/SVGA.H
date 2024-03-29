/****************************************************************************
*
*			The SuperVGA Kit - UniVBE Software Development Kit
*
*                   Copyright (C) 1996 SciTech Software
*                           All rights reserved.
*
* Filename:     $Workfile:   svga.h  $
* Version:      $Revision:   1.16  $
*
* Language:     ANSI C
* Environment:  IBM PC (MSDOS) Real Mode and 16/32 bit Protected Mode.
*
* Description:  Header file for the small SuperVGA Kit Library. This
*				library provides a simplified interface to SuperVGA
*               cards that have a VESA VBE (or with the Universal VESA
*               VBE installed). This library requires at least a
*               VBE 1.2 interface to function correctly.
*
*				Under Windows this library is provided as a staticly
*				linkable library or as a DLL.
*
*               NOTE: This library only works in the large or flat models.
*
* $Date:   04 Nov 1996 12:11:22  $ $Author:   KendallB  $
*
****************************************************************************/

#ifndef __SVGA_H
#define __SVGA_H

#ifndef	__VESAVBE_H
#include "vesavbe.h"
#endif

#ifndef	__VBEAF_H
#include "vbeaf.h"
#endif

/*---------------------- Macros and type definitions ----------------------*/

/* Define the calling conventions for the code in this module */

#if		defined(SVGA_DLL) || defined(BUILD_DLL)

#ifdef	__WINDOWS32__
#define	SVAPI	_DLLASM			/* 'C' calling conventions for Win32	*/
#elif	defined(__WINDOWS16__)
#define	SVAPI	_DLLAPI			/* Pascal calling conventions for Win16	*/
#else
#define	SVAPI  _ASMAPI			/* 'C' calling conventions for DOS		*/
#endif

#else
#define	SVAPI  _ASMAPI			/* 'C' calling conventions for libs		*/
#endif

#pragma	pack(1)

/* Flags for combining with video modes during mode set */

#define	svDontClear		0x8000		/* Dont clear display memory		*/
#define	svLinearBuffer	0x4000		/* Enable linear framebuffer mode	*/
#define	svMultiBuffer	0x2000		/* Enable multi buffered mode		*/
#define	svModeMask		0x01FF		/* Mask for mode numbers			*/

/* Graphics mode information block.
 *
 * Note that this is very similar to the standard VBE/AF and VBE 1.2/2.0 mode
 * information blocks, but since the SuperVGA Kit works with either VBE
 * 1.2/2.0 or VBE/AF devices we provide our own common structures routines
 * to insulate the developers from the differences between VBE and VBE/AF.
 */

typedef struct {
	ushort	Mode;					/* VBE or VBE/AF mode number		*/
	ushort	Attributes;				/* Mode attributes					*/
	ushort	XResolution;			/* Horizontal resolution in pixels	*/
	ushort	YResolution;			/* Vertical resolution in pixels	*/
	ushort	BytesPerScanLine;		/* Bytes per horizontal scan line	*/
	ushort	BitsPerPixel;			/* Bits per pixel					*/
	ushort	NumberOfPages;			/* Total number of available pages	*/

	/* RGB pixel format info */
	uchar	RedMaskSize;        	/* Size of direct color red mask    */
	uchar	RedFieldPosition;   	/* Bit posn of lsb of red mask      */
	uchar	GreenMaskSize;      	/* Size of direct color green mask  */
	uchar	GreenFieldPosition; 	/* Bit posn of lsb of green mask    */
	uchar	BlueMaskSize;       	/* Size of direct color blue mask   */
	uchar	BlueFieldPosition;  	/* Bit posn of lsb of blue mask     */
	uchar	RsvdMaskSize;       	/* Size of direct color res mask    */
	uchar	RsvdFieldPosition;		/* Bit posn of lsb of res mask      */

	/* Linear buffer mode extra information */
	ushort  LinBytesPerScanLine;    /* Bytes per scanline               */
	uchar   BnkNumberOfPages;  		/* Number of images pages (banked)  */
	uchar   LinNumberOfPages;  		/* Number of images pages (linear)  */
	uchar   LinRedMaskSize;         /* Size of direct color red mask    */
	uchar   LinRedFieldPosition;    /* Bit posn of lsb of red mask      */
	uchar   LinGreenMaskSize;       /* Size of direct color green mask  */
	uchar   LinGreenFieldPosition;  /* Bit posn of lsb of green mask    */
	uchar   LinBlueMaskSize;        /* Size of direct color blue mask   */
	uchar   LinBlueFieldPosition;   /* Bit posn of lsb of blue mask     */
	uchar   LinRsvdMaskSize;        /* Size of direct color res mask    */
	uchar   LinRsvdFieldPosition; 	/* Bit posn of lsb of res mask      */
	} SV_modeInfo;

/* Flags for the mode attributes returned by SV_getModeInfo */

#define	svHaveMultiBuffer	0x0001	/* Mode supports multi buffering	*/
#define	svHaveVirtualScroll	0x0002	/* Mode supports virtual scrolling	*/
#define	svHaveBankedBuffer	0x0004	/* Mode supports banked framebuffer	*/
#define	svHaveLinearBuffer	0x0008	/* Mode supports linear framebuffer	*/
#define	svHaveAccel2D		0x0010	/* Mode supports 2D acceleration	*/
#define	svHaveDualBuffers	0x0020	/* Mode uses dual buffers			*/
#define	svHaveHWCursor		0x0040	/* Mode supports a hardware cursor	*/
#define	svHave8BitDAC		0x0080	/* Mode uses an 8 bit palette DAC	*/
#define	svNonVGAMode		0x0100	/* Mode is a NonVGA mode			*/
#define	svIsVBEMode			0x8000	/* Flags info is for a VBE mode		*/

/* Palette entry structure, always in 8 bits per primary format */

typedef struct {
	uchar	blue;					/* Blue component of color			*/
	uchar	green;					/* Green component of color			*/
	uchar	red;					/* Blue component of color			*/
	uchar	alpha;					/* Alpha or alignment byte			*/
	} SV_palette;

/* SuperVGA Kit device context block. This structure contains all the
 * global information used by the SuperVGA Kit, and is stored in a
 * single common structure that is stored internally in the SuperVGA Kit
 * libraries. When the SuperVGA Kit is initialised, it returns a pointer
 * to the global device context structure so that the application can get
 * direct access to the global state information for the library. This
 * also allows the entire SuperVGA Kit to be supplied as a DLL under
 * Windows since we no longer have any public global variables.
 *
 * Stored in this structure is also the addresses of the currently active
 * rendering routines, so for maximum speed you can call these functions
 * directly rather than going through the slower DLL function call
 * interface under Windows.
 */

typedef struct {
	int     VBEVersion;			/* VBE Version number detected		*/
	int     maxx;				/* Maximum X coordinate value		*/
	int		maxy;          		/* Maximum Y coordinate value		*/
	ulong	maxcolor;       	/* Maximum and color value			*/
	ulong	defcolor;			/* Default color value (white)		*/
	int     maxpage;        	/* Maximum video page number        */
	ulong	bytesperline;   	/* Bytes in a logical scanline      */
	int		bitsperpixel;		/* Bits per pixel for mode			*/
	int		bytesperpixel;		/* Bytes in a pixel (if > 1)		*/
	int     memory;         	/* Memory on board in k             */
	long	linearAddr;			/* Address of linear framebuffer	*/
	ushort	*modeList;     		/* List of available video modes    */
	char    *OEMString;    		/* OEM string from VBE              */
	int     curBank;			/* Current read/write bank          */
	int		haveVirtualBuffer;	/* True if virtual buffer is avail	*/
	int		haveMultiBuffer;	/* True for multi buffering			*/
	int		haveVirtualScroll;	/* True for virtual scroll			*/
	int		haveWideDAC;		/* True for 8 bit wide DAC			*/
	int		haveAccel2D;		/* True for 2D acceleration			*/
	int		haveHWCursor;		/* True for Hardware Cursor			*/
	int		virtualBuffer;		/* True if using virtual buffer		*/
	int		isNonVGA;			/* True if mode is NonVGA			*/
	void	*videoMem;			/* Pointer to video memory			*/
	ulong	originOffset;		/* Offset into video memory region	*/
	ushort	bankOffset;			/* Bank offset in banked SVGA modes	*/

	/* Pixel format information - used by the rgbColor() routine to
	 * build the correct pixel format, but you can use it yourself
	 * to build scanline information in the desired format.
	 */
	uchar	redMask;
	uchar	greenMask;
	uchar	blueMask;
	int		redPos;
	int		redAdjust;
	int		greenPos;
	int		greenAdjust;
	int		bluePos;
	int		blueAdjust;

	/* VBE/AF Accelerator Functions device context block. This variable
	 * will be NULL if the VBE/AF Acclerator Functions are not available,
	 * or will be a valid pointer when using the VBE/AF functions.
	 */
	AF_devCtx *AFDC;

	/* Currently active rendering functions ('C' calling conventions!) */
	void 	(_ASMAPI *putPixel)(int x,int y,ulong color);
	void 	(_ASMAPI *beginPixel)(void);
	void 	(_ASMAPI *putPixelFast)(int x,int y,ulong color);
	void 	(_ASMAPI *endPixel)(void);
	void 	(_ASMAPI *clear)(ulong color);
	void 	(_ASMAPI *line)(int x1,int y1,int x2,int y2,ulong color);
	void 	(_ASMAPI *beginLine)(void);
	void 	(_ASMAPI *lineFast)(int x1,int y1,int x2,int y2,ulong color);
	void 	(_ASMAPI *endLine)(void);
	void 	(_ASMAPI *setActivePage)(int page);
	void 	(_ASMAPI *setVisualPage)(int page,bool waitVRT);
	void 	(_ASMAPI *beginDirectAccess)(void);
	void 	(_ASMAPI *endDirectAccess)(void);
	} SV_devCtx;

#pragma	pack()

/*------------------------- Function Prototypes ---------------------------*/

#ifdef  __cplusplus
extern "C" {            /* Use "C" linkage when in C++ mode */
#endif

/* Call the following if you need to call VESAVBE.C before SV_init is called */

void 	SVAPI SV_initRMBuf(void);

SV_devCtx * SVAPI SV_init(bool useVBEAF);
void	SVAPI SV_exit(void);
bool	SVAPI SV_getModeInfo(ushort mode,SV_modeInfo *modeInfo);
int 	SVAPI SV_getModeName(char *buf,SV_modeInfo *mi,ushort mode,bool useLinear);
bool	SVAPI SV_setMode(ushort mode,bool use8BitDAC,bool useVirtualBuffer,int numBuffers);
bool	SVAPI SV_setVirtualMode(ushort mode,int virtualX,int virtualY,bool use8BitDAC,bool useVirtualBuffer,int numBuffers);
void 	SVAPI SV_restoreMode(void);
ulong 	SVAPI SV_rgbColor(uchar r,uchar g,uchar b);
void 	SVAPI SV_writeText(int x,int y,char *str,ulong color);
void	SVAPI SV_setPalette(int start,int num,SV_palette *pal,int maxProg);
void	SVAPI SV_setDisplayStart(int x,int y,bool waitVRT);
void 	SVAPI SV_setBank(int bank);
void 	SVAPI SV_beginPixel(void);
void 	SVAPI SV_putPixel(int x,int y,ulong color);
void 	SVAPI SV_putPixelFast(int x,int y,ulong color);
void 	SVAPI SV_endPixel(void);
void 	SVAPI SV_clear(ulong color);
void 	SVAPI SV_beginLine(void);
void 	SVAPI SV_line(int x1,int y1,int x2,int y2,ulong color);
void 	SVAPI SV_lineFast(int x1,int y1,int x2,int y2,ulong color);
void 	SVAPI SV_endLine(void);
void 	SVAPI SV_setActivePage(int page);
void 	SVAPI SV_setVisualPage(int page,bool waitVRT);
void 	SVAPI SV_beginDirectAccess(void);
void 	SVAPI SV_endDirectAccess(void);
SV_palette * SVAPI SV_getDefPalette(void);

/* Tell the SuperVGA Kit what your applications main window is */

#ifdef	__WINDOWS__
void	SVAPI SV_setMainWindow(HWND hwnd);
#endif

/* Assembler callable bank switching function.
 *
 * Entry:		AL	- New read/write bank number
 *
 * Exit:		AL	- New read/write bank number
 *
 * Registers:	All preserved!
 */

void 	SVAPI SV_setBankASM(void);

/* Tell the SuperVGA Kit to use a pre-loaded ACCEL.DRV driver file. This
 * allows you to link with the SciTech UVBELib/Accel device support.
 * If the user has a real VBEAF.DRV driver file in the standard location
 * on their machine, this driver file will still be used.
 */

void	SVAPI SV_setACCELDriver(AF_devCtx *driver);

/* CPU detection routine */

typedef enum    {SV_cpu86,SV_cpu186,SV_cpu286,SV_cpu286p,
				 SV_cpu386,SV_cpu386p,SV_cpu486,SV_cpu486p,
				 SV_cpu586,SV_cpu586p} SV_cpuType;

int 	SVAPI SV_queryCpu(void);

#ifdef  __cplusplus
}                       /* End of "C" linkage for C++   */
#endif

#endif  /* __SVGA_H */
