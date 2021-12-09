/* setup.h */

/* $Id: setup.h,v 1.11 2015/02/24 23:41:16 kpettit1 Exp $ */

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

#ifndef SETUP_H
#define SETUP_H


#ifdef __cplusplus
void cb_PeripheralSetup (Fl_Widget* w, void*);
void cb_MemorySetup (Fl_Widget* w, void*);
void cb_SocketSetup (Fl_Widget* w, void*);
extern "C" {
#endif

void load_setup_preferences(void);
void load_memory_preferences(void);


typedef struct peripheral_setup
{
	// COM port emulation settings
	int		com_mode;					// Mode for COM emulation
	char	com_port[128];				// Port name of Host
	char	com_cmd[128];				// Command file for simulation
	char	com_other[256];				// Command file for simulation
	int		com_throttle;				// Flag if serial I/O should be throttled
	int		com_ignore_flow;			// Set if flow control is ignored

	// LPT port emulation settings

	// MDM port emulation settings

	// CAS port emulation settings

	// BCR port emulation settings

	// Sound emulation settings
	int		sound_enable;				// Set TRUE when sound is enabled
	double	sound_tone;					// Set the tone control for sound

} peripheral_setup_t;

extern peripheral_setup_t setup;

typedef struct memory_setup
{
	int		mem_mode;					// Mode for Memory emulation
	int		mem_installed;				// Amount of memory installed
	int		remem_override;				// Override setting for ReMem's Rampac
	char	remem_file[256];			// Filename for ReMem storage
	char	rampac_file[256];			// Filename for RamPac storage
	char	rex_flash_file[256];		// Rex flash file
	char	rex2_ram_file[256];			// Rex2 RAM file
} memory_setup_t;

extern memory_setup_t	mem_setup;

enum {
	SETUP_COM_NONE,
	SETUP_COM_SIMULATED,
	SETUP_COM_HOST,
	SETUP_COM_OTHER, 
	SETUP_COM_SIM_TPDD
};

enum {
	SETUP_MEM_BASE,
	SETUP_MEM_RAMPAC,
	SETUP_MEM_REMEM,
	SETUP_MEM_REMEM_RAMPAC,
	SETUP_MEM_REX,
	SETUP_MEM_REX2,
    SETUP_MEM_QUAD,
    SETUP_MEM_REX_QUAD
};

#ifdef __cplusplus
}
#endif

#endif
