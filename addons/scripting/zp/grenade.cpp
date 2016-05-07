/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          grenade.cpp
 *  Type:          Game 
 *  Description:   Grenade exploading events.
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
 * The decoy nade is fired.
 * 
 * @param nEntity		The entity index.  
 * @param flOrigin		The explosion origin.
 **/
void GrenadeOnDecoyDetonate(int nEntity, float flOrigin[3])
{
	// Initialize vector variables
	float flVictimOrigin[3];
	
	// Get radius of freezing
	float flGrenadeRadiusFreezing = GetConVarFloat(gCvarList[CVAR_GRENADE_FREEZING_RADIUS]);
	
	// i = client index
	for (int i = 1; i <= MaxClients; i++)
	{
		// Verify that the client is connected and alive
		if (IsPlayerExist(i))
		{
			// Get victim origin
			GetClientAbsOrigin(i, flVictimOrigin);
			flVictimOrigin[2] += 2.0;
			
			// If distance to the entity is less than the radius of explosion
			if (GetVectorDistance(flOrigin, flVictimOrigin) <= flGrenadeRadiusFreezing)
			{				
				// Create trace
				Handle hTrace = TR_TraceRayFilterEx(flOrigin, flVictimOrigin, MASK_SOLID, RayType_EndPoint, IsNonEntity, i);
																																							
				if ((TR_DidHit(hTrace) && TR_GetEntityIndex(hTrace) == i) || (GetVectorDistance(flOrigin, flVictimOrigin) <= 100.0))
				{
					// Freeze entity
					OnEntityFrozen(i);
					
					// We're done with this trace now, so we can close it
					CloseHandle(hTrace);
				}	
				else
				{
					// We're done with this trace now, so we can close it
					CloseHandle(hTrace);
					
					// Get position of victim
					GetClientEyePosition(i, flVictimOrigin);
					flVictimOrigin[2] -= 2.0;
					
					// Create new trace
					hTrace = TR_TraceRayFilterEx(flOrigin, flVictimOrigin, MASK_SOLID, RayType_EndPoint, IsNonEntity, i);
				
					// Freeze entity
					if ((TR_DidHit(hTrace) && TR_GetEntityIndex(hTrace) == i) || (GetVectorDistance(flOrigin, flVictimOrigin) <= 100.0))
					{
						OnEntityFrozen(i);
					}
					
					// We're done with this trace now, so we can close it
					CloseHandle(hTrace);
				}
			}
		}
	}

	// Create sparks splash effect
	SetupSparksFunction(flOrigin, 5000, 1000);

	// Remove grenade
	RemoveEdict(nEntity);
}

/**
 * Trace filter.
 *
 * @param iEntity		The entity index.  
 * @param iContentsMask	The contents mask.
 * @param iVictim		The index of the victim.
 **/
public bool IsNonEntity(int iEntity, int iContentsMask, any iVictim)
{
	return (iVictim == iEntity);
}

/**
 * The smokegrenade nade is exployed.
 * 
 * @param nEntity		The entity index.  
 * @param flOrigin		The explosion origin.
 **/
void GrenadeOnSmokeDetonate(int nEntity, float flOrigin[3])
{
	// Get grenade color
	char sGrenadeColorLight[SMALL_LINE_LENGTH];
	GetConVarString(gCvarList[CVAR_GRENADE_LIGHT_COLOR], sGrenadeColorLight, sizeof(sGrenadeColorLight)); 
	
	// Generate color for random choose
	if(sGrenadeColorLight[0] == 'r')
	{
		Format(sGrenadeColorLight, sizeof(sGrenadeColorLight), "%i %i %i 255", Math_GetRandomInt(0,255), Math_GetRandomInt(0,255), Math_GetRandomInt(0,255));
	}
	
	// Create light effect
	VEffectLightDynamic(flOrigin, sGrenadeColorLight, GetConVarFloat(gCvarList[CVAR_GRENADE_LIGHT_DISTANCE]), GetConVarFloat(gCvarList[CVAR_GRENADE_LIGHT_COLOR]), GetConVarFloat(gCvarList[CVAR_GRENADE_LIGHT_DURATION]));
	
	// Create explosion effect
	SetupSparksFunction(flOrigin, _, _, true);
	SetupDustFunction(flOrigin);
	SetupSmokeFunction(flOrigin);
	
	// Remove grenade
	RemoveEdict(nEntity);
}

/**
 * Player is about to freeze.
 *
 * @param clientIndex	The client index.
 **/
void OnEntityFrozen(int clientIndex)
{
	// Get real player index from event key
	CBasePlayer* cBasePlayer  = view_as<CBasePlayer>(clientIndex);
	
	// Verify that the client is a zombie
	if(!cBasePlayer->m_bZombie || cBasePlayer->m_bNemesis)
	{
		return;
	}

	// Freeze client
	cBasePlayer->m_iMoveType = MOVETYPE_NONE;

	// Set blue render color
	cBasePlayer->m_iRender(120, 120, 255); 

	// Create freeze sound
	cBasePlayer->InputEmitAISound(SNDCHAN_VOICE, SNDLEVEL_RAIDSIREN, "zbm3/impalehit.mp3");

	// Create timer for removing freezing
	EndTimer(Task_ZombieFreezed[view_as<int>(cBasePlayer)]);
	Task_ZombieFreezed[view_as<int>(cBasePlayer)] = CreateTimer(GetConVarFloat(gCvarList[CVAR_GRENADE_FREEZING]), EventRemoveFreezing, cBasePlayer);
}

/**
 * Timer for remove freeze.
 *
 * @param hTimer		The timer handle.
 * @param clientIndex	The client index.
 **/
public Action EventRemoveFreezing(Handle hTimer, any clientIndex)
{
	// Get real player index from event key 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);
	
	// Clear timer 
	Task_ZombieFreezed[view_as<int>(cBasePlayer)] = NULL;

	// Verify that the client is connected and alive
	if(!IsPlayerExist(view_as<int>(cBasePlayer)))
	{
		return ACTION_STOP;
	}

	// Unfreeze client
	cBasePlayer->m_iMoveType = MOVETYPE_WALK;

	// Set standart render color
	cBasePlayer->m_iRender(255, 255, 255); 
	
	// Destroy timer
	return ACTION_STOP;
}