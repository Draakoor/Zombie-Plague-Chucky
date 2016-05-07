/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          playerclasses.cpp
 *  Type:          Game 
 *  Description:   Provides functions for managing classes.
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
 * Infects a client.
 *
 * @param iVictim	The victim index.
 * @param iAttaker	(Optional) The attacker index.
 * @param bRespawn	(Optional) Indicates that infection was on spawn.
 * @param bNemesis	(Optional) Indicates that victim will be a nemesis.
 **/
void InfectHumanToZombie(int iVictim, int iAttaker = 0, bool bRespawn = false, bool bNemesis = false)
{
	// Get real player index from event key
	CBasePlayer* cBaseVictim = view_as<CBasePlayer>(iVictim);
	CBasePlayer* cBaseAttacker = view_as<CBasePlayer>(iAttaker);

	// If round ended, stop
	if(bEndRound)
	{
		return;
	}
	
	// Verify that the client is connected and alive 
	if(!IsPlayerExist(view_as<int>(cBaseVictim)))
	{
		return;
	}
	
	// Delete light
	if(cBaseVictim->m_bSurvivor)
	{
		if(GetConVarBool(gCvarList[CVAR_SURVIVOR_GLOW])) VEffectRemoveLightDynamic(view_as<int>(cBaseVictim));
	}
	
	// Clear all timers
	ToolsKillAllTimers(view_as<int>(cBaseVictim));
	
	// Reset some variables
	cBaseVictim->m_bZombie 			= true;
	cBaseVictim->m_bSurvivor 		= false;
	cBaseVictim->m_iArmorValue 		= 0;
	cBaseVictim->m_iLevel 			= 1;
	cBaseVictim->m_iMoveType 		= MOVETYPE_WALK;
	cBaseVictim->m_bNightVisionOn	= GetConVarInt(gCvarList[CVAR_ZOMBIE_NIGHT_VISION]);
	cBaseVictim->m_iFOV(GetConVarInt(gCvarList[CVAR_ZOMBIE_FOV]));
	cBaseVictim->m_bFlashLightOn(false); 
	cBaseVictim->m_bSetGlow(false);
	cBaseVictim->m_iRender(255, 255, 255);
	
	// Show level overlay
	if(RoundEndGetRoundValidate())
	{
		ToolsSwitchLevel(cBaseVictim, bNemesis ? UNVALID_MESSAGE_ID : cBaseVictim->m_iLevel);
	}

	// Initialize model char
	char sModel[PLATFORM_MAX_PATH];
	
	// Set zombie models and speed, gravity
	if(bNemesis)
	{
		// Set nemesis variable
		cBaseVictim->m_bNemesis 			 = true;
		
		// Update zombie class
		cBaseVictim->m_nZombieClass 		 = 0;
		
		// Set health, speed and gravity
		cBaseVictim->m_iHealth 	   			 = (fnGetAlive() * GetConVarInt(gCvarList[CVAR_NEMESIS_HEALTH]));
		cBaseVictim->m_flLaggedMovementValue = GetConVarFloat(gCvarList[CVAR_NEMESIS_SPEED]); 
		cBaseVictim->m_flGravity 			 = GetConVarFloat(gCvarList[CVAR_NEMESIS_GRAVITY]);
		
		// Get nemesis model
		GetConVarString(gCvarList[CVAR_NEMESIS_PLAYER_MODEL], sModel, sizeof(sModel)); 

		// Set glow
		if(GetConVarBool(gCvarList[CVAR_NEMESIS_GLOW])) 
		{
			// Get glowing color
			char sColor[SMALL_LINE_LENGTH];
			GetConVarString(gCvarList[CVAR_NEMESIS_GLOW_COLOR], sColor, sizeof(sColor)); 
			
			// Create dymanic light
			VEffectLightDynamic(_, sColor, 2000.0, 150.0, 999.9, true, view_as<int>(cBaseVictim));
		}
	}
	else
	{
		// Update zombie class
		cBaseVictim->m_nZombieClass 		 = IsFakeClient(view_as<int>(cBaseVictim)) ? Math_GetRandomInt(0, gZombieClassNum-1) : cBaseVictim->m_nZombieNext;
		
		// If the client isn't privileged
		if(!IsClientPrivileged(view_as<int>(cBaseVictim), OperationType_VIP))
		{
			// But have privileged zombie class by default
			if(ZombiesIsVIP(cBaseVictim->m_nZombieClass))
			{
				// Choose any non privileged zombie class
				for(int i = 0; i < gZombieClassNum; i++)
				{
					// Skip all privileged zombie class
					if(ZombiesIsVIP(i))
						continue;
					
					// Update zombie class
					cBaseVictim->m_nZombieClass = i;
				}
			}
		}
		
		// Set health, speed and gravity
		cBaseVictim->m_iHealth 				 = ZombieGetHealth(cBaseVictim->m_nZombieClass) + ((fnGetZombies() <= 1) ? GetConVarInt(gCvarList[CVAR_ZOMBIE_FISRT_HEALTH]) : 0);
		cBaseVictim->m_flLaggedMovementValue = ZombieGetSpeed(cBaseVictim->m_nZombieClass);
		cBaseVictim->m_flGravity 			 = ZombieGetGravity(cBaseVictim->m_nZombieClass);
		
		// Get zombie class model
		ZombieGetModel(cBaseVictim->m_nZombieClass, sModel, sizeof(sModel));
	}
	
	// Clear weapon slots
	WeaponsRemoveAll(cBaseVictim);
	
	// Set model
	cBaseVictim->m_ModelName(sModel);

	// Is zombie respawn ?
	if(bRespawn)
	{
		// Create spawn effect
		VEffectFireEffect(view_as<int>(cBaseVictim));
		
		// Play respawning sound
		cBaseVictim->InputEmitAISound(SNDCHAN_VOICE, SNDLEVEL_SCREAMING, "ZOMBIE_COMEBACK_SOUNDS", true);
	}
	else
	{
		// Create infect effect
		VEffectInfectEffect(view_as<int>(cBaseVictim));
		
		// Emit sound
		cBaseVictim->InputEmitAISound(SNDCHAN_VOICE, SNDLEVEL_SCREAMING, (ZombieIsFemale(cBaseVictim->m_nZombieClass)) ? "ZOMBIE_FEMALE_INFECTION_SOUNDS" : "ZOMBIE_INFECTION_SOUNDS", true);
	}

	// Create bleeding footsteps ?
	if(GetConVarBool(gCvarList[CVAR_ZOMBIE_BLEEDING]))
	{
		CreateTimer(4.0, EventZombieBleeding, view_as<int>(cBaseVictim), TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
	}
	
	//*********************************************************************
	//*           REWARDS AND BONUSES FOR INFECTOR OF THE HUMAN           *
	//*********************************************************************
	
	// Verify that the infector is connected and alive
	if(IsPlayerExist(view_as<int>(cBaseAttacker))) 
	{
		// Set additional health
		cBaseAttacker->m_iHealth += GetConVarInt(gCvarList[CVAR_BONUS_HEALTH]);
		
		// Set death and kill frags
		cBaseAttacker->m_iFrags++;
		cBaseVictim->m_iDeaths++;
		
		// Create and send custom player_death event
		Event sEvent = CreateEvent("player_death");
		if (sEvent != NULL)
		{
			SetEventInt(sEvent, "userid", GetClientUserId(view_as<int>(cBaseVictim)));
			SetEventInt(sEvent, "attacker", GetClientUserId(view_as<int>(cBaseAttacker)));
			SetEventString(sEvent, "weapon", "weapon_zombie_claws");
			SetEventBool(sEvent, "headshot", true);
			FireEvent(sEvent, false);
		}

		// Give ammopacks for infection
		cBaseAttacker->m_nAmmoPacks += GetConVarInt(gCvarList[CVAR_BONUS_INFECT]);
		
		// Update level index and show overlay
		if(GetConVarBool(gCvarList[CVAR_LVL_SYSTEM]) && cBaseAttacker->m_iLevel < 10)
		{
			// Increment level
			cBaseAttacker->m_iLevel++;

			// Update overlays and speed
			ToolsSwitchLevel(cBaseAttacker, cBaseAttacker->m_iLevel);
			ToolsIncreaseSpeed(cBaseAttacker, cBaseAttacker->m_iLevel);
		}
	}
	
	// Call forward
	APIOnClientInfected(cBaseVictim, cBaseAttacker);
	
	//*********************************************************************
	//*          		 CHANGE TEAM ON THE NEXT FRAME           		  *
	//*     _MAKED FOR CORRECT SHOWING TEAM IN THE FAKE DEATH EVENT_      *
	//*********************************************************************
	
	RequestFrame(view_as<RequestFrameCallback>(EventZombieSwitchTeam), cBaseVictim);
}

/**
 * Make a human.
 * 
 * @param clientIndex	The client index.
 **/
void MakeZombieToHuman(int clientIndex)
{
	// Get real player index from event key 
	CBasePlayer* cBaseVictim = view_as<CBasePlayer>(clientIndex);

	// Delete light
	if(cBaseVictim->m_bNemesis)
	{
		if(GetConVarBool(gCvarList[CVAR_NEMESIS_GLOW])) VEffectRemoveLightDynamic(view_as<int>(cBaseVictim));
	}
	
	// Make a human
	cBaseVictim->m_bRespawn = ZP_TEAM_HUMAN;
	cBaseVictim->m_iRespawnPlayer();
	cBaseVictim->m_iTeamNum = ZP_TEAM_HUMAN;
	
	// Set xray for zombie
	cBaseVictim->m_bSetGlow(GetConVarBool(gCvarList[CVAR_ZOMBIE_XRAY]) ? true : false);
}

/**
 * Make a survivor.
 *
 * @param clientIndex	The client index.
 **/
void MakeHumanIntoSurvivor(int clientIndex)
{
	// Get real player index from event key
	CBasePlayer* cBaseVictim = view_as<CBasePlayer>(clientIndex);

	// If player is zombie
	if(cBaseVictim->m_bZombie)
	{
		MakeZombieToHuman(view_as<int>(cBaseVictim));
	}
	
	// Remove player weapons
	WeaponsRemoveAll(cBaseVictim);
	
	// Remove glowing
	cBaseVictim->m_bSetGlow(false);

	// Set flag
	cBaseVictim->m_bSurvivor = true;

	// Set survivor health, speed, gravity
	cBaseVictim->m_iHealth 					= GetConVarInt(gCvarList[CVAR_SURVIVOR_HEALTH]);
	cBaseVictim->m_flLaggedMovementValue 	= GetConVarFloat(gCvarList[CVAR_SURVIVOR_SPEED]); 
	cBaseVictim->m_flGravity 				= GetConVarFloat(gCvarList[CVAR_SURVIVOR_GRAVITY]);
	
	// Clear any existing overlay from screen
	ToolsSwitchLevel(cBaseVictim, UNVALID_MESSAGE_ID);
	
	// Initialize model char
	char sModel[PLATFORM_MAX_PATH];
	
	// Get survivor model
	GetConVarString(gCvarList[CVAR_SURVIVOR_PLAYER_MODEL], sModel, sizeof(sModel));
	
	// Set model
	cBaseVictim->m_ModelName(sModel);
	
	// Get color
	char sColor[SMALL_LINE_LENGTH];
	GetConVarString(gCvarList[CVAR_SURVIVOR_GLOW_COLOR], sColor, sizeof(sColor)); 
	
	// Create dymanic light
	if(GetConVarBool(gCvarList[CVAR_SURVIVOR_GLOW])) VEffectLightDynamic(_, sColor, 2000.0, 150.0, 999.9, true, view_as<int>(cBaseVictim));
	
	// Give weapons
	cBaseVictim->CItemMaterialize("weapon_elite");
	cBaseVictim->CItemMaterialize("weapon_m249");

	// Call forward
	APIOnClientHeroed(cBaseVictim);
}

/**
 * Switch zombie to terrorist team.
 *
 * @param cBaseVictim    The client index.
 **/
public void EventZombieSwitchTeam(CBasePlayer* cBaseVictim)
{
	// Verify that the client is connected and alive
	if(!IsPlayerExist(view_as<int>(cBaseVictim)))
	{
		return;
	}
	
	// Force client to change team
	cBaseVictim->m_iTeamNum = ZP_TEAM_ZOMBIE;
}

/**
 * Make zombies leave footsteps and bloodstains on the floor.
 *
 * @param sTimer     	 The timer index.
 * @param clientIndex    The client index.
 **/
public Action EventZombieBleeding(Handle sTimer, any clientIndex)
{
	// Get real player index from event key 
	CBasePlayer* cBaseVictim = view_as<CBasePlayer>(clientIndex);
	
	// Verify that the client is connected and alive
	if(!IsPlayerExist(view_as<int>(cBaseVictim)))
	{
		return ACTION_STOP;
	}
	
	// Verify that the client is zombie
	if(cBaseVictim->m_bZombie)
	{
		// If player on the ground
		if(cBaseVictim->m_iFlags & FL_ONGROUND)
		{
			// Create bleeding particle
			CreateBloodDecalFunction(view_as<int>(cBaseVictim));
		}
		
		// Allow bleeding
		return ACTION_CONTINUE;
	}
	
	// Reset bleeding
	return ACTION_STOP;
}