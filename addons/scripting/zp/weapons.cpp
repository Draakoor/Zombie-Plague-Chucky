/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          weapons.cpp
 *  Type:          Core 
 *  Description:   All weapon-related functions.
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
 * @section CS:GO start weapons.
 **/
#define WEAPONS_SPAWN_WEAPON  "weapon_glock"
 
/**
 * Number of REAL weapon slots (For CS:GO)
 **/
#define WEAPONS_SLOTS_MAX 	5

/**
 * Maximum length of a weapon name string
 **/
#define WEAPONS_MAX_LENGTH 32
 
/**
 * Max amount of ammo for weapon.
 **/
int CS_WeaponMaxAmmo[56] = { -1,20,120,90,-1,32,-1,100,90,-1,120,100,100,90,90,90,52,30,120,200,32,40,120,90,-1,35,90,90,-1,100,-1,-1,-1,-1,90,120,32,200,32,120,-1,52,120,120,32,12,90,90,90,90,-1,-1,-1,-1,-1 };

/**
 * Array handle to store weapon config data.
 **/
ArrayList arrayWeapons;

/**
 * Weapon config data indexes.
 **/
enum
{
    WEAPONS_DATA_NAME = 0,
	WEAPONS_DATA_ENTITY,
    WEAPONS_DATA_AMMOPRICE,
    WEAPONS_DATA_SLOT,
    WEAPONS_DATA_LEVEL,
    WEAPONS_DATA_ONLINE,
    WEAPONS_DATA_KNOCKBACK
}

/**
 * Number of valid player slots.
 **/
enum
{ 
	WEAPON_SLOT_INVALID = -1, 
	WEAPON_SLOT_PRIMARY, 
	WEAPON_SLOT_SECONDARY, 
	WEAPON_SLOT_MELEE, 
	WEAPON_SLOT_EQUEPMENT
};

/**
 * Prepare all weapon data.
 **/
void WeaponsLoad(/*void*/)
{
	// Register config file
	ConfigRegisterConfig(File_Weapons, Structure_Keyvalue, CONFIG_FILE_ALIAS_WEAPONS);

	// Get weapons config path
	char sPathWeapons[PLATFORM_MAX_PATH];
	bool bExists = ConfigGetCvarFilePath(CVAR_CONFIG_PATH_WEAPONS, sPathWeapons);

	// If file doesn't exist, then log and stop
	if (!bExists)
	{
		// Log failure
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Weapons, "Config Validation", "Missing weapons config file: %s", sPathWeapons);

		return;
	}

	// Set the path to the config file
	ConfigSetConfigPath(File_Weapons, sPathWeapons);

	// Load config from file and create array structure
	bool bSuccess = ConfigLoadConfig(File_Weapons, arrayWeapons);

	// Unexpected error, stop plugin
	if (!bSuccess)
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Weapons, "Config Validation", "Unexpected error encountered loading: %s", sPathWeapons);
	}

	// Validate weapons config
	int iSize = GetArraySize(arrayWeapons);
	if (!iSize)
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Weapons, "Config Validation", "No usable data found in weapons config file: %s", sPathWeapons);
	}

	// Now copy data to array structure
	WeaponsCacheData();

	// Set config data
	ConfigSetConfigLoaded(File_Weapons, true);
	ConfigSetConfigReloadFunc(File_Weapons, GetFunctionByName(GetMyHandle(), "WeaponsOnConfigReload"));
	ConfigSetConfigHandle(File_Weapons, arrayWeapons);
}

/**
 * Caches weapon data from file into arrays.
 * Make sure the file is loaded before (ConfigLoadConfig) to prep array structure.
 **/
void WeaponsCacheData(/*void*/)
{
	// Get config's file path.
	char sPathWeapons[PLATFORM_MAX_PATH];
	ConfigGetConfigPath(File_Weapons, sPathWeapons, sizeof(sPathWeapons));

	Handle kvWeapons;
	bool bSuccess = ConfigOpenConfigFile(File_Weapons, kvWeapons);

	if (!bSuccess)
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Weapons, "Config Validation", "Unexpected error caching data from weapons config file: %s", sPathWeapons);
	}

	char sWeaponName[WEAPONS_MAX_LENGTH];

	// i = array index
	int iSize = GetArraySize(arrayWeapons);
	for (int i = 0; i < iSize; i++)
	{
		WeaponsGetName(i, sWeaponName, sizeof(sWeaponName));
		KvRewind(kvWeapons);
		if (!KvJumpToKey(kvWeapons, sWeaponName))
		{
			LogEvent(false, LogType_Error, LOG_CORE_EVENTS, LogModule_Weapons, "Config Validation", "Couldn't cache weapon data for: %s (check weapons config)", sWeaponName);
			continue;
		}

		// Get config data
		char sWeaponEntity[CONFIG_MAX_LENGTH];

		// General
		KvGetString(kvWeapons, "weaponentity", sWeaponEntity, sizeof(sWeaponEntity));
		int iWeaponCost 	= KvGetNum(kvWeapons, 	"weaponcost", 	0);
		int iWeaponSlot		= KvGetNum(kvWeapons, 	"weaponslot", 	0);
		int iWeaponLevel 	= KvGetNum(kvWeapons, 	"weaponlvl", 	0);
		int iWeaponOnline  	= KvGetNum(kvWeapons, 	"weapononline", 0);
		float flWeaponKnock = KvGetFloat(kvWeapons, "weaponknock", 	1.0);
		
		// Get array size
		Handle arrayWeapon = GetArrayCell(arrayWeapons, i);

		// Push data into array
		PushArrayString(arrayWeapon, sWeaponEntity);         // Index: 1
		PushArrayCell(arrayWeapon, 	 iWeaponCost);           // Index: 2
		PushArrayCell(arrayWeapon,   iWeaponSlot);       	 // Index: 3
		PushArrayCell(arrayWeapon,   iWeaponLevel);          // Index: 4
		PushArrayCell(arrayWeapon,   iWeaponOnline);         // Index: 5
		PushArrayCell(arrayWeapon,   flWeaponKnock);         // Index: 6
		
		// Gets a weaponID from a alias
		WeaponsGetAlias(sWeaponEntity, sizeof(sWeaponEntity));

		// If weapon alias invalid, then remove, log, and stop
		if(CS_AliasToWeaponID(sWeaponEntity) == CSWeapon_NONE)
		{
			// Remove weapon from array
			RemoveFromArray(arrayWeapons, i);

			// Subtract one from count
			iSize--;

			// Backtrack one index, because we deleted it out from under the loop
			i--;

			LogEvent(false, LogType_Error, LOG_CORE_EVENTS, LogModule_Weapons, "Config Validation", "Invalid weapon alias \"%s\"", sWeaponEntity);
			continue;
		}
	}

	// We're done with this file now, so we can close it
	CloseHandle(kvWeapons);
}

/**
 * Called when config is being reloaded.
 **/
public void WeaponsOnConfigReload(/*void*/)
{
    // Reload weapons config
    WeaponsLoad();
}

/**
 * Weapon data reading API.
 **/
 
/**
 * Clear cache for a given weapon.
 * 
 * @param iD	The weapon index.
 **/
stock void WeaponsClearCache(int iD)
{
    // Get array handle of weapon at given index
    Handle hWeapon = GetArrayCell(arrayWeapons, iD);
    
    // Clear array
    ClearArray(hWeapon);
}

/**
 * Find the index at which the weapon's name is at.
 * 
 * @param sAlias	The weapon alias.
 * @return			The array index containing the given weapon name.
 **/
stock int WeaponsEntityToIndex(char[] sAlias)
{
	// Initialize chars 
	char sWeapon[WEAPONS_MAX_LENGTH];
	char sClassname[WEAPONS_MAX_LENGTH];

	// Convert alias to weapon name
	if(!StrContains(sAlias, "weapon_")) Format(sWeapon, sizeof(sWeapon), "weapon_%s ", sAlias);

	// i = Array index
	int iSize = GetArraySize(arrayWeapons);
	for (int i = 0; i < iSize; i++)
	{
		WeaponsGetEntity(i, sClassname, sizeof(sClassname));

		// If names match, then return index
		if (StrEqual(sWeapon, sClassname, false))
		{
			return i;
		}
	}

	// Name doesn't exist
	return -1;
}

/**
 * Takes a weapon's entity name and returns the display name.
 *
 * @param sClassname	The entity to find the display of.
 * @param iMaxLen	The max length of the string.
 **/
stock void WeaponsEntityToDisplay(char[] sClassname, int iMaxLen)
{
	// Change entity to name
	sClassname[7] = CharToUpper(sClassname[7]);
	WeaponsGetAlias(sClassname, iMaxLen);
}

/**
 * Takes a weapon's entity name and returns weapon alias.
 *
 * @param sClassname	The string to return name in.
 * @param iMaxLen	The max length of the string.
 **/
stock void WeaponsGetAlias(char[] sClassname, int iMaxLen)
{
	ReplaceString(sClassname, iMaxLen, "weapon_", "");
}

/**
 * Gets the name of a weapon at a given index.
 *
 * @param iD		The weapon index.
 * @param sClassname	The string to return name in.
 * @param iMaxLen	The max length of the string.
 **/
stock void WeaponsGetName(int iD, char[] sClassname, int iMaxLen)
{
    // Get array handle of weapon at given index
    Handle arrayWeapon = GetArrayCell(arrayWeapons, iD);
    
    // Get weapon name
    GetArrayString(arrayWeapon, WEAPONS_DATA_NAME, sClassname, iMaxLen);
}

/**
 * Gets the entity of a weapon at a given index.
 *
 * @param iD		The weapon index.
 * @param sType		The string to return entity in.
 * @param iMaxLen	The max length of the string.
 **/
stock void WeaponsGetEntity(int iD, char[] sType, int iMaxLen)
{
    // Get array handle of weapon at given index
    Handle arrayWeapon = GetArrayCell(arrayWeapons, iD);
    
    // Get weapon type
    GetArrayString(arrayWeapon, WEAPONS_DATA_ENTITY, sType, iMaxLen);
}

/**
 * Gets the price of ammo for the weapon.
 *
 * @param iD	The weapon index.
 * @return		The ammo price.	
 **/
stock int WeaponsGetCost(int iD)
{
    // Get array handle of weapon at given index
    Handle arrayWeapon = GetArrayCell(arrayWeapons, iD);
    
    // Return default restriction status
    return GetArrayCell(arrayWeapon, WEAPONS_DATA_AMMOPRICE);
}

/**
 * Gets the slot index of a weapon at a given index.
 *
 * @param iD	The weapon index.
 * @return		The slot index of the weapon.
 **/
stock int WeaponsGetSlot(int iD)
{
    // Get array handle of weapon at given index
    Handle arrayWeapon = GetArrayCell(arrayWeapons, iD);
    
    // Return default restriction status
    return GetArrayCell(arrayWeapon, WEAPONS_DATA_SLOT);
}

/**
 * Gets the level of the weapon.
 *
 * @param iD	The weapon index.
 * @return		The level of the weapon.
 **/
stock int WeaponsGetLevel(int iD)
{
    // Get array handle of weapon at given index
    Handle arrayWeapon = GetArrayCell(arrayWeapons, iD);
    
    // Return default restriction status
    return GetArrayCell(arrayWeapon, WEAPONS_DATA_LEVEL);
}

/**
 * Gets the online of the weapon.
 *
 * @param iD	The weapon index.
 * @return		The online of the weapon.
 **/
stock int WeaponsGetOnline(int iD)
{
    // Get array handle of weapon at given index
    Handle arrayWeapon = GetArrayCell(arrayWeapons, iD);
    
    // Return default restriction status
    return GetArrayCell(arrayWeapon, WEAPONS_DATA_ONLINE);
}

/**
 * Gets the knockback multiplier for the weapon.
 *
 * @param iD	The weapon index.
 * @return		The weapon knockback multiplier.
 **/
stock float WeaponsGetKnockBack(int iD)
{
    // Get array handle of weapon at given index
    Handle arrayWeapon = GetArrayCell(arrayWeapons, iD);
    
    // Return default restriction status
    return GetArrayCell(arrayWeapon, WEAPONS_DATA_KNOCKBACK);
}

/**
 * General weapon API.
 **/

/**
 * Remove all weapons and give knife.
 *
 * @param cBasePlayer	The client index.
 **/
void WeaponsRemoveAll(CBasePlayer* cBasePlayer)
{
	// Get a list of all client's weapon indexes
	int weaponIndex;

	// Loop through array slots and remove weapons
	for (int WeaponSlot = 0; WeaponSlot < WEAPONS_SLOTS_MAX; WeaponSlot++)
	{
		while ((weaponIndex = GetPlayerWeaponSlot(view_as<int>(cBasePlayer), WeaponSlot)) != WEAPON_SLOT_INVALID)
		{
			// Force server to delete weapon
			RemovePlayerItem(view_as<int>(cBasePlayer), weaponIndex);
			AcceptEntityInput(weaponIndex, "Kill");
		}
	}

	// Create a new knife
	cBasePlayer->CItemMaterialize("weapon_knife");
}

/**
 * Returns true if the player has a current weapon, false if not.
 *
 * @param clientIndex	The client index.
 *  
 * @return				True or false.
 **/
bool WeaponsHasEntity(int clientIndex, char[] sWeapon)
{
	// Initialize char
	char sClassname[SMALL_LINE_LENGTH];

	// Get amount of all client weapons
	int iSize = GetEntPropArraySize(clientIndex, Prop_Send, "m_hMyWeapons");

	// i = index of the weapon
	for (int i = 0; i < iSize; i++)
	{
		// Get weapon index
		int weaponIndex = GetEntPropEnt(clientIndex, Prop_Send, "m_hMyWeapons", i);
		
		// If entity isn't valid, then skip
		if (IsValidEdict(weaponIndex))
		{
			// Get weapon classname
			GetEdictClassname(weaponIndex, sClassname, sizeof(sClassname));
			
			// If was found
			if (StrEqual(sClassname, sWeapon, false))
			{
				return true;
			}
		}
		
		// Go to next weapon
		continue;
	}
	
	// If wasn't found
	return false;
} 
 
/* 
 * Thanks CareFully for improving my method :)
 */

/**
 * Hook: WeaponSwitchPost
 * Called, when player deploy any weapon.
 *
 * @param clientIndex	 The client index.
 * @param weaponIndex    The weapon index.
 **/
public void WeaponOnDeployPost(int clientIndex, int weaponIndex) 
{
	// If weapon isn't valid, then stop
	if(!IsValidEdict(weaponIndex))
	{
		return;
	}

	// Get real player index from event key 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);

	// Verify that the client is connected
	if(!IsPlayerExist(view_as<int>(cBasePlayer)))
	{
		return;
	}
	
	// If custom weapons models enabled
	if(GetConVarBool(gCvarList[CVAR_CUSTOM_MODELS]))
	{
		// Get weapon name
		char sClassname[NORMAL_LINE_LENGTH];
		GetEntityClassname(weaponIndex, sClassname, sizeof(sClassname));
		WeaponsGetAlias(sClassname, sizeof(sClassname));
		
		// Gets a weaponID from a alias
		switch(CS_AliasToWeaponID(sClassname))
		{
			// Zombie's knife models
			case CSWeapon_KNIFE :
			{
				if(cBasePlayer->m_bZombie)
				{
					SetViewModel(weaponIndex, cBasePlayer->m_nModelIndex, cBasePlayer->m_bNemesis ? gNemesisKnife : ZombieGetClawIndex(cBasePlayer->m_nZombieClass));
					SetWorldModel(weaponIndex, gWorldKnife);
					return;
				}
			}
			
			// Survivor's M249
			case CSWeapon_M249  :
			{
				if(cBasePlayer->m_bSurvivor)
				{
					SetViewModel(weaponIndex, cBasePlayer->m_nModelIndex, gSurvivorMach[ModelType_View]);
					SetWorldModel(weaponIndex, gSurvivorMach[ModelType_World]);
					return;
				}
			}
			
			// Survivor's Dual Elite
			case CSWeapon_ELITE :
			{
				if(cBasePlayer->m_bSurvivor)
				{
					SetViewModel(weaponIndex, cBasePlayer->m_nModelIndex, gSurvivorPist[ModelType_View]);
					SetWorldModel(weaponIndex, gSurvivorPist[ModelType_World]);
					return;
				}
			}
		}
	}
} 

/**
 * Hook: WeaponCanUse
 * Called, when player deploy or pick-up any weapon.
 *
 * @param clientIndex	 The client index.
 * @param weaponIndex    The weapon index.
 **/
public Action WeaponOnCanUse(int clientIndex, int weaponIndex)
{
	// If weapon isn't valid, then stop
	if(!IsValidEdict(weaponIndex))
	{
		return ACTION_HANDLED;
	}

	// Get real player index from event key 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);

	// Verify that the client is connected and alive
	if (!IsPlayerExist(view_as<int>(cBasePlayer)))
	{
		return ACTION_HANDLED;
	}

	// Initialize weapon classname
	char sClassname[SMALL_LINE_LENGTH];
	GetEdictClassname(weaponIndex, sClassname, sizeof(sClassname));

	// Allow pickup
	if (StrEqual(sClassname, "weapon_knife"))
	{
		return ACTION_CONTINUE;
	}

	// Block pickup
	if(cBasePlayer->m_bZombie || StrEqual(sClassname, "weapon_c4"))
	{
		return ACTION_HANDLED;
	}
	
	// Allow pickup
	if(IsFakeClient(view_as<int>(cBasePlayer)))
	{
		return ACTION_CONTINUE;
	}	
		
	// Weapons available for use ?
	int iIndex = WeaponsEntityToIndex(sClassname);
	if(iIndex != -1)
	{
		// If current level is less than weapon level, block pickup
		if(cBasePlayer->m_iLevel < WeaponsGetLevel(iIndex))
		{
			return ACTION_HANDLED;
		}
	}
	
	// Allow pickup
	return ACTION_CONTINUE;
}

/**
 * Called after dropping weapon.
 *
 * @param clientIndex	 The client index.
 * @param weaponIndex    The weapon index.
 **/
public Action WeaponOnDropPost(int clientIndex, int weaponIndex)
{
	// Get delay for removing weapon
	float flDeleteDelay = GetConVarFloat(gCvarList[CVAR_REMOVE_DROPPED]);
	
	// If time is equal to 0, then stop
	if(flDeleteDelay <= 0.0)
	{
		return;
	}
	
	// Create timer if weapon is valid
	if(IsValidEdict(weaponIndex)) 
	{
		CreateTimer(flDeleteDelay, EventRemoveDropedWeapon, weaponIndex, TIMER_FLAG_NO_MAPCHANGE);
	}
}

/**
 * Timer which remove dropped weapon, from a ground.
 *
 * @param hTimer     	 The timer handle.
 * @param weaponIndex	 The weapon index.
 **/
public Action EventRemoveDropedWeapon(Handle hTimer, any weaponIndex)
{
	// If weapon isn't valid, then stop
	if(!IsValidEdict(weaponIndex))
	{
		return ACTION_STOP;
	}

	// If weapon isn't have owner delete it
	if(GetEntPropEnt(weaponIndex, Prop_Data, "m_pParent") == -1)
	{
		RemoveEdict(weaponIndex);
	}
	
	// Destroy timer
	return ACTION_STOP;
}

/**
 * Useful stocks for weapon API.
 **/

/**
 * Get view model index.
 *
 * @param clientIndex    The client index.
 * @return 				 Returns the entity index.	
 **/
stock int WeaponGetViewModelIndex(int clientIndex)
{
	// Initialize vars
	int nIndex = -1;
	
	// Find entity and return index
	while ((nIndex = GetEntityByClassname(nIndex, "predicted_viewmodel")) != -1)
	{
		// Get owner of the entity
		int iOwner = GetEntPropEnt(nIndex, Prop_Data, "m_hOwner");

		// If owner isn't equal to client index, skip
		if (iOwner != clientIndex)
		{	
			continue;
		}
		
		// Return index of the entity
		return nIndex;
	}
	
	// If didn't find
	return -1;
}

/**
 * Stock for finding entity by classname.
 *
 * @param iStartEnt		The entity index.
 * @param sClassname	The entity classname.
 * @return				Returns the entity index.	
 **/
stock int GetEntityByClassname(int iStartEnt, char[] sClassname)
{
	// Skip some invalid entities
	while (iStartEnt > -1 && !IsValidEntity(iStartEnt)) iStartEnt--;
	
	// Index of an entity by classname
	return FindEntityByClassname(iStartEnt, sClassname);
}

/**
 * Sets the view weapon model.
 *
 * @param weaponIndex	The weapon index.
 * @param viewIndex		The view model index.
 * @param model			The model index. (Must be precached)
 **/
stock void SetViewModel(int weaponIndex, int viewIndex, int model)
{
	// Delete standart model index
	SetEntProp(weaponIndex, Prop_Send, "m_nModelIndex", 0);
	
	// Set view model for weapon
	SetEntProp(viewIndex, Prop_Send, "m_nModelIndex", model);
}

/**
 * Sets the attached world weapon model.
 *
 * @param weaponIndex	The weapon index.
 * @param model			The model index. (Must be precached)
 **/
stock void SetWorldModel(int weaponIndex, int model)
{
	// Get world model index
	int worldIndex = GetEntPropEnt(weaponIndex, Prop_Send, "m_hWeaponWorldModel");
	
	// Verify that the world weapon is valid
	if(!IsValidEdict(worldIndex))
	{
		return;
	}
	
	// Set world model for weapon
	SetEntProp(worldIndex, Prop_Send, "m_nModelIndex", model);
}