/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          downloads.cpp
 *  Type:          Core 
 *  Description:   Download validation.
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
 * Array that stores a list of downloadable files.
 **/
ArrayList arrayDownloads;

/**
 * Prepare all model/download data.
 **/
void DownloadsLoad(/*void*/)
{
	// Register config file
	ConfigRegisterConfig(File_Downloads, Structure_List, CONFIG_FILE_ALIAS_DOWNLOADS);

	// Get downloads file path
	char sDownloadsPath[PLATFORM_MAX_PATH];
	bool bExists = ConfigGetCvarFilePath(CVAR_CONFIG_PATH_DOWNLOADS, sDownloadsPath);

	// If file doesn't exist, then log and stop
	if (!bExists)
	{
		// Log failure and stop plugin
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Downloads, "Config Validation", "Missing downloads file: \"%s\"", sDownloadsPath);
	}

	// Set the path to the config file
	ConfigSetConfigPath(File_Downloads, sDownloadsPath);

	// Load config from file and create array structure
	bool bSuccess = ConfigLoadConfig(File_Downloads, arrayDownloads, PLATFORM_MAX_PATH);

	// Unexpected error, stop plugin
	if (!bSuccess)
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Downloads, "Config Validation", "Unexpected error encountered loading: %s", sDownloadsPath);
	}

	// Log what download file that is loaded
	LogEvent(true, LogType_Normal, LOG_CORE_EVENTS, LogModule_Downloads, "Config Validation", "Loading downloads from file \"%s\".", sDownloadsPath);

	// Initialize numbers of downloads
	int iDownloadCount;
	int iDownloadValidCount;

	// Initialize line char
	char sDownloadPath[PLATFORM_MAX_PATH];

	int iDownloads = iDownloadCount = GetArraySize(arrayDownloads);

	// i = download array index
	for (int i = 0; i < iDownloads; i++)
	{
		// Get download path
		GetArrayString(arrayDownloads, i, sDownloadPath, sizeof(sDownloadPath));

		// If file exist
		if (FileExists(sDownloadPath))
		{
			// Increment downloadvalidcount
			iDownloadValidCount++;
			
			// Precache file and add to downloads table
			AddFileToPrecacheList(sDownloadPath);
		}
		// If doesn't exist, may be this is directory ?
		else
		{
			// Get last char in the string
			int iLastChar = strlen(sDownloadPath) - 1;
			
			// Open directory
			DirectoryListing sDirectory = OpenDirectory(sDownloadPath);
			
			// If directory doesn't exist, then log, and stop
			if(sDirectory == NULL || sDownloadPath[iLastChar] != '/')
			{
				// Remove download from array
				RemoveFromArray(arrayDownloads, i);

				// Subtract one from count
				iDownloads--;

				// Backtrack one index, because we deleted it out from under the loop
				i--;

				LogEvent(false, LogType_Error, LOG_CORE_EVENTS, LogModule_Downloads, "Config Validation", "Incorrect path \"%s\"", sDownloadPath);
				continue;
			}
	
			// Initialize some variables
			char sFile[NORMAL_LINE_LENGTH];
			char sLine[PLATFORM_MAX_PATH];
			
			// File types
			FileType sType;
			
			// Search any files in directory and precache them
			while (ReadDirEntry(sDirectory, sFile, NORMAL_LINE_LENGTH, sType)) 
			{ 
				if (sType == FileType_File) 
				{
					// Format full path to file
					Format(sLine, sizeof(sLine), "%s%s", sDownloadPath, sFile);
					
					// Add to server precache list
					AddFileToPrecacheList(sLine);
					
					// Increment downloadvalidcount
					iDownloadValidCount++;
				}
			}
		
			// Close directory
			CloseHandle(sDirectory); 
		}
	}

	// Log download validation info
	LogEvent(true, LogType_Normal, LOG_CORE_EVENTS, LogModule_Downloads, "Config Validation", "Total: %d | Successful: %d | Unsuccessful: %d", iDownloadCount, iDownloadValidCount, iDownloadCount - iDownloadValidCount);

	// Set config data
	ConfigSetConfigLoaded(File_Downloads, true);
	ConfigSetConfigReloadFunc(File_Downloads, GetFunctionByName(GetMyHandle(), "DownloadsOnConfigReload"));
	ConfigSetConfigHandle(File_Downloads, arrayDownloads);
}

/**
 * Called when configs are being reloaded.
 * 
 * @param config    The config being reloaded. (only if 'all' is false)
 **/
public void DownloadsOnConfigReload(ConfigFile iConfig)
{
    // Reload download config
    DownloadsLoad();
}