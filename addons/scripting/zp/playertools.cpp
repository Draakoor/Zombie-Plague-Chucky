/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          playertools.cpp
 *  Type:          Game 
 *  Description:   Useful stocks.
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
 * Reset all values.
 *
 * @param clientIndex	The client index.
 **/
void ToolsResetVars(int clientIndex)
{
	// Get real player index from event key 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);

	// Reset all variables
	cBasePlayer->m_bZombie 				= false;
	cBasePlayer->m_bSurvivor 			= false;
	cBasePlayer->m_bNemesis 			= false;
	cBasePlayer->m_bSkill		 		= false;
	cBasePlayer->m_nSkillCountDown 		= 0;
	cBasePlayer->m_nZombieClass 		= 0;
	cBasePlayer->m_nZombieNext 			= 0;
	cBasePlayer->m_nModelIndex 			= 0;
	cBasePlayer->m_nLastBoughtAmount 	= 0;
	cBasePlayer->m_nRespawnTimes 		= 0;
	cBasePlayer->m_nAmmoPacks 			= 0;
	cBasePlayer->m_nDamageFilter 		= 0;
	cBasePlayer->m_iLevel 				= 0;
	cBasePlayer->m_bRespawn 		 	= ZP_TEAM_HUMAN;
}

/**
 * Get current speed of a player.
 *
 * @param clientIndex	The client index.
 * @param flVelocity	The vector for storing velocity.
 **/
float ToolsGetVelosity(int clientIndex, float flVelocity[3])
{
	// Initialize velocity vector
	GetEntPropVector(clientIndex, Prop_Data, "m_vecVelocity", flVelocity);
	
	// Get speed of the client
	return SquareRoot(Pow(flVelocity[0], 2.0) + Pow(flVelocity[1], 2.0));
}

/**
 * Display level overlays.
 *
 * @param cBasePlayer	The client index.
 * @param iLevel		The level of the player.
 **/
void ToolsSwitchLevel(CBasePlayer* cBasePlayer, int iLevel)
{
	// Initilize overlay path
	char sOverlay[NORMAL_LINE_LENGTH];
	
	// If level system disabled or empty, just reset any overlay on client screen
	if(!iLevel || !GetConVarBool(gCvarList[CVAR_LVL_SYSTEM]))
	{
		strcopy(sOverlay, sizeof(sOverlay), "");
	}
	else
	{
		// Load level overlay
		GetConVarString(gCvarList[CVAR_HUD_LEVELS], sOverlay, sizeof(sOverlay));
		
		// Initialize number string
		char sNum[4];
		IntToString(iLevel, sNum, sizeof(sNum));
		ReplaceString(sOverlay, sizeof(sOverlay), "@", sNum);
	}
		
	// Display overlay to client
	SetCommandFlags("r_screenoverlay", GetCommandFlags("r_screenoverlay") & (~FCVAR_CHEAT)); 
	ClientCommand(view_as<int>(cBasePlayer), "r_screenoverlay \"%s\"", sOverlay);
	SetCommandFlags("r_screenoverlay", GetCommandFlags("r_screenoverlay") | FCVAR_CHEAT);
	
	// Play level sound
	if(iLevel > 1) cBasePlayer->InputEmitAISound(SNDCHAN_VOICE, SNDLEVEL_CONVO, "zbm3/levelup.mp3");
}

/**
 * Update zombie speed and gravity.
 *
 * @param cBasePlayer	The client index.
 * @param iLevel		The level of the player.
 **/
void ToolsIncreaseSpeed(CBasePlayer* cBasePlayer, int iLevel)
{
	// Validate that player is a zombie
	if(cBasePlayer->m_bSkill || !cBasePlayer->m_bZombie)
		return;
	
	// Get zombie increase level ratio
	float flIncrease 						= GetConVarFloat(gCvarList[CVAR_ZOMBIE_LEVEL_RATIO]);
	
	// Change statistic values
	cBasePlayer->m_flLaggedMovementValue 	= gZombieClassData[cBasePlayer->m_nZombieClass][gZombieClassSpeed] 		+ (flIncrease * iLevel); 
	cBasePlayer->m_flGravity 			 	= gZombieClassData[cBasePlayer->m_nZombieClass][gZombieClassGravity] 	+ (flIncrease * iLevel);
}

/**
 * Kill all player's timer.
 *
 * @param clientIndex	The client index.
 **/
void ToolsKillAllTimers(int clientIndex)
{
	EndTimer(Task_ZombieRespawn[clientIndex]);
	EndTimer(Task_ZombieFreezed[clientIndex]);
	EndTimer(Task_ZombieSkill[clientIndex]);
}

/**
 * Respawn a player.
 *
 * @param clientIndex	The client index.
 **/
void ToolsRespawn(int clientIndex)
{
	// Get real player index from event key 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);
	
	// If player in spectator team, then stop
	if(!fnGetTeam(view_as<int>(cBasePlayer)))
	{
		return;
	}
	
	// Respawn as zombie?
	if (GetConVarInt(gCvarList[CVAR_RESPAWN_DEATHMATCH]) == 2 || (GetConVarInt(gCvarList[CVAR_RESPAWN_DEATHMATCH]) == 3 && Math_GetRandomInt(0, 1)) || (GetConVarInt(gCvarList[CVAR_RESPAWN_DEATHMATCH]) == 4 && fnGetZombies() < fnGetAlive()/2))
	{
		cBasePlayer->m_bRespawn = ZP_TEAM_ZOMBIE;
	}
	else
	{
		cBasePlayer->m_bRespawn = ZP_TEAM_HUMAN;
	}
	
	// Override respawn as zombie setting on nemesis and survivor rounds
	if (gMode == MODE_SURVIVOR) cBasePlayer->m_bRespawn = ZP_TEAM_ZOMBIE;
	else if (gMode == MODE_NEMESIS) cBasePlayer->m_bRespawn = ZP_TEAM_HUMAN;
	
	// Respawn a player
	cBasePlayer->m_iRespawnPlayer();
	
	// Switch team to default
	cBasePlayer->m_iTeamNum = ZP_TEAM_HUMAN;
}