/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          config.inc
 *  Type:          Core
 *  Description:   Config API and executing.
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

/*
    Using config API:
    
    -Before any of these helper functions can be used on a config file you must
     "register" the module handling the data.
    
    Example:
    
    ConfigRegisterConfig(File_Example, Structure_List, "example");
    
    * The first parameter of this call is the config file we want to register.
      this needs to be listed in the "ConfigFile" enum in config.inc.
      
    * The second parameter is the structure of the config file we are loading.
      The supported structures are listed in the "ConfigStructure" enum in config.inc
      
    * The last parameter is the file's alias.  Or what we use to refer to the
      config file from a non-developer's point of view.  For example zp_config_reload
      requires the file alias to identify the config file the user wants to reload.
    
    -Next we need to define the config file's path.  To do this we first need to
     retrieve the path file from cvar.
     
    Example:
    
    ConfigSetConfigPath(File_Example, pathexample);
    
    * The first parameter is the config file we want to set path to.
      
    * The second parameter is the path we want to set to the config file.
    
    -Next we load config file and prepare its nested array structure.
    
    Example:
    
    new bool:success = ConfigLoadConfig(File_Example, arrayExample);
    
    * The first parameter is the config file we want to load.
      
    * The second parameter is the array handle we want to prepare data structure in.
      
    * The return value is true if the file was successfully loaded, false if the
      config file couldn't be loaded.  (Invalid data, missing quotes, brackets, etc)
    
    -Next validate the config so far, stopping if no data was found or if ConfigLoadConfig
     returned false.
    
    -Then cache the config file data into the arrays (only for Keyvalue structures)
     by iterating through the data and pushing the values into the array.
    
    -Validate the values of the data.
    
    -Lastly we need to set specific info to the module now that it has successfully
     loaded.
    
    Example:
    
    ConfigSetConfigLoaded(File_Example, true);
    ConfigSetConfigReloadFunc(File_Example, GetFunctionByName(GetMyHandle(), "ExampleOnConfigReload"));
    ConfigSetConfigHandle(File_Example, arrayExample);
    
    These functions will modify the config file data for other things to use.
    (such as zp_config_reload)
    
    * The first function call will set the loaded state of the config file to
      true, failing to do this will cause the config module to think your
      config file isn't loaded, therefore causing some undesired erroring.
      
    * The second function sets the reload function of the config file.  This
      function will be called upon its config file being reloaded.
      
    * The third function stores the array handle for use by other parts of the
      module.
*/

/**
 * The max length of any config string value.
 **/
#define CONFIG_MAX_LENGTH 32

/**
 * @section Config file reference aliases.
 **/
#define CONFIG_FILE_ALIAS_MODELS        "models"
#define CONFIG_FILE_ALIAS_DOWNLOADS     "downloads"
#define CONFIG_FILE_ALIAS_WEAPONS       "weapons"
#define CONFIG_FILE_ALIAS_HITGROUPS     "hitgroups"
#define CONFIG_FILE_ALIAS_SOUNDS     	"sounds"
/**
 * @endsection
 **/

/**
 * List of config formats used by the plugin.
 **/
enum ConfigStructure
{
	Structure_List,     /** Config is structured as a simple list of strings. */
	Structure_ArrayList,/** Config is structured as an array list of strings. */
	Structure_Keyvalue, /** Config is a keyvalue structure */
}

/**
 * List of config files used by the plugin.
 **/
enum ConfigFile
{
    File_Invalid = -1,  /** Invalid config file. */
    File_Models,        /** <sourcemod root>/zombieplague/models.ini (default) */
    File_Downloads,     /** <sourcemod root>/zombieplague/downloads.ini (default) */
    File_Weapons,       /** <sourcemod root>/zombieplague/weapons.ini (default) */
	File_Hitgroups,		/** <sourcemod root>/zombieplague/hitgroups.ini (default) */
	File_Sounds			/** <sourcemod root>/zombieplague/sounds.ini (default) */
}

/**
 * Data container for each config file.
 **/
enum ConfigData
{
    bool:Data_Loaded,                    	/** True if config is loaded, false if not. */
    ConfigStructure:Data_Structure,       	/** Format of the config */
    Function:Data_ReloadFunc,               /** Function to call to reload config. */
    Handle:Data_Handle,                     /** Handle of the config file. */
    String:Data_Path[PLATFORM_MAX_PATH],    /** Full path to config file. */
    String:Data_Alias[CONFIG_MAX_LENGTH],   /** Config file alias, used for client interaction. */
}

/**
 * Stores all config data.
 **/
int gConfigData[ConfigFile][ConfigData];

/**
 * Actions to use when working on key/values.
 **/
enum ConfigKvAction
{
    KvAction_Create,    /** Create a key. */
    KvAction_KVDelete,  /** Delete a key. */
    KvAction_KVSet,     /** Modify setting of a key. */
    KvAction_KVGet,     /** Get setting of a key. */
}

/**
 * Create commands related to config here.
 **/
void ConfigOnCommandsCreate(/*void*/)
{
    // Create config admin commands
    RegConsoleCmd("zp_config_reload", ConfigReloadCommand, "Reloads a config file. Usage: zp_config_reload <file alias>");
    RegConsoleCmd("zp_config_reloadall", ConfigReloadAllCommand,"Reloads all config files. Usage: zp_config_reloadall");
}

/**
 * Load plugin configs. Executes map configs.
 **/
void ConfigLoad(/*void*/)
{
	// Reset count of rounds
	gRoundNum = 0;
	
	// Initialize some chars
	char sFile[NORMAL_LINE_LENGTH];
	char sPath[BIG_LINE_LENGTH];
	char sMapName[BIG_LINE_LENGTH];
	
	// Get current map name
	GetCurrentMap(sMapName, sizeof(sMapName));
	
	// Format full path to custom config
	Format(sFile, sizeof(sFile), "sourcemod/zombieplague/%s.cfg", sMapName); 
	Format(sPath, sizeof(sPath), "cfg/%s", sFile); 
	
	// Check if the file exist
	if (!FileExists(sPath))
	{
		// File doesn't exist  then stop
		return;
	}

	// Execute config file
	ServerCommand("exec %s", sFile);

	// Log action
	LogEvent(false, LogType_Normal, LOG_CORE_EVENTS, LogModule_Config, "Map Configs", "Executed map config file: %s", sPath);
}

/**
 * Used by modules that rely on configs to register their config file info.
 * (Don't forget to set 'loaded' to 'true' (ConfigSetConfigLoaded) in config load function)
 *
 * @param iFile      Config file entry to register.
 * @param sAlias     Config file alias, used for client interaction.
 **/
stock void ConfigRegisterConfig(ConfigFile iFile, ConfigStructure iStructure, const char[] sAlias = "")
{
    // Copy file info to data container
    gConfigData[iFile][Data_Loaded] = false;
    gConfigData[iFile][Data_Structure] = iStructure;
    gConfigData[iFile][Data_Handle] = NULL;
    gConfigData[iFile][Data_ReloadFunc] = INVALID_FUNCTION;
    strcopy(gConfigData[iFile][Data_Path], PLATFORM_MAX_PATH, "");
    strcopy(gConfigData[iFile][Data_Alias], CONFIG_MAX_LENGTH, sAlias);
}

/**
 * Set the loaded state of a config file entry.
 * 
 * @param iConfig    Config file to set load state of.
 * @param bLoaded    True to set as loaded, false to set as unloaded.
 **/
stock void ConfigSetConfigLoaded(ConfigFile iConfig, bool bLoaded)
{
    // Set load state
    gConfigData[iConfig][Data_Loaded] = bLoaded;
}

/**
 * Set the structure type of a config file entry.
 * 
 * @param iConfig    Config file to set structure type of.
 * @param iStructure Structure to set as.
 **/
stock void ConfigSetConfigStructure(ConfigFile iConfig, ConfigStructure iStructure)
{
    // Set load state
    gConfigData[iConfig][Data_Structure] = iStructure;
}

/**
 * Set the reload function of a config file entry.
 * 
 * @param iConfig        Config file to set reload function of.
 * @param iReloadfunc    Reload function.
 **/
stock void ConfigSetConfigReloadFunc(ConfigFile iConfig, Function iReloadfunc)
{
    // Set reload function.
    gConfigData[iConfig][Data_ReloadFunc] = iReloadfunc;
}

/**
 * Set the file handle of a config file entry.
 * 
 * @param iConfig    Config file to set handle of.
 * @param iFile      Config file handle.
 **/
stock void ConfigSetConfigHandle(ConfigFile iConfig, Handle iFile)
{
    // Set file handle
    gConfigData[iConfig][Data_Handle] = iFile;
}

/**
 * Set the config file path of a config file entry.
 * 
 * @param iConfig    Config file to set file path of.
 * @param sPath      File path.
 **/
stock void ConfigSetConfigPath(ConfigFile iConfig, const char[] sPath)
{
    // Set config file path
    strcopy(gConfigData[iConfig][Data_Path], PLATFORM_MAX_PATH, sPath);
}

/**
 * Set the alias of a config file entry.
 * 
 * @param iConfig    Config file to set alias of.
 * @param sAlias     Alias of the config file entry.
 **/
stock void ConfigSetConfigAlias(ConfigFile iConfig, const char[] sAlias)
{
    // Set config alias
    strcopy(gConfigData[iConfig][Data_Alias], CONFIG_MAX_LENGTH, sAlias);
}

/**
 * Returns if a config was successfully loaded.
 * 
 * @param iConfig    Config file to check load status of.
 * @return           True if config is loaded, false otherwise.
 **/
stock bool ConfigIsConfigLoaded(ConfigFile iConfig)
{
    // Return load status
    return gConfigData[iConfig][Data_Loaded];
}

/**
 * Returns config's structure type.
 * 
 * @param iConfig    Config file to get structure type of.
 * @return           Config structure type.
 **/
stock ConfigStructure ConfigGetConfigStructure(ConfigFile iConfig)
{
    // Return load status
    return gConfigData[iConfig][Data_Structure];
}

/**
 * Returns config's reload function.
 * 
 * @param iConfig    Config file to get reload function of.
 * @return           Config reload function.
 **/
stock Function ConfigGetConfigReloadFunc(ConfigFile iConfig)
{
    // Return load status
    return gConfigData[iConfig][Data_ReloadFunc];
}

/**
 * Returns config's file handle.
 * 
 * @param iConfig    Config file to get file handle of.
 * @return           Config file handle.
 **/
stock Handle ConfigGetConfigHandle(ConfigFile iConfig)
{
    // Return load status
    return gConfigData[iConfig][Data_Handle];
}

/**
 * Returns the path for a given config file entry.
 * 
 * @param iConfig    Config file to get path of. (see ConfigFile enum)
 **/
stock void ConfigGetConfigPath(ConfigFile iConfig, char[] sPath, int iMaxLen)
{
    // Copy path to return string
    strcopy(sPath, iMaxLen, gConfigData[iConfig][Data_Path]);
}

/**
 * Returns the alias for a given config file entry.
 * 
 * @param iConfig    Config file to get alias of. (see ConfigFile enum)
 **/
stock void ConfigGetConfigAlias(ConfigFile iConfig, char[] sAlias, int iMaxLen)
{
    // Copy alias to return string
    strcopy(sAlias, iMaxLen, gConfigData[iConfig][Data_Alias]);
}

/**
 * Loads a config file and sets up a nested array type data storage.
 * 
 * @param iConfig        The config file to load.
 * @param arrayConfig    Handle of the main array containing file data.
 * @param blockSize      The max length of the contained strings. 
 * @return               True if file was loaded successfuly, false otherwise.
 **/
stock bool ConfigLoadConfig(ConfigFile iConfig, ArrayList &arrayConfig, int blockSize = CONFIG_MAX_LENGTH)
{
    // Get config's structure
    ConfigStructure iStructure = ConfigGetConfigStructure(iConfig);
    
    // Get config's alias
    char sConfigAlias[CONFIG_MAX_LENGTH];
    ConfigGetConfigAlias(iConfig, sConfigAlias, sizeof(sConfigAlias));
    
    // Get config's file path
    char sConfigPath[PLATFORM_MAX_PATH];
    ConfigGetConfigPath(iConfig, sConfigPath, sizeof(sConfigPath));
    
    // If array hasn't been created, then create
    if (arrayConfig == NULL)
    {
        // Create array in handle
        arrayConfig = CreateArray(blockSize);
    }
    
    switch(iStructure)
    {
        case Structure_List :
        {
            // Open file
            Handle hFile;
            bool bSuccess = ConfigOpenConfigFile(iConfig, hFile);
            
            // If config file failed to open, then stop
            if (!bSuccess)
            {
                return false;
            }
            
            // Clear out array.
            ClearArray(arrayConfig);
            
            char sLine[PLATFORM_MAX_PATH];
            
            while(ReadFileLine(hFile, sLine, sizeof(sLine)))
            {
                // Cut out comments at the end of a line
                if (StrContains(sLine, "//") != -1)
                {
                    SplitString(sLine, "//", sLine, sizeof(sLine));
                }
                
                // Trim off whitespace
                TrimString(sLine);
                
                // If line is empty, then stop
                if (!sLine[0])
                {
                    continue;
                }
                
                // Push line into array
                PushArrayString(arrayConfig, sLine);
            }
            
            // We're done this file, so now we can destory it from memory
            CloseHandle(hFile);
            
            return true;
        }
		
		case Structure_ArrayList :
        {
            // Open file
            Handle hFile;
            bool bSuccess = ConfigOpenConfigFile(iConfig, hFile);
            
            // If config file failed to open, then stop
            if (!bSuccess)
            {
                return false;
            }
            
            // Clear out array
            ClearArray(arrayConfig);
            
            char sLine[PLATFORM_MAX_PATH];
            
            while(ReadFileLine(hFile, sLine, sizeof(sLine)))
            {
				// Cut out comments at the end of a line
				if (StrContains(sLine, "//") != -1)
				{
					SplitString(sLine, "//", sLine, sizeof(sLine));
				}

				// Trim off whitespace
				TrimString(sLine);

				// If line is empty, then stop
				if (!sLine[0])
				{
					continue;
				}

				// Create new array to store information for config entry
				ArrayList arrayConfigEntry = CreateArray(blockSize);

				// Push line into array
				PushArrayString(arrayConfigEntry, sLine); // Index: 0

				// Store this handle in the main array
				PushArrayCell(arrayConfig, arrayConfigEntry);
            }
            
            // We're done this file, so now we can destory it from memory
            CloseHandle(hFile);
            
            return true;
        }
		
        case Structure_Keyvalue :
        {
            // Open file
            Handle hKeyvalue;
            bool bSuccess = ConfigOpenConfigFile(iConfig, hKeyvalue);
            
            // If config file failed to open, then stop
            if (!bSuccess)
            {
                return false;
            }
            
            // Destroy all old data
            ConfigClearKvArray(arrayConfig);
            
            if (KvGotoFirstSubKey(hKeyvalue))
            {
                do
                {
                    // Create new array to store information for config entry
                    ArrayList arrayConfigEntry = CreateArray(blockSize);
                    
                    // Push the key name into the config entry's array
                    char sKeyName[CONFIG_MAX_LENGTH];
                    KvGetSectionName(hKeyvalue, sKeyName, sizeof(sKeyName));
                    
                    PushArrayString(arrayConfigEntry, sKeyName); // Index: 0
                    
                    // Store this handle in the main array
                    PushArrayCell(arrayConfig, arrayConfigEntry);
                } 
				while(KvGotoNextKey(hKeyvalue));
            }
            
            // We're done this file for now, so now we can destory it from memory 
            CloseHandle(hKeyvalue);
            
            return true;
        }
    }
    
    return false;
}

/**
 * Reload a config file.
 * 
 * @param iConfig    The config file entry to reload.
 * @return           True if the config is loaded, false if not.
 **/
stock bool ConfigReloadConfig(ConfigFile iConfig)
{
    // If file isn't loaded, then stop
    bool bLoaded = ConfigIsConfigLoaded(iConfig);
    if (!bLoaded)
    {
        return false;
    }
    
    // Call reload function
    Function iReloadfunc = ConfigGetConfigReloadFunc(iConfig);
    
    // Call reload function
    Call_StartFunction(GetMyHandle(), iReloadfunc);
    Call_Finish();
    
    return true;
}

/**
 * Opens a config file with appropriate method.
 * 
 * @param iConfig    The config file.
 * @param iStructure The structure of the config file.
 * @param hConfig    The handle of the opened file.
 **/
stock bool ConfigOpenConfigFile(ConfigFile iConfig, Handle &hConfig)
{
    // Get config's structure
    ConfigStructure iStructure = ConfigGetConfigStructure(iConfig);
    
    // Get config's file path
    char sConfigPath[PLATFORM_MAX_PATH];
    ConfigGetConfigPath(iConfig, sConfigPath, sizeof(sConfigPath));
    
    // Get config's alias
    char sConfigAlias[CONFIG_MAX_LENGTH];
    ConfigGetConfigAlias(iConfig, sConfigAlias, sizeof(sConfigAlias));
    
    switch(iStructure)
    {
		case Structure_Keyvalue :
		{
			hConfig = CreateKeyValues(sConfigAlias);
			return FileToKeyValues(hConfig, sConfigPath);
		}
		default :
		{
			// Open file
			hConfig = OpenFile(sConfigPath, "r");
			
			// If file couldn't be opened, then stop
			if (hConfig == NULL)
			{
				return false;
			}
			
			return true;
		}
    }
    
    return false;
}

/**
 * Creates, deletes, sets, or gets any key/setting of any ZP config keyvalue file in memory.
 * Only use when interacting with a command or manipulating single keys/values,
 * using this function everywhere would be EXTREMELY inefficient.
 * 
 * @param config        Config file to modify.
 * @param action        Action to perform on keyvalue tree. (see enum ConfigKeyvalueAction)
 * @param keys          Array containing keys to traverse into.
 * @param keysMax       The size of the 'keys' array.
 * @param setting       (Optional) The name of the setting to modify.
 * @param value         (Optional) The new value to set.
 * @param iMaxLen       (Optional) The maxlength of the retrieved value.
 * @return              True if the change was made successfully, false otherwise. 
 **/
stock bool ConfigKeyvalueTreeSetting(ConfigFile iConfig, ConfigKvAction iAction = KvAction_Create, const char[][] sKeys, int keysMax, const char[] sSetting = "", char[] sValue = "", int iMaxLen = 0)
{
    // Get config file's structure
    ConfigStructure iStructure = ConfigGetConfigStructure(iConfig);
    
    // If the config is any other structure beside keyvalue, then stop
    if (iStructure != Structure_Keyvalue)
    {
        return false;
    }
    
    // Retrieve handle of the keyvalue tree
    Handle hConfig;
    bool bSuccess = ConfigOpenConfigFile(iConfig, hConfig);
    
    // If the file couldn't be opened, then stop
    if (!bSuccess)
    {
        return false;
    }
    
    // Rewind keyvalue tree
    KvRewind(hConfig);
    
    // i = keys index.
    // Traverse into the keygroup, stop if it fails
    for (int i = 0; i < keysMax; v++)
    {
        // If key is empty, then break the loop
        if (!sKeys[i][0])
        {
            break;
        }
        
        // Try to jump to next level in the transversal stack, create key if specified
        bool bExists = KvJumpToKey(hConfig, sKeys[i], (iAction == KvAction_Create));
        
        // If exists is false, then stop
        if (!bExists)
        {
            // Key doesn't exist
            return false;
        }
    }
    
    switch(iAction)
    {
        case KvAction_Create :
        {
            if (!sSetting[0] || !sValue[0])
            {
                // We created the key already, so return true
                return true;
            }
            
            // Set new value
            KvSetString(hConfig, sSetting, sValue);
        }
        case KvAction_Delete :
        {
            // Return deletion result
            return KvDeleteKey(hConfig, sSetting);
        }
        case KvAction_Set :
        {
            // Set new value
            KvSetString(hConfig, sSetting, sValue);
        }
        case KvAction_Get:
        {
            // Get current value
            KvGetString(hConfig, sSetting, sValue, iMaxLen);
        }
    }
    
    // We successfully set or got the value
    return true;
}

/**
 * Destroy all array handles within an array, and clear main array.
 * 
 * @param arrayKv   The array converted from a keyvalue structure.
 **/
void ConfigClearKvArray(Handle arrayKv)
{
    //  i = array index
    int iSize = GetArraySize(arrayKv);
    for (int i = 0; i < iSize; i++)
    {
        // Destroy nested arrays
        Handle arrayKvKey = GetArrayCell(arrayKv, i);
        CloseHandle(arrayKvKey);
    }
    
    // Now that all data within has been destroyed, we can clear the main array
    ClearArray(arrayKv);
}

/**
 * Finds a config file entry, (see ConfigFile enum) for a given alias.
 * 
 * @param alias     The alias to find config file entry of.
 * @return          Config file entry, ConfigInvalid is returned if alias was not found.
 **/
stock ConfigFile ConfigAliasToConfigFile(const char[] sAlias)
{
    char sCheckAlias[CONFIG_MAX_LENGTH];
    
    // i = config file entry index
    for (int i = 0; i < sizeof(gConfigData); i++)
    {
        // Get config alias.
        ConfigGetConfigAlias(view_as<ConfigFile>(i), sCheckAlias, sizeof(sCheckAlias));
        
        // If alias doesn't match, then stop
        if (!StrEqual(sAlias, sCheckAlias, false))
        {
            continue;
        }
        
        // Return config file entry
        return view_as<ConfigFile>(i);
    }
    
    // Invalid config file
    return File_Invalid;
}

/**
 * Command callback (zp_config_reload)
 * Reloads a config file and forwards event to modules.
 * 
 * @param clientIndex	 The client index.
 * @param ArgCommands    Number of arguments that were in the argument string.
 **/
public Action ConfigReloadCommand(int clientIndex, int ArgCommands)
{
	// Check if privileged
	if (!IsClientPrivileged(clientIndex, OperationType_Config))
	{
		TranslationReplyToCommand(clientIndex, "Can not do it");
		return ACTION_HANDLED;
	}

	// If not enough arguments given, then stop
	if (ArgCommands < 1)
	{
		TranslationReplyToCommand(clientIndex, "Config reload");
		TranslationReplyToCommand(clientIndex, "Config reload commands");
		TranslationReplyToCommand(clientIndex, "Config reload commands aliases", CONFIG_FILE_ALIAS_MODELS, CONFIG_FILE_ALIAS_DOWNLOADS, CONFIG_FILE_ALIAS_WEAPONS, CONFIG_FILE_ALIAS_HITGROUPS, CONFIG_FILE_ALIAS_SOUNDS);
		return ACTION_HANDLED;
	}

	char sFileAlias[CONFIG_MAX_LENGTH];
	char sPath[PLATFORM_MAX_PATH];
	char sLogMessage[PLATFORM_MAX_PATH];

	int iArgs = GetCmdArgs();
	for (int i = 1; i <= iArgs; i++)
	{
		// Get alias to restrict
		GetCmdArg(i, sFileAlias, sizeof(sFileAlias));

		// If alias is invalid, then stop
		ConfigFile iConfig = ConfigAliasToConfigFile(sFileAlias);
		if (iConfig == File_Invalid)
		{
			TranslationReplyToCommand(clientIndex, "Config reload invalid");
			return ACTION_HANDLED;
		}

		// Reload config file
		bool bLoaded = ConfigReloadConfig(iConfig);

		// Get config file path
		ConfigGetConfigPath(iConfig, sPath, sizeof(sPath));

		// Format log message
		Format(sLogMessage, sizeof(sLogMessage), "[%N] reloaded config file \"%s\". (zp_config_reload)", clientIndex, sPath);

		// If file isn't loaded then tell client, then stop
		if (!bLoaded)
		{
			TranslationReplyToCommand(clientIndex, "Config reload not load");

			// Format a failed attempt string to the end of the log message
			Format(sLogMessage, sizeof(sLogMessage), "%s -- attempt failed, config file not loaded", sLogMessage);
		}

		// Log action to game events
		LogEvent(false, LogType_Command, LOG_GAME_EVENTS, LogModule_Admin, "Admin Command", sLogMessage);
	}

	return ACTION_HANDLED;
}

/**
 * Command callback (zp_config_reloadall)
 * Reloads all config files and forwards event to all modules.
 * 
 * @param clientIndex	 The client index.
 * @param ArgCommands    Number of arguments that were in the argument string.
 **/
public Action ConfigReloadAllCommand(int clientIndex, int ArgCommands)
{
	// Check if privileged
	if (!IsClientPrivileged(clientIndex, OperationType_Config))
	{
		TranslationReplyToCommand(clientIndex, "Can not do it");
		return ACTION_HANDLED;
	}

	// Begin statistics
	TranslationReplyToCommand(clientIndex, "Config reload begin");

	char sConfigAlias[CONFIG_MAX_LENGTH];

	// i = config file entry index
	for (int i = 0; i < sizeof(gConfigData); i++)
	{
		// Reload config file
		bool bSuccessful = ConfigReloadConfig(view_as<ConfigFile>(i));

		// Get config's alias
		ConfigGetConfigAlias(view_as<ConfigFile>(i), sConfigAlias, sizeof(sConfigAlias));

		if (bSuccessful)
		{
			TranslationReplyToCommand(clientIndex, "Config reload finish");
		}
		else
		{
			TranslationReplyToCommand(clientIndex, "Config reload falied");
		}
	}

	// Log action to game events
	LogEvent(false, LogType_Command, LOG_GAME_EVENTS, LogModule_Admin, "Admin Command", "[%N] reloaded all config files", clientIndex);

	return ACTION_HANDLED;
}

/**
 * Load config file.
 * 
 * @param iConVar  The cvar define of the path to the file.
 * @return     	   True if the file exists, false if not.
 **/
stock bool ConfigGetCvarFilePath(CvarsList iConVar, char[] sPath)
{
	// Get cvar's path
	char sFilePath[PLATFORM_MAX_PATH];
	GetConVarString(view_as<ConVar>(gCvarList[iConVar]), sFilePath, sizeof(sFilePath));

	// Build full path in return string
	BuildPath(Path_SM, sPath, PLATFORM_MAX_PATH, sFilePath);

	// If file is exist, then return true
	return FileExists(sPath);
}

/**
 * Converts string of "yes/on" or "no/off" to a boolean value.  Always uses english as yes/no/on/off language.
 * 
 * @param sOption    "yes/on" or "no/off" string to be converted.
 * @return           True if string is "yes", false otherwise.
 **/
stock bool ConfigSettingToBool(const char[] sOption)
{
    // If option is equal to "yes", then return true
    if (StrEqual(sOption, "yes", false) || StrEqual(sOption, "on", false) || StrEqual(sOption, "1", false))
    {
        return true;
    }
    
    // Option isn't yes
    return false;
}

/**
 * Converts boolean value to "yes" or "no".
 * 
 * @param bOption    True/false value to be converted to "yes/on"/"no/off", respectively.
 * @param sOption    Destination string buffer to store "yes/on" or "no/off" in.
 * @param iMaxLen    Length of destination string buffer.
 * @param yesno      When true, returns "yes/no", false returns "on/off."
 **/
stock void ConfigBoolToSetting(bool bOption, char[] sOption, int iMaxLen, bool yesno = true)
{
    // If option is true, then copy "yes" to return string
    if (bOption)
    {
        yesno ? strcopy(sOption, iMaxLen, "Yes") : strcopy(sOption, iMaxLen, "On");
    }
    // If option is false, then copy "no" to return string
    else
    {
        yesno ? strcopy(sOption, iMaxLen, "No") : strcopy(sOption, iMaxLen, "Off");
    }
}

/**
 * Returns a "yes/no" string from config as a bool.
 * 
 * @param kv              The keyvalue handle.
 * @param sKey            The keyname the value is under.
 * @param sDefaultValue  (Optional) Value to return if setting is missing.
 **/
stock bool ConfigKvGetStringBool(Handle kv, const char[] sKey, const char[] sDefaultValue = "yes")
{
	// Get string from key
    char sValue[CONFIG_MAX_LENGTH];
    KvGetString(kv, sKey, sValue, sizeof(sValue), sDefaultValue);
    
	// Convert string to bool
    return ConfigSettingToBool(sValue);
}