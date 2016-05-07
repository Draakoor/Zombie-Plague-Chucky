/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          zombieplague.cpp
 *  Type:          Main 
 *  Description:   General plugin functions and defines.
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
 * @section All Engine versions.
 **/
#define ENGINE_UNKNOWN              "could not determine the engine version"    
#define ENGINE_ORIGINAL             "Original Source Engine"         
#define ENGINE_SOURCESDK2006        "Episode 1 Source Engine"         
#define ENGINE_SOURCESDK2007        "Orange Box Source Engine"        
#define ENGINE_LEFT4DEAD            "Left 4 Dead"   
#define ENGINE_DARKMESSIAH          "Dark Messiah Multiplayer"
#define ENGINE_LEFT4DEAD2           "Left 4 Dead 2"
#define ENGINE_ALIENSWARM           "Alien Swarm"
#define ENGINE_BLOODYGOODTIME       "Bloody Good Time"
#define ENGINE_EYE                  "E.Y.E Divine Cybermancy"
#define ENGINE_PORTAL2              "Portal 2"
#define ENGINE_CSGO                 "Counter-Strike: Global Offensive"
#define ENGINE_CSS                  "Counter-Strike: Source"
#define ENGINE_DOTA                 "Dota 2"
#define ENGINE_HL2DM                "Half-Life 2 Deathmatch"
#define ENGINE_DODS                 "Day of Defeat: Source"
#define ENGINE_TF2                  "Team Fortress 2"
#define ENGINE_NUCLEARDAWN          "Nuclear Dawn"
#define ENGINE_SDK2013              "Source SDK 2013"
#define ENGINE_BLADE                "Blade Symphony"
#define ENGINE_INSURGENCY			"Insurgency"
#define ENGINE_CONTAGION            "Contagion"
/**
 * @endsection
 **/
 
/**
 * Called once when server is started. Will log a warning if a unsupported game is detected.
 **/
void UpdateGameFolder(/*void*/)
{
	// Get engine of the game
	switch(GetEngineVersion(/*void*/))
	{
		case Engine_Unknown :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague %s", ENGINE_UNKNOWN);
		}
		case Engine_Original :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_ORIGINAL);
		}
		case Engine_SourceSDK2006 :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_SOURCESDK2006);
		}
		case Engine_SourceSDK2007 :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_SOURCESDK2007);
		}
		case Engine_Left4Dead :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_LEFT4DEAD);
		}
		case Engine_DarkMessiah :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_DARKMESSIAH);
		}
		case Engine_Left4Dead2 :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_LEFT4DEAD2);
		}
		case Engine_AlienSwarm :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_ALIENSWARM);
		}
		case Engine_BloodyGoodTime :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_BLOODYGOODTIME);
		}
		case Engine_EYE :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_EYE);
		}
		case Engine_Portal2 :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_PORTAL2);
		}
		case Engine_CSGO :	
		{
			LogEvent(true, LogType_Normal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Engine catched: %s", ENGINE_CSGO);
		}
		case Engine_CSS :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_CSS);
		}
		case Engine_DOTA :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_DOTA);
		}
		case Engine_HL2DM :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_HL2DM);
		}
		case Engine_DODS :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_DODS);
		}
		case Engine_TF2 :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_TF2);
		}
		case Engine_NuclearDawn :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_NUCLEARDAWN);
		}
		case Engine_SDK2013 :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_SDK2013);
		}
		case Engine_Blade :	
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_BLADE);
		}
		case Engine_Insurgency :
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_INSURGENCY);
		}
		case Engine_Contagion :
		{
			LogEvent(true, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Engine, "Engine Validation", "Zombie Plague doesn't support %s", ENGINE_CONTAGION);
		}
	}
}

/**
 * Create an array populated with eligible clients.
 * 
 * @param arrayEligibleClients  The handle of the array, don't forget to call CloseHandle on it when finished!
 **/  
stock int CreateEligibleClientList(ArrayList &arrayEligibleClients)
{
    // Create array
    arrayEligibleClients = CreateArray();
    
    // Populate list with eligible clients
    // i = client index
    for (int i = 1; i <= MaxClients; i++)
    {
        // If client isn't in-game, then stop
        if (!IsPlayerExist(i, false))
        {
            continue;
        }
        
        // If client isn't on a team, then stop
        if (!fnGetTeam(i))
        {
            continue;
        }
        
        // Add eligible client to array
        PushArrayCell(arrayEligibleClients, i);
    }
    
    return GetArraySize(arrayEligibleClients);
}

/**
 * Checks if a timer is currently running.
 * 
 * @param timer     The timer handle.
 **/
stock bool IsTimerRunning(Handle hTimer)
{
    // Return true if the handle isn't empty
    return (hTimer != NULL);
}

/**
 * Wrapper functions for KilLTimer.
 * Ends a timer if running, and resets its timer handle variable.
 * 
 * @param timer     The timer handle.
 *
 **/
stock void EndTimer(Handle &hTimer)
{
    // If the timer is running, then kill it
    if (IsTimerRunning(hTimer))
    {
        // Kill
        KillTimer(hTimer);
        
        // Reset variable
        hTimer = NULL;
    }
}

/**
 * Check if a client index is on a team.
 * 
 * @param clientIndex	 The client index.
 * @return          	 True if client is on a team, false otherwise. 
 **/    
stock bool fnGetTeam(int clientIndex)
{
    // Get client team
    int iTeam = GetClientTeam(clientIndex);
    
	// Return if client is on a team
    return (iTeam == ZP_TEAM_ZOMBIE || iTeam == ZP_TEAM_HUMAN);
}

/**
 * Returns true if the player is connected and alive, false if not.
 *
 * @param clientIndex	The client index.
 * @param aliveness		(Optional) Set to true to validate that the client is alive, false to ignore.
 * @return				True or false.
 **/
stock bool IsPlayerExist(int clientIndex, bool aliveness = true)
{
	// If client isn't valid
	if (clientIndex <= 0 || clientIndex > MaxClients)
	{
		return false;
	}
	
	// If client isn't connected
	if (!IsClientConnected(clientIndex))
	{
		return false;
	}

	// If client isn't in game
	if (!IsClientInGame(clientIndex))
	{
		return false;
	}

	// If client isn't alive
	if(aliveness && !IsPlayerAlive(clientIndex))
	{
		return false;
	}
	
	// If client exist
	return true;
}

/**
 * Get amount of alive humans.
 * 
 * @return	 Amount of humans.
 **/
stock int fnGetHumans(/*void*/)
{
	// Initialize max amount of humans
	int iHumans;
	
	// i = client index
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsPlayerExist(i) && !gZombie[i])
		{
			iHumans++;
		}
	}
	
	return iHumans;
}

/**
 * Get amount of alive zombies.
 *
 * @return	 Amount of zombies.
 **/
stock int fnGetZombies(/*void*/)
{
	// Initialize max amount of zombies
	int iZombies;
	
	// i = client index
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsPlayerExist(i) && gZombie[i])
		{
			iZombies++;
		}
	}
	
	return iZombies;
}

/**
 * Get amount of alive players.
 *
 * @return	 Amount of alive players.
 **/
stock int fnGetAlive(/*void*/)
{
	// Initialize max amount of alive players
	int iAlive;

	// i = client index
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsPlayerExist(i))
		{
			iAlive++;
		}
	}
	
	return iAlive;
}

/**
 * Get random alive player index.
 *
 * @return	 The index of random player.
 **/
stock int fnGetRandomAlive(int iCount)
{
	// Initialize max amount of alive players
	int iAlive;
	
	// i = client index
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsPlayerExist(i))
		{
			iAlive++;
		}
		
		if (iAlive == iCount)
		{
			return i;
		}
	}
	
	return -1;
}

/**
 * Get amount of players.
 **/
stock int fnGetPlaying(/*void*/)
{
	// Initialize max amount of players
	int iPlaying;

	// i = client index
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsPlayerExist(i, false))
		{
			if (fnGetTeam(i))
			{
				iPlaying++;
			}
		}
	}
	
	return iPlaying;
}

/**
 * Check some important holiday days.
 **/
stock int GetCurrentHoliday(/*void*/)
{
	// Initialize time char
	char sTime[BIG_LINE_LENGTH];
	
	// Get month and store it
	FormatTime(sTime, sizeof(sTime), "%m", GetTime());
	int iMonth = StringToInt(sTime);
	
	// Get day and store it
	FormatTime(sTime, sizeof(sTime), "%d", GetTime());
	int iDay = StringToInt(sTime);
	
	// Check month
	switch(iMonth)
	{
		case 1 :
		{
			/// X-Mas
			switch (iDay)
			{
				case 1,2,3,4,5,6,7,8,9,10,11,12,13,14 : 				  return 1; 
			}
		}
		case 10 :
		{
			/// Halloween
			switch (iDay)
			{
				case 29, 30,31 : 										  return 1;
			}
		}
		case 12 :
		{
			/// X-Mas
			switch (iDay)
			{
				case 15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 : return 1;
			}
		}
	}

	return 0;
}

/**
 * Return itemdraw flag for radio menus.
 * 
 * @param bCondition     If this is true, item will be drawn normally.
 **/
stock int MenuGetItemDraw(bool bCondition)
{
    return bCondition ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED;
}

/**
 * Converts a boolean value into a string.
 * 
 * @param bValue     The value to convert to string.
 * @param sOutput    The converted string.
 * @param maxlen     The maximum length of the string.
 **/
stock void BoolToString(bool bValue, char[] sOutput, int iMaxLen)
{
    // If the value is true, then set string to "1".
    if (bValue)
    {
        strcopy(sOutput, iMaxLen, "1");
    }
    // If the value is false, then set string to "0".
    else
    {
        strcopy(sOutput, iMaxLen, "0");
    }
}

/**
 * Adds support for printing strings longer than 1 KB to console. Max 4 KB.
 *
 * Note: 1024 characters is max for the console, including newline and null
 *       terminator.
 *
 * @param clientIndex    The client index.
 * @param sText          Long text to write.
 * @param iSplitSize     Optional. Sets the split size. 1022 is default.
 *                       Allowed range: 128 to 1022.
 **/
stock void PrintToConsoleLong(int clientIndex, const char[] sText, int iSplitSize = 1022)
{
    // Validate split size
    if (iSplitSize < 128 || iSplitSize > 1022)
    {
        return;
    }
    
    char sPartbuffer[iSplitSize];
    int  iPos;
    int  iCellsWritten = 1;           // Initialize for the loop

    while (iCellsWritten)
    {
        iCellsWritten = strcopy(sPartbuffer, iSplitSize, sText[iPos]);
        (clientIndex > 0) ? PrintToConsole(clientIndex, sPartbuffer) : PrintToServer(sPartbuffer);
        iPos += iCellsWritten;
    }
}

/**
 * (from SMLIB 0.10.2)
 * 
 * Returns a random, uniform Integer number in the specified (inclusive) range.
 * This is safe to use multiple times in a function.
 * The seed is set automatically for each plugin.
 * Rewritten by MatthiasVance, thanks.
 * 
 * @param min			Min value used as lower border
 * @param max			Max value used as upper border
 * @return				Random Integer number between min and max
 **/
#define MAX_INT 		2147483647		// without 0
stock int Math_GetRandomInt(int min, int max)
{
	int random = GetURandomInt();
	
	if (random == 0) {
		random++;
	}

	return RoundToCeil(float(random) / (float(MAX_INT) / float(max - min + 1))) + min - 1;
}