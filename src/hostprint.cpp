/* hostprint.cpp */

/* $Id: hostprint.cpp,v 1.10 2013/01/23 01:00:08 kpettit1 Exp $ */

/*
 * Copyright 2008 Ken Pettit
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
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Preferences.H>
#include <FL/fl_ask.H>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#ifdef __unix__
#include <unistd.h>
#endif

#include "VirtualT.h"
#include "hostprint.h"

/*
================================================================================
VTHostPrint:	This is the class construcor for the HostPrint Device emulation.
================================================================================
*/
VTHostPrint::VTHostPrint(void)
{
	#ifdef WIN32
		m_OutFd = NULL;
	#else
		m_OutFd = -1;
		m_PrevChar = 0;
	#endif
}

/*
=======================================================
GetName:	Returns the name of the printer.
=======================================================
*/
MString VTHostPrint::GetName()
{
	return "Host Device";
}

/*
=======================================================
Print a byte to the Host device.
=======================================================
*/
void VTHostPrint::PrintByte(unsigned char byte)
{
	#ifdef WIN32
		unsigned long dwWritten;

		if (m_OutFd == NULL)
			return;
		WriteFile(m_OutFd, &byte, 1, &dwWritten, NULL);
	#else
		// Write byte to the host
		if (m_OutFd != -1)
		{
			int size = write(m_OutFd, &byte, 1); 
			(void) size;
		}
	#endif

	// Save byte so we know if we should do an AFF
	m_PrevChar = byte;

}

/*
=======================================================
Print a byte to the Host device.
=======================================================
*/
void VTHostPrint::BuildPropertyDialog(Fl_Window* pParent)
{
	// Create controls for Host Port emulation mode
	Fl_Box* o = new Fl_Box(20, 20, 360, 20, "Host Port / Device Printer");

	// Host port name control
	o = new Fl_Box(20, 50, 300, 20, "Host Port Name");
	o->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
	m_pHostPort = new Fl_Input(50, 75, 300, 20, "");
	m_pHostPort->value(m_HostPort);

	m_pClosePort = new Fl_Check_Button(20, 115, 215, 20, "Close port between sessions");
	m_pClosePort->value(m_ClosePort);

	m_pReadBusy = new Fl_Check_Button(20, 140, 205, 20, "Read BUSY status from port");
	m_pReadBusy->value(m_ReadBusy);
}

/*
=======================================================
Print a byte to the Host device.
=======================================================
*/
int VTHostPrint::GetBusyStatus(void)
{
	return FALSE;
}

/*
=======================================================
Init routine for reading prefs, setting up, etc.
=======================================================
*/
void VTHostPrint::Init(void)
{
	m_pPref->get("HostPrint_HostPort", m_HostPort, "", 256);
	m_pPref->get("HostPrint_ClosePort", m_ClosePort, 1);
	m_pPref->get("HostPrint_ReadBusy", m_ReadBusy, 0);
	return;
}

/*
=======================================================
Opens a new Print Session
=======================================================
*/
int VTHostPrint::OpenSession(void)
{
	char		str[512];

	// Validate the host port name exists
	if (strlen(m_HostPort) == 0)
		return PRINT_ERROR_IO_ERROR;

	// Attempt to open the Host Port
	#ifdef WIN32

		// Attempt to open the port specified by sp.port_name
		m_OutFd = CreateFile( m_HostPort,  
				GENERIC_READ | GENERIC_WRITE, 
				0, 
				0, 
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				0);

		// Check for error opening port
		if (m_OutFd == INVALID_HANDLE_VALUE)
		{
			sprintf(str, "Error opening port %s", m_HostPort);
			AddError(str);
			return PRINT_ERROR_IO_ERROR;
		}
		
	#else
		// Open file in Read/Write mode
		m_OutFd = open(m_HostPort, O_RDWR);
		if (m_OutFd == -1)
		{
			sprintf(str, "Error - unable to open %s", m_HostPort);
			AddError(str);
			return PRINT_ERROR_IO_ERROR;
		}
	#endif

	return PRINT_ERROR_NONE;
}

/*
=======================================================
Closes the active Print Session
=======================================================
*/
int VTHostPrint::CloseSession(void)
{
	// Clear session variables
	m_PrevChar = 0;

	// Test if the user want's to close the port between sessions
	if (m_ClosePort)
	{
		#ifdef WIN32
			if (m_OutFd != NULL)
				CloseHandle(m_OutFd);
			m_OutFd = NULL;

		#else
			// Close the file descriptor
			if (m_OutFd != -1)
			{
				close(m_OutFd);
				m_OutFd = -1;
			}
		#endif
	}

	return PRINT_ERROR_NONE;
}

/*
=======================================================
Get the Printer Properties from dialog and save.
=======================================================
*/
int VTHostPrint::GetProperties(void)
{
	// Get values from dialog
	strcpy(m_HostPort, m_pHostPort->value());
	m_ClosePort = m_pClosePort->value();
	m_ReadBusy = m_pReadBusy->value();

	// Save values to preferences
	m_pPref->set("HostPrint_HostPort", m_HostPort);
	m_pPref->set("HostPrint_ClosePort", m_ClosePort);
	m_pPref->set("HostPrint_ReadBusy", m_ReadBusy);

	return PRINT_ERROR_NONE;
}

/*
=======================================================
Send a FF if last byte sent not 0x0C
=======================================================
*/
void VTHostPrint::SendAutoFF(void)
{
	if (m_PrevChar != 0x0C)
		PrintByte(0x0C);

	return;
}

/*
=======================================================
Deinitialize the printer
=======================================================
*/
void VTHostPrint::Deinit(void)
{
	m_initialized = FALSE;

	#ifdef WIN32
		if (m_OutFd != NULL)
			CloseHandle(m_OutFd);
		m_OutFd = NULL;
	#else
		// Close the host device if open
		if (m_OutFd != -1)
		{
			close(m_OutFd);
			m_OutFd = -1;
		}
	#endif
}

/*
=======================================================
Cancel the current print job
=======================================================
*/
int VTHostPrint::CancelPrintJob(void)
{
	CloseSession();

	return PRINT_ERROR_NONE;
}

/*
=======================================================
Builds the monitor tab
=======================================================
*/
void VTHostPrint::BuildMonTab(void)
{
	Fl_Box*		o;

	o = new Fl_Box(20, 45+MENU_HEIGHT, 100, 20, "Host Port:");
	o->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
	o = new Fl_Box(20, 70+MENU_HEIGHT, 100, 20, "Port Status:");
	o->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

	m_pStatHostPort = new Fl_Box(150, 45+MENU_HEIGHT, 300, 20, "");
	m_pStatHostPort->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
	m_pStatPortStatus = new Fl_Box(150, 70+MENU_HEIGHT, 300, 20, "");
	m_pStatPortStatus->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

	UpdateMonTab();
}

/*
=======================================================
Updates the monitor tab
=======================================================
*/
void VTHostPrint::UpdateMonTab(int forceUpdate)
{
	char		temp[20];

	// Update host port
	if ((strcmp(m_HostPort, m_sStatHostPort) != 0) || forceUpdate)
	{
		strcpy(m_sStatHostPort, m_HostPort);
		m_pStatHostPort->label(m_sStatHostPort);
	}

#ifdef WIN32
	if (m_OutFd == NULL)
		strcpy(temp, "Closed");
	else
		strcpy(temp, "Open");
#else
	// Update port status
	if (m_OutFd == -1)
		strcpy(temp, "Closed");
	else
		strcpy(temp, "Open");

#endif

	if ((strcmp(temp, m_sStatPortStatus) != 0) || forceUpdate)
	{
		strcpy(m_sStatPortStatus, temp);
		m_pStatPortStatus->label(m_sStatPortStatus);
	}
}

