/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          gamemodes.cpp
 *  Type:          Core
 *  Description:   Select mode types.
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
 * Game modes.
 **/
enum
{
	MODE_NONE = -1,
	MODE_INFECTION,
	MODE_MULTI,
	MODE_SWARM,
	MODE_NEMESIS,
	MODE_SURVIVOR,
	MODE_ARMAGEDDON
}

/**
 * Main timer for start zombie round.
 *
 * @param hTimer	The timer handle.
 **/
public Action EventMakeZombie(Handle hTimer)
{
	// If round is didn't start yet
	if(bNewRound && !bModeStarted && !bEndRound)
	{
		// Check players
		switch(fnGetAlive())
		{
			// Destroy timer
			case 0 : return ACTION_STOP;
			// Wait other players
			case 1 : return ACTION_CONTINUE;
			// If players exists
			default :
			{
				// If counter is count ?
				if(gZombieCount)
				{
					// Play counter sounds
					if(1 <= gZombieCount <= 10) SoundsTransmitToAll("ZOMBIE_COUNTER_SOUNDS", gZombieCount);
					
					// If help messages enabled
					if(GetConVarBool(gCvarList[CVAR_HELP_MESSAGES]))
					{
						// Show help information
						if(gZombieCount == (GetConVarInt(gCvarList[CVAR_ROUND_START])-2)) TranslationPrintHintTextAll("Help Message");
					}
					
					// Show counter message
					if(gZombieCount <= 20)
					{
						// Initialize string and format her
						TranslationPrintHintTextAll("Zombie comming", gZombieCount);
					}
				}
				// If else than start game mode
				else 
				{
					Make_A_Zombie(MODE_NONE);
					return ACTION_STOP;
				}
				
				// Substitute second
				gZombieCount--;
				return ACTION_CONTINUE;
			}
		}
	}

	// Destroy timer
	return ACTION_STOP;
}

/**
 * Called right before mode is started.
 *
 * @param modeIndex		The mod index. 
 * @param clientIndex	(Optional) The client index.
 **/
void Make_A_Zombie(int modeIndex, int clientIndex = 0)
{
	// Get real player index from event key
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);
	
	// Get alive players count
	int iPlayersnum = fnGetAlive();
	
	// Initialize max amount of zombies 
	int iMaxZombies;

	// Mode fully started
	bNewRound = false;
	bModeStarted = true;

	// Store current mode
	static int iLastMode;
	
	if(modeIndex == MODE_NONE && iLastMode != MODE_MULTI && Math_GetRandomInt(1, GetConVarInt(gCvarList[CVAR_MODE_MULTI_CHANCE])) == GetConVarInt(gCvarList[CVAR_MODE_MULTI]) && iPlayersnum > GetConVarInt(gCvarList[CVAR_MODE_MULTI_MIN]) || modeIndex == MODE_MULTI)
	{
		// Show mod info
		iMaxZombies = RoundToCeil(iPlayersnum * GetConVarFloat(gCvarList[CVAR_MODE_MULTI_RATIO]));
		TranslationPrintHintTextAll("Multi mode");
		gMode = MODE_MULTI;
		iLastMode = MODE_MULTI;
		
		// Make random zombies
		TurnPlayersIntoZombie(cBasePlayer, iMaxZombies, iPlayersnum);
		
		// Player game round sounds
		SoundsTransmitToAll("ZOMBIE_MULTI_ROUND");
	}
	else if(modeIndex == MODE_NONE && iLastMode != MODE_SWARM && Math_GetRandomInt(1, GetConVarInt(gCvarList[CVAR_MODE_SWARM_CHANCE])) == GetConVarInt(gCvarList[CVAR_MODE_SWARM]) && iPlayersnum > GetConVarInt(gCvarList[CVAR_MODE_SWARM_MIN]) || modeIndex == MODE_SWARM)
	{
		// Show mod info
		iMaxZombies = RoundToCeil(iPlayersnum / GetConVarFloat(gCvarList[CVAR_MODE_SWARM_RATIO]));
		TranslationPrintHintTextAll("Multi swarm"); 
		gMode = MODE_SWARM;
		iLastMode = MODE_SWARM;
		
		// Make random zombies
		TurnPlayersIntoZombie(cBasePlayer, iMaxZombies, iPlayersnum);
		
		// Player game round sounds
		SoundsTransmitToAll("ZOMBIE_SWARM_ROUND");
	}
	else if(modeIndex == MODE_NONE && iLastMode != MODE_SURVIVOR && Math_GetRandomInt(1, GetConVarInt(gCvarList[CVAR_MODE_SURVIVOR_CHANCE])) == GetConVarInt(gCvarList[CVAR_MODE_SURVIVOR]) && iPlayersnum > GetConVarInt(gCvarList[CVAR_MODE_SURVIVOR_MIN]) || modeIndex == MODE_SURVIVOR)
	{
		// Get a random player
		if(modeIndex == MODE_NONE)
		{
			cBasePlayer = view_as<CBasePlayer>(fnGetRandomAlive(Math_GetRandomInt(1, iPlayersnum)));
		}
		
		// Show mod info
		iMaxZombies = (iPlayersnum - 1);
		TranslationPrintHintTextAll("Mode survivor"); 
		gMode = MODE_SURVIVOR;
		iLastMode = MODE_SURVIVOR;

		// Turn player into survivor
		MakeHumanIntoSurvivor(view_as<int>(cBasePlayer));
		
		// Make random zombies
		TurnPlayersIntoZombie(cBasePlayer, iMaxZombies, iPlayersnum);

		// Player game round sounds
		SoundsTransmitToAll("ZOMBIE_SURVIVOR_ROUND");
	}
	else if(modeIndex == MODE_NONE && iLastMode != MODE_ARMAGEDDON && Math_GetRandomInt(1, GetConVarInt(gCvarList[CVAR_MODE_ARMAGEDDON_CHANCE])) == GetConVarInt(gCvarList[CVAR_MODE_ARMAGEDDON]) && iPlayersnum > GetConVarInt(gCvarList[CVAR_MODE_ARMAGEDDON_MIN]) || modeIndex == MODE_ARMAGEDDON)
	{
		// Show mod info
		iMaxZombies = RoundToCeil(iPlayersnum / 2.0);
		TranslationPrintHintTextAll("Mode armageddon"); 
		gMode = MODE_ARMAGEDDON;
		iLastMode = MODE_ARMAGEDDON;
		
		// Make random zombies
		TurnPlayersIntoZombie(cBasePlayer, iMaxZombies, iPlayersnum);

		// Player game round sounds
		SoundsTransmitToAll("ZOMBIE_ARMAGEDDON_ROUND");
	}
	else
	{
		// Get a random player
		if(modeIndex == MODE_NONE)
		{
			cBasePlayer = view_as<CBasePlayer>(fnGetRandomAlive(Math_GetRandomInt(1, iPlayersnum)));
		}
		
		if(modeIndex == MODE_NONE && iLastMode != MODE_NEMESIS && Math_GetRandomInt(1, GetConVarInt(gCvarList[CVAR_MODE_NEMESIS_CHANCE])) == GetConVarInt(gCvarList[CVAR_MODE_NEMESIS]) && iPlayersnum > GetConVarInt(gCvarList[CVAR_MODE_NEMESIS_MIN]) || modeIndex == MODE_NEMESIS)
		{
			// Show mod info
			TranslationPrintHintTextAll("Mode nemesis");
			gMode = MODE_NEMESIS;
			iLastMode = MODE_NEMESIS;
			
			// Make a nemesis
			InfectHumanToZombie(view_as<int>(cBasePlayer), false, false, true);
			
			// Player game round sounds
			SoundsTransmitToAll("ZOMBIE_NEMESIS_ROUND");
		}
		else
		{
			// Show mod info
			gMode = MODE_INFECTION;
			iLastMode = MODE_INFECTION;
			TranslationPrintHintTextAll("Mode infection");
			
			// Make a zombie
			InfectHumanToZombie(view_as<int>(cBasePlayer));
		}
		
		// Return ammopacks
		cBasePlayer->m_nAmmoPacks += cBasePlayer->m_nLastBoughtAmount;
	}
	
	// Remaining players should be humans (CTs)
	for (cBasePlayer = view_as<CBasePlayer>(1); view_as<int>(cBasePlayer) <= MaxClients; view_as<int>(cBasePlayer++))
	{
		// Only those of them who aren't zombies
		if(!IsPlayerExist(view_as<int>(cBasePlayer)) || cBasePlayer->m_bZombie)
			continue;

		// Switch to CT
		cBasePlayer->m_iTeamNum = ZP_TEAM_HUMAN;

		// Set xray for zombie
		cBasePlayer->m_bSetGlow(GetConVarBool(gCvarList[CVAR_ZOMBIE_XRAY]) ? true : false);
		
		// Turn specified amount of players into survivors
		if(gMode == MODE_ARMAGEDDON) 
		{
			MakeHumanIntoSurvivor(view_as<int>(cBasePlayer));
		}
	}
	
	// Round start forward
	APIOnModStarted(gMode);
}

/**
 * Turn random players into the zombies.
 *
 * @param cBasePlayer		The index of the client.
 * @param iMaxZombies		The amount of zombies.
 * @param iPlayersnum		The number of players.
 **/
void TurnPlayersIntoZombie(CBasePlayer* cBasePlayer, int iMaxZombies, int iPlayersnum)
{
	// Initialize number of zombie 
	int iZombies;

	// Randomly turn players into zombies
	while (iZombies < iMaxZombies)
	{
		// Choose random guy
		cBasePlayer = view_as<CBasePlayer>(fnGetRandomAlive(Math_GetRandomInt(1, iPlayersnum)));
		
		// Verify that the client is connected and alive and non-zombie
		if(!IsPlayerExist(view_as<int>(cBasePlayer)) || cBasePlayer->m_bZombie || cBasePlayer->m_bSurvivor)
			continue;
		
		// Make a zombie
		InfectHumanToZombie(view_as<int>(cBasePlayer), false, false, (gMode == MODE_ARMAGEDDON) ? true : false);
		
		// Return ammopacks
		cBasePlayer->m_nAmmoPacks += cBasePlayer->m_nLastBoughtAmount;
		
		// Add zombie number to counter
		iZombies++;
	}
}