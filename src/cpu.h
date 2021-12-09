/* cpu.h */

/* $Id: cpu.h,v 1.9 2013/02/11 08:37:17 kpettit1 Exp $ */

/*
 * Copyright 2004 Stephen Hurd and Ken Pettit
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


#ifndef _CPU_H_
#define _CPU_H_

#include <sys/types.h>

#include "gen_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RAMSIZE		(gModel==MODEL_T200?24576:32768)
//#define ROMSIZE		(gModel==MODEL_T200?40960:32768)
#define ROMSIZE		gRomSize
#define RAMSTART	(gModel==MODEL_T200?40960:32768)

#define OPTROMSIZE		32768
#define ADDRESSSPACE	65536

extern volatile uchar cpu[14];
extern uchar *gMemory[64];
extern int gRomSize;

//extern uchar gSysROM[65536];
//extern uchar gOptROM[32768];

#define A cpu[1]
#define F cpu[0]
#define B cpu[3]
#define C cpu[2]
#define D cpu[5]
#define E cpu[4]
#define H cpu[7]
#define L cpu[6]
#define PCH cpu[9]
#define PCL cpu[8]
#define SPH cpu[11]
#define SPL cpu[10]

/* Thoses macros can NOT be set */
#ifdef WIN32

#define AF (*(ushort*)&F)
#define BC (*(ushort*)&C)
#define DE (*(ushort*)&E)
#define HL (*(ushort*)&L)
#define PC (*(ushort*)&PCL)
#define INCPC   ((*(ushort*)&PCL)++)
#define INCPC2   ((*(ushort*)&PCL)+=2)
#define DECSP   ((*(ushort*)&SPL)--)
#define DECSP2  ((*(ushort*)&SPL)-=2)

#else

#define AF ((((ushort)A)<<8)|F)
#define BC ((((ushort)B)<<8)|C)
#define DE ((((ushort)D)<<8)|E)
#define HL ((((ushort)H)<<8)|L)
#define PC ((((ushort)PCH)<<8)|PCL)
#define INCPC	PCL++;  if(PCL==0) PCH++;
#define INCPC2	{PCL+=2; if(PCL<2)  PCH++;}
#define DECSP	{SPL--;  if(SPL==0xff) SPH--;}
#define DECSP2	{SPL-=2; if(SPL>0xfd)  SPH--;}

#endif

#define DECPC	{PCL--;  if(PCL==0xff) PCH--;}
#define DECPC2	{PCL-=2; if(PCL>0xfd)  PCH--;}
#define SP ((((ushort)SPH)<<8)|SPL)
#define INCSP	{SPL++;  if(SPL==0) SPH++;}
#define INCSP2	{SPL+=2; if(SPL<2)  SPH++;}

extern int gIndex[65536];

#define cpuMISC		cpu[13]
#define CF			(F&0x01)
#define	CF_BIT		0x01
#define	OV			((F&0x02)>>1)
#define	OV_BIT		0x02
#define PF			((F&0x04)>>2)
#define PF_BIT		0x04
#define	TS			((F&0x08)>>3)
#define	TS_BIT		0x08
#define AC			((F&0x10)>>4)
#define AC_BIT		0x10
#define XF			((F&0x20)>>5)
#define XF_BIT		0x20
#define ZF			((F&0x40)>>6)
#define	ZF_BIT		0x40
#define SF			((F&0x80)>>7)
#define	SF_BIT		0x80
#define RST55MASK	(IM&0x01)
#define RST65MASK	((IM&0x02)>>1)
#define RST75MASK	((IM&0x04)>>2)
#define INTDIS		((IM&0x08)>>3)
#define RST55PEND	((IM&0x10)>>4)
#define RST65PEND	((IM&0x20)>>5)
#define RST75PEND	((IM&0x40)>>6)
#define SOD			((IM&0x80)>>7)
#define IM			cpu[12]

#ifdef __cplusplus
}
#endif

#endif


/*
=====================================================================
The following defines allow compiling the instruction decoding either
with or without ReMem support.  ReMem support slows down the 
emulation so the main emulatin routine has 2 loops - one with ReMem
support and one without.  The appropriate loop is executed based on
the settings for ReMem support.
=====================================================================
*/
#undef	SETPCINS16
#undef	M
#undef	INS
#undef	NXTINS
#undef	INS16
#undef	MEM
#undef	MDM16
#undef	MEMSET
#undef	INS_INC

#define	RAMBOTTOM	gRamBottom

#ifdef NO_REMEM

#define SETPCINS16 {int pc=PC; PCL=gBaseMemory[pc++]; PCH=gBaseMemory[pc];}
#define M			((HL>ROMSIZE&&HL<RAMBOTTOM)?0xFF:gBaseMemory[HL])
#define INS			((PC>ROMSIZE&&PC<RAMBOTTOM)?0xFF:gBaseMemory[PC])
#define INS_INC		gBaseMemory[PC]; INCPC;
#define NXTINS		((PC+1>ROMSIZE&&PC<RAMBOTTOM)?0xFF:gBaseMemory[PC+1])
#define INS16		(((int)gBaseMemory[PC])|((int)gBaseMemory[PC+1]<<8))
#define MEM(x)		((x>ROMSIZE&&x<RAMBOTTOM)?0xFF:gBaseMemory[x])
#define MEM16(x)	(((ushort)MEM(x))|((ushort)MEM(x+1))<<8)
#define	MEMSET(a,v)	{if(a>=ROMSIZE) gBaseMemory[a] = v; }

#else

#define SETPCINS16 {int pc=PC; PCL=get_memory8((unsigned short) pc++); PCH=get_memory8((unsigned short) pc);}
#define M			(get_memory8(HL))
#define INS			(get_memory8(PC))
#define INS_INC		(get_memory8(PC); INCPC;)
#define NXTINS		(get_memory8((unsigned short) (PC+1)))
#define INS16		(((int)get_memory8((unsigned short) (PC)))|(((int)get_memory8((unsigned short) (PC+1)))<<8))
#define MEM(x)		(get_memory8(x))
#define MEM16(x)	(((ushort)MEM(x))|((ushort)MEM(x+1))<<8)
#define	MEMSET(a,v)	{gRex?rex_set8((unsigned short)(a),v):remem_set8((unsigned short) (a), v); }


//#define SETPCINS16 {int pc=PC; PCL=get_memory8((unsigned short) pc++); PCH=get_memory8((unsigned short) pc);}
//#define M			(gMemory[gIndex[HL]][HL & 0x3FF])
//#define INS			(gMemory[gIndex[PC]][PC & 0x3FF])
//#define INS_INC		(gMemory[gIndex[PC]][PC & 0x3FF]); INCPC;
//#define NXTINS		(get_memory8((unsigned short) (PC+1)))
//#define INS16		(((int)get_memory8((unsigned short) (PC)))|(((int)get_memory8((unsigned short) (PC+1)))<<8))
//#define MEM(x)		(gMemory[gIndex[x]][x & 0x3FF])
//#define MEM16(x)	(((ushort)MEM(x))|((ushort)MEM(x+1))<<8)
//#define MEMSET(a,v)	{remem_set8((unsigned short) (a), v); }


#endif

