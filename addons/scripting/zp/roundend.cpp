/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          roundend.cpp
 *  Type:          Game 
 *  Description:   Round end event.
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
 * Delay between round ending and new round starting. (Normal)
 **/
#define ROUNDEND_DELAY 5.0

/**
 * Possible round end outcomes.
 **/
#define CSRoundEnd_ZombiesWin 	CSRoundEnd_TerroristWin
#define CSRoundEnd_HumansWin 	CSRoundEnd_CTWin

/**
 * The round is ending.
 **/
public Action RoundEndOnRoundEnd(/*void*/)
{
	// Reset server grobal bools
	bEndRound = true;
	bModeStarted = false;

	// If round end timer is running, then kill it
	EndTimer(Task_ZombieRound);
}

/**
 * Ends the round with the given outcome and delay.
 * 
 * @param flDelay	Delay before new round starts. 
 * @param iReason	The outcome of the round.
 **/
public Action CS_OnTerminateRound(float &flDelay, CSRoundEndReason &iReason)
{
	// If round didn't started
	if(!bModeStarted)
	{
		return ACTION_CHANGED;
	}
	
	// Initialize teams score
	static int nScore[2];
	
	// If there aren't clients on both teams, then stop
	if (!fnGetPlaying())
	{
		return ACTION_CHANGED;
	}
	
	// Get team
	switch(iReason)
	{
		// Zombie win
		case CSRoundEnd_ZombiesWin :
		{
			// Increment T score
			nScore[0]++;
			
			// Give bonuses and display the overlay to all clients
			RoundEndOverlay(CVAR_HUD_ZOMBIE_WIN, GetConVarInt(gCvarList[CVAR_AMMOPACKS_ZOMBIE_WIN]), GetConVarInt(gCvarList[CVAR_AMMOPACKS_HUMAN_FAIL]));
		}
		case CSRoundEnd_HumansWin :
		{
			// Increment CT score
			nScore[1]++;
			
			// Give bonuses and display the overlay to all clients
			RoundEndOverlay(CVAR_HUD_HUMAN_WIN, GetConVarInt(gCvarList[CVAR_AMMOPACKS_ZOMBIE_FAIL]), GetConVarInt(gCvarList[CVAR_AMMOPACKS_HUMAN_WIN]));
		}
	}
	
	// Set score in scoreboard
	SetTeamScore(ZP_TEAM_ZOMBIE,  nScore[0]);
	SetTeamScore(ZP_TEAM_HUMAN,   nScore[1]);
	
	// Allow to terminate
	return ACTION_CHANGED;
}

/**
 * Displays overlays to clients and give bonuses.
 * 
 * @param iConVar		The cvar define of the path to the overlay.
 * @param iZombieBonus	The amount of bonus for zombie.
 * @param iHumanBonus	The amount of bonus for human.	
 **/
void RoundEndOverlay(CvarsList iConVar, int iZombieBonus, int iHumanBonus)
{
	// Initialize char
	char sOverlay[BIG_LINE_LENGTH];
	
	// Get overlays
	GetConVarString(view_as<ConVar>(gCvarList[iConVar]), sOverlay, sizeof(sOverlay)); 	
	
	// Show overlay and give bonus
	for (int i = 1; i <= MaxClients; i++)
	{
		// Verify that the client is connected
		if (!IsPlayerExist(i, false))
			continue;

		// Display overlay to client
		SetCommandFlags("r_screenoverlay", GetCommandFlags("r_screenoverlay") & (~FCVAR_CHEAT)); 
		ClientCommand(i, "r_screenoverlay \"%s.vmt\"", sOverlay);
		SetCommandFlags("r_screenoverlay", GetCommandFlags("r_screenoverlay") | FCVAR_CHEAT);
		
		// Give bonus
		RoundEndBonus(i, iZombieBonus, iHumanBonus);
	}
}

/**
 * Timer for call round end.
 *
 * @param hTimer	The timer handle.
 **/
public Action EventRoundZombieEnd(Handle hTimer)
{
	// Clear timer
	Task_ZombieRound = NULL;

	// If there aren't clients on both teams, then stop
	if (!fnGetPlaying())
	{
		return ACTION_STOP;
	}
	
	// Terminate the round
	RoundEndGetRoundValidate();
	
	// Destroy timer
	return ACTION_STOP;
}

/**
 * Balances teams.
 **/
void RoundEndBalanceTeams(/*void*/)
{
    // Create eligible player list
    ArrayList arrayEligibleClients;
    int eligibleClients = CreateEligibleClientList(arrayEligibleClients);
    
    // If there are no eligible client's then stop
    if (!eligibleClients)
    {
        // Destroy handle
        CloseHandle(arrayEligibleClients);
        return;
    }
    
    int clientIndex;
    
    // Move all clients to T
    
    // i = Array index
    // clientIndex = client index
    for (int i = 0; i < eligibleClients; i++)
    {
        // Get client stored in array index
        clientIndex = GetArrayCell(arrayEligibleClients, i);
        
        // Switch client to T
        CS_SwitchTeam(clientIndex, ZP_TEAM_ZOMBIE);
    }
    
    // Move every other client back to CT
    
    // i = array index
    // clientIndex = client index
    for (int i = 0; i < eligibleClients; i += 2)
    {
        // Get client stored in array index
        clientIndex = GetArrayCell(arrayEligibleClients, i);
        
        // Switch client to CT
        CS_SwitchTeam(clientIndex, ZP_TEAM_HUMAN);
    }
    
    // Destroy handle
    CloseHandle(arrayEligibleClients);
}

/**
 * Check last human or zombie.
 **/
void RoundEndGetRoundCheck(/*void*/)
{
	// Round ended or just started
	if (bEndRound || bNewRound)
	{
		return;
	}

	// Last alive player, don't bother
	if (fnGetAlive() <= 1)
	{
		return;
	}

	// Last zombie disconnecting
	if (fnGetZombies() <= 0)
	{
		// Only one CT left, don't bother
		if (fnGetHumans() <= 1)
		{
			return;
		}
		
		// Show last zombie left notice
		TranslationPrintHintTextAll("Zombie Left"); 
		
		// Terminate the round with humans as the winner
		CS_TerminateRound(ROUNDEND_DELAY, CSRoundEnd_HumansWin, false);
	}
	else if (fnGetHumans() <= 0)
	{
		// Only one T left, don't bother
		if (fnGetZombies() <= 1)
		{
			return;
		}
		
		// Show last human left notice
		TranslationPrintHintTextAll("Human Left"); 

		// Terminate the round with zombies as the winner
		CS_TerminateRound(ROUNDEND_DELAY, CSRoundEnd_ZombiesWin, false);
	}
}

/**
 * Checks if the round is over.
 **/
bool RoundEndGetRoundValidate(/*void*/)
{
	// Mode didn't started yet?
	if(bNewRound)
	{
		return true;
	}

	// Get human and zombie number
	int iHumanssnum = fnGetHumans();
	int iZombiesnum = fnGetZombies();

	// If there are no clients on either teams, then stop
	if (!iZombiesnum && !iHumanssnum)
	{
		// Round isn't active.
		return true;
	}

	// If there are clients on both teams, then stop
	if (iZombiesnum && iHumanssnum)
	{
		// Round isn't over.
		return true;
	}

	// We know here, that either iZombiesnum or iHumanssnum is 0. (not both)

	// If there are no zombies, that means there must be humans, they win the round
	if (iHumanssnum)
	{
		CS_TerminateRound(ROUNDEND_DELAY, CSRoundEnd_HumansWin, false);
	}
	// If there are zombies, then zombies won the round
	else
	{
		CS_TerminateRound(ROUNDEND_DELAY, CSRoundEnd_ZombiesWin, false);
	}

	// Round is over.
	return false;
}

/**
 * Give ammopack bonuses.
 * 
 * @param clientIndex	The client index.
 * @param iZombieBonus	The amount of bonus for zombie.
 * @param iHumanBonus	The amount of bonus for human.	
 **/
void RoundEndBonus(int clientIndex, int iZombieBonus, int iHumanBonus)
{
	// Get real player index from event key 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);
	
	// Give bonus ammopacks
	cBasePlayer->m_nAmmoPacks += cBasePlayer->m_bZombie ? iZombieBonus : iHumanBonus;
}