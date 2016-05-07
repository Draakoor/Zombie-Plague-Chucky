/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          version.cpp
 *  Description:   Command for version information.
 *
 *  Copyright (C) 2015-2016 Nikita Ushakov (Ireland, Dublin)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ============================================================================
 **/
 
/**
 * @section	Modification information.
 **/
#define PLUGIN_NAME    				"Zombie Plague"
#define PLUGIN_VERSION 				"5.1"
#define PLUGIN_AUTHOR				"qubka (Nikita Ushakov), Greyscale, Richard Helgeby"
#define PLUGIN_COPYRIGHT        	"Copyright (C) 2015-2016 Nikita Ushakov (Ireland, Dublin)"
#define PLUGIN_BRANCH           	"zp-5.1"
#define PLUGIN_LINK        			"https://forums.alliedmods.net/showthread.php?t=272546"
#define PLUGIN_LICENSE          	"GNU GPL, Version 3"
#define PLUGIN_DATE             	"05 May 2016 00:12:34 +0200"
/**
 * @endsection
 **/

/**
 * Creates commands for plugin's version. Called when commands are created.
 **/
void VersionOnCommandsCreate(/*void*/)
{
    RegConsoleCmd("zp_version", CommandVersion, "Prints version info about this plugin.");
}

/**
 * Sent version information into the server's console.
 **/
void VersionPrint(/*void*/)
{
	PrintToServer("%s [%s]\n%s\n%s\n%s\n%s\n", PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR, PLUGIN_COPYRIGHT, PLUGIN_LICENSE, PLUGIN_DATE);
}

/**
 * Called when a generic console command is invoked.
 * 
 * @param clientIndex	The client index.
 * @param ArgCommands	Number of arguments that were in the argument string.
 **/ 
public Action CommandVersion(int clientIndex, int iArguments)
{
	// Initialize chars
    char sBuffer[512];
    char sLine[128];
    sBuffer[0] = 0;
    sLine[0] = 0;
    
    #define FORMATSTRING "%24s: %s\n"
    
	// Format strings
    Format(sLine, sizeof(sLine), "%s\n", PLUGIN_NAME);
    StrCat(sBuffer, sizeof(sBuffer), sLine);
    
    Format(sLine, sizeof(sLine), "%s\n\n", PLUGIN_COPYRIGHT);
    StrCat(sBuffer, sizeof(sBuffer), sLine);
    
    Format(sLine, sizeof(sLine), FORMATSTRING, "Version", PLUGIN_VERSION);
    StrCat(sBuffer, sizeof(sBuffer), sLine);
    
    Format(sLine, sizeof(sLine), FORMATSTRING, "Last edit", PLUGIN_DATE);
    StrCat(sBuffer, sizeof(sBuffer), sLine);
    
    Format(sLine, sizeof(sLine), FORMATSTRING, "License", PLUGIN_LICENSE);
    StrCat(sBuffer, sizeof(sBuffer), sLine);
    
    Format(sLine, sizeof(sLine), FORMATSTRING, "Link+", PLUGIN_LINK);
    StrCat(sBuffer, sizeof(sBuffer), sLine);
    
    Format(sLine, sizeof(sLine), FORMATSTRING, "Branch", PLUGIN_BRANCH);
    StrCat(sBuffer, sizeof(sBuffer), sLine);
    
	// Send information into the client's console
    ReplyToCommand(clientIndex, sBuffer);
    return Plugin_Handled;
}
