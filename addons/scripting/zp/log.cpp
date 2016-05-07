/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          log.cpp
 *  Type:          Core 
 *  Description:   Logging API.
 *
 *  Copyright (C) 2015-2016 Greyscale, Richard Helgeby
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
 * @section Log message max lengths.
 **/
#define LOG_MAX_LENGTH_FILE 2048
#define LOG_MAX_LENGTH_CHAT 192
/**
 * @endsection
 **/
 
/**
 * @section Custrom log file.
 **/
#define LOG_FILE "addons/sourcemod/logs/zombieplague.log"
/**
 * @endsection
 **/
/**
 * @section Log flags.
 **/
#define LOG_CORE_EVENTS             (1 << 0)    /** Log events from the plugin core like config validation and other messages. */
#define LOG_GAME_EVENTS             (1 << 1)    /** Log admin commands, console commands, and game related events from modules like suicide attempts and weapon restrictions. */
#define LOG_PLAYER_COMMANDS         (1 << 2)    /** Log events that are triggered by players, like chat triggers. */
#define LOG_DEBUG                   (1 << 3)    /** Log debug messages, if any. Usually only developers enable this log flag. */
#define LOG_DEBUG_DETAIL            (1 << 4)    /** Log additional debug messages with more detail. May cause spam depending on filter settings. Usually only developers enable this log flag. */
/**
 * @endsection
 **/
 
/**
 * Log format types.
 **/
enum LogTypes
{
    LogType_Normal = 0,     // Normal log message. Printed in SourceMod logs.
    LogType_Error,          // Error message. Printed in SourceMod error logs.
    LogType_Fatal,          // Fatal error. Stops the plugin with the specified message.
	LogType_Native,			// Throws an error in the calling plugin of a native, instead of your own plugin.
	LogType_Command			// Command log message. Printed in SourceMod logs and in chat to all.
}

/**
 * List of modules that write log events. Add new modules if needed (in
 * alphabetical order).
 * 
 * Update following when adding modules:
 * - Admin log flag menu
 * - LogGetModuleNameString
 * - LogGetModule
 **/
enum LogModules
{
	bool:LogModule_Invalid = 0,         /** Used as return value when an error occoured.*/

    bool:LogModule_Engine,
	bool:LogModule_Config,
	bool:LogModule_DataBase,
	bool:LogModule_Models,
	bool:LogModule_Sounds,
	bool:LogModule_Hitgroups,
	bool:LogModule_Downloads,
	bool:LogModule_Weapons,
	bool:LogModule_Zombieclasses,
	bool:LogModule_Extraitems,
	bool:LogModule_Admin,
	bool:LogModule_Native
}

/**
 * Dynamic string array for module filtering.
 **/
ArrayList ArrayLogModuleFilter;

/**
 * Cache of current module filter settings. For fast and easy access.
 **/
int LogModuleFilterCache[LogModules];


/**
 * List of modules that write log events. 
 **/
void LogInit()
{
    // Destroy existing handle to prevent memory leak
    if (ArrayLogModuleFilter != INVALID_HANDLE)
    {
        CloseHandle(ArrayLogModuleFilter);
    }
    
    // Initialize module filter array
    ArrayLogModuleFilter = CreateArray(32);
}

/**
 * Converts a string module name into a module type.
 *
 * @param sModuleName	A string with the short module name. Case insensitive,
 *                      but not trimmed for white space.
 * @return              The matcing module type or LogModules_Invalid if failed.
 **/
LogModules LogGetModule(char[] sModuleName)
{
	if(StrEqual(sModuleName, "engine", false))
	{
		return LogModule_Engine;
	}
	else if(StrEqual(sModuleName, "config", false))
	{
		return LogModule_Config;
	}
	else if(StrEqual(sModuleName, "database", false))
	{
		return LogModule_DataBase;
	}
	else if(StrEqual(sModuleName, "models", false))
	{
		return LogModule_Models;
	}
	else if(StrEqual(sModuleName, "sounds", false))
	{
		return LogModule_Sounds;
	}
	else if(StrEqual(sModuleName, "hitgroups", false))
	{
		return LogModule_Hitgroups;
	}
	else if(StrEqual(sModuleName, "downloads", false))
	{
		return LogModule_Downloads;
	}
	else if(StrEqual(sModuleName, "weapons", false))
	{
		return LogModule_Weapons;
	}
	else if(StrEqual(sModuleName, "zombieclasses", false))
	{
		return LogModule_Zombieclasses;
	}
	else if(StrEqual(sModuleName, "extraitems", false))
	{
		return LogModule_Extraitems;
	}
	else if(StrEqual(sModuleName, "admin", false))
	{
		return LogModule_Admin;
	}
	else if(StrEqual(sModuleName, "native", false))
	{
		return LogModule_Native;
	}

	// No match
	return LogModule_Invalid;
}

/**
 * Check if the specified log flag is set.
 *
 * @param eventType		The log flag to check.
 * @return              True if set, false otherwise.
 **/
bool LogCheckFlag(int eventType)
{
    // Check if eventType is set
    if (eventType)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Check if the specified module is enabled in the LOG_MODULE filter cache.
 *
 * @param iModule		Module to check.
 * @return              True if enabled, false otherwise. 
 **/
bool LogCheckModuleFilter(LogModules iModule)
{
    if (LogModuleFilterCache[iModule])
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Convert module type to a string.
 *
 * @param sBuffer        Destination string buffer.
 * @param iMaxLen        Size of destination buffer.
 * @param iModule        Module type to convert.
 * @param shortName      Optional. Use short name instead of human readable names. Default is false
 *
 * @return               Number of cells written.
 **/
int LogGetModuleNameString(char[] sBuffer, int iMaxLen, LogModules iModule, bool shortName = false)
{
	switch (iModule)
	{
		case LogModule_Engine :
		{
			return shortName ? strcopy(sBuffer, iMaxLen, "engine") : strcopy(sBuffer, iMaxLen, "Engine");
		}
		case LogModule_Config :
		{
			return shortName ? strcopy(sBuffer, iMaxLen, "config") : strcopy(sBuffer, iMaxLen, "Config");
		}
		case LogModule_DataBase :
		{
			return shortName ? strcopy(sBuffer, iMaxLen, "database") : strcopy(sBuffer, iMaxLen, "Data Base");
		}
		case LogModule_Models :
		{
			return shortName ? strcopy(sBuffer, iMaxLen, "models") : strcopy(sBuffer, iMaxLen, "Models");
		}
		case LogModule_Sounds :
		{
			return shortName ? strcopy(sBuffer, iMaxLen, "sounds") : strcopy(sBuffer, iMaxLen, "Sounds");
		}
		case LogModule_Hitgroups :
		{
			return shortName ? strcopy(sBuffer, iMaxLen, "hitgroups") : strcopy(sBuffer, iMaxLen, "Hitgroups");
		}
		case LogModule_Downloads :
		{
			return shortName ? strcopy(sBuffer, iMaxLen, "downloads") : strcopy(sBuffer, iMaxLen, "Downloads");
		}
		case LogModule_Weapons:
		{
			return shortName ? strcopy(sBuffer, iMaxLen, "weapons") : strcopy(sBuffer, iMaxLen, "Weapons");
		}
		case LogModule_Zombieclasses :
		{
			return shortName ? strcopy(sBuffer, iMaxLen, "zombieclasses") : strcopy(sBuffer, iMaxLen, "Zombie Classes");
		}
		case LogModule_Extraitems :
		{
			return shortName ? strcopy(sBuffer, iMaxLen, "extraitems") : strcopy(sBuffer, iMaxLen, "Extra Items");
		}
		case LogModule_Admin :
		{
			return shortName ? strcopy(sBuffer, iMaxLen, "admin") : strcopy(sBuffer, iMaxLen, "Admin");
		}
		case LogModule_Native :
		{
			return shortName ? strcopy(sBuffer, iMaxLen, "native") : strcopy(sBuffer, iMaxLen, "Native");
		}
	}

	// Module mismatch
	return 0;
}

/**
 * Print a formatted message to logs depending on log settings.
 *
 * @param isConsole     Optional. Specifies whether the log event came from
 *                      client 0. Used in console commands, do not mix with
 *                      regular log events. Default is false.
 * @param logType       Optional. Log type and action. Default is
 *                      LogType_Normal.
 * @param eventType     Optional. A log flag describing What kind of log event
 *                      it is. Default is LOG_CORE_EVENTS.
 * @param iModule       Module the log event were executed in.
 * @param sDescription  Event type or function name. A short descriptive phrase
 *                      to group together similar logs.
 * @param sMessage      Log message. Can be formatted.
 * @param ...           Formatting parameters.
 **/
void LogEvent(bool isConsole = false, LogTypes logType = LogType_Normal, int eventType = LOG_CORE_EVENTS, LogModules iModule, char[] sDescription, const char[] sMessage, any ...)
{    
	// Check filter overrides. Always log fatal errors, and check error override setting on error log types
	if ((logType != LogType_Fatal && logType != LogType_Error) || (logType == LogType_Error && !GetConVarBool(gCvarList[CVAR_LOG_ERROR_OVERRIDE])))
	{
		// Check if logging is disabled
		if (!GetConVarBool(gCvarList[CVAR_LOG]))
		{
			return;
		}

		// Check if console is ignored
		if (isConsole && GetConVarBool(gCvarList[CVAR_LOG_IGNORE_CONSOLE]))
		{
			return;
		}

		// Check event type (log flag)
		if (!LogCheckFlag(eventType))
		{
			return;
		}

		// Check if module filtering is enabled
		if (GetConVarBool(gCvarList[CVAR_LOG_MODULE_FILTER]))
		{
			// Check if the specified module is enabled
			if (!LogCheckModuleFilter(iModule))
			{
				return;
			}
		}
	}

	// Format extra parameters into the log buffer
	char sLogBuffer[LOG_MAX_LENGTH_FILE];
	VFormat(sLogBuffer, sizeof(sLogBuffer), sMessage, 7);

	// Get human readable module name
	char sModule[NORMAL_LINE_LENGTH];
	LogGetModuleNameString(sModule, sizeof(sModule), iModule);

	// Format 
	Format(sLogBuffer, sizeof(sLogBuffer), "[%s] [%s] %s", sModule, sDescription, sLogBuffer);

	// Format other parameters onto the log text
	switch (logType)
	{
		// Log type is normal
		case LogType_Normal:
		{
			LogMessage(sLogBuffer);
		}
		// Log type is error
		case LogType_Error:
		{
			LogError(sLogBuffer);
		}
		// Log type is fatal error
		case LogType_Fatal:
		{
			SetFailState(sLogBuffer);
		}
		// Log type is native error
		case LogType_Native:
		{
			ThrowNativeError(SP_ERROR_NATIVE, sLogBuffer);
		}
		// Log type is command
		case LogType_Command:
		{
			LogToFile(LOG_FILE, sLogBuffer);
		}
	}

	// Check if printing log events to public chat is enabled
	if (GetConVarBool(gCvarList[CVAR_LOG_PRINT_CHAT]))
	{
		// Print text to public chat
		PrintToChatAll(sLogBuffer);
	}
}


/**
 * Adds a module to the module filter and updates the cache. If it already
 * exist the command is ignored.
 *
 * @param iModule   The module to add.
 * @return          True if added, false otherwise.
 **/
bool LogModuleFilterAdd(LogModules iModule)
{
    char sModuleName[NORMAL_LINE_LENGTH];
    
    // Check if empty
    if (strlen(sModuleName) == 0)
    {
        return false;
    }
    
    // Convert module name
    LogGetModuleNameString(sModuleName, sizeof(sModuleName), iModule, true);
    
    // Check if the module isn't already is listed
    if (FindStringInArray(ArrayLogModuleFilter, sModuleName) < 0)
    {
        // Add module to filter
        PushArrayString(ArrayLogModuleFilter, sModuleName);
        return true;
    }
    
    return false;
}

/**
 * Removes a module to the module filter and updates the cache. If it doesn't
 * exist the command is ignored.
 *
 * @param iModule	The module to remove.
 * @return			True if removed, false otherwise.
 **/
bool LogModuleFilterRemove(LogModules iModule)
{
    char sModuleName[NORMAL_LINE_LENGTH];
    int  iModuleIndex;
    
    // Check if empty
    if (strlen(sModuleName) == 0)
    {
        return false;
    }
    
    // Convert module name
    LogGetModuleNameString(sModuleName, sizeof(sModuleName), iModule, true);
    
    // Get the module index
    iModuleIndex = FindStringInArray(ArrayLogModuleFilter, sModuleName);
    
    // Check if successful
    if (iModuleIndex >= 0)
    {
        // Remove module from filter
        RemoveFromArray(ArrayLogModuleFilter, iModuleIndex);
        return true;
    }
    
    return false;
}

/**
 * Update module filter cache.
 **/
void LogModuleFilterCacheUpdate(/*void*/)
{
    char sModuleName[NORMAL_LINE_LENGTH];
    LogModules iModuleType;
    
    // Clear all entries in module cache
    int iModuleCount = sizeof(LogModuleFilterCache);
    for (int i = 1; i < iModuleCount; i++)
    {
        LogModuleFilterCache[view_as<LogModules>(i)] = false;
    }
    
    // Loop through the module array
    int iSize = GetArraySize(ArrayLogModuleFilter);
    for (int i = 0; i < iSize; i++)
    {
        // Get the module name
        GetArrayString(ArrayLogModuleFilter, i, sModuleName, sizeof(sModuleName));
        
        // Convert to type
        iModuleType = LogGetModule(sModuleName);
        
        // Validate type
        if (iModuleType != LogModule_Invalid)
        {
            // Set value in cache
            LogModuleFilterCache[iModuleType] = true;
        }
    }
}

/**
 * Creates commands for logging module. Called when commands are created.
 **/
void LogOnCommandsCreate(/*void*/)
{
    RegConsoleCmd("zp_log_list", Command_LogList, "List available logging flags and modules with their status values.");
    RegConsoleCmd("zp_log_add_module", Command_LogAddModule, "Add one or more modules to the module filter. Usage: zp_log_add_module <module> [module] ...");
    RegConsoleCmd("zp_log_remove_module", Command_LogRemoveModule, "Remove one or more modules from the module filter. Usage: zp_log_remove_module <module> [module] ...");
}

/**
 * Handles the zp_log_list command. Displays flags and module filter cache.
 * 
 * @param clientIndex	The client index.
 * @param ArgCommands	Number of arguments that were in the argument string.
 **/ 
public Action Command_LogList(int clientIndex, int iArguments)
{
	// Initialize some chars
    char sBuffer[LOG_MAX_LENGTH_FILE];
    char sLineBuffer[NORMAL_LINE_LENGTH];
    char sModuleName[NORMAL_LINE_LENGTH];
    
    // Strings to store translated phrases. Because formatting width settings
    // doesn't work with "%t", but "%s"
    char sPhraseGenericFlag[SMALL_LINE_LENGTH];
    char sPhraseValue[SMALL_LINE_LENGTH];
    char sPhraseModule[SMALL_LINE_LENGTH];
    char sPhraseShortName[SMALL_LINE_LENGTH];
    
    // Quick initialize string buffer
    sBuffer[0] = 0;
    
    // Set language
    SetGlobalTransTarget(clientIndex);
    
    // Get phrases
    Format(sPhraseGenericFlag, sizeof(sPhraseGenericFlag), "%t", "Log generic flag");
    Format(sPhraseValue, sizeof(sPhraseValue), "%t", "Log value");
    Format(sPhraseModule, sizeof(sPhraseModule), "%t", "Log module");
    Format(sPhraseShortName, sizeof(sPhraseShortName), "%t", "Log module short name");
    
    // Log flags:
    Format(sLineBuffer, sizeof(sLineBuffer), "%-19s %-7s %t\n", sPhraseGenericFlag, sPhraseValue, "Log status");
    StrCat(sBuffer, sizeof(sBuffer), sLineBuffer);
    StrCat(sBuffer, sizeof(sBuffer), "--------------------------------------------------------------------------------\n");
    
    Format(sLineBuffer, sizeof(sLineBuffer), "LOG_CORE_EVENTS     1       %s\n", LogCheckFlag(LOG_CORE_EVENTS) ? "On" : "Off");
    StrCat(sBuffer, sizeof(sBuffer), sLineBuffer);
    
    Format(sLineBuffer, sizeof(sLineBuffer), "LOG_GAME_EVENTS     2       %s\n", LogCheckFlag(LOG_GAME_EVENTS) ? "On" : "Off");
    StrCat(sBuffer, sizeof(sBuffer), sLineBuffer);
    
    Format(sLineBuffer, sizeof(sLineBuffer), "LOG_PLAYER_COMMANDS 4       %s\n", LogCheckFlag(LOG_PLAYER_COMMANDS) ? "On" : "Off");
    StrCat(sBuffer, sizeof(sBuffer), sLineBuffer);
    
    Format(sLineBuffer, sizeof(sLineBuffer), "LOG_DEBUG           8       %s\n", LogCheckFlag(LOG_DEBUG) ? "On" : "Off");
    StrCat(sBuffer, sizeof(sBuffer), sLineBuffer);
    
    Format(sLineBuffer, sizeof(sLineBuffer), "LOG_DEBUG_DETAIL    16      %s\n", LogCheckFlag(LOG_DEBUG_DETAIL) ? "On" : "Off");
    StrCat(sBuffer, sizeof(sBuffer), sLineBuffer);
    
    ReplyToCommand(clientIndex, sBuffer);
    sBuffer[0] = 0;
    
    // Module filtering status:
    Format(sLineBuffer, sizeof(sLineBuffer), "%t %ы\n\n", "Log module filter", GetConVarBool(gCvarList[CVAR_LOG_MODULE_FILTER]) ? "On" : "Off");
    StrCat(sBuffer, sizeof(sBuffer), sLineBuffer);
    
    Format(sLineBuffer, sizeof(sLineBuffer), "%-23s %-19s %t\n", sPhraseModule, sPhraseShortName, "Log status");
    StrCat(sBuffer, sizeof(sBuffer), sLineBuffer);
    StrCat(sBuffer, sizeof(sBuffer), "--------------------------------------------------------------------------------");
    
    ReplyToCommand(clientIndex, sBuffer);
    sBuffer[0] = 0;
    
    // Module status:
    int iModulecount = sizeof(LogModuleFilterCache);
    for (int i = 1; i < iModulecount; i++)
    {
        LogGetModuleNameString(sModuleName, sizeof(sModuleName), view_as<LogModules>(i));
        LogGetModuleNameString(sPhraseShortName, sizeof(sPhraseShortName), view_as<LogModules>(i), true);
        Format(sLineBuffer, sizeof(sLineBuffer), "%-23s %-19s %s", sModuleName, sPhraseShortName, LogModuleFilterCache[view_as<LogModules>(i)] ? "On" : "Off");
        ReplyToCommand(clientIndex, sLineBuffer);
    }
    
    return ACTION_HANDLED;
}

/**
 * Handles the zp_log_add_module command. Add one or modules to module filter.
 * 
 * @param clientIndex	The client index.
 * @param ArgCommands	Number of arguments that were in the argument string.
 **/
public Action Command_LogAddModule(int clientIndex, int iArguments)
{
	// Initialize some chars
	char sArgument[SMALL_LINE_LENGTH];

	// Check if privileged
	if (!IsClientPrivileged(clientIndex))
	{
		TranslationReplyToCommand(clientIndex, "Can not do it");
		return ACTION_HANDLED;
	}

	// Module
	LogModules iModule;

	// Check if no arguments
	if (iArguments < 1)
	{
		// Display syntax info
		char sBuffer[PLATFORM_MAX_PATH];
		sBuffer[0] = 0;
		StrCat(sBuffer, sizeof(sBuffer), "Add one or more modules to the module filter. Usage: zp_log_add_module <module> [module] ...\n");
		StrCat(sBuffer, sizeof(sBuffer), "See zp_log_list to list available module names (short names).");
		ReplyToCommand(clientIndex, sBuffer);
	}

	// Loop through each argument
	for (int i = 1; i <= iArguments; i++)
	{
		// Get argument string
		GetCmdArg(i, sArgument, sizeof(sArgument));

		// Convert to module type
		iModule = LogGetModule(sArgument);

		// Check if invalid
		if (iModule == LogModule_Invalid)
		{
			ReplyToCommand(clientIndex, "Invalid module name: \"%s\"", sArgument);

			// Skip to next argument
			continue;
		}

		LogModuleFilterAdd(iModule);
		ReplyToCommand(clientIndex, "Added \"%s\" to module filter.", sArgument);
	}

	// Update cache
	LogModuleFilterCacheUpdate();

	return ACTION_HANDLED;
}

/**
 * Handles the zp_log_add_module command. Remove one or modules to module filter.
 * 
 * @param clientIndex	The client index.
 * @param ArgCommands	Number of arguments that were in the argument string.
 **/
public Action Command_LogRemoveModule(int clientIndex, int iArguments)
{
	// Initialize some chars
    char sArgument[SMALL_LINE_LENGTH];
    
    // Check if privileged
    if (!IsClientPrivileged(clientIndex))
    {
        TranslationReplyToCommand(clientIndex, "Can not do it");
        return ACTION_HANDLED;
    }
    
	// Module
    LogModules iModule;
    
    // Check if no arguments
    if (iArguments < 1)
    {
		// Display syntax info
		char sBuffer[PLATFORM_MAX_PATH];
		sBuffer[0] = 0;
		StrCat(sBuffer, sizeof(sBuffer), "Remove one or more modules to the module filter. Usage: zp_log_remove_module <module> [module] ...\n");
		StrCat(sBuffer, sizeof(sBuffer), "See zp_log_list to list available module names (short names).");
		ReplyToCommand(clientIndex, sBuffer);
    }
    
    // Loop through each argument
    for (int i = 1; i <= iArguments; i++)
    {
        // Get argument string
        GetCmdArg(i, sArgument, sizeof(sArgument));
        
        // Convert to module type
        iModule = LogGetModule(sArgument);
        
        // Check if invalid
        if (iModule == LogModule_Invalid)
        {
            ReplyToCommand(clientIndex, "Invalid module name: \"%s\"", sArgument);
            
            // Skip to next argument
            continue;
        }
        
        LogModuleFilterRemove(iModule);
        ReplyToCommand(clientIndex, "Removed \"%s\" from module filter.", sArgument);
    }
    
    // Update cache.
    LogModuleFilterCacheUpdate();
    
    return ACTION_HANDLED;
}
