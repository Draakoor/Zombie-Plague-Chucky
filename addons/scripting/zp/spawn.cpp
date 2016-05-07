/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          spawn.cpp
 *  Type:          Game 
 *  Description:   Spawn event.
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
 * Global variable to store infect timer handle.
 **/
float flSpawnOrigin[MAXPLAYERS+1][3];
 
/**
 * The player is spawning.
 *
 * @param clientIndex	The client index.
 **/
void SpawnOnClientSpawn(int clientIndex)
{
	// Get real player index from event key
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex); 

	// Update weapons and model indexes
	if(cBasePlayer->m_bSurvivor || cBasePlayer->m_bZombie || gRoundNum <= 1)
	{
		// Remove all weapons
		WeaponsRemoveAll(cBasePlayer);
	
		// Give default weapon
		cBasePlayer->CItemMaterialize(WEAPONS_SPAWN_WEAPON);
	}

	// If gamemode didn't started
	if(bNewRound)
	{
		// Reset respawn count
		cBasePlayer->m_nRespawnTimes = 0;
		
		// Reset limit of extra items
		ItemsRemoveLimit(view_as<int>(cBasePlayer));
	}
	
	// Reset all variables
	cBasePlayer->m_bZombie 					= false;
	cBasePlayer->m_bSurvivor 				= false;
	cBasePlayer->m_bNemesis 				= false;
	cBasePlayer->m_bSkill 					= false;
	cBasePlayer->m_bNightVisionOn 			= 0;
	cBasePlayer->m_nLastBoughtAmount 		= 0;
	cBasePlayer->m_nSkillCountDown 			= 0;
	cBasePlayer->m_iLevel 					= 1;
	cBasePlayer->m_iMoveType 				= MOVETYPE_WALK;
	cBasePlayer->m_nModelIndex 				= WeaponGetViewModelIndex(view_as<int>(cBasePlayer));
	cBasePlayer->m_iHealth 	   				= GetConVarInt(gCvarList[CVAR_HUMAN_HEALTH]);
	cBasePlayer->m_iArmorValue 				= GetConVarInt(gCvarList[CVAR_HUMAN_ARMOR]);
	cBasePlayer->m_flLaggedMovementValue 	= GetConVarFloat(gCvarList[CVAR_HUMAN_SPEED]); 
	cBasePlayer->m_flGravity 				= GetConVarFloat(gCvarList[CVAR_HUMAN_GRAVITY]);
	cBasePlayer->m_iFOV(90);
	cBasePlayer->m_bSetGlow(false);
	cBasePlayer->m_bFlashLightOn(false); 
	cBasePlayer->m_iRender(255, 255, 255);
	
	// Store spawn origin
	cBasePlayer->m_flGetOrigin(flSpawnOrigin[view_as<int>(cBasePlayer)]);

	// Respawn as zombie?
	if(cBasePlayer->m_bRespawn == ZP_TEAM_ZOMBIE)
	{
		// Is mode started ?
		if (bModeStarted)
		{
			// Make a zombie
			InfectHumanToZombie(view_as<int>(cBasePlayer), false, true);
			return;
		}
	}
	
	// Update human model
	ModelsOnClientSpawn(view_as<int>(cBasePlayer));
	
	// Update level overlay
	ToolsSwitchLevel(cBasePlayer, cBasePlayer->m_iLevel);
}

/**
 * Checks if a player is stuck. 
 *  
 * @param cBasePlayer	The client index.
 **/ 
void SpawnOnClientStuck(CBasePlayer* cBasePlayer)
{
	/* 
	 * Checks to see if a player would collide with MASK_SOLID. (i.e. they would be stuck)
	 * Inflates player mins/maxs a little bit for better protection against sticking.
	 * Thanks to Andersso for the basis of this function.
	 */
	
	// Initialize vector variables
	float flMax[3]; 
	float flMin[3]; 
	float flOrigin[3];

	// Get the client's min and max size vector
	GetClientMins(view_as<int>(cBasePlayer), flMin);
	GetClientMaxs(view_as<int>(cBasePlayer), flMax);

	// Get player origin
	cBasePlayer->m_flGetOrigin(flOrigin);

	// Starts up a new trace hull using a global trace result and a customized trace ray filter
	TR_TraceHullFilter(flOrigin, flOrigin, flMin, flMax, MASK_SOLID, IsStuck, view_as<int>(cBasePlayer));

	// Returns if there was any kind of collision along the trace ray
	if(TR_DidHit())
	{
		cBasePlayer->m_iTeleportPlayer(flSpawnOrigin[view_as<int>(cBasePlayer)]);
	}
}

/**
 * Trace filter.
 *
 * @param iEntity		The entity index.
 * @param iContentsMask	The contents mask.
 * @param iVictim		The victim index.
 **/
public bool IsStuck(int iEntity, int iContentsMask, any iVictim) 
{
    return (iEntity != iVictim);
}