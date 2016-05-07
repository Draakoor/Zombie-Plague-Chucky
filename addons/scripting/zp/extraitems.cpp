/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          extraitems.cpp
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
 * Number of max valid extra items.
 **/
#define ExtraItemMax 64
 
/**
 * List of operation types for items data.
 **/
enum ExtraItemData
{
	String:gExtraItemsName[NORMAL_LINE_LENGTH],
	gExtraItemCost,
	gExtraItemTeam,
	gExtraItemLevel,
	gExtraItemOnline,
	gExtraItemLimit
};

/**
 * Arrays to store the server's extra items.
 **/
int  gExtraItemsData[ExtraItemMax][ExtraItemData];
int  gExtraItemsNum;

/**
 * Array to store the item limit to player
 **/
int  gExtraBuyLimit[MAXPLAYERS+1][ExtraItemMax];

/**
 * Called, when initialization of extra items is done. 
 **/
void ExtraItemsItemsLoad(/*void*/)
{
	// No extra items?
	if(!gExtraItemsNum)
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_Extraitems, "ExtraItems Validation", "No extra items loaded");
		return;
	}
}

/**
 * Load extra items from other plugin.
 *
 * native int ZP_RegisterExtraItem(name, cost, team, level, online, limit)
 **/
public int APIRegisterExtraItem(Handle isPlugin, int iNumParams)
{
	// Retrieves the string length from a native parameter string
	int iLenth;
	GetNativeStringLength(1, iLenth);
	
	// Strings are empty ?
	if (iLenth <= 0)
	{
		LogEvent(false, LogType_Native, LOG_CORE_EVENTS, LogModule_Extraitems, "Native Validation", "Can't register extra item with an empty name");
		return -1;
	}
	
	// Maximum amout of extra items
	if(gExtraItemsNum >= ExtraItemMax)
	{
		LogEvent(false, LogType_Normal, LOG_CORE_EVENTS, LogModule_Extraitems, "ExtraItems Validation",  "Maximum number of extra items reached (%d). Skipping other items.", ExtraItemMax);
		return -1;
	}
	
	// Get main values from native
	GetNativeString(1, gExtraItemsData[gExtraItemsNum][gExtraItemsName], SMALL_LINE_LENGTH);
	
	gExtraItemsData[gExtraItemsNum][gExtraItemCost]   = view_as<int>(GetNativeCell(2));  
	gExtraItemsData[gExtraItemsNum][gExtraItemTeam]   = view_as<int>(GetNativeCell(3));
	gExtraItemsData[gExtraItemsNum][gExtraItemLevel]  = view_as<int>(GetNativeCell(4));
	gExtraItemsData[gExtraItemsNum][gExtraItemOnline] = view_as<int>(GetNativeCell(5));
	gExtraItemsData[gExtraItemsNum][gExtraItemLimit]  = view_as<int>(GetNativeCell(6));

	// Removes whitespace characters from the beginning and end of a string
	TrimString(gExtraItemsData[gExtraItemsNum][gExtraItemsName]);
	
	// Add loaded extra item
	gExtraItemsNum++;
	
	// Return id under which we registered the item
	return gExtraItemsNum-1;
}

/**
 * Extra items data reading API.
 **/

/**
 * Gets the name of a item at a given index.
 *
 * @param iD		The item index.
 * @param sClassname	The string to return name in.
 * @param iMaxLen	The max length of the string.
 **/
stock void ItemsGetName(int iD, char[] sClassname, int iMaxLen)
{
    // Get item name
	strcopy(sClassname, iMaxLen, gExtraItemsData[iD][gExtraItemsName]);
}

/**
 * Gets the price of ammo for the item.
 *
 * @param iD		The item index.
 * @return			The ammo price.	
 **/
stock int ItemsGetCost(int iD)
{
    // Return default restriction status
    return gExtraItemsData[iD][gExtraItemCost];
}

/**
 * Gets the team for the item.
 *
 * @param iD		The item index.
 * @return			The team index.	
 **/
stock int ItemsGetTeam(int iD)
{
    // Return default restriction status
	return gExtraItemsData[iD][gExtraItemTeam];
}

/**
 * Gets the level for the item.
 *
 * @param iD		The item index.
 * @return			The level value.	
 **/
stock int ItemsGetLevel(int iD)
{
    // Return default restriction status
	return gExtraItemsData[iD][gExtraItemLevel];
}

/**
 * Gets the online for the item.
 *
 * @param iD        The item index.
 * @return          The online value.	
 **/
stock int ItemsGetOnline(int iD)
{
    // Return default restriction status
	return gExtraItemsData[iD][gExtraItemOnline];
}

/**
 * Gets the limit for the item.
 * @param iD		The item index.
 * @return          The limit value.	
 **/
stock int ItemsGetLimit(int iD)
{
    // Return default restriction status
	return gExtraItemsData[iD][gExtraItemLimit];
}

/**
 * Remove the limit from a player.
 *
 * @param clientIndex	The player index.
 **/
stock void ItemsRemoveLimit(int clientIndex)
{
	// Remove all extraitems limit
	for(int i = 0; i < gExtraItemsNum; i++)
	{
		gExtraBuyLimit[clientIndex][i] = 0;
	}
}