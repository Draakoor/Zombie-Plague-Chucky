/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          roundstart.cpp
 *  Type:          Game 
 *  Description:   Round start event.
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
 * List of objective entities.
 **/
#define ROUNDSTART_OBJECTIVE_ENTITIES "func_bomb_target_hostage_entity_func_hostage_rescue_func_buyzone"
 
/**
 * The round is pre starting.
 **/
void RoundStartOnRoundPreStart(/*void*/)
{
	// Reset server grobal bools
	bNewRound = true;
	bEndRound = false;
	bModeStarted = false;
	
	// Reset mode
	gMode = MODE_INFECTION;
	
	// Increment count of rounds
	gRoundNum++;
	
	// Restore default time for zombie event timer
	gZombieCount = GetConVarInt(gCvarList[CVAR_ROUND_START]);
}
 
/**
 * The round is started.
 **/
void RoundStartOnRoundStart(/*void*/)
{
	// Kill all objective entities
	RoundStartKillObjectives(ROUNDSTART_OBJECTIVE_ENTITIES);

	// Get round time
	float flRoundTime = GetConVarFloat(gRoundTime);
	
	// Convert time in minutes to seconds
	flRoundTime *= 60.0;
	
	/*
	 * Subtract one and half second if running CS:GO to prevent round draw when round ends. 
	 * For some reason the timing doesn't match the actual round end. Thanks to Jargon.
	 */
	
	flRoundTime -= 1.5;
	
	// Create timer for starting game mode
	CreateTimer(1.0, EventMakeZombie, _, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);

	// Create timer for terminate round
	EndTimer(Task_ZombieRound);
	Task_ZombieRound = CreateTimer(flRoundTime, EventRoundZombieEnd);

	// Player round start sounds
	SoundsTransmitToAll("ZOMBIE_ROUND_START_SOUNDS");
}

/**
 * Kills all objective entities.
 *
 * @param sBuffer	The string with the list of objectives.
 **/
public void RoundStartKillObjectives(char[] sBuffer)
{
	// Delete doors entities ?
	if(GetConVarBool(gCvarList[CVAR_REMOVE_DOORS])) StrCat(sBuffer, BIG_LINE_LENGTH, "_func_door");
	
	// Initialize char
	char sClassname[NORMAL_LINE_LENGTH];
	
	// Save max amount of entities
	int nGetMaxEnt = GetMaxEntities();
	
	// Check all entities and remove it
	for (int nEntity = 0; nEntity <= nGetMaxEnt; nEntity++)
	{
		// If entity isn't valid, then stop
		if(!IsValidEdict(nEntity))
		{
			continue;	
		}
		
		// Get valid edict's classname
		GetEdictClassname(nEntity, sClassname, sizeof(sClassname));
		
		// Check if it matches any objective entities, then stop if it doesn't
		if(StrContains(sBuffer, sClassname) != -1) 
		{
			// Entity is an objective, kill it
			RemoveEdict(nEntity);
		}
	}
}