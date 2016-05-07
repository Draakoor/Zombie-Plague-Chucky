/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          natives.h.cpp
 *  Type:          API 
 *  Description:   Natives handlers for the ZP API.
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
 * List of statuses.
 **/
enum ValidateList
{
    couldDead = 0,
    shouldAlive
};

/**
 * Initializes all natives.
 */
void APINativesInit(/*void*/)
{
	// Create natives
	CreateNative("ZP_IsPlayerVIP", 				APIIsVIP);
	CreateNative("ZP_IsPlayerZombie", 			APIIsZombie);
	CreateNative("ZP_IsPlayerHuman", 			APIIsHuman);
	CreateNative("ZP_IsPlayerNemesis",			APIIsNemesis);
	CreateNative("ZP_IsPlayerSurvivor",			APIIsSurvivor);
	CreateNative("ZP_SwitchClass",				APISetType);
	CreateNative("ZP_GetClientAmmoPack", 		APICallAmmoPacks);
	CreateNative("ZP_SetClientAmmoPack", 		APISetAmmoPacks);
	CreateNative("ZP_GetClientZombieClass", 	APICallZombieType);
	CreateNative("ZP_GetClientLevel", 			APICallLevelNum);
	CreateNative("ZP_GetClientViewModel",		APICallViewModel);
	CreateNative("ZP_GetRoundState", 			APICallRoundState);
	
	// Create addons natives
	CreateNative("ZP_RegisterZombieClass", 		APIRegisterZombieClass); 
	CreateNative("ZP_RegisterExtraItem", 		APIRegisterExtraItem); 
}

/**
 * Native call function (ZP_IsPlayerVIP)
 *
 * native bool ZP_IsPlayerVIP(client)
 **/
public int APIIsVIP(Handle isPlugin, int iNumParams)
{
    // Get real player index from native cell 
    CBasePlayer* cBasePlayer = view_as<CBasePlayer>(GetNativeCell(1));
    
    // Validate the client index
    initializeClientIndex(view_as<int>(cBasePlayer), couldDead);
    
	// Return boolean
    return IsClientPrivileged(view_as<int>(cBasePlayer), OperationType_VIP);
}

/**
 * Native call function (ZP_IsPlayerZombie)
 *
 * native bool ZP_IsPlayerZombie(client)
 **/
public int APIIsZombie(Handle isPlugin, int iNumParams)
{
	// Get real player index from native cell 
    CBasePlayer* cBasePlayer = view_as<CBasePlayer>(GetNativeCell(1));
    
    // Validate the client index
    initializeClientIndex(view_as<int>(cBasePlayer), shouldAlive);
    
	// Return boolean
    return cBasePlayer->m_bZombie;
}

/**
 * Native call function (ZP_IsPlayerHuman)
 *
 * native bool ZP_IsPlayerHuman(client)
 **/
public int APIIsHuman(Handle isPlugin, int iNumParams)
{
	// Get real player index from native cell 
    CBasePlayer* cBasePlayer = view_as<CBasePlayer>(GetNativeCell(1));
    
    // Validate the client index
    initializeClientIndex(view_as<int>(cBasePlayer), shouldAlive);
    
	// Return boolean
    return !(cBasePlayer->m_bZombie);
}

/**
 * Native call function (ZP_IsPlayerNemesis)
 *
 * native bool ZP_IsPlayerNemesis(client)
 **/
public int APIIsNemesis(Handle isPlugin, int iNumParams)
{
    // Get real player index from native cell 
    CBasePlayer* cBasePlayer = view_as<CBasePlayer>(GetNativeCell(1));
    
    // Validate the client index
    initializeClientIndex(view_as<int>(cBasePlayer), shouldAlive);
    
	// Return boolean
    return (cBasePlayer->m_bNemesis);
}

/**
 * Native call function (ZP_IsPlayerSurvivor)
 *
 * native bool ZP_IsPlayerSurvivor(client)
 **/
public int APIIsSurvivor(Handle isPlugin, int iNumParams)
{
    // Get real player index from native cell 
    CBasePlayer* cBasePlayer = view_as<CBasePlayer>(GetNativeCell(1));
    
    // Validate the client index
    initializeClientIndex(view_as<int>(cBasePlayer), shouldAlive);
    
	// Return boolean
    return (cBasePlayer->m_bSurvivor);
}

/**
 * Native call function (ZP_SwitchClass)
 *
 * native void ZP_SwitchClass(type, client)
 **/
public int APISetType(Handle isPlugin, int iNumParams)
{
	// Get type of native
	int iMode = GetNativeCell(1);
	
	// Get real player index from native cell 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(GetNativeCell(2));

	// Validate the client index
	initializeClientIndex(view_as<int>(cBasePlayer), shouldAlive);

	// Set class
	switch(iMode)
	{
		case 0 :  InfectHumanToZombie(view_as<int>(cBasePlayer));
		case 1 :  InfectHumanToZombie(view_as<int>(cBasePlayer), false, false, true);
		case 2 :  MakeHumanIntoSurvivor(view_as<int>(cBasePlayer));
		case 3 :  MakeZombieToHuman(view_as<int>(cBasePlayer));
	}
}

/**
 * Native call function (ZP_GetClientAmmoPack)
 *
 * native int ZP_GetClientAmmoPack(client)
 **/
public int APICallAmmoPacks(Handle isPlugin, int iNumParams)
{
    // Get real player index from native cell 
    CBasePlayer* cBasePlayer = view_as<CBasePlayer>(GetNativeCell(1));
    
    // Validate the client index
    initializeClientIndex(view_as<int>(cBasePlayer), couldDead);
    
	// Return value
    return cBasePlayer->m_nAmmoPacks;
}

/**
 * Native call function (ZP_SetClientAmmoPack)
 *
 * native void ZP_SetClientAmmoPack(client, ammo)
 **/
public int APISetAmmoPacks(Handle isPlugin, int iNumParams)
{
	// Get real player index from native cell 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(GetNativeCell(1));

	// Get amount of ammopacks
	int iAmount = GetNativeCell(2);

	// Validate the client index
	initializeClientIndex(view_as<int>(cBasePlayer), couldDead);

	// Set ammopacks
	if(iAmount >= 0)
	{
		// Max amount of CS:GO Hud
		if(iAmount > 65000) 
		{
			iAmount = 65000;
		}
		
		cBasePlayer->m_nAmmoPacks = iAmount;
	}
}

/**
 * Native call function (ZP_GetClientZombieClass)
 *
 * native int ZP_GetClientZombieClass(client)
 **/
public int APICallZombieType(Handle isPlugin, int iNumParams)
{
    // Get real player index from native cell 
    CBasePlayer* cBasePlayer = view_as<CBasePlayer>(GetNativeCell(1));
    
    // Validate the client index
    initializeClientIndex(view_as<int>(cBasePlayer), shouldAlive);
    
	// Return value
    return cBasePlayer->m_nZombieClass;
}

/**
 * Native call function (ZP_GetClientLevel)
 *
 * native int ZP_GetClientLevel(client)
 **/
public int APICallLevelNum(Handle isPlugin, int iNumParams)
{
    // Get real player index from native cell 
    CBasePlayer* cBasePlayer = view_as<CBasePlayer>(GetNativeCell(1));
    
    // Validate the client index
    initializeClientIndex(view_as<int>(cBasePlayer), shouldAlive);
    
	// Return value
    return cBasePlayer->m_iLevel;
}

/**
 * Native call function (ZP_GetClientViewModel)
 *
 * native int ZP_GetClientViewModel(client)
 **/
public int APICallViewModel(Handle isPlugin, int iNumParams)
{
    // Get real player index from native cell 
    CBasePlayer* cBasePlayer = view_as<CBasePlayer>(GetNativeCell(1));
    
    // Validate the client index
    initializeClientIndex(view_as<int>(cBasePlayer), shouldAlive);
    
	// Return value
    return cBasePlayer->m_nModelIndex;
}

/**
 * Native call function (ZP_GetRoundState)
 *
 * native int ZP_GetRoundState(type)
 **/
public int APICallRoundState(Handle isPlugin, int iNumParams)
{
	// Get type of native
	int iType = GetNativeCell(1);
	
	// Return value
	switch(iType)
	{
		case 0 :  return bModeStarted ? 1 : 0;
		case 1 :  return gRoundNum;
		case 2 :  return gMode;
		case 3 :  return bNewRound;
		case 4 :  return bEndRound;
	}
	
	return 0;
}

/**
 * Validates a client index and when it fails, an error is thrown.
 * 
 * @error 	Throws an error when the client isn't valid.
 **/
void initializeClientIndex(int clientIndex, ValidateList iStatus)
{
	// Verify client index
	if (clientIndex < 1 || clientIndex > MaxClients)
	{
		LogEvent(false, LogType_Native, LOG_CORE_EVENTS, LogModule_Native, "Native Validation", "Invalid client index: [%d]", clientIndex);
		return;
	}
	
	// Verify that the client is connected
	if (!IsPlayerExist(clientIndex, false))
	{
		LogEvent(false, LogType_Native, LOG_CORE_EVENTS, LogModule_Native, "Native Validation", "Client [%d] is not connected.", clientIndex);
		return;
	}
	
	//! Verify that the client is alive ?
	if(iStatus)
	{
		if (!IsPlayerAlive(clientIndex))
		{
			LogEvent(false, LogType_Native, LOG_CORE_EVENTS, LogModule_Native, "Native Validation", "Client [%d] is dead.", clientIndex);
			return;
		}
	}
}
