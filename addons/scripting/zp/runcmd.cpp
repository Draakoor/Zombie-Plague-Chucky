/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          runcmd.cpp
 *  Type:          Game
 *  Description:   Hook buttons, and initiliaze commands and menus.
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
 * Called when a clients movement buttons are being processed.
 *  
 * @param clientIndex	The client index.
 * @param iButtons		Copyback buffer containing the current commands (as bitflags - see entity_prop_stocks.inc).
 **/ 
Action RunCmdOnPlayerRunCmd(int clientIndex, int &iButtons)
{
	// Get real player index from event key 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);

	// Update client money HUD
	cBasePlayer->m_iAccount(cBasePlayer->m_nAmmoPacks);
	
	// If player is alive ?
	if(IsPlayerAlive(view_as<int>(cBasePlayer)))
	{
		if(iButtons & IN_USE)
		{
			// Switch tool
			OnPlayerTools(cBasePlayer);
		}
		else if((iButtons & IN_JUMP) && (iButtons & IN_DUCK))
		{	
			// Do leap jump
			OnPlayerLeapJump(cBasePlayer);
		}
		else
		{
			// Restore health
			OnPlayerHealthRegen(cBasePlayer);
		}
	}
	// Or block bot control button
	else 
	{
		iButtons &= (~IN_USE);
		return ACTION_CHANGED;
	}
	
	return ACTION_CONTINUE;
}

/**
 * Called when player press E button. (Use)
 *
 * @param cBasePlayer	The client index.
 **/
void OnPlayerTools(CBasePlayer* cBasePlayer)
{
	// Returns the game time based on the game tick
	float flCurrentTime = GetGameTime();
	
	// Cooldown not over yet
	if (flCurrentTime - cBasePlayer->m_flToolCountDown < 0.3)
	{
		return;
	}

	// If zombie's nightvision ?
	if(cBasePlayer->m_bZombie)
	{
		// Switch nightvision
		cBasePlayer->m_bNightVisionOn = !cBasePlayer->m_bNightVisionOn;
		
		// Emit sound
		cBasePlayer->InputEmitAISound(SNDCHAN_VOICE, SNDLEVEL_NORMAL, cBasePlayer->m_bNightVisionOn ? "zbm3/nvg_on.mp3" : "zbm3/nvg_off.mp3");		
	}
	// If human's flashlight ?
	else
	{
		// Switch flashlight
		cBasePlayer->m_bFlashLightOn(true);
		
		// Emit sound
		cBasePlayer->InputEmitAISound(SNDCHAN_VOICE, SNDLEVEL_NORMAL, "zbm3/flashlight1.mp3");
	}
	
	// Update countdown time
	cBasePlayer->m_flToolCountDown = flCurrentTime;
}

/**
 * Called when player do leap jump.
 *
 * @param cBasePlayer	The client index.
 **/
void OnPlayerLeapJump(CBasePlayer* cBasePlayer)
{
	// Initialize countdown variable
	float flCountDown;
	
	// If player is zombie
	if(cBasePlayer->m_bZombie)
	{
		// If player is nemesis
		if(cBasePlayer->m_bNemesis)
		{
			// If nemesis leap disabled
			if(!GetConVarBool(gCvarList[CVAR_LEAP_NEMESIS])) return;
			
			// Get countdown time
			flCountDown = GetConVarFloat(gCvarList[CVAR_LEAP_NEMESIS_COUNTDOWN]);
		}
		else
		{
			// If zombie leap disabled
			switch(GetConVarInt(gCvarList[CVAR_LEAP_ZOMBIE]))
			{
				case 0 : return;
				case 2 : if (fnGetZombies() > 1) return;
			}
			
			// Get countdown time
			flCountDown = GetConVarFloat(gCvarList[CVAR_LEAP_ZOMBIE_COUNTDOWN]);
		}
	}
	else
	{
		// If player is survivor
		if (cBasePlayer->m_bSurvivor)
		{
			// If survivor leap disabled
			if (!GetConVarBool(gCvarList[CVAR_LEAP_SURVIVOR])) return;
			
			// Get countdown time
			flCountDown = GetConVarFloat(gCvarList[CVAR_LEAP_SURVIVOR_COUNTDOWN]);
		}
		else return; // If player is human, stop
	}
	
	// Returns the game time based on the game tick
	float flCurrentTime = GetGameTime();
	
	// Cooldown not over yet
	if (flCurrentTime - cBasePlayer->m_flLeapCountDown < flCountDown)
	{
		return;
	}

	// If not on the ground, stop
	if (!(cBasePlayer->m_iFlags & FL_ONGROUND))
	{
		return;
	}
	
	// Initialize some floats
	float flEyeAngle[3];
	float flAbsOrigin[3];
	float flVelocity[3];
	
	// Client location and view direction
	cBasePlayer->m_flGetOrigin(flAbsOrigin);
	cBasePlayer->m_flGetEyeAngles(flEyeAngle);
	
	// Store eye angle
	float flEyeAngleZero = flEyeAngle[0];	
	flEyeAngle[0] = -30.0;
	
	// Get angle vectors
	GetAngleVectors(flEyeAngle, flVelocity, NULL_VECTOR, NULL_VECTOR);
	
	// Scales a force vector
	ScaleVector(flVelocity, cBasePlayer->m_bSurvivor ? GetConVarFloat(gCvarList[CVAR_LEAP_SURVIVOR_FORCE]) : (cBasePlayer->m_bNemesis ? GetConVarFloat(gCvarList[CVAR_LEAP_NEMESIS_FORCE]) : GetConVarFloat(gCvarList[CVAR_LEAP_ZOMBIE_FORCE])));
	
	// Restore eye angle
	flEyeAngle[0] = flEyeAngleZero;
	
	// Toss client
	cBasePlayer->m_iTeleportPlayer(flAbsOrigin, flEyeAngle, flVelocity);
	
	// Update leap time
	cBasePlayer->m_flLeapCountDown = flCurrentTime;
}

/**
 * Called when player restore health.
 *
 * @param cBasePlayer	The client index.
 **/
void OnPlayerHealthRegen(CBasePlayer* cBasePlayer)
{
	// If health restoring disabled
	if(!GetConVarBool(gCvarList[CVAR_ZOMBIE_RESTORE]))
	{
		return;
	}
	
	// Verify that the client is zombie
	if(!cBasePlayer->m_bZombie)
	{
		return;
	}
	
	// Get client's current speed
	float flVelocity[3];
	
	// Returns the game time based on the game tick
	if (!ToolsGetVelosity(view_as<int>(cBasePlayer), flVelocity))
	{
		if (cBasePlayer->m_flHealthRegen == 0.0) cBasePlayer->m_flHealthRegen = GetGameTime();
	}
	else cBasePlayer->m_flHealthRegen = 0.0;
	
	// Restoring available in client is non move
	if (cBasePlayer->m_flHealthRegen)
	{
		if (cBasePlayer->m_iHealth < ZombieGetHealth(cBasePlayer->m_nZombieClass) && GetGameTime() - cBasePlayer->m_flHealthRegen == GetConVarFloat(gCvarList[CVAR_ZOMBIE_RESTORE_TIME]) + 1.0)
		{
			// Count new amount of health
			int iNewHealth = cBasePlayer->m_iHealth + GetConVarInt(gCvarList[CVAR_ZOMBIE_RESTORE_AMOUNT]);
			
			// If new health more, than class health
			if(iNewHealth > ZombieGetHealth(cBasePlayer->m_nZombieClass))
			{
				iNewHealth = ZombieGetHealth(cBasePlayer->m_nZombieClass);
			}
			
			// Update health
			cBasePlayer->m_iHealth = iNewHealth;
			cBasePlayer->m_flHealthRegen += 1.0;
			
			// Create regeneration effect
			VEffectFireEffect(view_as<int>(cBasePlayer));
			
			// Emit heal sound
			cBasePlayer->InputEmitAISound(SNDCHAN_VOICE, SNDLEVEL_CONVO, "zbm3/voodoo_heal.mp3");
		}
	}
}