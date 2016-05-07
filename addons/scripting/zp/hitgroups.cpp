/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          hitgroup.inc
 *  Type:          Core
 *  Description:   API for loading hitgroup specific settings.
 *
 *  Copyright (C) 2015-2016  Greyscale, Richard Helgeby
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
 * Maximum length for a hitgroup name
 **/
#define HITGROUPS_MAX_LENGTH 32

/**
 * @section Player hitgroup values.
 **/
#define HITGROUP_GENERIC    0
#define HITGROUP_HEAD       1
#define HITGROUP_CHEST      2
#define HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4
#define HITGROUP_RIGHTARM   5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG   7
#define HITGROUP_GEAR       10
/**
 * @endsection
 **/

/**
 * Hitgroup config data indexes.
 **/
enum
{
    HITGROUPS_DATA_NAME = 0,
    HITGROUPS_DATA_INDEX,
    HITGROUPS_DATA_DAMAGE,
    HITGROUPS_DATA_KNOCKBACK,
}

/**
 * Array handle to store hitgroups data.
 **/
ArrayList arrayHitgroups;

/**
 * Loads hitgroup data from file.
 **/ 
void HitgroupsLoad(/*void*/)
{
	// Register config file
	ConfigRegisterConfig(File_Hitgroups, Structure_Keyvalue, CONFIG_FILE_ALIAS_HITGROUPS);

	// If module is disabled, then stop
	if (!GetConVarBool(gCvarList[CVAR_HITGROUPS]))
	{
		return;
	}

	// Get hitgroups config path
	char sHitGroupPath[PLATFORM_MAX_PATH];
	bool bExists = ConfigGetCvarFilePath(CVAR_CONFIG_PATH_HITGROUPS, sHitGroupPath);

	// If file doesn't exist, then log and stop
	if (!bExists)
	{
		// Log failure
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Hitgroups, "Config Validation", "Missing hitgroups config file: %s", sHitGroupPath);

		return;
	}

	// Set the path to the config file
	ConfigSetConfigPath(File_Hitgroups, sHitGroupPath);

	// Load config from file and create array structure
	bool bSuccess = ConfigLoadConfig(File_Hitgroups, arrayHitgroups);

	// Unexpected error, stop plugin
	if (!bSuccess)
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Hitgroups, "Config Validation", "Unexpected error encountered loading: %s", sHitGroupPath);

		return;
	}

	// Validate hitgroups config
	int iSize = GetArraySize(arrayHitgroups);
	if (!iSize)
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Hitgroups, "Config Validation", "No usable data found in hitgroups config file: %s", sHitGroupPath);
	}

	// Now copy data to array structure
	HitgroupsCacheData();

	// Set config data
	ConfigSetConfigLoaded(File_Hitgroups, true);
	ConfigSetConfigReloadFunc(File_Hitgroups, GetFunctionByName(GetMyHandle(), "HitgroupsOnConfigReload"));
	ConfigSetConfigHandle(File_Hitgroups, arrayHitgroups);
}

/**
 * Caches hitgroup data from file into arrays.
 * Make sure the file is loaded before (ConfigLoadConfig) to prep array structure.
 **/
void HitgroupsCacheData(/*void*/)
{
    // Get config's file path
    char sHitGroupsPath[PLATFORM_MAX_PATH];
    ConfigGetConfigPath(File_Hitgroups, sHitGroupsPath, sizeof(sHitGroupsPath));
    
    Handle kvHitgroups;
    bool bSuccess = ConfigOpenConfigFile(File_Hitgroups, kvHitgroups);
    
    if (!bSuccess)
    {
     LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Hitgroups, "Config Validation", "Unexpected error caching data from hitgroups config file: %s", sHitGroupsPath);
    }
    
    char sHitGroupName[HITGROUPS_MAX_LENGTH];
    
    // i = array index
    int iSize = GetArraySize(arrayHitgroups);
    for (int i = 0; i < iSize; i++)
    {
        HitgroupsGetName(i, sHitGroupName, sizeof(sHitGroupName));
        
        KvRewind(kvHitgroups);
        if (!KvJumpToKey(kvHitgroups, sHitGroupName))
        {
            LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Hitgroups, "Config Validation", "Couldn't cache hitgroup data for: %s (check hitgroup config)", sHitGroupName);
            continue;
        }
        
        // General
        int iIndex = KvGetNum(kvHitgroups, "index", -1);
        
        // Damage
        bool bDamage = ConfigKvGetStringBool(kvHitgroups, "damage", "yes");
        
        // Knockback (module)
        float flKnockback = KvGetFloat(kvHitgroups, "knockback", 1.0);
        
        Handle arrayHitgroup = GetArrayCell(arrayHitgroups, i);
        
        // Push data into array
        PushArrayCell(arrayHitgroup, iIndex);        // Index: 1
        PushArrayCell(arrayHitgroup, bDamage);       // Index: 2
        PushArrayCell(arrayHitgroup, flKnockback);   // Index: 3
    }
    
    // We're done with this file now, so we can close it
    CloseHandle(kvHitgroups);
}

/**
 * Called when configs are being reloaded.
 * 
 * @param config    The config being reloaded. (only if 'all' is false)
 **/
public void HitgroupsOnConfigReload(ConfigFile iConfig)
{
    // Reload hitgroups config
    HitgroupsLoad();
}

/**
 * Find the index at which the hitgroup's name is at.
 * 
 * @param sHitGroup			The higroup name.
 * @param iMaxLen			(Only if 'overwritename' is true) The max length of the hitgroup name. 
 * @param bOverWriteName	(Optional) If true, the hitgroup given will be overwritten with the name from the config.
 * @return          	  	The array index containing the given hitgroup name.
 **/
stock int HitgroupsNameToIndex(char[] sHitGroup, int iMaxLen = 0, bool bOverWriteName = false)
{
    char sHitGroupName[HITGROUPS_MAX_LENGTH];
    
    // x = Array index
    int iSize = GetArraySize(arrayHitgroups);
    for (int i = 0; i < iSize; i++)
    {
        HitgroupsGetName(i, sHitGroupName, sizeof(sHitGroupName));
        
        // If names match, then return index
        if (StrEqual(sHitGroup, sHitGroupName, false))
        {
            // If 'overwrite' name is true, then overwrite the old string with new
            if (bOverWriteName)
            {
                // Copy config name to return string
                strcopy(sHitGroup, iMaxLen, sHitGroupName);
            }
            
            // Return this index
            return i;
        }
    }
    
    // Name doesn't exist
    return -1;
}

/**
 * Find the array index at which the hitgroup index is at.
 * 
 * @param hitgroup	The hitgroup index to search for.
 * @return          The array index that contains the given hitgroup index.
 **/
stock int HitgroupToIndex(int iHitGroup)
{
    // i = Array index
    int iSize = GetArraySize(arrayHitgroups);
    for (int i = 0; i < iSize; i++)
    {
        // Get hitgroup index at this array index
        int iIndex = HitgroupsGetIndex(i);
        
        // If hitgroup indexes match, then return array index
        if (iHitGroup == iIndex)
        {
            return i;
        }
    }
    
    // Hitgroup index doesn't exist
    return -1;
}

/**
 * Gets the name of a hitgroup at a given index. (static)
 *
 * @param iD    	 The hitgroup index.
 * @param sHitGroup	 The string to return name in.
 * @param iMaxLen    The max length of the string.
 **/
stock void HitgroupsGetName(int iD, char[] sHitGroup, int iMaxLen)
{
    // Get array handle of hitgroup at given index
    Handle arrayHitgroup = GetArrayCell(arrayHitgroups, iD);
    
    // Get hitgroup name
    GetArrayString(arrayHitgroup, HITGROUPS_DATA_NAME, sHitGroup, iMaxLen);
}

/**
 * Retrieve hitgroup index. (static)
 * 
 * @param iD	The array index.
 * @return		The hitgroup index.
 **/
stock int HitgroupsGetIndex(int iD)
{
    // Get array handle of hitgroup at given index
    Handle arrayHitgroup = GetArrayCell(arrayHitgroups, iD);
    
    // Return hitgroup index of the hitgroup
    return GetArrayCell(arrayHitgroup, HITGROUPS_DATA_INDEX);
}

/**
 * Set hitgroup damage value. (dynamic)
 * 
 * @param iD     	 The array index.
 * @param bCanDamage True to allow damage to hitgroup, false to block damage.
 **/
stock void HitgroupsSetDamage(int iD, bool bCanDamage)
{
    // Get array handle of hitgroup at given index
    Handle arrayHitgroup = GetArrayCell(arrayHitgroups, iD);
    
    // Return true if hitgroup can be damaged, false if not
    SetArrayCell(arrayHitgroup, HITGROUPS_DATA_DAMAGE, bCanDamage);
}

/**
 * Retrieve hitgroup damage value. (dynamic)
 * 
 * @param iD	The array index.
 * @return		True if hitgroup can be damaged, false if not.
 **/
stock bool HitgroupsCanDamage(int iD)
{
    // Get array handle of hitgroup at given index
    Handle arrayHitgroup = GetArrayCell(arrayHitgroups, iD);
    
    // Return true if hitgroup can be damaged, false if not
    return GetArrayCell(arrayHitgroup, HITGROUPS_DATA_DAMAGE);
}

/**
 * Set hitgroup knockback value. (dynamic)
 * 
 * @param iD     	  The array index.
 * @param flKnockback The knockback multiplier for the hitgroup.
 **/
stock void HitgroupsSetKnockback(int iD, float flKnockback)
{
    // Get array handle of hitgroup at given index
    Handle arrayHitgroup = GetArrayCell(arrayHitgroups, iD);
    
    // Return the knockback multiplier for the hitgroup
    SetArrayCell(arrayHitgroup, HITGROUPS_DATA_KNOCKBACK, flKnockback);
}

/**
 * Retrieve hitgroup knockback value. (dynamic)
 * 
 * @param iD	The array index.
 * @return		The knockback multiplier of the hitgroup.
 **/
stock float HitgroupsGetKnockback(int iD)
{
    // Get array handle of hitgroup at given index
    Handle arrayHitgroup = GetArrayCell(arrayHitgroups, iD);
    
    // Return the knockback multiplier for the hitgroup
    return GetArrayCell(arrayHitgroup, HITGROUPS_DATA_KNOCKBACK);
}