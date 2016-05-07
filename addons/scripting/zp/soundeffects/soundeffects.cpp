/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          soundeffect.cpp
 *  Type:          Core 
 *  Description:   Basic sound-management API.
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
 * Number of max valid sounds blocks.
 **/
#define SoundBlocksMax 32

/**
 * Sounds config data indexes.
 **/
enum
{
    SOUNDS_DATA_LINE = 0
};
 
/**
 * The list of zombie footsteps sounds.
 **/
char FootStepSound[24][NORMAL_LINE_LENGTH] =
{
	"zpzs/player_zombie_nomal_male_footstep1.mp3",		/** Footstep Male  Sound **/
	"zpzs/player_zombie_nomal_male_footstep2.mp3",		/** Footstep Male  Sound **/
	"zpzs/player_zombie_nomal_male_footstep3.mp3",		/** Footstep Male  Sound **/
	"zpzs/player_zombie_nomal_male_footstep4.mp3",		/** Footstep Male  Sound **/
	"zpzs/player_zombie_nomal_male_footstep5.mp3",		/** Footstep Male  Sound **/
	"zpzs/player_zombie_nomal_male_footstep6.mp3",		/** Footstep Male  Sound **/
	"zpzs/player_zombie_nomal_male_footstep7.mp3",		/** Footstep Male  Sound **/
	"zpzs/player_zombie_nomal_male_footstep8.mp3",		/** Footstep Male  Sound **/
	"zpzs/player_zombie_nomal_male_footstep9.mp3",		/** Footstep Male  Sound **/
	"zpzs/player_zombie_nomal_male_footstep10.mp3",		/** Footstep Male  Sound **/
	"zpzs/player_zombie_nomal_male_footstep11.mp3",		/** Footstep Male  Sound **/
	"zpzs/player_zombie_nomal_male_footstep12.mp3",		/** Footstep Male  Sound **/
	
	"zpzs/player_zombie_normal_female_footstep1.mp3",	/** Footstep Fale  Sound **/
	"zpzs/player_zombie_normal_female_footstep2.mp3",	/** Footstep Fale  Sound **/
	"zpzs/player_zombie_normal_female_footstep3.mp3",	/** Footstep Fale  Sound **/
	"zpzs/player_zombie_normal_female_footstep4.mp3",	/** Footstep Fale  Sound **/
	"zpzs/player_zombie_normal_female_footstep5.mp3",	/** Footstep Fale  Sound **/
	"zpzs/player_zombie_normal_female_footstep6.mp3",	/** Footstep Fale  Sound **/
	"zpzs/player_zombie_normal_female_footstep7.mp3",	/** Footstep Fale  Sound **/
	"zpzs/player_zombie_normal_female_footstep8.mp3",	/** Footstep Fale  Sound **/
	"zpzs/player_zombie_normal_female_footstep9.mp3",	/** Footstep Fale  Sound **/
	"zpzs/player_zombie_normal_female_footstep10.mp3",	/** Footstep Fale  Sound **/
	"zpzs/player_zombie_normal_female_footstep11.mp3",	/** Footstep Fale  Sound **/
	"zpzs/player_zombie_normal_female_footstep12.mp3"	/** Footstep Fale  Sound **/
};

/**
 * The list of standart sounds.
 **/
char OtherSounds[9][NORMAL_LINE_LENGTH] =
{
	"zbm3/nvg_off.mp3",									/** Nightvision    Sound **/
	"zbm3/nvg_on.mp3",									/** Nightvision    Sound **/
	"zbm3/flashlight1.mp3",								/** Flashlight 	   Sound **/
	"zbm3/ammo.mp3",									/** Ammo purchase  Sound **/
	"zbm3/levelup.mp3",									/** Level up  	   Sound **/
	"zbm3/antidamage.mp3",								/** Block damage   Sound **/
	"zbm3/zombie_madness1.mp3",							/** Madness  	   Sound **/
	"zbm3/voodoo_heal.mp3",								/** Heal  	       Sound **/
	"zbm3/impalehit.mp3",								/** Freeze  	   Sound **/
};

/**
 * Array that stores a list of sounds files.
 **/
ArrayList arraySounds;

/**
 * Array for parsing strings.
 **/
int SoundBuffer[SoundBlocksMax][ParamParseResult];

/**
 * Weapon sounds.
 **/
char sMachinegunSound[BIG_LINE_LENGTH];
char sPistolSound[BIG_LINE_LENGTH];

/**
 * Prepare all sound/download data.
 **/
void SoundsLoad(/*void*/)
{
	// Register config file
	ConfigRegisterConfig(File_Sounds, Structure_ArrayList, CONFIG_FILE_ALIAS_SOUNDS);

	// Get sounds file path
	char sSoundsPath[PLATFORM_MAX_PATH];
	bool bExists = ConfigGetCvarFilePath(CVAR_CONFIG_PATH_SOUNDS, sSoundsPath);

	// If file doesn't exist, then log and stop
	if (!bExists)
	{
		// Log failure and stop plugin
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Sounds, "Config Validation", "Missing sounds file: \"%s\"", sSoundsPath);
	}

	// Set the path to the config file
	ConfigSetConfigPath(File_Sounds, sSoundsPath);

	// Load config from file and create array structure
	bool bSuccess = ConfigLoadConfig(File_Sounds, arraySounds, PLATFORM_MAX_PATH);

	// Unexpected error, stop plugin
	if (!bSuccess)
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Sounds, "Config Validation", "Unexpected error encountered loading: %s", sSoundsPath);
	}
	
	// Log what sounds file that is loaded
	LogEvent(true, LogType_Normal, LOG_CORE_EVENTS, LogModule_Sounds, "Config Validation", "Loading sounds from file \"%s\".", sSoundsPath);
	
	// Validate sound config
	int iSize = GetArraySize(arraySounds);
	if (!iSize)
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Sounds, "Config Validation", "No usable data found in sounds config file: %s", sSoundsPath);
	}
	
	// Initialize numbers of sounds
	int iSoundValidCount;
	int iSoundUnValidCount;
	
	// Initialize line char
	char sLine[PLATFORM_MAX_PATH];
	
	// i = sound array index
	for (int i = 0; i < iSize; i++)
	{
		// Get array line
		SoundsGetLine(i, sLine, sizeof(sLine));

		// Parses a parameter string in key="value" format and store the result in a ParamParseResult array
		if(ParamParseString(SoundBuffer, sLine, sizeof(sLine), i) == PARAM_ERROR_NO)
		{
			// Count number of parts inside of string
			char sSound[PARAM_VALUE_MAXPARTS][PARAM_VALUE_MAXLEN];
			int nSounds = ExplodeString(sLine, ",", sSound, sizeof(sSound), sizeof(sSound[]));
			
			// Get array size
			Handle arraySound = GetArrayCell(arraySounds, i);
			
			// Breaks a string into pieces and stores each piece into an array of buffers
			for(int x = 0; x < nSounds; x++)
			{
				// Trim string
				TrimString(sSound[x]);
				
				// Strips a quote pair off a string 
				StripQuotes(sSound[x]);

				// Push data into array
				PushArrayString(arraySound, sSound[x]);
				
				// Precache sound
				Format(sLine, sizeof(sLine), "sound/%s", sSound[x]);
				AddFileToDownloadsTable(sLine);

				// If file doesn't exist, then log, and stop
				if (!FileExists(sLine))
				{
					iSoundUnValidCount++;
					LogEvent(false, LogType_Error, LOG_CORE_EVENTS, LogModule_Sounds, "Config Validation", "Missing file \"%s\"", sLine);
					continue;
				}
				
				// Fake precaching
				Format(sLine, sizeof(sLine), "*/%s", sSound[x]);
				AddToStringTable(FindStringTable("soundprecache"), sLine);
				
				// Increment downloadvalidcount
				iSoundValidCount++;
			}
		}
		else
		{
			LogEvent(false, LogType_Error, LOG_CORE_EVENTS, LogModule_Sounds, "Config Validation", "Error with parsing of sound block: %i in sounds config file: %s", i + 1, sSoundsPath);
			
			// Remove sound block from array
			RemoveFromArray(arraySounds, i);

			// Subtract one from count
			iSize--;

			// Backtrack one index, because we deleted it out from under the loop
			i--;

			continue;
		}
	}
	
	// Log model validation info
	LogEvent(true, LogType_Normal, LOG_CORE_EVENTS, LogModule_Sounds, "Config Validation", "Total: %d | Successful: %d | Unsuccessful: %d", iSoundValidCount + iSoundUnValidCount, iSoundValidCount, iSoundUnValidCount);

	// Set config data
	ConfigSetConfigLoaded(File_Sounds, true);
	ConfigSetConfigReloadFunc(File_Sounds, GetFunctionByName(GetMyHandle(), "SoundsOnConfigReload"));
	ConfigSetConfigHandle(File_Sounds, arraySounds);
	
	// Precache other sounds
	PrecacheSoundsCacheData();
}

/**
 * Called when configs are being reloaded.
 * 
 * @param config    The config being reloaded. (only if 'all' is false)
 */
public void SoundsOnConfigReload(ConfigFile iConfig)
{
    // Reload download config
    SoundsLoad();
}

/**
 * Precache other sounds.
 **/
void PrecacheSoundsCacheData(/*void*/)
{
	// Hooks all played normal sounds
	AddNormalSoundHook(view_as<NormalSHook>(EventSound));
	
	//*********************************************************************
	//*              PRECACHE OF ZOMBIE FOOTSTEPS SOUNDS                  *
	//*********************************************************************
	
	// Initialize char
	char sPath[BIG_LINE_LENGTH];
	
	// Precache of the footsteps sounds
	for (int i = 0; i < sizeof(FootStepSound); i++)
	{
		Format(sPath, sizeof(sPath), "sound/%s", FootStepSound[i]);
		AddFileToDownloadsTable(sPath);

		Format(sPath, sizeof(sPath), "*/%s", FootStepSound[i]);
		AddToStringTable(FindStringTable("soundprecache"), sPath);
	}
	
	//*********************************************************************
	//*              PRECACHE OF OTHER SOUNDS                  			  *
	//*********************************************************************
	
	// Precache of the tools sounds
	for (int i = 0; i < sizeof(OtherSounds); i++)
	{
		Format(sPath, sizeof(sPath), "sound/%s", OtherSounds[i]);
		AddFileToDownloadsTable(sPath);

		Format(sPath, sizeof(sPath), "*/%s", OtherSounds[i]);
		AddToStringTable(FindStringTable("soundprecache"), sPath);
	}
	
	//*********************************************************************
	//*             PRECACHE OF SURVIVOR MACHINEGUN SOUNDS                *
	//*********************************************************************
	
	// Precache of the machinegun sounds
	GetConVarString(gCvarList[CVAR_SURVIVOR_M249_SHOOT], sMachinegunSound, sizeof(sMachinegunSound));
	
	Format(sPath, sizeof(sPath), "sound/%s", sMachinegunSound);
	AddFileToDownloadsTable(sPath);
	
	// Validate sound
	if(!FileExists(sPath))
	{
		LogEvent(false, LogType_Error, LOG_CORE_EVENTS, LogModule_Sounds, "Config Validation", "File not found: \"%s\".", sPath);
	}

	Format(sPath, sizeof(sPath), "*/%s", sMachinegunSound);
	AddToStringTable(FindStringTable("soundprecache"), sPath);
	
	//*********************************************************************
	//*             PRECACHE OF SURVIVOR PISTOL SOUNDS                	  *
	//*********************************************************************
	
	// Precache of the pistol sounds
	GetConVarString(gCvarList[CVAR_SURVIVOR_PISTOL_SHOOT], sPistolSound, sizeof(sPistolSound));
	
	Format(sPath, sizeof(sPath), "sound/%s", sPistolSound);
	AddFileToDownloadsTable(sPath);

	// Validate sound
	if(!FileExists(sPath))
	{
		LogEvent(false, LogType_Error, LOG_CORE_EVENTS, LogModule_Sounds, "Config Validation", "File not found: \"%s\".", sPath);
	}
	
	Format(sPath, sizeof(sPath), "*/%s", sPistolSound);
	AddToStringTable(FindStringTable("soundprecache"), sPath);
}

/**
 * Gets the line from a sound list.
 * 
 * @param iD		The sound array index.
 * @param sLine		The string to return name in.
 * @param iMaxLen	The max length of the string.
 **/
stock void SoundsGetLine(int iD, char[] sLine, int iMaxLen)
{
    // Get array handle of weapon at given index
    Handle arraySound = GetArrayCell(arraySounds, iD);
    
    // Get line
    GetArrayString(arraySound, SOUNDS_DATA_LINE, sLine, iMaxLen);
}

/**
 * Gets the random sound from a 2D array.
 * 
 * @param sLine		The string to return name in.
 * @param iMaxLen	The max length of the string.
 * @param sKey		The key to search for array ID.
 **/
stock void SoundsGetRandomSound(char[] sLine, int iMaxLen, char[] sKey)
{
	// Get array handle of weapon at given index
	Handle arraySound = GetArrayCell(arraySounds, ParamFindKey(SoundBuffer, SoundBlocksMax, sKey));

	// Get size of array handle
	int iSize = GetArraySize(arraySound);

	// Get random sound name
	GetArrayString(arraySound, Math_GetRandomInt(1, iSize - 1), sLine, iMaxLen);
}

/**
 * Gets the current sound from a 2D array.
 * 
 * @param sLine		The string to return name in.
 * @param iMaxLen	The max length of the string.
 * @param sKey		The key to search for array ID.
 * @param iNum		The number of sound in 2D array.
 **/
stock void SoundsGetSound(char[] sLine, int iMaxLen, char[] sKey, int iNum)
{
	// Get array handle of weapon at given index
	Handle arraySound = GetArrayCell(arraySounds, ParamFindKey(SoundBuffer, SoundBlocksMax, sKey));

	// Get sound name
	GetArrayString(arraySound, iNum, sLine, iMaxLen);
}

/**
 * Called when a sound is going to be emitted to one or more clients. NOTICE: all params can be overwritten to modify the default behaviour.
 *  
 * @param iClients		Array of client indexes.
 * @param numClients	Number of clients in the array (modify this value if you add/remove elements from the client array).
 * @param sSample		Sound file name relative to the "sounds" folder.
 * @param iEntity		Entity emitting the sound.
 * @param sChannel		Channel emitting the sound.
 * @param sVolume		Sound volume.
 * @param sLevel		Sound level.
 * @param sPitch		Sound pitch.
 * @param sFrags		Sound flags.
 **/ 
public Action EventSound(int iClients[64], int &numClients, char[] sSample, int &iEntity, int &sChannel, float &sVolume, int &sLevel, int &sPitch, int &sFrags)
{
	// Get real player index from event key 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(iEntity);

	// Verify that the client is connected and alive
	if(!IsPlayerExist(view_as<int>(cBasePlayer)))
	{
		return ACTION_CONTINUE;
	}

	// Verify that the client is zombie
	if(cBasePlayer->m_bZombie)
	{
		// If this footstep sounds, change its
		if(StrContains(sSample, "footsteps") != -1)
		{
			// If the silent footsteps enabled
			if(GetConVarBool(gCvarList[CVAR_ZOMBIE_SILENT]))
			{
				return ACTION_STOP;
			}
			
			// Emit footsteps sounds
			cBasePlayer->InputEmitAISound(SNDCHAN_STREAM, SNDLEVEL_LIBRARY, FootStepSound[(ZombieIsFemale(cBasePlayer->m_nZombieClass)) ? (Math_GetRandomInt(0, 11)) : (Math_GetRandomInt(12, 23))]);
			return ACTION_CHANGED; 
		}
	}
	
	// Allow sounds
	return ACTION_CONTINUE;
}

/**
 * Emits sounds to all players
 * 
 * @param sKey		The key to search for array ID.
 * @param nNum		The number of sound. (Optional) If 0 sound will be choose randomly from key.
 **/
void SoundsTransmitToAll(char[] sKey, int nNum = 0)
{
	// Initialize max number of clients
	int MaxPlayer = MaxClients;
	
	// Initialize char
	char sSound[BIG_LINE_LENGTH];
	
	// Select sound by number in array
	if(nNum)
	{
		SoundsGetSound(sSound, sizeof(sSound), sKey, nNum);
	}
	// Or select random one in array
	else
	{
		SoundsGetRandomSound(sSound, sizeof(sSound), sKey);
	}
	
	// i = client index
	for (int i = 1; i <= MaxPlayer; i++)
	{
		// Verify that the client is connected
		if(!IsPlayerExist(i, false))
			continue;

		// Play Sounds
		ClientCommand(i, "play */%s", sSound);
	}
}