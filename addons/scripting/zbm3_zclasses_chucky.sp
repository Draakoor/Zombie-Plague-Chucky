/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *
 *  Copyright (C) 2015 Nikita Ushakov (Ireland, Dublin)
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

#include <sourcemod>
#include <sdktools>
#include <sdkhooks>
#include <zombieplague>

/**
 * Record plugin info.
 **/
public Plugin myinfo =
{
	name        	= "[ZP] Zombie Class: Chucky",
	author      	= "Draakoor",
	description 	= "Adds chucky the murder doll",
	version     	= "1.0.0",
	url         	= "www.thegermanfortress.de"
}

/**
 * @section Information about zombie class.
 **/
#define ZOMBIE_CLASS_NAME				"Chucky" // If string has @, phrase will be taken from translation file
#define ZOMBIE_CLASS_MODEL				"models/player/custom_player/legacy/chucky/chucky.mdl"
#define ZOMBIE_CLASS_CLAW				"models/zombie/normalhost_female/hand/hand_zombie_normalhost_f.mdl"
#define ZOMBIE_CLASS_HEALTH				1500
#define ZOMBIE_CLASS_SPEED				2.0
#define ZOMBIE_CLASS_GRAVITY			0.5
#define ZOMBIE_CLASS_FEMALE				YES
#define ZOMBIE_CLASS_VIP			 	NO
#define ZOMBIE_CLASS_DURATION			10	
#define ZOMBIE_CLASS_COUNTDOWN			30

#define SKILL_SPEED_FAST 				1.5
/**
 * @endsection
 **/

// Array for storing speed value
float gFastSpeed[MAXPLAYERS+1];

// Initialize zombie class index
int gZombieFast;

/**
 * Plugin is loading.
 **/
public void OnPluginStart()
{
	// Initilizate zombie class
	gZombieFast = ZP_RegisterZombieClass(ZOMBIE_CLASS_NAME, ZOMBIE_CLASS_MODEL, ZOMBIE_CLASS_CLAW, ZOMBIE_CLASS_HEALTH, ZOMBIE_CLASS_SPEED, ZOMBIE_CLASS_GRAVITY, ZOMBIE_CLASS_FEMALE, ZOMBIE_CLASS_VIP, ZOMBIE_CLASS_DURATION, ZOMBIE_CLASS_COUNTDOWN);
}

/**
 * Called when a zombie press G button. Activation of skill.
 * 
 * @param clientIndex		The client index.
 *
 * @return					Plugin_Handled to block using skill. Anything else
 *                          	(like Plugin_Continue) to allow use.
 **/
public Action ZP_OnSkillUsed(int clientIndex)
{
	// Verify that the client is connected and alive
	if(!IsPlayerExist(clientIndex))
	{
		return Plugin_Handled;
	}
	
	// Get zombie class
	if(ZP_GetClientZombieClass(clientIndex) == gZombieFast)
	{
		// Get and set speed
		gFastSpeed[clientIndex] = GetEntPropFloat(clientIndex, Prop_Data, "m_flLaggedMovementValue");  //! Store speed
		SetEntPropFloat(clientIndex, Prop_Data, "m_flLaggedMovementValue", (SKILL_SPEED_FAST + (ZP_GetClientLevel(clientIndex) * 0.03))); //! Set speed
		
		// Emit sound
		EmitSoundToAll("*/zbm3/zombie_madness1.mp3", clientIndex, SNDCHAN_VOICE, SNDLEVEL_SCREAMING);
	}
	
	// Allow usage
	return Plugin_Continue;
}

/**
 * Called when a zombie skill duration is over, for reset variables.
 * 
 * @param clientIndex		The client index.
 **/
public void ZP_OnSkillOver(int clientIndex)
{
	// Verify that the client is connected and alive
	if(!IsPlayerExist(clientIndex))
	{
		return;
	}

	// Get zombie class
	if(ZP_GetClientZombieClass(clientIndex) == gZombieFast) 
	{
		SetEntPropFloat(clientIndex, Prop_Data, "m_flLaggedMovementValue", gFastSpeed[clientIndex]); //! Return standart speed
	}
}