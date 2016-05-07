/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          zombieclases.cpp
 *  Type:          API 
 *  Description:   Native handlers for the ZP API.
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
 * Number of max valid zombie classes.
 **/
#define ZombieClassMax 32

/**
 * List of operation types for zombie classes data.
 **/
enum ZombieClassData
{
	String:gZombieClassName[NORMAL_LINE_LENGTH],
	String:gZombieClassModel[PLATFORM_MAX_PATH],
	String:gZombieClassClaw[PLATFORM_MAX_PATH],
	gZombieClassHealth,
	Float:gZombieClassSpeed,
	Float:gZombieClassGravity,
	gZombieClassFemale,
	gZombieClassVip,
	gZombieClassDuration,
	gZombieClassCountDown,
	gZombieClassClawID
};

/**
 * Arrays to store the server's zombie classes.
 **/
int   gZombieClassNum;
int   gZombieClassData[ZombieClassMax][ZombieClassData];

/**
 * Called, when initialization of zombie classes is done. 
 **/
void ZombieClassesLoad(/*void*/)
{
	// No zombie classes?
	if(!gZombieClassNum)
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Zombieclasses, "ZombieClass Validation", "No zombie classes loaded");
		return;
	}
	
	// Initialize char
	char sModel[BIG_LINE_LENGTH];
	
	// Precache of the zombie classes
	for(int i = 0; i < gZombieClassNum; i++)
	{
		// Validate player model
		ZombieGetModel(i, sModel, sizeof(sModel));
		if(!ModelsPrecache(sModel))
		{
			LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Zombieclasses, "Model Validation", "Invalid model path. File not found: \"%s\".", sModel);
		}
		
		// If custom weapons models enabled
		if(GetConVarBool(gCvarList[CVAR_CUSTOM_MODELS]))
		{
			// Validate knife model
			ZombieGetClawModel(i, sModel, sizeof(sModel));
			ZombieSetClawIndex(i, WeaponPrecache(sModel));
		}
	}
}

/**
 * Load zombie class from other plugin.
 *
 * native int ZP_RegisterZombieClass(name, model, claw_model, health, speed, gravity, female, vip, duration, countdown)
 **/
public int APIRegisterZombieClass(Handle isPlugin, int iNumParams)
{
	// Retrieves the string length from a native parameter string
	int iLenth;
	GetNativeStringLength(1, iLenth);
	
	// Strings are empty ?
	if (iLenth <= 0)
	{
		LogEvent(false, LogType_Native, LOG_CORE_EVENTS, LogModule_Zombieclasses, "Native Validation", "Can't register zombie class with an empty name");
		return -1;
	}
	
	// Maximum amout of zombie classes
	if(gZombieClassNum >= ZombieClassMax)
	{
		LogEvent(false, LogType_Normal, LOG_CORE_EVENTS, LogModule_Zombieclasses, "ZombieClass Validation", "Maximum number of zombie classes reached (%d). Skipping other classes.", ZombieClassMax);
		return -1;
	}
	
	// Get main values from native
	GetNativeString(1, gZombieClassData[gZombieClassNum][gZombieClassName], 	SMALL_LINE_LENGTH);
	GetNativeString(2, gZombieClassData[gZombieClassNum][gZombieClassModel], 	BIG_LINE_LENGTH);
	GetNativeString(3, gZombieClassData[gZombieClassNum][gZombieClassClaw], 	BIG_LINE_LENGTH);
	
	gZombieClassData[gZombieClassNum][gZombieClassHealth]   	= view_as<int>(GetNativeCell(4));
	
	gZombieClassData[gZombieClassNum][gZombieClassSpeed] 	 	= view_as<float>(GetNativeCell(5));
	gZombieClassData[gZombieClassNum][gZombieClassGravity] 	 	= view_as<float>(GetNativeCell(6));
	
	gZombieClassData[gZombieClassNum][gZombieClassFemale]   	= view_as<int>(GetNativeCell(7));
	gZombieClassData[gZombieClassNum][gZombieClassVip] 	  		= view_as<int>(GetNativeCell(8));
	gZombieClassData[gZombieClassNum][gZombieClassDuration]   	= view_as<int>(GetNativeCell(9));
	gZombieClassData[gZombieClassNum][gZombieClassCountDown]  	= view_as<int>(GetNativeCell(10));
	
	// Removes whitespace characters from the beginning and end of a string
	TrimString(gZombieClassData[gZombieClassNum][gZombieClassName]);
	TrimString(gZombieClassData[gZombieClassNum][gZombieClassModel]);
	TrimString(gZombieClassData[gZombieClassNum][gZombieClassClaw]);

	// Add loaded zombie class
	gZombieClassNum++;

	// Return id under which we registered the class
	return gZombieClassNum-1;
}

/**
 * Zombie classes data reading API.
 **/

/**
 * Gets the name of a zombie class at a given index.
 *
 * @param iD     	 The class index.
 * @param sName      The string to return name in.
 * @param iMaxLen    The max length of the string.
 **/
stock void ZombieGetName(int iD, char[] sName, int iMaxLen)
{
    // Get class name
	strcopy(sName, iMaxLen, gZombieClassData[iD][gZombieClassName]);
}

/**
 * Gets the player model of a zombie class at a given index.
 *
 * @param iD     	 The class index.
 * @param sName      The string to return name in.
 * @param iMaxLen    The max length of the string.
 **/
stock void ZombieGetModel(int iD, char[] sName, int iMaxLen)
{
    // Get model name
	strcopy(sName, iMaxLen, gZombieClassData[iD][gZombieClassModel]);
}

/**
 * Gets the knife model of a zombie class at a given index.
 *
 * @param iD     	 The class index.
 * @param sName      The string to return name in.
 * @param iMaxLen    The max length of the string.
 **/
stock void ZombieGetClawModel(int iD, char[] sName, int iMaxLen)
{
    // Get knife model name
	strcopy(sName, iMaxLen, gZombieClassData[iD][gZombieClassClaw]);
}

/**
 * Gets the health of the zombie class.
 *
 * @param iD        The class index.
 * @return          The health amount.	
 **/
stock int ZombieGetHealth(int iD)
{
    // Return default restriction status
    return gZombieClassData[iD][gZombieClassHealth];
}

/**
 * Gets the speed of the zombie class.
 *
 * @param iD        The class index.
 * @return          The speed amount.	
 **/
stock float ZombieGetSpeed(int iD)
{
    // Return default restriction status
    return gZombieClassData[iD][gZombieClassSpeed];
}

/**
 * Gets the gravity of the zombie class.
 *
 * @param iD        The class index.
 * @return          The gravity amount.	
 **/
stock float ZombieGetGravity(int iD)
{
    // Return default restriction status
    return gZombieClassData[iD][gZombieClassGravity];
}

/**
 * Check the gender of the zombie class.
 *
 * @param iD        The class index.
 * @return          Returns true if female, false it not.
 **/
stock bool ZombieIsFemale(int iD)
{
    // Return default restriction status
    return gZombieClassData[iD][gZombieClassFemale] ? true : false;
}

/**
 * Check the access to the zombie class.
 *
 * @param iD        The class index.
 * @return          Returns true if vip access, false it not.
 **/
stock bool ZombiesIsVIP(int iD)
{
    // Return default restriction status
    return gZombieClassData[iD][gZombieClassVip] ? true : false;
}

/**
 * Gets the skill duration of the zombie class.
 *
 * @param iD        The class index.
 * @return          The duration amount.	
 **/
stock int ZombieGetSkillDuration(int iD)
{
    // Return default restriction status
    return gZombieClassData[iD][gZombieClassDuration];
}

/**
 * Gets the skill countdown of the zombie class.
 *
 * @param iD        The class index.
 * @return          The countdown amount.	
 **/
stock int ZombieGetSkillCountDown(int iD)
{
    // Return default restriction status
    return gZombieClassData[iD][gZombieClassCountDown];
}

/**
 * Gets the index of the zombie class claw model.
 *
 * @param iD        The class index.
 * @return          The model index.	
 **/
stock int ZombieGetClawIndex(int iD)
{
    // Return default restriction status
    return gZombieClassData[iD][gZombieClassClawID];
}

/**
 * Sets the index of the zombie class claw model.
 *
 * @param iD          The class index.
 * @param iModelIndex The model index.	
 **/
stock void ZombieSetClawIndex(int iD, int iModelIndex)
{
    // Set knife model index
    gZombieClassData[iD][gZombieClassClawID] = iModelIndex;
}