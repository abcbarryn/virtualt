/* setup.cpp */

/* $Id: setup.cpp,v 1.27 2015/02/24 23:41:16 kpettit1 Exp $ */

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


#include <FL/Fl.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Round_Button.H>

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#ifdef __APPLE__
#include <sys/stat.h>
#endif

#include "FLU/Flu_File_Chooser.h"

#include "VirtualT.h"
#include "m100emu.h"
#include "serial.h"
#include "setup.h"
#include "memedit.h"
#include "io.h"
#include "lpt.h"
#include "clock.h"
#include "cpu.h"
#include "tpddserver.h"
#include "file.h"

extern	Fl_Preferences virtualt_prefs;
void 	init_menus(void);

extern "C"
{
void	enable_tpdd_log_menu(int bEnabled);
void	sound_set_tone_control(double tone);
double	sound_get_tone_control(void);

extern	const unsigned char gRex148Image[16384];
extern	const unsigned char gRex248Image[16384];
extern	const unsigned char gRexDirectories[1152];

extern	const unsigned char gTsDos100Image[21352];
extern	const unsigned char gTsDos200Image[21346];
extern	const unsigned char gTsDosNecImage[21344];
}

typedef struct setup_ctrl_struct	
{
	Fl_Tabs*				pTabs;

	struct
	{
		Fl_Group*			g;
		Fl_Round_Button*	pNone;
		Fl_Round_Button*	pSim;
		Fl_Input*			pCmd;
		Fl_Button*			pTpddConfig;
		Fl_Round_Button*	pHost;
		Fl_Choice*			pPort;
		Fl_Round_Button*	pOther;
		Fl_Input*			pOtherName;
		Fl_Check_Button*	pIgnoreFlow;
	} com;
	struct 
	{
		Fl_Group*			g;
//		Fl_Box*				pText;
	} lpt;
	struct 
	{
		Fl_Group*			g;
		Fl_Box*				pText;
	} mdm;
	struct 
	{
		Fl_Group*			g;
		Fl_Box*				pText;
	} cas;
	struct 
	{
		Fl_Group*			g;
		Fl_Box*				pText;
	} bcr;
	struct 
	{
		Fl_Group*			g;
		Fl_Box*				pText;
		Fl_Check_Button*	pEnable;
		Fl_Slider*			pTone;
	} sound;
	struct 
	{
		Fl_Group*			g;
	} clock;
} setup_ctrl_t;

typedef struct memory_ctrl_struct	
{
	Fl_Round_Button*	pNone;
	Fl_Choice*			pMemInstalled;
	Fl_Round_Button*	pReMem;
	Fl_Round_Button*	pRampac;
	Fl_Round_Button*	pReMem_Rampac;
	Fl_Round_Button*	pRex;
	Fl_Round_Button*	pRex2;
	Fl_Check_Button*	pReMemOverride;
	Fl_Round_Button*	pQuad;
	Fl_Round_Button*	pRexQuad;
	Fl_Input*			pReMemFile;
	Fl_Input*			pRampacFile;
	Fl_Input*			pRexFlashFile;
	Fl_Input*			pRex2RamFile;
	Fl_Button*			pRexCreateFlash;
	Fl_Button*			pReMemBrowse;
	Fl_Button*			pRampacBrowse;
	Fl_Button*			pRexFlashBrowse;
	Fl_Button*			pRex2RamBrowse;
	Fl_Box*				pReMemText;
	Fl_Check_Button*	pOptRomRW;
	Fl_Check_Button*	pShowVersion;
} memory_ctrl_t;

extern "C" {
extern int			gOptRomRW;
extern int			gShowVersion;
extern int			gRamBottom;
}


Fl_Window	*gpsw;				// Peripheral Setup Window
Fl_Window	*gmsw;				// Memory Setup Window

setup_ctrl_t		setup_ctrl;	// Setup window controls
memory_ctrl_t		mem_ctrl;	// Memory setup window
peripheral_setup_t	setup;		// Setup options
memory_setup_t		mem_setup;	// Memory setup options

extern "C"
{
extern int			sound_enable;	// C-style variable to enable sound
}

/*
============================================================================
Routines to load and save setup structure to the user preferences
============================================================================
*/
void save_setup_preferences(void)
{
	// Save COM emulation settings
	virtualt_prefs.set("ComMode", setup.com_mode);
	virtualt_prefs.set("ComCmd", setup.com_cmd);
	virtualt_prefs.set("ComPort", setup.com_port);
	virtualt_prefs.set("ComOther", setup.com_other);
	virtualt_prefs.set("ComIgnoreFlow", setup.com_ignore_flow);
	if (setup.com_mode == SETUP_COM_SIM_TPDD)
		tpdd_save_prefs(ser_get_tpdd_context());

	// Save LPT emulation settings
	save_lpt_preferences(&virtualt_prefs);

	// Save MDM emulation settings

	// Save CAS emulation settings

	// Save BCR emulation settings

	// Save Sound emulation settings
	Fl_Preferences g(virtualt_prefs, "SoundGroup");
	g.set("SoundEnable", setup.sound_enable);
	g.set("ToneControl", setup.sound_tone);

	// Save Clock preference settings
	save_clock_preferences(&virtualt_prefs);
}

void load_setup_preferences(void)
{
	#ifdef	__APPLE__
		//JV 10/08/05
		int ex;
		const char *ret;
		//---------JV
	#endif

	// Load COM emulation settings
	virtualt_prefs.get("ComMode", setup.com_mode,0);
	virtualt_prefs.get("ComCmd",  setup.com_cmd,"", 128);
	virtualt_prefs.get("ComPort", setup.com_port,"", 128);
	virtualt_prefs.get("ComOther", setup.com_other,"", 128);
	virtualt_prefs.get("ComIgnoreFlow", setup.com_ignore_flow, 0);

	// Load LPT emulation settings
	load_lpt_preferences(&virtualt_prefs);

	// Load MDM emulation settings

	// Load CAS emulation settings

	// Load BCR emulation settings

	// Load Sound emulation settings
	Fl_Preferences g(virtualt_prefs, "SoundGroup");
	g.get("SoundEnable", setup.sound_enable, 0);
	g.get("ToneControl", setup.sound_tone, 1.0);
	
	// Now configure the sound system with the values
	sound_enable = setup.sound_enable;
	sound_set_tone_control(setup.sound_tone);

	// Load Clock emulation settings
	load_clock_preferences(&virtualt_prefs);

#ifdef	__APPLE__
	char	temp_path[512];
	struct stat	pathStat;

	//JV 08/10/05
	// Load Path Preferences
	ex = virtualt_prefs.get("Path", temp_path, ".", 512); 
	
	// Test if path exists in the pref file 
	if ((ex == 0) && (strlen(path) == 0))
	{
		ret = ChooseWorkDir(); // return the directory
		if(ret==NULL) 
			return; //nothing choose: do nothing....
		else 
		{
			strcpy(path,ret);
			strcat(path,"/");
			virtualt_prefs.set("Path",path); // set pref path
			load_sys_rom();
			resetcpu();
		}
	}

	// Override a program guessed path with the user's preference
	else if ((ex != 0) && (strlen(temp_path) > 0))
	{
		// Override the path only if we can stat the directory
		if (stat(temp_path, &pathStat) == 0)
		{
			strcpy(path, temp_path);
		}
	}
#if 0
	if (strcmp(path, "./") == 0)
	{
		getcwd(path, sizeof(path));
		strcat(path, "/");
	}
#endif

#endif
}

/*
============================================================================
Callback routines for the Setup Window
============================================================================
*/
void cb_setup_cancel (Fl_Widget* w, void*)
{
	gpsw->hide();
	delete gpsw;
	gpsw = NULL;
}

void cb_setup_OK(Fl_Widget* w, void*)
{
	int		open_port = FALSE;

	// First check if Host port needs to be closed
	if (setup.com_mode == SETUP_COM_HOST)
	{
		// Check if we are turning Host port emulation off
		if (setup_ctrl.com.pHost->value() != 1)
		{
			ser_close_port();
			open_port = TRUE;
		}

		// Check if we are changing ports
		if (setup_ctrl.com.pPort->text() != NULL)
		{
			if (strcmp(setup_ctrl.com.pPort->text(), setup.com_port) != 0)
			{	
				ser_close_port();
				open_port = TRUE;
			}
		}
	}

	if (setup.com_mode == SETUP_COM_OTHER)
	{
		// Check if we are turning Host port emulation off
		if (setup_ctrl.com.pOther->value() != 1)
		{
			ser_close_port();
			open_port = TRUE;
		}

		// Check if we are changing ports
		if (strcmp(setup_ctrl.com.pOtherName->value(), setup.com_other) != 0)
		{
			ser_close_port();
			open_port = TRUE;
		}
	}

	// ===========================
	// Get COM options
	// ===========================
	if (setup_ctrl.com.pCmd != NULL)
		strcpy(setup.com_cmd, setup_ctrl.com.pCmd->value());
	else
		setup.com_cmd[0] = '\0';
	if (setup_ctrl.com.pPort->text() != NULL)
		strcpy(setup.com_port, setup_ctrl.com.pPort->text());
	else
		strcpy(setup.com_port, "");
	strcpy(setup.com_other, setup_ctrl.com.pOtherName->value());

	// Get the com mode
	enable_tpdd_log_menu(FALSE);
	if (setup_ctrl.com.pNone->value() == 1)
	{
		ser_set_port((char *) "No Emulation");
		setup.com_mode = SETUP_COM_NONE;
	}
	else if (setup_ctrl.com.pSim->value() == 1)
	{
		// Open the Script file
//		ser_set_port(setup.com_cmd);
		ser_set_port(tpdd_get_port_name(ser_get_tpdd_context()));
		setup.com_mode = SETUP_COM_SIM_TPDD;
		ser_open_port();
		enable_tpdd_log_menu(TRUE);
	}
	else if (setup_ctrl.com.pHost->value() == 1)
	{
		// Set preference in structure
		setup.com_mode = SETUP_COM_HOST;

		// Open the Host port
		if (open_port)
		{
			ser_set_port(setup.com_port);
			ser_open_port();
		}
	}
	else if (setup_ctrl.com.pOther->value() == 1)
	{
		// Set preference in structure
		setup.com_mode = SETUP_COM_OTHER;

		// Open the Host port
		if (open_port)
		{
			ser_set_port(setup.com_other);
			ser_open_port();
		}
	}

	setup.com_ignore_flow = setup_ctrl.com.pIgnoreFlow->value();

	// ===========================
	// Get LPT options
	// ===========================
	get_lpt_options();

	// ===========================
	// Get MDM options
	// ===========================
//	get_mdm_options();

	// ===========================
	// Get CAS options
	// ===========================
//	get_cas_options();

	// ===========================
	// Get BCR options
	// ===========================

	// ===========================
	// Get Sound options
	// ===========================
	setup.sound_enable = setup_ctrl.sound.pEnable->value();
	setup.sound_tone = setup_ctrl.sound.pTone->value();
	sound_set_tone_control(setup.sound_tone);
	sound_enable = setup.sound_enable;

	// ===========================
	// Get Clock options
	// ===========================
	get_clock_options();

	// Save setup preferences to file
	save_setup_preferences();

	// Destroy the window
	gpsw->hide();
	delete gpsw;
}

void cb_setupwin (Fl_Widget* w, void*)
{
	gpsw->hide();
	delete gpsw;
	gpsw = NULL;
}

void cb_memorywin (Fl_Widget* w, void*)
{
	gmsw->hide();
	delete gmsw;
	gpsw = NULL;
}

/*
============================================================================
Callback routines for the COM Tab
============================================================================
*/
void cb_com_radio_none (Fl_Widget* w, void*)
{
	setup_ctrl.com.pTpddConfig->deactivate();
	setup_ctrl.com.pPort->deactivate();
	setup_ctrl.com.pOtherName->deactivate();
}

void cb_com_radio_sim (Fl_Widget* w, void*)
{
	setup_ctrl.com.pTpddConfig->activate();
	setup_ctrl.com.pPort->deactivate();
	setup_ctrl.com.pOtherName->deactivate();
}

void cb_com_radio_sim_tpdd (Fl_Widget* w, void*)
{
	setup_ctrl.com.pTpddConfig->activate();
	setup_ctrl.com.pPort->deactivate();
	setup_ctrl.com.pOtherName->deactivate();
}

void cb_com_radio_host (Fl_Widget* w, void*)
{
	setup_ctrl.com.pTpddConfig->deactivate();
	setup_ctrl.com.pPort->activate();
	setup_ctrl.com.pOtherName->deactivate();
}

void cb_com_radio_other (Fl_Widget* w, void*)
{
	setup_ctrl.com.pTpddConfig->deactivate();
	setup_ctrl.com.pPort->deactivate();
	setup_ctrl.com.pOtherName->activate();
}

void cb_com_sim_tpdd_config(Fl_Widget* w, void*)
{
	// Call the TPDD server's configure routine
	tpdd_server_config();
}

/*
============================================================================
Routine to create the PeripheralSetup Window and tabs
============================================================================
*/
void cb_PeripheralSetup (Fl_Widget* w, void*)
{
	char	com_port_list[256];
	int		count;
	char	*token;
	int		index;

	// Create Peripheral Setup window
	gpsw = new Fl_Window(350, 300, "Peripheral Setup");
	gpsw->callback(cb_setupwin);

	// Create Peripheral Tabs
    {  
		setup_ctrl.pTabs = new Fl_Tabs(10, 10, 330, 240);

		// COM port Tab
		{ 
			setup_ctrl.com.g = new Fl_Group(10, 30, 350, 260, " COM ");

			// Create items on the Tab
			setup_ctrl.com.pNone = new Fl_Round_Button(20, 40, 180, 20, "No emulation");
			setup_ctrl.com.pNone->type(FL_RADIO_BUTTON);
			setup_ctrl.com.pNone->callback(cb_com_radio_none);

			// Create controls to select simulated TPDD client connection
			setup_ctrl.com.pSim = new Fl_Round_Button(20, 65, 220, 20, "Connect to simulated NADSBox");
			setup_ctrl.com.pSim->type(FL_RADIO_BUTTON);
			setup_ctrl.com.pSim->callback(cb_com_radio_sim_tpdd);

			setup_ctrl.com.pTpddConfig = new Fl_Button(50, 90, 80, 20, "Configure");
			if (setup.com_mode != SETUP_COM_SIM_TPDD)
				setup_ctrl.com.pTpddConfig->deactivate();
			setup_ctrl.com.pTpddConfig->callback(cb_com_sim_tpdd_config);

			setup_ctrl.com.pCmd = NULL;

#if 0
			// Old simulation file controls...
			setup_ctrl.com.pSim = new Fl_Round_Button(20, 65, 180, 20, "Use Simulated Port (not supported yet)");
			setup_ctrl.com.pSim->type(FL_RADIO_BUTTON);
			setup_ctrl.com.pSim->callback(cb_com_radio_sim);

			setup_ctrl.com.pCmd = new Fl_Input(105, 90, 200, 20, "Cmd File:");
			if (setup.com_mode != SETUP_COM_SIMULATED)
				setup_ctrl.com.pCmd->deactivate();
			setup_ctrl.com.pCmd->value(setup.com_cmd);
#endif
			setup_ctrl.com.pHost = new Fl_Round_Button(20, 115, 180, 20, "Use Host Port");
			setup_ctrl.com.pHost->type(FL_RADIO_BUTTON);
			setup_ctrl.com.pHost->callback(cb_com_radio_host);

			setup_ctrl.com.pPort = new Fl_Choice(50, 137, 240, 20, "");
			if (setup.com_mode != SETUP_COM_HOST)
				setup_ctrl.com.pPort->deactivate();

			setup_ctrl.com.pOther = new Fl_Round_Button(20, 165, 180, 20, "Other Host Port");
			setup_ctrl.com.pOther->type(FL_RADIO_BUTTON);
			setup_ctrl.com.pOther->callback(cb_com_radio_other);

			setup_ctrl.com.pOtherName = new Fl_Input(50, 185, 240, 20, "");
			if (setup.com_mode != SETUP_COM_OTHER)
				setup_ctrl.com.pOtherName->deactivate();
			setup_ctrl.com.pOtherName->value(setup.com_other);

			setup_ctrl.com.pIgnoreFlow = new Fl_Check_Button(20, 210, 180, 20, "Ignore Flow Control");
			setup_ctrl.com.pIgnoreFlow->value(setup.com_ignore_flow);


			// Get list of COM ports on the host
			ser_get_port_list(com_port_list, 256, &count);
			token = strtok(com_port_list, ",");
			while (token != 0)
			{
				index = setup_ctrl.com.pPort->add(token);

				if (strcmp(token, setup.com_port) == 0)
					setup_ctrl.com.pPort->value(index);


				// Get next item from list
				token = strtok(NULL, ",");
			}

			if (setup.com_mode == SETUP_COM_NONE)
				setup_ctrl.com.pNone->value(1);
			else if (setup.com_mode == SETUP_COM_HOST)
				setup_ctrl.com.pHost->value(1);
			else if (setup.com_mode == SETUP_COM_SIM_TPDD)
				setup_ctrl.com.pSim->value(1);
			else if (setup.com_mode == SETUP_COM_OTHER)
				setup_ctrl.com.pOther->value(1);

			// End of Controls for this tab
			setup_ctrl.com.g->end();
		}

		// LPT Port Tab
		{ 
			// Create the Group item (the "Tab")
			setup_ctrl.lpt.g = new Fl_Group(10, 30, 350, 260, " LPT ");

			// Create controls
//			setup_ctrl.lpt.pText = new Fl_Box(120, 60, 60, 80, "Parallel Port not supported yet");
			build_lpt_setup_tab();

			// End of control for this tab
			setup_ctrl.lpt.g->end();
		}

		// Modem Port Tab
		{
			// Create the Group item (the "Tab")
			setup_ctrl.mdm.g = new Fl_Group(10, 30, 350, 260, " MDM ");

			// Create controls
			setup_ctrl.mdm.pText = new Fl_Box(120, 60, 60, 80, "Modem Port not supported yet");

			// End of control for this tab
			setup_ctrl.mdm.g->end();
		}

		// Cassette Port Tab
		{ 
			// Create the Group item (the "Tab")
			setup_ctrl.cas.g = new Fl_Group(10, 30, 300, 260, " CAS ");

			// Create controls
			setup_ctrl.cas.pText = new Fl_Box(120, 60, 60, 80, "Cassette Port not supported yet");

			// End of control for this tab
			setup_ctrl.cas.g->end();
		}

		// BCR Port Tab
		{ 
			// Create the Group item (the "Tab")
//			setup_ctrl.bcr.g = new Fl_Group(10, 30, 300, 260, " BCR ");

			// Create controls
//			setup_ctrl.bcr.pText = new Fl_Box(120, 60, 60, 80, "BCR Port not supported yet");

			// End of control for this tab
//			setup_ctrl.bcr.g->end();
		}

		// Sound Port Tab
		{ 
			// Create the Group item (the "Tab")
			setup_ctrl.sound.g = new Fl_Group(10, 30, 300, 260, " Sound ");

			// Create controls
			setup_ctrl.sound.pEnable = new Fl_Check_Button(20, 40, 180, 20, "Enable Sound");
			setup_ctrl.sound.pEnable->value(setup.sound_enable);

			// Create a tone control slider
			//Fl_Box* b = new Fl_Box(20, 80, 100, 20, "Tone Control");
			setup_ctrl.sound.pTone = new Fl_Slider(140, 80, 180, 20, "");
			setup_ctrl.sound.pTone->type(FL_HOR_NICE_SLIDER);
			setup_ctrl.sound.pTone->slider(FL_NO_BOX);
			setup_ctrl.sound.pTone->align(FL_ALIGN_LEFT);
			setup_ctrl.sound.pTone->minimum(1);
			setup_ctrl.sound.pTone->maximum(5);
			double tone = sound_get_tone_control();
			setup_ctrl.sound.pTone->value(tone);
			setup_ctrl.sound.pTone->hide();
//			setup_ctrl.sound.pTone->step(1);

			// Create text to describe tone control
			//b = new Fl_Box(130, 100, 50, 20, "Agressive");
			//b = new Fl_Box(280, 100, 50, 20, "Soft");

			// End of control for this tab
			setup_ctrl.sound.g->end();
		}

		// Clock Port Tab
		{ 
			// Create the Group item (the "Tab")
			setup_ctrl.clock.g = new Fl_Group(10, 30, 350, 260, " Clock ");

			// Create controls
			build_clock_setup_tab();

			// End of control for this tab
			setup_ctrl.clock.g->end();
		}

		setup_ctrl.pTabs->value(setup_ctrl.com.g);
		setup_ctrl.pTabs->end();
																															 
	}

	// OK button
    { Fl_Button* o = new Fl_Button(180, 260, 60, 30, "Cancel");
      o->callback((Fl_Callback*)cb_setup_cancel);
    }
    { Fl_Return_Button* o = new Fl_Return_Button(250, 260, 60, 30, "OK");
      o->callback((Fl_Callback*)cb_setup_OK);
    }

	gpsw->show();
}

/*
============================================================================
Sets the current gRamBottom base on model.
============================================================================
*/
void set_memory_base(void)
{
	switch (gModel)
	{
	case MODEL_T200:
		gRamBottom = ROMSIZE;
		break;

	case MODEL_PC8201:
		gRamBottom = ROMSIZE;
		break;

	default:
		switch (mem_setup.mem_installed)
		{
		case 0:  gRamBottom = 0x8000; break;
		case 1:	 gRamBottom = 0xA000; break;
		case 2:  gRamBottom = 0xC000; break;
		case 3:  gRamBottom = 0xE000; break;
		default: gRamBottom = 0x8000; break;
		}

		break;
	}
}

/*
============================================================================
Routines to load and save setup structure to the user preferences
============================================================================
*/
void save_memory_preferences(void)
{
	char	str[16];
	char	pref[64];

	get_model_string(str, gModel);

	// Save Memory emulation settings
	strcpy(pref, str);
	strcat(pref, "_MemMode");
	virtualt_prefs.set(pref, mem_setup.mem_mode);

	strcpy(pref, str);
	strcat(pref, "_MemInstalled");
	virtualt_prefs.set(pref, mem_setup.mem_installed);

	strcpy(pref, str);
	strcat(pref, "_ReMemOverride");
	virtualt_prefs.set(pref, mem_setup.remem_override);

	strcpy(pref, str);
	strcat(pref, "_ReMemFile");
	virtualt_prefs.set(pref, mem_setup.remem_file);

	strcpy(pref, str);
	strcat(pref, "_RampacFile");
	virtualt_prefs.set(pref, mem_setup.rampac_file);

	strcpy(pref, str);
	strcat(pref, "_RexFlashFile");
	virtualt_prefs.set(pref, mem_setup.rex_flash_file);

	strcpy(pref, str);
	strcat(pref, "_Rex2RamFile");
	virtualt_prefs.set(pref, mem_setup.rex2_ram_file);

	strcpy(pref, str);
	strcat(pref, "_OptRomRW");
	virtualt_prefs.set(pref, gOptRomRW);

	strcpy(pref, str);
	strcat(pref, "_ShowVersion");
	virtualt_prefs.set(pref, gShowVersion);

	// Update the BASE RAM size
	set_memory_base();
}

/*
============================================================================
Routine to laod the memory preferences from the preferences file.
============================================================================
*/
void load_memory_preferences(void)
{
	char	str[16];
	char	pref[64];
	char	path[256];

	get_model_string(str, gModel);

	// Load mem emulation mode base on Model
	strcpy(pref, str);
	strcat(pref, "_MemMode");
	virtualt_prefs.get(pref, mem_setup.mem_mode,0);

	// Load mem size installed based on Model
	strcpy(pref, str);
	strcat(pref, "_MemInstalled");
	virtualt_prefs.get(pref, mem_setup.mem_installed,0);

	// Load ReMemOverride setting
	strcpy(pref, str);
	strcat(pref, "_ReMemOverride");
	virtualt_prefs.get(pref, mem_setup.remem_override,1);

	// Load ReMem filename base on Model
	strcpy(pref, str);
	strcat(pref, "_ReMemFile");
	get_emulation_path(path, gModel);
	strcat(path, "remem.bin");
	virtualt_prefs.get(pref,  mem_setup.remem_file, path, 256);
	if (strlen(mem_setup.remem_file) == 0)
		strcpy(mem_setup.remem_file, path);

	// Load Rampac filename base on Model
	strcpy(pref, str);
	strcat(pref, "_RampacFile");
	get_emulation_path(path, gModel);
	strcat(path, "rampac.bin");
	virtualt_prefs.get(pref, mem_setup.rampac_file, path, 256);
	if (strlen(mem_setup.rampac_file) == 0)
		strcpy(mem_setup.rampac_file, path);

	// Load Rex Flash filename based on Model
	strcpy(pref, str);
	strcat(pref, "_RexFlashFile");
	get_emulation_path(path, gModel);
	strcat(path, "rex_flash.bin");
	virtualt_prefs.get(pref, mem_setup.rex_flash_file, path, 256);
	if (strlen(mem_setup.rex_flash_file) == 0)
		strcpy(mem_setup.rex_flash_file, path);

	// Load Rex2 Ram filename based on Model
	strcpy(pref, str);
	strcat(pref, "_Rex2RamFile");
	get_emulation_path(path, gModel);
	strcat(path, "rex2_ram.bin");
	virtualt_prefs.get(pref, mem_setup.rex2_ram_file, path, 256);
	if (strlen(mem_setup.rex2_ram_file) == 0)
		strcpy(mem_setup.rex2_ram_file, path);

	// Load OptRom R/W or R/O option
	strcpy(pref, str);
	strcat(pref, "_OptRomRW");
	virtualt_prefs.get(pref, gOptRomRW, 0);

	// Load Show Version on MENU option
	strcpy(pref, str);
	strcat(pref, "_ShowVersion");
	virtualt_prefs.get(pref, gShowVersion, 1);

	// Update the BASE RAM size
	set_memory_base();
}

/*
============================================================================
Callback routines for the Memory options window
============================================================================
*/
void cb_memory_OK(Fl_Widget* w, void*)
{
	int		old_mode;

	/* 
	===================================================
	Save Base memory if needed
	===================================================
	*/
	if ((mem_setup.mem_mode == SETUP_MEM_RAMPAC) || (mem_setup.mem_mode == SETUP_MEM_BASE) ||
		(mem_setup.mem_mode == SETUP_MEM_REX) || (mem_setup.mem_mode == SETUP_MEM_QUAD) ||
        (mem_setup.mem_mode == SETUP_MEM_REX_QUAD))
		save_ram();

	/* 
	===================================================
	Check if ReMem memory needs to be deallocated
	===================================================
	*/
	if ((mem_setup.mem_mode == SETUP_MEM_REMEM) || (mem_setup.mem_mode == SETUP_MEM_REMEM_RAMPAC))
	{
		// Check if we are turning ReMem emulation off
		if ((mem_ctrl.pReMem->value() != 1) && (mem_ctrl.pReMem_Rampac->value() != 1))
		{
			save_remem_ram();		// Write ReMem memory to file
			free_remem_mem();		// Deallocate ReMem memory
		}
	}

	/* 
	===================================================
	Next check if Rampac memory needs to be deallocated
	===================================================
	*/
	if ((mem_setup.mem_mode == SETUP_MEM_RAMPAC) || (mem_setup.mem_mode == SETUP_MEM_REMEM_RAMPAC))
	{
		// Check if we are turning Host port emulation off
			save_rampac_ram();		// Write Rampac memory to file
	}

	/* 
	===================================================
	Check if Rex Flash needs to be saved / deallocated
	===================================================
	*/
	if ((mem_setup.mem_mode == SETUP_MEM_REX) || (mem_setup.mem_mode == SETUP_MEM_REX2))
	{
		// Save Rex Flash 
		save_rex_flash();		// Write Rex flash memory to file
	}

	/* 
	===================================================
	Check if Rex Flash needs to be saved / deallocated
	===================================================
	*/
	if (mem_setup.mem_mode == SETUP_MEM_REX2)
	{
		// Save Rex Flash 
		save_rex2_ram();			// Write Rex RAM memory to file
	}

	// Save old mem_mode so we know when to load data from file
	old_mode = mem_setup.mem_mode;

	// ===========================
	// Get memory options
	// ===========================
	if (mem_ctrl.pNone->value() == 1)
		mem_setup.mem_mode = SETUP_MEM_BASE;
	else if (mem_ctrl.pRampac->value() == 1)
		mem_setup.mem_mode = SETUP_MEM_RAMPAC;
	else if (mem_ctrl.pReMem->value() == 1)
		mem_setup.mem_mode = SETUP_MEM_REMEM;
	else if (mem_ctrl.pReMem_Rampac->value() == 1)
		mem_setup.mem_mode = SETUP_MEM_REMEM_RAMPAC;
	else if (mem_ctrl.pRex->value() == 1)
		mem_setup.mem_mode = SETUP_MEM_REX;
	else if (mem_ctrl.pRex2->value() == 1)
		mem_setup.mem_mode = SETUP_MEM_REX2;
	else if (mem_ctrl.pQuad->value() == 1)
		mem_setup.mem_mode = SETUP_MEM_QUAD;
	else if (mem_ctrl.pRexQuad->value() == 1)
		mem_setup.mem_mode = SETUP_MEM_REX_QUAD;

	// Get Memory installed selection
	mem_setup.mem_installed = mem_ctrl.pMemInstalled->value();

	// Get OptRom R/W Enable setting
	gOptRomRW = mem_ctrl.pOptRomRW->value();

	// Get Show Version setting
	gShowVersion = mem_ctrl.pShowVersion->value();

	// Get ReMemOverride settin
	mem_setup.remem_override = mem_ctrl.pReMemOverride->value();

	// Allocate ReMem and / or Rampac memory if not already
	init_mem();
	init_menus();

	/* 
	===================================================
	Load Base memory if needed
	===================================================
	*/
	if ((mem_setup.mem_mode == SETUP_MEM_RAMPAC) || (mem_setup.mem_mode == SETUP_MEM_BASE) ||
		(mem_setup.mem_mode == SETUP_MEM_REX) || (mem_setup.mem_mode == SETUP_MEM_QUAD) ||
        (mem_setup.mem_mode == SETUP_MEM_REX_QUAD))
			load_ram();

	// If we are in ReMem or ReMem_Rampac mode, check if ReMem filename changed
	if ((mem_setup.mem_mode == SETUP_MEM_REMEM) || (mem_setup.mem_mode == SETUP_MEM_REMEM_RAMPAC))
	{
		// Check if we are changing ReMem filename
		if (strcmp(mem_ctrl.pReMemFile->value(), mem_setup.remem_file) != 0)
		{
			// Save memory to old file
			save_remem_ram();

			// Copy new filename to preferences
			strcpy(mem_setup.remem_file, mem_ctrl.pReMemFile->value());

			// Load ReMem data from new file
			load_remem_ram();
		}
		else if ((old_mode != SETUP_MEM_REMEM) && (old_mode != SETUP_MEM_REMEM_RAMPAC))
		{
			// Load ReMem data from file
			load_remem_ram();
		}
	}

	// If we are in Rampac or ReMem_Rampac mode, check if Rampac filename changed
	if ((mem_setup.mem_mode == SETUP_MEM_RAMPAC) || (mem_setup.mem_mode == SETUP_MEM_REMEM_RAMPAC))
	{
		// Check if we are changing Rampac filename
		if (strcmp(mem_ctrl.pRampacFile->value(), mem_setup.rampac_file) != 0)
		{
			// Save memory to old file
			save_rampac_ram();

			// Copy new filename to preferences
			strcpy(mem_setup.rampac_file, mem_ctrl.pRampacFile->value());

			// Load Rampac data from new file
			load_rampac_ram();
		}
		else if ((old_mode != SETUP_MEM_RAMPAC) && (old_mode != SETUP_MEM_REMEM_RAMPAC))
		{
			// Load Rampac data from file
			load_rampac_ram();
		}
	}

	// If we are in Rex or Rex2 mode, check if Rex Flash filename changed
	if ((mem_setup.mem_mode == SETUP_MEM_REX) || (mem_setup.mem_mode == SETUP_MEM_REX2))
	{
		// Check if we are changing Rampac filename
		if (strcmp(mem_ctrl.pRexFlashFile->value(), mem_setup.rex_flash_file) != 0)
		{
			// Save memory to old file
			save_rex_flash();

			// Copy new filename to preferences
			strcpy(mem_setup.rex_flash_file, mem_ctrl.pRexFlashFile->value());

			// Load Rampac data from new file
			load_rex_flash();
		}
		else if ((old_mode != SETUP_MEM_REX) && (old_mode != SETUP_MEM_REX2))
		{
			// Load Rampac data from file
			load_rex_flash();
		}
	}

	// If we are in Rex or Rex2 mode, check if Rex Flash filename changed
	if (mem_setup.mem_mode == SETUP_MEM_REX2)
	{
		// Check if we are changing Rampac filename
		if (strcmp(mem_ctrl.pRex2RamFile->value(), mem_setup.rex2_ram_file) != 0)
		{
			// Save memory to old file
			save_rex2_ram();

			// Copy new filename to preferences
			strcpy(mem_setup.rex2_ram_file, mem_ctrl.pRex2RamFile->value());

			// Load Rampac data from new file
			load_rex2_ram();
		}
		else if (old_mode != SETUP_MEM_REX2)
		{
			// Load Rampac data from file
			load_rex2_ram();
		}
	}

	// Copy new ReMem filename and Rampac filename to preferences
	strcpy(mem_setup.remem_file, mem_ctrl.pReMemFile->value());
	strcpy(mem_setup.rampac_file, mem_ctrl.pRampacFile->value());
	strcpy(mem_setup.rex_flash_file, mem_ctrl.pRexFlashFile->value());
	strcpy(mem_setup.rex2_ram_file, mem_ctrl.pRex2RamFile->value());

	// Save memory preferences to file
	save_memory_preferences();

	// Update Memory Editor
	cb_MemoryEditorUpdate();

	// Reload the SysROM in case the Show Verion changed
	load_sys_rom();

	/* Reset the CPU */
	init_cpu();
	resetcpu();
	gExitLoop = 1;

    show_remem_mode();

	// Destroy the window
	gmsw->hide();
	delete gmsw;
}

void cb_remem_browse(Fl_Widget* w, void*)
{
	int					count;
	Flu_File_Chooser		*fc;
	const char			*filename;
	const char			*filename_name;
	int					len;
	char				mstr[16];
	char				mstr_upper[16];
	char				path[256];
	int					c;
	
	// Create chooser window to pick file
	strcpy(path, mem_ctrl.pReMemFile->value());
	fl_cursor(FL_CURSOR_WAIT);
	fc = new Flu_File_Chooser(path,"Binary Files (*.bin)",2,"Choose ReMem File");
	fl_cursor(FL_CURSOR_DEFAULT);
	fc->preview(0);
	fc->show();

	// Show Chooser window
	while (fc->visible())
		Fl::wait();

	count = fc->count();
	if (count == 0)
	{
		delete fc;
		return;
	}

	// Get Filename
	filename = fc->value(1);
	if (filename == 0)
	{
		delete fc;
		return;
	}
	len = strlen(filename);

	// Copy filename to edit field
	filename_name = fl_filename_name(filename);

	get_model_string(mstr, gModel);
	strcpy(mstr_upper, mstr);
	for (c = strlen(mstr_upper)-1; c >= 0; c--)
		mstr_upper[c] = toupper(mstr_upper[c]);
	if (strstr(filename, mstr) || strstr(filename, mstr_upper))
	{
		get_emulation_path(path, gModel);
		strcat(path, filename_name);
		mem_ctrl.pReMemFile->value(path);
	}
	else
		mem_ctrl.pReMemFile->value(filename);

	delete fc;
}

void cb_rex_browse(Fl_Widget* w, void*)
{
	int					count;
	Flu_File_Chooser		*fc;
	const char			*filename;
	const char			*filename_name;
	int					len;
	char				mstr[16];
	char				mstr_upper[16];
	char				path[256];
	int					c;
	
	// Create chooser window to pick file
	strcpy(path, mem_ctrl.pRexFlashFile->value());
	fl_cursor(FL_CURSOR_WAIT);
	fc = new Flu_File_Chooser(path,"Binary Files (*.bin)",2,"Choose Rex Flash File");
	fl_cursor(FL_CURSOR_DEFAULT);
	fc->preview(0);
	fc->show();

	// Show Chooser window
	while (fc->visible())
		Fl::wait();

	count = fc->count();
	if (count == 0)
	{
		delete fc;
		return;
	}

	// Get Filename
	filename = fc->value(1);
	if (filename == 0)
	{
		delete fc;
		return;
	}
	len = strlen(filename);

	// Copy filename to edit field
	filename_name = fl_filename_name(filename);

	get_model_string(mstr, gModel);
	strcpy(mstr_upper, mstr);
	for (c = strlen(mstr_upper)-1; c >= 0; c--)
		mstr_upper[c] = toupper(mstr_upper[c]);
	if (strstr(filename, mstr) || strstr(filename, mstr_upper))
	{
		get_emulation_path(path, gModel);
		strcat(path, filename_name);
		mem_ctrl.pRexFlashFile->value(path);
	}
	else
		mem_ctrl.pRexFlashFile->value(filename);

	delete fc;
}

void cb_rex2_browse(Fl_Widget* w, void*)
{
	int					count;
	Flu_File_Chooser		*fc;
	const char			*filename;
	const char			*filename_name;
	int					len;
	char				mstr[16];
	char				mstr_upper[16];
	char				path[256];
	int					c;
	
	// Create chooser window to pick file
	strcpy(path, mem_ctrl.pRex2RamFile->value());
	fl_cursor(FL_CURSOR_WAIT);
	fc = new Flu_File_Chooser(path,"Binary Files (*.bin)",2,"Choose Rex2 RAM File");
	fl_cursor(FL_CURSOR_DEFAULT);
	fc->preview(0);
	fc->show();

	// Show Chooser window
	while (fc->visible())
		Fl::wait();

	count = fc->count();
	if (count == 0)
	{
		delete fc;
		return;
	}

	// Get Filename
	filename = fc->value(1);
	if (filename == 0)
	{
		delete fc;
		return;
	}
	len = strlen(filename);

	// Copy filename to edit field
	filename_name = fl_filename_name(filename);

	get_model_string(mstr, gModel);
	strcpy(mstr_upper, mstr);
	for (c = strlen(mstr_upper)-1; c >= 0; c--)
		mstr_upper[c] = toupper(mstr_upper[c]);
	if (strstr(filename, mstr) || strstr(filename, mstr_upper))
	{
		get_emulation_path(path, gModel);
		strcat(path, filename_name);
		mem_ctrl.pRex2RamFile->value(path);
	}
	else
		mem_ctrl.pRex2RamFile->value(filename);

	delete fc;
}

void cb_radio_base_memory (Fl_Widget* w, void*)
{
	mem_ctrl.pRampacFile->deactivate();
	mem_ctrl.pRampacBrowse->deactivate();
	mem_ctrl.pReMemOverride->deactivate();
	mem_ctrl.pReMemFile->deactivate();
	mem_ctrl.pReMemBrowse->deactivate();
	mem_ctrl.pReMemText->hide();
	mem_ctrl.pRexFlashFile->deactivate();
	mem_ctrl.pRexFlashBrowse->deactivate();
	mem_ctrl.pRex2RamFile->deactivate();
	mem_ctrl.pRex2RamBrowse->deactivate();
	mem_ctrl.pRexCreateFlash->deactivate();
}

void cb_radio_remem (Fl_Widget* w, void*)
{
	mem_ctrl.pReMemFile->activate();
	mem_ctrl.pReMemBrowse->activate();
	mem_ctrl.pReMemOverride->deactivate();
	mem_ctrl.pRampacFile->deactivate();
	mem_ctrl.pRampacBrowse->deactivate();
	mem_ctrl.pReMemText->show();
	mem_ctrl.pRexFlashFile->deactivate();
	mem_ctrl.pRexFlashBrowse->deactivate();
	mem_ctrl.pRex2RamFile->deactivate();
	mem_ctrl.pRex2RamBrowse->deactivate();
	mem_ctrl.pRexCreateFlash->deactivate();
}

void cb_radio_rampac (Fl_Widget* w, void*)
{
	mem_ctrl.pRampacFile->activate();
	mem_ctrl.pRampacBrowse->activate();
	mem_ctrl.pReMemOverride->deactivate();
	mem_ctrl.pReMemFile->deactivate();
	mem_ctrl.pReMemBrowse->deactivate();
	mem_ctrl.pReMemText->hide();
	mem_ctrl.pRexFlashFile->deactivate();
	mem_ctrl.pRexFlashBrowse->deactivate();
	mem_ctrl.pRex2RamFile->deactivate();
	mem_ctrl.pRex2RamBrowse->deactivate();
	mem_ctrl.pRexCreateFlash->deactivate();
}

void cb_radio_remem_and_rampac (Fl_Widget* w, void*)
{
	mem_ctrl.pReMemFile->activate();
	mem_ctrl.pReMemBrowse->activate();
	mem_ctrl.pReMemOverride->activate();
	mem_ctrl.pRampacFile->activate();
	mem_ctrl.pRampacBrowse->activate();
	mem_ctrl.pReMemText->show();
	mem_ctrl.pRexFlashFile->deactivate();
	mem_ctrl.pRexFlashBrowse->deactivate();
	mem_ctrl.pRex2RamFile->deactivate();
	mem_ctrl.pRex2RamBrowse->deactivate();
	mem_ctrl.pRexCreateFlash->deactivate();
}

void cb_radio_rex (Fl_Widget* w, void*)
{
	mem_ctrl.pRampacFile->deactivate();
	mem_ctrl.pRampacBrowse->deactivate();
	mem_ctrl.pReMemOverride->deactivate();
	mem_ctrl.pReMemFile->deactivate();
	mem_ctrl.pReMemBrowse->deactivate();
	mem_ctrl.pReMemText->hide();
	mem_ctrl.pRexFlashFile->activate();
	mem_ctrl.pRexFlashBrowse->activate();
	mem_ctrl.pRex2RamFile->deactivate();
	mem_ctrl.pRex2RamBrowse->deactivate();
	mem_ctrl.pRexCreateFlash->activate();
}

void cb_radio_rex2 (Fl_Widget* w, void*)
{
	mem_ctrl.pRampacFile->deactivate();
	mem_ctrl.pRampacBrowse->deactivate();
	mem_ctrl.pReMemOverride->deactivate();
	mem_ctrl.pReMemFile->deactivate();
	mem_ctrl.pReMemBrowse->deactivate();
	mem_ctrl.pReMemText->hide();
	mem_ctrl.pRexFlashFile->activate();
	mem_ctrl.pRexFlashBrowse->activate();
	mem_ctrl.pRex2RamFile->activate();
	mem_ctrl.pRex2RamBrowse->activate();
	mem_ctrl.pRexCreateFlash->activate();
}

void cb_radio_quad (Fl_Widget* w, void*)
{
	mem_ctrl.pRampacFile->deactivate();
	mem_ctrl.pRampacBrowse->deactivate();
	mem_ctrl.pReMemOverride->deactivate();
	mem_ctrl.pReMemFile->deactivate();
	mem_ctrl.pReMemBrowse->deactivate();
	mem_ctrl.pReMemText->hide();
	mem_ctrl.pRexFlashFile->deactivate();
	mem_ctrl.pRexFlashBrowse->deactivate();
	mem_ctrl.pRex2RamFile->deactivate();
	mem_ctrl.pRex2RamBrowse->deactivate();
	mem_ctrl.pRexCreateFlash->deactivate();
}

void cb_radio_rex_quad (Fl_Widget* w, void*)
{
	mem_ctrl.pRampacFile->deactivate();
	mem_ctrl.pRampacBrowse->deactivate();
	mem_ctrl.pReMemOverride->deactivate();
	mem_ctrl.pReMemFile->deactivate();
	mem_ctrl.pReMemBrowse->deactivate();
	mem_ctrl.pReMemText->hide();
	mem_ctrl.pRexFlashFile->activate();
	mem_ctrl.pRexFlashBrowse->activate();
	mem_ctrl.pRex2RamFile->deactivate();
	mem_ctrl.pRex2RamBrowse->deactivate();
	mem_ctrl.pRexCreateFlash->activate();
}

void cb_memory_cancel (Fl_Widget* w, void*)
{
	gmsw->hide();
	delete gpsw;
}

void cb_rampac_browse (Fl_Widget* w, void*)
{
	int					count;
	Flu_File_Chooser		*fc;
	const char			*filename;
	const char			*filename_name;
	int					len;
	char				mstr[16];
	char				mstr_upper[16];
	char				path[256];
	int					c;
	
	// Create chooser window to pick file
	strcpy(path, mem_ctrl.pRampacFile->value());
	fl_cursor(FL_CURSOR_WAIT);
	fc = new Flu_File_Chooser(path,"Binary Files (*.bin)",2,"Choose Rampac File");
	fl_cursor(FL_CURSOR_DEFAULT);
	fc->preview(0);
	fc->show();

	// Show Chooser window
	while (fc->visible())
		Fl::wait();

	count = fc->count();
	if (count == 0)
	{
		delete fc;
		return;
	}

	// Get Filename
	filename = fc->value(1);
	if (filename == 0)
	{
		delete fc;
		return;
	}
	len = strlen(filename);

	// Copy filename to edit field
	filename_name = fl_filename_name(filename);

	get_model_string(mstr, gModel);
	strcpy(mstr_upper, mstr);
	for (c = strlen(mstr_upper)-1; c >= 0; c--)
		mstr_upper[c] = toupper(mstr_upper[c]);
	if (strstr(filename, mstr) || strstr(filename, mstr_upper))
	{
		get_emulation_path(path, gModel);
		strcat(path, filename_name);
		mem_ctrl.pRampacFile->value(path);
	}
	else
		mem_ctrl.pRampacFile->value(filename);

	delete fc;
}

/*
============================================================================
Callback routine to create default REX Flash
============================================================================
*/
void cb_create_flash (Fl_Widget* w, void*)
{
	unsigned char	*pData;
	FILE*			fd;
	int				x;

	// Allocate a 1M buffer
	pData = new unsigned char[1024 * 1024];
	if (pData == NULL)
	{
		// Report error
		fl_alert("Unable to allocate a 1M buffer!");
		return;
	}

	// Check if the file already exists
	if ((fd = fopen(mem_ctrl.pRexFlashFile->value(), "r")) != NULL)
	{
		// Close the file 
		fclose(fd);
		int ret = fl_choice("Overwrite existing file?", "Yes", "No", NULL);
		if (ret == 1)
			return;
	}

	// Validate the file can be opened for write mode
	if ((fd = fopen(mem_ctrl.pRexFlashFile->value(), "wb")) == NULL)
	{
		// Report failure ot open file
		fl_alert("Unable to open file %s", mem_ctrl.pRexFlashFile->value());
		delete[] pData;
		return;
	}

	// Fill the buffer with 0xFF
	for (x = 0; x < 1024 * 1024; x++)
		pData[x] = 0xFF;

	// Now copy in the data
	if (gModel == MODEL_T200)
	{
		// Copy in REX MGR
		memcpy(pData, gRex248Image, sizeof(gRex248Image));
		memcpy(&pData[0x4000], gRexDirectories, sizeof(gRexDirectories));
		memcpy(&pData[0x8000], gRex248Image, sizeof(gRex248Image));
		memcpy(&pData[0xC000], gRexDirectories, sizeof(gRexDirectories));

		// Copy in TS-DOS
		memcpy(&pData[0x040000], gTsDos200Image, sizeof(gTsDos200Image));
	}
	else if (gModel == MODEL_M100 || gModel == MODEL_M102)
	{
		// Copy in REX MGR
		memcpy(pData, gRex148Image, sizeof(gRex148Image));
		memcpy(&pData[0x4000], gRexDirectories, sizeof(gRexDirectories));
		memcpy(&pData[0x8000], gRex148Image, sizeof(gRex148Image));
		memcpy(&pData[0xC000], gRexDirectories, sizeof(gRexDirectories));

		// Copy in TS-DOS
		memcpy(&pData[0x040000], gTsDos100Image, sizeof(gTsDos100Image));
	}

	// Write data to the file
	fwrite(pData, 1, 1024 * 1024, fd);
	fclose(fd);

	// Read the new flash
	if ((mem_setup.mem_mode == SETUP_MEM_REX) || (mem_setup.mem_mode == SETUP_MEM_REX2))
		load_rex_flash();

	// Free the buffer 
	delete[] pData;
}

/*
============================================================================
Routine to create the PeripheralSetup Window and tabs
============================================================================
*/
void cb_MemorySetup (Fl_Widget* w, void*)
{
	// Create Peripheral Setup window
	gmsw = new Fl_Window(520, 465, "Memory Emulation Options");
	gmsw->callback(cb_memorywin);

	// Create items on the Tab
	mem_ctrl.pNone = new Fl_Round_Button(20, 20, 120, 20, "Base Memory");
	mem_ctrl.pNone->type(FL_RADIO_BUTTON);
	mem_ctrl.pNone->callback(cb_radio_base_memory);
	if (mem_setup.mem_mode == SETUP_MEM_BASE)
		mem_ctrl.pNone->value(1);

	// Create list box for the amount of installed memory
	mem_ctrl.pMemInstalled = new Fl_Choice(280, 20, 60, 20, "Memory Installed:");
	mem_ctrl.pMemInstalled->align(FL_ALIGN_LEFT);
	if (gModel == MODEL_T200)
	{
		mem_ctrl.pMemInstalled->add("72K");
		mem_ctrl.pMemInstalled->add("48K");
		mem_ctrl.pMemInstalled->add("24K");
	}
	else if (gModel == MODEL_PC8201)
	{
		mem_ctrl.pMemInstalled->add("96K");
		mem_ctrl.pMemInstalled->add("64K");
		mem_ctrl.pMemInstalled->add("32K");
	}
	else if (gModel != MODEL_T200)
	{
		mem_ctrl.pMemInstalled->add("32K");
		mem_ctrl.pMemInstalled->add("24K");
		mem_ctrl.pMemInstalled->add("16K");
		mem_ctrl.pMemInstalled->add("8K");
	}
	mem_ctrl.pMemInstalled->value(mem_setup.mem_installed);

	mem_ctrl.pRampac = new Fl_Round_Button(20, 45, 180, 20, "RamPac  (256K RAM)");
	mem_ctrl.pRampac->type(FL_RADIO_BUTTON);
	mem_ctrl.pRampac->callback(cb_radio_rampac);
	if (mem_setup.mem_mode == SETUP_MEM_RAMPAC)
		mem_ctrl.pRampac->value(1);

	mem_ctrl.pReMem = new Fl_Round_Button(20, 70, 220, 20, "ReMem   (2M RAM, 4M FLASH)");
	mem_ctrl.pReMem->type(FL_RADIO_BUTTON);
	mem_ctrl.pReMem->callback(cb_radio_remem);
	if (mem_setup.mem_mode == SETUP_MEM_REMEM)
		mem_ctrl.pReMem->value(1);

	mem_ctrl.pReMem_Rampac = new Fl_Round_Button(20, 95, 180, 20, "ReMem + RamPac");
	mem_ctrl.pReMem_Rampac->type(FL_RADIO_BUTTON);
	mem_ctrl.pReMem_Rampac->callback(cb_radio_remem_and_rampac);
	if (mem_setup.mem_mode == SETUP_MEM_REMEM_RAMPAC)
		mem_ctrl.pReMem_Rampac->value(1);

	mem_ctrl.pReMemOverride = new Fl_Check_Button(210, 95, 210, 20, "ReMem overrides Rampac");
	if (mem_setup.remem_override)
		mem_ctrl.pReMemOverride->value(1);
	if (mem_setup.mem_mode != SETUP_MEM_REMEM_RAMPAC)
		mem_ctrl.pReMemOverride->deactivate();

	// ===============================================
	// Setup Rampac File Edit field and Browser button
	// ===============================================
	mem_ctrl.pRampacFile = new Fl_Input(105, 130, 310, 20, "RamPac File");
	mem_ctrl.pRampacFile->value(mem_setup.rampac_file);

	mem_ctrl.pRampacBrowse =	new Fl_Button(430, 125, 60, 30, "Browse");
    mem_ctrl.pRampacBrowse->callback((Fl_Callback*)cb_rampac_browse);

	if ((mem_setup.mem_mode != SETUP_MEM_RAMPAC) && (mem_setup.mem_mode != SETUP_MEM_REMEM_RAMPAC))
	{
		mem_ctrl.pRampacFile->deactivate();
		mem_ctrl.pRampacBrowse->deactivate();
	}

	// ===============================================
	// Setup ReMem File edit field and Browser button
	// ===============================================
	mem_ctrl.pReMemFile = new Fl_Input(105, 170, 310, 20, "ReMem  File");
	mem_ctrl.pReMemFile->value(mem_setup.remem_file);
    mem_ctrl.pReMemText = new Fl_Box(45, 190, 325, 20, "(Use Memory Editor to load FLASH)");
    mem_ctrl.pReMemText->labelsize(12);

	mem_ctrl.pReMemBrowse = new Fl_Button(430, 165, 60, 30, "Browse");
    mem_ctrl.pReMemBrowse->callback((Fl_Callback*)cb_remem_browse);

	if ((mem_setup.mem_mode != SETUP_MEM_REMEM) && (mem_setup.mem_mode != SETUP_MEM_REMEM_RAMPAC))
	{
		mem_ctrl.pReMemFile->deactivate();
		mem_ctrl.pReMemBrowse->deactivate();
		mem_ctrl.pReMemText->hide();
	}


    // Add radio button for QUAD
	mem_ctrl.pQuad = new Fl_Round_Button(20, 210, 270, 20, "QUAD    (128K Banked RAM)");
	mem_ctrl.pQuad->type(FL_RADIO_BUTTON);
	mem_ctrl.pQuad->callback(cb_radio_quad);
	if (mem_setup.mem_mode == SETUP_MEM_QUAD)
		mem_ctrl.pQuad->value(1);

    // Add radio button for REX+QUAD
	mem_ctrl.pRexQuad = new Fl_Round_Button(20, 235, 270, 20, "QUAD + REX");
	mem_ctrl.pRexQuad->type(FL_RADIO_BUTTON);
	mem_ctrl.pRexQuad->callback(cb_radio_rex_quad);
	if (mem_setup.mem_mode == SETUP_MEM_REX_QUAD)
		mem_ctrl.pRexQuad->value(1);

    if (gModel != MODEL_M100)
    {
        mem_ctrl.pQuad->deactivate();
        mem_ctrl.pRexQuad->deactivate();
    }

	// Create Rex radio button
	mem_ctrl.pRex = new Fl_Round_Button(20, 260, 270, 20, "Rex     (1M Flash Option ROM)");
	mem_ctrl.pRex->type(FL_RADIO_BUTTON);
	mem_ctrl.pRex->callback(cb_radio_rex);
	if (mem_setup.mem_mode == SETUP_MEM_REX)
		mem_ctrl.pRex->value(1);

	// Create Rex radio button
	mem_ctrl.pRex2 = new Fl_Round_Button(20, 285, 270, 20, "Rex2   (1M Opt ROM + 128K SRAM)");
	mem_ctrl.pRex2->type(FL_RADIO_BUTTON);
	mem_ctrl.pRex2->callback(cb_radio_rex2);
	if (mem_setup.mem_mode == SETUP_MEM_REX2)
		mem_ctrl.pRex2->value(1);

	mem_ctrl.pRexCreateFlash = new Fl_Button(300, 270, 170, 20, "Create Default Flash");
	if (mem_setup.mem_mode != SETUP_MEM_REX && mem_setup.mem_mode != SETUP_MEM_REX2 &&
        mem_setup.mem_mode != SETUP_MEM_REX_QUAD)
            mem_ctrl.pRexCreateFlash->deactivate();
	mem_ctrl.pRexCreateFlash->callback(cb_create_flash, &mem_ctrl);

	// ===============================================
	// Setup Rex Flash File edit field and Browser button
	// ===============================================
	mem_ctrl.pRexFlashFile = new Fl_Input(105, 310, 310, 20, "Flash File");
	mem_ctrl.pRexFlashFile->value(mem_setup.rex_flash_file);
	mem_ctrl.pRexFlashBrowse = new Fl_Button(430, 307, 60, 30, "Browse");
    mem_ctrl.pRexFlashBrowse->callback((Fl_Callback*)cb_rex_browse);

	if ((mem_setup.mem_mode != SETUP_MEM_REX) && (mem_setup.mem_mode != SETUP_MEM_REX2) && 
        (mem_setup.mem_mode != SETUP_MEM_REX_QUAD))
	{
		mem_ctrl.pRexFlashFile->deactivate();
		mem_ctrl.pRexFlashBrowse->deactivate();
	}

	// ===============================================
	// Setup Rex Flash File edit field and Browser button
	// ===============================================
	mem_ctrl.pRex2RamFile = new Fl_Input(105, 345, 310, 20, "RAM File");
	mem_ctrl.pRex2RamFile->value(mem_setup.rex2_ram_file);
	mem_ctrl.pRex2RamBrowse = new Fl_Button(430, 342, 60, 30, "Browse");
    mem_ctrl.pRex2RamBrowse->callback((Fl_Callback*)cb_rex2_browse);

	if (mem_setup.mem_mode != SETUP_MEM_REX2)
	{
		mem_ctrl.pRex2RamFile->deactivate();
		mem_ctrl.pRex2RamBrowse->deactivate();
	}


	// Option ROM RW Enable
	mem_ctrl.pOptRomRW = new Fl_Check_Button(20, 370, 210, 20, "Make Option ROM R/W");
	mem_ctrl.pOptRomRW->value(gOptRomRW);

	// Show Version Checkbox
	mem_ctrl.pShowVersion = new Fl_Check_Button(20, 395, 210, 20, "Patch ROM on load to show VirtualT version");
	mem_ctrl.pShowVersion->value(gShowVersion);

	// OK button
    { Fl_Button* o = new Fl_Button(140, 425, 60, 30, "Cancel");
      o->callback((Fl_Callback*)cb_memory_cancel);
    }
    { Fl_Return_Button* o = new Fl_Return_Button(220, 425, 60, 30, "OK");
      o->callback((Fl_Callback*)cb_memory_OK);
    }

	gmsw->show();
}
