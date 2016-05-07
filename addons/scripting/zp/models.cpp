/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          models.cpp
 *  Type:          Core
 *  Description:   Model manager.
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
 * Maximum number of models.
 **/
#define ModelsMax 48

/**
 * Maximum string length of a folder a model file is located under.
 **/
#define MODELS_PATH_DIR_MAX_LENGTH 32

/**
 * Model access settings.
 **/
enum ModelAccess
{
    ModelAccess_Invalid = -1,   /* Invalid access type. */
    ModelAccess_Public = 0,     /* Everyone can use the model. */
    ModelAccess_Admins,         /* Model can only be used by admins. */
    ModelAccess_Vips,  		  	/* Model can only be used by vips. */
    ModelAccess_Group           /* Enables group authentication. */
}

/**
 * Model types.
 **/
enum ModelType
{
    ModelType_View = 0,       /* View model. */
    ModelType_World,          /* World model. */
}

/**
 * @section Model access flags.
 **/
#define MODEL_ACCESS_PUBLIC             (1<<0)
#define MODEL_ACCESS_ADMINS             (1<<1)
#define MODEL_ACCESS_HIDDEN             (1<<2)
#define MODEL_ACCESS_VIPS     			(1<<3)
#define MODEL_ACCESS_GROUP              (1<<4)
/**
 * @endsection
 **/

/**
 * Model settings structure.
 **/
enum ModelAttributes
{
    String:Model_Path[PLATFORM_MAX_PATH],       /* File name of model (no file extension). */
    String:Arm_Path[PLATFORM_MAX_PATH],      	/* Path to model files. */
    ModelAccess:Model_Access,                   /* Access settings. */
    String:Model_Group[NORMAL_LINE_LENGTH]      /* Group authentication (if used). */
}
 
/**
 * Parsed model data.
 **/
int   gModelData[ModelsMax][ModelAttributes];

/**
 * Number of valid models.
 **/
int   gModelCount;

/**
 * Other models.
 **/
int gWorldKnife;
int gNemesisKnife;
int gSurvivorMach[ModelType];
int gSurvivorPist[ModelType];

/**
 * The map is ending.
 **/
void ModelsOnMapEnd(/*void*/)
{
	// Reset the number of models, to be re-cached for the next map
	gModelCount = 0;
}

/**
 * Prepare all model/download data.
 **/
void ModelsLoad(/*void*/)
{
	Handle kvModels = NULL;

	// Register config file
	ConfigRegisterConfig(File_Models, Structure_List, CONFIG_FILE_ALIAS_MODELS);

	// Get models file path
	char sModelPath[PLATFORM_MAX_PATH];
	bool bExists = ConfigGetCvarFilePath(CVAR_CONFIG_PATH_MODELS, sModelPath);

	// If file doesn't exist, then log and stop
	if (!bExists)
	{
		// Log failure and stop plugin
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Models, "Config Validation", "Missing model list: \"%s\"", sModelPath);
	}

	// Set the path to the config file
	ConfigSetConfigPath(File_Models, sModelPath);

	// Prepare key/value structure.
	kvModels = CreateKeyValues(CONFIG_FILE_ALIAS_MODELS);

	// Log what models file that is loaded
	LogEvent(true, LogType_Normal, LOG_CORE_EVENTS, LogModule_Models, "Config Validation", "Loading models from file \"%s\".", sModelPath);

	// Load model data file.
	FileToKeyValues(kvModels, sModelPath);

	// Try to find the first model
	KvRewind(kvModels);
	if (!KvGotoFirstSubKey(kvModels))
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Models, "Config Validation", "Can't find any models in \"%s\"", sModelPath);
	}

	// Initilizate chars
	char sPath[PLATFORM_MAX_PATH];
	char sArmPath[PLATFORM_MAX_PATH];
	char sAccess[NORMAL_LINE_LENGTH];
	char sGroup[NORMAL_LINE_LENGTH];

	gModelCount = 0;
	int iFailedCount;
	int iPublicCount;

	// Loop through all models and store attributes in model data array
	do
	{
		if (gModelCount > ModelsMax)
		{
			// Maximum number of models reached. Log a warning and exit the loop
			LogEvent(false, LogType_Normal, LOG_CORE_EVENTS, LogModule_Models, "Config Validation", "Maximum number of models reached (%i). Skipping other models.", ModelsMax + 1);

			break;
		}

		KvGetString(kvModels, "player", sPath, sizeof(sPath));
		strcopy(gModelData[gModelCount][Model_Path], PLATFORM_MAX_PATH, sPath);
		TrimString(sPath);

		KvGetString(kvModels, "arm", sArmPath, sizeof(sArmPath));
		strcopy(gModelData[gModelCount][Arm_Path], PLATFORM_MAX_PATH, sArmPath);
		TrimString(sArmPath);
		
		KvGetString(kvModels, "access", sAccess, sizeof(sAccess));
		gModelData[gModelCount][Model_Access] = ModelsStringToAccess(sAccess);

		KvGetString(kvModels, "group", sGroup, sizeof(sGroup));
		strcopy(gModelData[gModelCount][Model_Group], NORMAL_LINE_LENGTH, sGroup);
		
		/*  
		 * VALIDATE MODEL ATTRIBUTES
		 */

		// Validate player model
		if (!ModelsPrecache(sPath))
		{
			LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Models, "Config Validation", "Invalid model path setting at index %d. File not found: \"%s\".", gModelCount + iFailedCount, sPath);
			iFailedCount++;
			continue;
		}
		
		// Validate arm model
		if (!ModelsPrecache(sArmPath))
		{
			LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Models, "Config Validation", "Invalid model path setting at index %d. File not found: \"%s\".", gModelCount + iFailedCount, sArmPath);
			iFailedCount++;
			continue;
		}
		
		// Validate access
		if (gModelData[gModelCount][Model_Access] == ModelAccess_Invalid)
		{
			LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Models, "Config Validation", "Invalid model access setting at index %i: \"%s\".", gModelCount + iFailedCount, sAccess);
			iFailedCount++;
			continue;
		}
		else
		{
			// Increment public model counter for the current team
			if (gModelData[gModelCount][Model_Access] == ModelAccess_Public)
			{
				iPublicCount++;
			}
		}
		
		// Validate group
		if (gModelData[gModelCount][Model_Access] == ModelAccess_Group && FindAdmGroup(sGroup) == INVALID_GROUP_ID)
		{
			LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Models, "Config Validation", "Invalid model group setting at index %i. Couldn't find SourceMod group \"%s\".", gModelCount + iFailedCount, sGroup);
			iFailedCount++;
			continue;
		}
		
		// Check if model files were found.
		gModelCount++;
	}
	while (KvGotoNextKey(kvModels));

	CloseHandle(kvModels);

	// Check if there are no public models
	if (!iPublicCount)
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Models, "Config Validation", "Missing public model in \"%s\". There must be at least one public model.", sModelPath);
	}

	// Log model validation info
	LogEvent(true, LogType_Normal, LOG_CORE_EVENTS, LogModule_Models, "Config Validation", "Successful: %d | Unsuccessful: %d", gModelCount, iFailedCount);

	// Set config data
	ConfigSetConfigLoaded(File_Models, true);
	ConfigSetConfigReloadFunc(File_Models, GetFunctionByName(GetMyHandle(), "ModelsOnConfigReload"));
	
	// Precache other models
	PrecacheModelsCacheData();
}

/**
 * Called when config is being reloaded.
 **/
public void ModelsOnConfigReload(ConfigFile iConfig)
{
    // Reload models config
    ModelsLoad();
}

/**
 * Precache other models.
 **/
void PrecacheModelsCacheData(/*void*/)
{
	// Invisible knife model
	gWorldKnife = WeaponPrecache("models/weapons/caleon1/screwdriver/w_claw.mdl");

	//*********************************************************************
	//*               PRECACHE OF LEVEL OVERLAYS FILES            		  *
	//*********************************************************************
	
	// Initialize char
	char sPath[PLATFORM_MAX_PATH];
	char sCvar[PLATFORM_MAX_PATH];
	
	// If the level system enabled
	if(GetConVarBool(gCvarList[CVAR_LVL_SYSTEM]))
	{
		// Precache of the overlays
		for (int i = 1; i <= 10; i++)
		{
			// Initialize number string
			char sNum[4];
			IntToString(i, sNum, sizeof(sNum));
			
			// Load level overlay
			GetConVarString(gCvarList[CVAR_HUD_LEVELS], sCvar, sizeof(sCvar));
			
			// Replace number symbol @ to current numner
			ReplaceString(sCvar, sizeof(sCvar), "@", sNum);
			
			Format(sPath, sizeof(sPath), "materials/%s.vmt", sCvar);
			AddFileToDownloadsTable(sPath);
			
			// Load level overlay
			GetConVarString(gCvarList[CVAR_HUD_LEVELS], sCvar, sizeof(sCvar));
			
			// Replace number symbol @ to current numner
			ReplaceString(sCvar, sizeof(sCvar), "@", sNum);
			
			Format(sPath, sizeof(sPath), "materials/%s.vtf", sCvar);
			AddFileToDownloadsTable(sPath);
			PrecacheDecal(sPath);
		}
	}

	//*********************************************************************
	//*               PRECACHE OF WIN OVERLAYS FILES            		  *
	//*********************************************************************
	
	// Load zombie win overlay
	GetConVarString(gCvarList[CVAR_HUD_ZOMBIE_WIN], sCvar, sizeof(sCvar));
	
	Format(sPath, sizeof(sPath), "materials/%s.vmt", sCvar);
	AddFileToDownloadsTable(sPath);
	
	// Load zombie win overlay
	GetConVarString(gCvarList[CVAR_HUD_ZOMBIE_WIN], sCvar, sizeof(sCvar));
	
	Format(sPath, sizeof(sPath), "materials/%s.vtf", sCvar);
	AddFileToDownloadsTable(sPath);
	PrecacheDecal(sPath);

	// Load human win overlay
	GetConVarString(gCvarList[CVAR_HUD_HUMAN_WIN], sCvar, sizeof(sCvar));
	
	// Precache decal
	Format(sPath, sizeof(sPath), "materials/%s.vmt", sCvar);
	AddFileToDownloadsTable(sPath);
	
	// Load human win overlay
	GetConVarString(gCvarList[CVAR_HUD_HUMAN_WIN], sCvar, sizeof(sCvar));

	// Precache decal
	Format(sPath, sizeof(sPath), "materials/%s.vtf", sCvar);
	AddFileToDownloadsTable(sPath);
	PrecacheDecal(sPath);
	
	//*********************************************************************
	//*               PRECACHE OF NEMESIS MODELS FILES            		  *
	//*********************************************************************
	
	// Load nemesis player model
	GetConVarString(gCvarList[CVAR_NEMESIS_PLAYER_MODEL], sPath, sizeof(sPath)); 
	
	// Validate player model
	if(!ModelsPrecache(sPath))
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Models, "Config Validation", "Invalid nemesis model path. File not found: \"%s\".", sPath);
	}
	
	// If custom weapons models enabled
	if(GetConVarBool(gCvarList[CVAR_CUSTOM_MODELS]))
	{
		// Load nemesis knife model
		GetConVarString(gCvarList[CVAR_NEMESIS_KNIFE_MODEL], sPath, sizeof(sPath)); 
		gNemesisKnife = WeaponPrecache(sPath);
	}

	//*********************************************************************
	//*               PRECACHE OF SURVIVOR MODELS FILES            		  *
	//*********************************************************************
	
	// Load survivor player model
	GetConVarString(gCvarList[CVAR_SURVIVOR_PLAYER_MODEL], sPath, sizeof(sPath)); 
	
	// Validate player model
	if(!ModelsPrecache(sPath))
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Models, "Config Validation", "Invalid survivor model path. File not found: \"%s\".", sPath);
	}
	
	// If custom weapons models enabled
	if(GetConVarBool(gCvarList[CVAR_CUSTOM_MODELS]))
	{
		// Load survivor machinegun models
		GetConVarString(gCvarList[CVAR_SURVIVOR_M249_V_MODEL], sPath, sizeof(sPath)); 
		gSurvivorMach[ModelType_View] = WeaponPrecache(sPath);
		GetConVarString(gCvarList[CVAR_SURVIVOR_M249_W_MODEL], sPath, sizeof(sPath)); 
		gSurvivorMach[ModelType_World] = WeaponPrecache(sPath);
		
		// Load survivor pistol models
		GetConVarString(gCvarList[CVAR_SURVIVOR_PISTOL_V_MODEL], sPath, sizeof(sPath));
		gSurvivorPist[ModelType_View] = WeaponPrecache(sPath);
		GetConVarString(gCvarList[CVAR_SURVIVOR_PISTOL_W_MODEL], sPath, sizeof(sPath)); 
		gSurvivorPist[ModelType_World] = WeaponPrecache(sPath);
	}
}

/**
 * Changes the model on a player.
 *
 * @param clientIndex	The client index.
 * @return				True on success, false otherwise.
 **/
void ModelsOnClientSpawn(int clientIndex)
{
	// Initialize model chars
	char sModel[PLATFORM_MAX_PATH];
	char sArm[PLATFORM_MAX_PATH];

	// Get model based on filter settings set up earlier
	int iModel = ModelsGetRandomModel(clientIndex);
	
	// Check if din't found
	if (iModel == -1)
	{
		return;
	}
	
	// Get model path
	ModelsGetPlayerPath(iModel, sModel, sizeof(sModel));
	ModelsGetArmPath(iModel, sArm, sizeof(sArm));

	// Set models
	SetEntityModel(clientIndex, sModel);
	SetEntPropString(clientIndex, Prop_Send, "m_szArmsModel", sArm);
}

/**
 * Returns a random model index according to the specified filter settings.
 *
 * @param clientIndex	Optional. Client index used to check for
 *						permissions in "group" or "admins" access mode.
 *							NOTE: Use negative index to disable permission check. (default).
 *
 * @return				Random model index according to filter, or -1 on error.
 **/
int ModelsGetRandomModel(int clientIndex)
{
	// Initialize some variables
	int iListCount[ModelAccess];
	int iModelIndex[ModelsMax][ModelAccess];
	
	// Loop through all models
	for (int i = 0; i < gModelCount; i++)
	{
		// Cache current model access flag
		ModelAccess iAccess = ModelsGetAccess(i);

		// Check authentication
		switch (iAccess)
		{
			// Check if current model use group authentication
			case ModelAccess_Group :
			{
				// Get model group
				char sGroup[NORMAL_LINE_LENGTH];
				ModelsGetGroup(i, sGroup, sizeof(sGroup));

				if (!IsClientInGroup(clientIndex, sGroup))
				{
					// Client not authorized to use this model
					continue;
				}
				
				// Model passed filter tests. Add to list
				iModelIndex[iListCount[ModelAccess_Group]][ModelAccess_Group] = i;
				iListCount[ModelAccess_Group]++;
			}
			// Check if current model use admin authentication
			case ModelAccess_Admins :
			{
				if (!IsClientPrivileged(clientIndex))
				{
					// Client not authorized to use this model
					continue;
				}
				
				// Model passed filter tests. Add to list
				iModelIndex[iListCount[ModelAccess_Admins]][ModelAccess_Admins] = i;
				iListCount[ModelAccess_Admins]++;
			}
			// Check if current model use Vip authentication
			case ModelAccess_Vips :
			{
				if (!IsClientPrivileged(clientIndex, OperationType_VIP))
				{
					// Client not authorized to use this model
					continue;
				}
				
				// Model passed filter tests. Add to list
				iModelIndex[iListCount[ModelAccess_Vips]][ModelAccess_Vips] = i;
				iListCount[ModelAccess_Vips]++;
			}
			// Check if current model use normal authentication
			case ModelAccess_Public :
			{
				// Model passed filter tests. Add to list
				iModelIndex[iListCount[ModelAccess_Public]][ModelAccess_Public] = i;
				iListCount[ModelAccess_Public]++;
			}
		}
	}

	// Check if any models passed the filter
	if(iListCount[ModelAccess_Group])
	{
		return iModelIndex[Math_GetRandomInt(0, iListCount[ModelAccess_Group] - 1)][ModelAccess_Group];
	}
	else if(iListCount[ModelAccess_Admins])
	{
		return iModelIndex[Math_GetRandomInt(0, iListCount[ModelAccess_Admins] - 1)][ModelAccess_Admins];
	}
	else if(iListCount[ModelAccess_Vips])
	{
		return iModelIndex[Math_GetRandomInt(0, iListCount[ModelAccess_Vips] - 1)][ModelAccess_Vips];
	}
	else if(iListCount[ModelAccess_Public])
	{
		return iModelIndex[Math_GetRandomInt(0, iListCount[ModelAccess_Public] - 1)][ModelAccess_Public];
	}
	
	// If model didn't find
	return -1;
}

/**
 * Validates the specified index according to maximum number of models, and
 * number of models in use. Unused indexes will fail validation by default.
 *
 * @param index			Model index to validate.
 * @param rangeOnly		Optional. Do not check if the index is in use. Default
 *                      is false, check if in use.
 * @return              True if valid, false otherwise.
 **/
bool ModelsIsValidIndex(int iD, bool rangeOnly = false)
{
    bool rangeValid = (iD >= 0 && iD < ModelsMax);
    
    if (rangeOnly)
    {
        // Only check if the index is valid
        return rangeValid;
    }
    else
    {
        // Check if the index is valid, and if it's in use
        return rangeValid && (iD < gModelCount);
    }
}

/**
 * Gets the full player model file path for the specified model.
 *
 * @param iD		The model index.
 * @param sName		The string to return name in.
 * @param iMaxLen	The max length of the string.
 **/
stock void ModelsGetPlayerPath(int iD, char[] sName, int iMaxLen)
{
	strcopy(sName, iMaxLen, gModelData[iD][Model_Path]);
}

/**
 * Gets the full arm model file path for the specified model.
 *
 * @param iD		The model index.
 * @param sName		The string to return name in.
 * @param iMaxLen	The max length of the string.
 **/
stock void ModelsGetArmPath(int iD, char[] sName, int iMaxLen)
{
	strcopy(sName, iMaxLen, gModelData[iD][Arm_Path]);
}

/**
 * Gets the access setting for the specified model.
 *
 * @param iD	Model index.
 * @return		Access setting for the specified model, ModelAccess_Invalid on error.
 **/
ModelAccess ModelsGetAccess(int iD)
{
    // Validate index
    if (!ModelsIsValidIndex(iD))
    {
        return ModelAccess_Invalid;
    }
    
    return gModelData[iD][Model_Access];
}

/**
 * Gets the group for the specified model.
 *
 * @param iD		Model index.
 * @param sBuffer	Destination string buffer.
 * @param iMaxLen	Size of buffer.
 * @return          Number of cells written, or -1 on error.
 **/
int ModelsGetGroup(int iD, char[] sBuffer, int iMaxLen)
{
    // Validate index
    if (!ModelsIsValidIndex(iD))
    {
        return -1;
    }
    
    return strcopy(sBuffer, iMaxLen, gModelData[iD][Model_Group]);
}

/**
 * Converts the specified string to a access setting.
 *
 * @param sAccess	String to convert.
 * @return			Access setting, or ModelAccess_Invalid on error.
 **/
ModelAccess ModelsStringToAccess(const char[] sAccess)
{
    if (StrEqual(sAccess, "public", false))
    {
        return ModelAccess_Public;
    }
    else if (StrEqual(sAccess, "admins", false))
    {
        return ModelAccess_Admins;
    }
    else if (StrEqual(sAccess, "vips", false))
    {
        return ModelAccess_Vips;
    }
    else if (StrEqual(sAccess, "group", false))
    {
        return ModelAccess_Group;
    }
    
    return ModelAccess_Invalid;
}

/**
 * Precache file and add it to download table.
 *
 * @param sPath    The path to file.
 **/
void AddFileToPrecacheList(const char[] sPath)
{
	// Add to downloads
	AddFileToDownloadsTable(sPath);
	
	// Add to precache if path contains '.mdl'
	if(StrContains(sPath, ".mdl", false) != -1)
	{
		PrecacheModel(sPath, true);
	}
}

/**
 * Precache models and return model index.
 *   	NOTE: Precache with hiding models include.
 *
 * @param sModel	The model path. 
 **/
stock bool ModelsPrecache(const char[] sModel)
{
	// If model didn't exist
	if(!FileExists(sModel))
	{
		// Try to find model in game folder by name
		return ModelsIsStandart(sModel);
	}

	// Search for model files with the specified name and add them to downloads table
	PrecacheDirFromMainFile(sModel);
	return true;
}

/**
 * Precache weapon models and return model index.
 *
 * @param sModel	The model path. 
 * @return			Returns the model index.
 **/
stock int WeaponPrecache(const char[] sModel)
{
	// If model didn't exist
	if(!FileExists(sModel))
	{
		// Return error
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Models, "Config Validation", "Invalid model path. File not found: \"%s\".", sModel);
	}
	
	// Convert model to index
	int iModelIndex = PrecacheModel(sModel);

	// Search for model files with the specified name and add them to downloads table
	PrecacheDirFromMainFile(sModel);
	
	// Return model index
	return iModelIndex;
}

/**
 * Reads and precache the current directory from a local filename.
 *
 * @param sModel	The model path.
 **/
void PrecacheDirFromMainFile(const char[] sModel)
{
	// Automatic precache of player models
	int iLenth = FindCharInString(sModel, '/', true);
	
	// The index of the first occurrence of the character in the string
	if(iLenth != -1)
	{
		// Copy one string onto another
		char sPath[PLATFORM_MAX_PATH];
		strcopy(sPath, sizeof(sPath), sModel);
		
		// Cut the string after last slash
		sPath[iLenth] = '\0';
		
		// Open directory of file
		DirectoryListing sDirectory = OpenDirectory(sPath);
		
		// If doesn't exist stop
		if(sDirectory == NULL)
		{
			LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Models, "Config Validation", "Error opening directory: %s", sPath);
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
				Format(sLine, sizeof(sLine), "%s/%s", sPath, sFile);
				
				// Add to server precache list
				AddFileToPrecacheList(sLine);
			}
		}
	
		// Close directory
		CloseHandle(sDirectory); 
	}
}

/**
 * Validates the specified standart models from game folder.
 *	NOTE: Method do not guarantees to recognise all standart models.
 *
 * @param sModel	The model path for validation.
 * @return			True if standart model, false otherwise.
 **/
bool ModelsIsStandart(const char[] sModel)
{
	// Copy one string onto another
	char sPath[PLATFORM_MAX_PATH];
	strcopy(sPath, sizeof(sPath), sModel);
	
	//*********************************************************************
	//*                VALIDATE STANDART MODELS FILES            		  *
	//*********************************************************************
	if(!strncmp(sPath, "models/player/", 14))
	{
		// Given a string, replaces all occurrences of a search string with a replacement string
		ReplaceString(sPath, PLATFORM_MAX_PATH, "models/player/", "");
		
		// If path contains standart path
		if(!strncmp(sPath, "custom_player/legacy/", 21))
		{
			// Given a string, replaces all occurrences of a search string with a replacement string
			ReplaceString(sPath, PLATFORM_MAX_PATH, "custom_player/legacy/", "");
			
			// If path contains standart path
			if(!strncmp(sPath, "ctm_", 4) || !strncmp(sPath, "tm_", 3))
			{
				// Precache models
				if(!IsModelPrecached(sModel)) PrecacheModel(sModel);
				return true;
			}
		}
		else
		{
			// If path contains standart path
			if(!strncmp(sPath, "ctm_", 4) || !strncmp(sPath, "tm_", 3))
			{
				// Precache models
				if(!IsModelPrecached(sModel)) PrecacheModel(sModel);
				return true;
			}
		}
	}
	//*********************************************************************
	//*               VALIDATE STANDART ARM MODELS FILES            	  *
	//*********************************************************************
	else if(!strncmp(sPath, "models/weapons/", 15))
	{
		// Given a string, replaces all occurrences of a search string with a replacement string
		ReplaceString(sPath, PLATFORM_MAX_PATH, "models/weapons/", "");
		
		// If path contains standart path
		if(!strncmp(sPath, "ct_arms_", 8) || !strncmp(sPath, "t_arms_", 7))
		{
			// Precache models
			if(!IsModelPrecached(sModel)) PrecacheModel(sModel);
			return true;
		}
			
	}
	
	// Model didn't exist
	return false;
}