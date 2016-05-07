/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          death.cpp
 *  Type:          Game 
 *  Description:   Death event.
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
 * Event callback (player_death)
 * The player is about to die.
 * 
 * @param iVictim		The victim index.
 * @param iAttacker		The attacker index.
 **/
void DeathOnClientDeath(int iVictim, int iAttacker)
{
	// Get real player index from event key
	CBasePlayer* cBaseVictim  = view_as<CBasePlayer>(iVictim);
	CBasePlayer* cBaseAttacker = view_as<CBasePlayer>(iAttacker);

	// Remove glowing
	cBaseVictim->m_bSetGlow(false);
	
	// Remove night vision
	cBaseVictim->m_bNightVisionOn = 0;
	
	// Clear any existing overlay from screen
	if(RoundEndGetRoundValidate())
	{
		ToolsSwitchLevel(cBaseVictim, UNVALID_MESSAGE_ID);
	}

	// Emit scream sound
	cBaseVictim->InputEmitAISound(SNDCHAN_VOICE, SNDLEVEL_SCREAMING, (ZombieIsFemale(cBaseVictim->m_nZombieClass)) ? "ZOMBIE_FEMALE_DEATH_SOUNDS" : "ZOMBIE_DEATH_SOUNDS", true);
	
	// Player was killed by other ?
	if(cBaseVictim != cBaseAttacker) 
	{
		// If respawn amount more, than limit, stop
		if(cBaseVictim->m_nRespawnTimes > GetConVarInt(gCvarList[CVAR_RESPAWN_AMOUNT]))
		{
			return;
		}
		
		// Verify that the attacker is connected and alive
		if(IsPlayerExist(view_as<int>(cBaseAttacker)))
		{
			// Give ammopacks for kill
			cBaseAttacker->m_nAmmoPacks += GetConVarInt(gCvarList[CVAR_BONUS_KILL]);

			// Level system work only for humans and zombies
			if(!cBaseAttacker->m_bNemesis && !cBaseAttacker->m_bSurvivor)
			{
				if(GetConVarBool(gCvarList[CVAR_LVL_SYSTEM]) && cBaseAttacker->m_iLevel < 10)
				{
					// Increment level
					cBaseAttacker->m_iLevel++;

					// Update overlays and speed
					ToolsSwitchLevel(cBaseAttacker, cBaseAttacker->m_iLevel);
					ToolsIncreaseSpeed(cBaseAttacker, cBaseAttacker->m_iLevel);
				}
			}
		}
	}
	// If player was killed by world, respawn on suicide?
	else if (!GetConVarBool(gCvarList[CVAR_RESPAWN_WORLD]))
	{
		return;
	}

	// Respawn if human/zombie/nemesis/survivor?
	if ((cBaseVictim->m_bZombie && !cBaseVictim->m_bNemesis && !GetConVarBool(gCvarList[CVAR_RESPAWN_ZOMBIE])) || (!cBaseVictim->m_bZombie && !cBaseVictim->m_bSurvivor && !GetConVarBool(gCvarList[CVAR_RESPAWN_HUMAN])) || (cBaseVictim->m_bNemesis && !GetConVarBool(gCvarList[CVAR_RESPAWN_NEMESIS])) || (cBaseVictim->m_bSurvivor && !GetConVarBool(gCvarList[CVAR_RESPAWN_SURVIVOR])))
	{
		return;
	}
	
	// Increment count
	cBaseVictim->m_nRespawnTimes++;
	
	// Set timer for respawn player
	EndTimer(Task_ZombieRespawn[view_as<int>(cBaseVictim)]);
	Task_ZombieRespawn[view_as<int>(cBaseVictim)] = CreateTimer(GetConVarFloat(gCvarList[CVAR_RESPAWN_TIME]), EventRespawnZombie, view_as<int>(cBaseVictim));
}

/**
 * Timer which respawn zombie.
 *
 * @param hTimer     	 The timer handle.
 * @param victimIndex	 The victim index.
 **/
public Action EventRespawnZombie(Handle hTimer, any victimIndex)
{
	// Get real player index from event key 
	CBasePlayer* cBaseVictim = view_as<CBasePlayer>(victimIndex);
	
	// Clear timer
	Task_ZombieRespawn[view_as<int>(cBaseVictim)] = NULL;	
	
	// Round end or just started
	if(bNewRound || bEndRound)
	{
		return ACTION_STOP;
	}
		
	// Verify that the client is connected
	if(!IsPlayerExist(view_as<int>(cBaseVictim), false))
	{
		return ACTION_STOP;
	}
	
	// Verify that the client is dead
	if (IsPlayerAlive(view_as<int>(cBaseVictim)))
	{
		return ACTION_STOP;
	}
	
	// Respawn player automatically if allowed on current round
	if ((gMode != MODE_SURVIVOR || GetConVarBool(gCvarList[CVAR_RESPAWN_ALLOW_SURVIVOR])) && (gMode != MODE_SWARM || GetConVarBool(gCvarList[CVAR_RESPAWN_ALLOW_SWARM])) && (gMode != MODE_NEMESIS || GetConVarBool(gCvarList[CVAR_RESPAWN_ALLOW_NEMESIS])))
	{
		// Infection rounds = none of the above
		if (!GetConVarBool(gCvarList[CVAR_RESPAWN_INFETION]) && gMode != MODE_SURVIVOR && gMode != MODE_SWARM && gMode != MODE_NEMESIS && gMode != MODE_ARMAGEDDON)
		{
			return ACTION_STOP;
		}
		
		// Respawn if only the last human is left? (ignore this setting on survivor rounds)
		if (gMode != MODE_SURVIVOR && !GetConVarBool(gCvarList[CVAR_RESPAWN_LAST]) && fnGetHumans() <= 1)
		{
			return ACTION_STOP;
		}

		// Respawn a player
		ToolsRespawn(view_as<int>(cBaseVictim));
	}
	
	// Destroy timer
	return ACTION_STOP;
}