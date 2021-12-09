/* elf.h */

/* $Id: elf.h,v 1.5 2015/03/06 01:41:11 kpettit1 Exp $ */

/*
 * Copyright 2004 Ken Pettit
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


#ifndef		ELF_H
#define		ELF_H

#ifdef WIN32
typedef __int32				int32_t;
typedef unsigned __int32	uint32_t;
typedef unsigned __int16	uint16_t;
#else
#include <stdint.h>
#endif

typedef	uint32_t        Elf32_Addr;
typedef	uint16_t	    Elf32_Half;
typedef	uint32_t        Elf32_Off;
typedef	int32_t		    Elf32_Sword;
typedef	uint32_t	    Elf32_Word;
typedef	unsigned char	Elf32_Char;

#define	EI_NIDENT		16

typedef struct {
	Elf32_Char	e_ident[EI_NIDENT];
	Elf32_Half	e_type;
	Elf32_Half	e_machine;
	Elf32_Word	e_version;
	Elf32_Addr	e_entry;
	Elf32_Off	e_phoff;
	Elf32_Off	e_shoff;
	Elf32_Word	e_flags;
	Elf32_Half	e_ehsize;
	Elf32_Half	e_phentsize;
	Elf32_Half	e_phnum;
	Elf32_Half	e_shentsize;
	Elf32_Half	e_shnum;
	Elf32_Half	e_shstrndx;
} Elf32_Ehdr;

// Define file types
#define		ET_NONE		0		// No file type
#define		ET_REL		1		// Relocatable file
#define		ET_EXEC		2		// Executable file
#define		ET_DYN		3		// Shared object file
#define		ET_CORE		4		// Core file
#define		ET_LOOS		0xFE00	// Operating system specific
#define		ET_HIOS		0xFEFF	// Operating system specific
#define		ET_LOPROC	0xFF00	// Processor specific
#define		ET_HIPROC	0xFFFF	// Processor specific


// Define machine types
#define		EM_NONE			0		// No machine type
#define		EM_M32			1		// AT&T WE 32100
#define		EM_SPARC		2		// SPARC
#define		EM_386			3		// Intel
#define		EM_68K			4		// Motorola
#define		EM_88K			5		// Motorola
#define		EM_860			7		// Intel i860
#define		EM_MIPS			8		// MIPS RS3000
#define		EM_S370			9		// IBM System/370 Processor
#define		EM_MIPS_RS4_BE	10		// MIPS RS4000

// Codes 11-14 reserved for future use, so we use #12 for 8085
#define		EM_8085			12		// Model T - 8085

#define		EM_PARISC		15		// HP PA-RISC
#define		EM_VPP500		17		// Fujitsu VPP500
#define		EM_SPARC32PLUS	18		// Enhanced instruction set SPARC
#define		EM_960			19		// Intel 80960
#define		EM_PPC			20		// PowerPC
#define		EM_PPC64		21		// 64-bit PowerPC
// Codes 22-35 reserved for future use
#define		EM_V800			36		// NEC V800
#define		EM_FR20			37		// Fujitsu FR20
#define		EM_RH32			38		// TRW RH-32
#define		EM_RCE			39		// Motorola RCE
#define		EM_ARM			40		// Advanced RISC Machines ARM
#define		EM_ALPHA		41		// Digital Alpha
#define		EM_SH			42		// Hitachi SH
#define		EM_SPARCV9		43		// SPARC Version 9
#define		EM_TRICORE		44		// Siemens Tricore embedded processor
#define		EM_ARC			45		// Argonaut RISC core
#define		EM_H8_300		46		// Hitachi H8/30
#define		EM_H8_300H		47		// Hitachi H8/300H
#define		EM_H8S			48		// Hitachi H8S
#define		EM_H8_500		49		// Hitachi H8/500
#define		EM_IA_64		50		// Intel IA-64 processor architecture
#define		EM_MIPS_X		51		// Stanford MIPS-X
#define		EM_COLDFIRE		52		// Motorola ColdFire
#define		EM_68HC12		53		// Motorola M68HC12
#define		EM_MMA			54		// Fujitsu MMA Multimedia Accelerator
#define		EM_PCP			55		// Siemens PCP
#define		EM_NCPU			56		// Sony nCPU embedded RISC processor
#define		EM_NDR1			57		// Denso NDR1 microprocessor
#define		EM_STARCORE		58		// Motorola Star*Core processor
#define		EM_ME16			59		// Toyota ME16 processor
#define		EM_ST100		60		// STMicroelectronics ST100 processor
#define		EM_TINYJ		61		// Advanced Logic Corp. TinyJ embedded processor
// Codes 52-65 reserved for future use
#define		EM_FX66			66		// Siemens FX66 microcontroller
#define		EM_ST9PLUS		67		// STMicroelectronics ST9+ 8/16 bit microcontroller
#define		EM_ST7			68		// STMicroelectronics ST7 8-bit microcontroller
#define		EM_68HC16		69		// Motorola MC68HC16 microcontroller
#define		EM_68HC11		70		// Motorola MC68HC11 microcontroller
#define		EM_68HC08		71		// Motorola MC68HC08 microcontroller
#define		EM_68HC05		72		// Motorola MC68HC05 microcontroller
#define		EM_SVX			73		// Silicon Graphics SVx
#define		EM_ST19			74		// STMicroelectronics ST19 8-bit microcontroller
#define		EM_VAX			75		// Digital VAX
#define		EM_CRIS			76		// Axis Communications 32-bit embedded processor
#define		EM_JAVELIN		77		// Infineon Technologies 32-bit embedded processor
#define		EM_FIREPATH		78		// Element 14 64-bit DSP processor
#define		EM_ZSP			79		// LSI Logic 16-bit DSP processor
#define		EM_MMIX			80		// Donald Knuth's educational 64-bit processor
#define		EM_HAUNY		81		// Harvard University machine-independent object files
#define		EM_PRISM		82		// SiTera Prism

// Define version
#define		EV_NONE			0		// Invalid version
#define		EV_CURRENT		1		// Current version

// Define Identification indexes
#define		EI_MAG0			0		// File Identification
#define		EI_MAG1			1		// File Identification
#define		EI_MAG2			2		// File Identification
#define		EI_MAG3			3		// File Identification
#define		EI_CLASS		4		// File class
#define		EI_DATA			5		// Data encoding
#define		EI_VERSION		6		// File version
#define		EI_PAD			7		// Start of padding

// Define values in e_ident
#define		ELFMAG0			0x7f	// Value in e_ident[EI_MAG0]
#define		ELFMAG1			'E'		// Value in e_ident[EI_MAG1]
#define		ELFMAG2			'L'		// Value in e_ident[EI_MAG2]
#define		ELFMAG3			'F'		// Valee in e_ident[EI_MAG3]

#define		ELFCLASSNONE	0		// Invalid class
#define		ELFCLASS32		1		// 32-bit objects
#define		ELFCLASS64		2		// 64-bit objects

#define		ELFDATANONE		0		// Invalid data encoding
#define		ELFDATA2LSB		1		// LSB first
#define		ELFDATA2MSB		2		// MSB first

// Define reserved section header indexes
#define		SHN_UNDEF		0
#define		SHN_LORESERVE	0xff00
#define		SHN_LOPROC		0xff00
#define		SHN_HIPROC		0xff1f
#define		SHN_ABS			0xfff1
#define		SHN_COMMON		0xfff2
#define		SHN_HIRESERVE	0xffff

// Define the section header
typedef struct {
	Elf32_Word	sh_name;
	Elf32_Word	sh_type;
	Elf32_Word	sh_flags;
	Elf32_Addr	sh_addr;
	Elf32_Off	sh_offset;
	Elf32_Word	sh_size;
	Elf32_Word	sh_link;
	Elf32_Word	sh_info;
	Elf32_Word	sh_addralign;
	Elf32_Word	sh_entsize;
} Elf32_Shdr;

#define		SHT_NULL			0
#define		SHT_PROGBITS		1
#define		SHT_SYMTAB			2
#define		SHT_STRTAB			3
#define		SHT_RELA			4
#define		SHT_HASH			5
#define		SHT_DYNAMIC			6
#define		SHT_NOTE			7
#define		SHT_NOBITS			8
#define		SHT_REL				9
#define		SHT_SHLIB			10
#define		SHT_DYNSYM			11
#define		SHT_INIT_ARRAY		14
#define		SHT_FINI_ARRAY		15
#define		SHT_PREINIT_ARRAY	16
#define		SHT_GROUP			17
#define		SHT_SYMTAB_SHNDX	18
#define		SHT_LINK_EQ			19
#define		SHT_LOOS			0x6000000
#define		SHT_HIOS			0x6ffffff
#define		SHT_LOPROC			0x7000000
#define		SHT_HIPROC			0x7ffffff
#define		SHT_LOUSER			0x8000000
#define		SHT_HIUSER			0xfffffff

// Define Flags
#define		SHF_WRITE				0x01
#define		SHF_ALLOC				0x02
#define		SHF_EXECINSTR			0x04
#define		SHF_MERGE				0x10
#define		SHF_STRINGS				0x20
#define		SHF_INFO_LINK			0x40
#define		SHF_LINK_ORDER			0x80
#define		SHF_OS_NONCONFORMING	0x100
#define		SHF_GROUP				0x200
#define		SHF_8085_ABSOLUTE		0x00100000
#define		SHF_MASKOS				0x0ff00000
#define		SFH_MASKPROC			0xf0000000

// Define Symbol table items
#define		STN_UNDEF		0

typedef	struct {
	Elf32_Word	st_name;	// Index into string table
	Elf32_Addr	st_value;	// Symbol value
	Elf32_Word	st_size;	// Symbol size
	Elf32_Char	st_info;
	Elf32_Char	st_other;
	Elf32_Half	st_shndx;
} Elf32_Sym;

#define		ELF32_ST_BIND(i)		((i)>>4)
#define		ELF32_ST_TYPE(i)		((i&0x0F))
#define		ELF32_ST_INFO(b,t)		(((b)<<4)+(t&0x0f))

// Define binding types
#define		STB_LOCAL		0
#define		STB_GLOBAL		1
#define		STB_WEAK		2
#define		STB_EXTERN		3
#define		STB_LOPROC		13
#define		STB_HIPROC		15

// Define symbol types
#define		STT_NOTYPE		0
#define		STT_OBJECT		1
#define		STT_FUNC		2
#define		STT_SECTION		3
#define		STT_FILE		4
#define     STT_EQUATE      5		// Absoute value (EQU) in a relocatable segment
#define		STT_LOPROC		13
#define		STT_HIPROC		15

// Define relocation entries
typedef struct {
	Elf32_Addr		r_offset;
	Elf32_Word		r_info;
} Elf32_Rel;

typedef struct {
	Elf32_Addr		r_offset;
	Elf32_Word		r_info;
	Elf32_Sword		r_addend;
} Elf32_Rela;

#define		ELF32_R_SYM(i)		((i)>>8)
#define		ELF32_R_TYPE(i)		((Elf32_Char)(i))
#define		ELF32_R_INFO(s,t)	(((s)<<8)+(Elf32_Char)(t))

#define		SR_UNDEF			0
#define		SR_ADDR_XLATE		1
#define		SR_EXTERN			2
#define		SR_PUBLIC			3
#define		SR_8BIT				4
#define		SR_24BIT			5
#define		SR_EXTERN8			6
#define		SR_ADDR_PROCESSED	0x81

// Define program header information
typedef struct {
	Elf32_Word		p_type;
	Elf32_Off		p_offset;
	Elf32_Addr		p_vaddr;
	Elf32_Addr		p_paddr;
	Elf32_Word		p_filesz;
	Elf32_Word		p_memsz;
	Elf32_Word		p_flags;
	Elf32_Word		p_align;
} Elf32_Phdr;

// Define program header types
#define		PT_NULL			0
#define		PT_LOAD			1
#define		PT_DYNAMIC		2
#define		PT_INTERP		3
#define		PT_NOTE			4
#define		PT_SHLIB		5
#define		PT_PHDR			6
#define		PT_LOPROC		0x70000000
#define		PT_HIPROC		0x7fffffff


// Define Linker Equation entry
typedef	struct {
	Elf32_Half	st_addr;	// Address of the relocation
	Elf32_Half	st_size;	// Symbol size
	Elf32_Half	st_len;		// Length of equation entry, counting this struct
	Elf32_Half	st_num;     // Number of entries in the RPN equation
	Elf32_Half	st_line;    // Line number where equation occurrs
	Elf32_Half	st_info;    // Index of the section this is relative to
} Elf32_LinkEq;


#endif
