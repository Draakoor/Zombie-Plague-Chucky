/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          commands.cpp
 *  Type:          Module 
 *  Description:   Console command initilization and hooking.
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
 * Commands are created.
 * 
 * @param bUnhook	If true, then unhook all listeners, if false, then hook.
 **/
void CommandsInit(bool bUnhook = false)
{
	// If unhook is true, then continue
	if (bUnhook)
	{
		// Unhook all listeners
		RemoveCommandListener(EventCommandHook, "+lookatweapon");
		RemoveCommandListener(EventCommandHook, "kill"); 
		RemoveCommandListener(EventCommandHook, "buyammo1"); 
		RemoveCommandListener(EventCommandHook, "buyammo2"); 
		RemoveCommandListener(EventCommandHook, "drop");
		RemoveCommandListener(EventCommandHook, "jointeam");
		UnhookUserMessage(GetUserMessageId("TextMsg"), EventMessageHook, true);
		
		// Stop after unhooking listeners
		return;
	}
	
	// Hook commands
	AddCommandListener(EventCommandHook, "+lookatweapon");
	AddCommandListener(EventCommandHook, "kill");
	AddCommandListener(EventCommandHook, "buyammo1");
	AddCommandListener(EventCommandHook, "buyammo2");
	AddCommandListener(EventCommandHook, "drop");
	AddCommandListener(EventCommandHook, "jointeam");
	
	// Hook messages
	HookUserMessage(GetUserMessageId("TextMsg"), EventMessageHook, true);

	// Forward event to modules (create commands)
	ConfigOnCommandsCreate();
	LogOnCommandsCreate();
	#if defined ADD_VERSION_INFO
		VersionOnCommandsCreate();
	#endif
}

/**
 * Called when a bit buffer based usermessage is hooked
 *
 * @param iMessage		Message index.
 * @param sMessage		Handle to the input bit buffer.
 * @param iPlayers		Array containing player indexes.
 * @param playersNum	Number of players in the array.
 * @param bReliable		True if message is reliable, false otherwise.
 * @param bInit			True if message is an initmsg, false otherwise.
 **/
public Action EventMessageHook(UserMsg iMessage, BfRead sMessage, const int[] iPlayers, int playersNum, bool bReliable, bool bInit)
{
	// Initialize some chars
	char sBuffer[NORMAL_LINE_LENGTH]; 
	PbReadString(sMessage, "params", sBuffer, sizeof(sBuffer), 0); 

	// Block money gain message
	if (StrEqual(sBuffer, "#Player_Cash_Award_Killed_Enemy"))
	{ 
		return ACTION_HANDLED; 
	} 

	// Allow messages
	return ACTION_CONTINUE; 
} 

/**
 * Hook client commands
 *
 * @param clientIndex	The client index.
 * @param sCommand		Command name, lower case. To get name as typed, use GetCmdArg() and specify argument 0.
 * @param iArgCommands	Argument count.
 **/
public Action EventCommandHook(int clientIndex, const char[] sCommand, int iArgCommands)
{
	// Get real player index from event key 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);

	// Verify that the client is connected and alive
	if(!IsPlayerExist(view_as<int>(cBasePlayer), false))
	{
		return ACTION_HANDLED;
	}
	
	// Get hooked command
	switch(sCommand[0])
	{
		// Jointeam
		case 'j' :
		{
			return IsPlayerAlive(view_as<int>(cBasePlayer)) ? ACTION_STOP : ACTION_CONTINUE;
		}
		// Suicide (kill)
		case 'k' : 
		{
			return GetConVarBool(gCvarList[CVAR_RESPAWN_SUICIDE]) ? ACTION_CONTINUE : ACTION_HANDLED;
		}
		// Main menu (+lookatweapon)
		case '+' :
		{
			OpenMainMenu(cBasePlayer, true);
			return ACTION_HANDLED;
		}
		// Drop
		case 'd' :
		{
			return cBasePlayer->m_bSurvivor ? ACTION_HANDLED : OnPlayerSkillStart(cBasePlayer);
		}
		// Buyammo
		default :
		{
			if(StrEqual(sCommand, "buyammo1") || StrEqual(sCommand, "buyammo2"))
			{
				return OnPlayerAmmoPurchase(cBasePlayer, sCommand);
			}
		}
	}

	// Allow other commands
	return ACTION_CONTINUE;
}

/**
 * Called when player buy ammo.
 *
 * @param cBasePlayer	The client index.
 * @param sCommand		Command name, lower case. To get name as typed, use GetCmdArg() and specify argument 0.
 **/
Action OnPlayerAmmoPurchase(CBasePlayer* cBasePlayer, const char[] sCommand)
{
	// Verify that infinity ammo disabled
	if(cBasePlayer->m_bSurvivor || GetConVarInt(gCvarList[CVAR_HUMAN_INF_AMMO]))
	{
		return ACTION_HANDLED;
	}

	// If not enought ammopacks
	if(cBasePlayer->m_nAmmoPacks < 1)
	{
		TranslationPrintHintText(view_as<int>(cBasePlayer), "You can not buy it");
		return ACTION_HANDLED;
	}
	
	// Get weapon entity
	int weaponIndex = GetPlayerWeaponSlot(view_as<int>(cBasePlayer), (StrEqual(sCommand, "buyammo1")) ? WEAPON_SLOT_PRIMARY : WEAPON_SLOT_SECONDARY); 
	
	// If weapon isn't valid, then stop
	if (weaponIndex != WEAPON_SLOT_INVALID) 
	{
		// Initialize offset variables
		int iAmmoType = GetEntProp(weaponIndex, Prop_Send, "m_iPrimaryAmmoType"); 
		int iCarryAmmo = GetEntProp(view_as<int>(cBasePlayer), Prop_Send, "m_iAmmo", _, iAmmoType);  
		
		// Get player weapon
		char sClassname[NORMAL_LINE_LENGTH];
		GetEntityClassname(weaponIndex, sClassname, sizeof(sClassname));
		WeaponsGetAlias(sClassname, sizeof(sClassname));
		
		// Initialize max ammo amount
		int iMaxAmmo = CS_WeaponMaxAmmo[CS_AliasToWeaponID(sClassname)];

		// Max carry ammo 
		if(iCarryAmmo < iMaxAmmo)
		{
			// Give ammo to the client
			SetEntProp(view_as<int>(cBasePlayer), Prop_Send, "m_iAmmo", iMaxAmmo, _, iAmmoType); 

			// Remove ammopacks
			cBasePlayer->m_nAmmoPacks--;

			// Emit buy sound
			cBasePlayer->InputEmitAISound(SNDCHAN_VOICE, SNDLEVEL_CONVO, "zbm3/ammo.mp3");
		}
	}
	
	// Allow command
	return ACTION_CONTINUE;
}

/**
 * Called when player press drop button. (G)
 *
 * @param cBasePlayer	The client index.
 **/
Action OnPlayerSkillStart(CBasePlayer* cBasePlayer)
{
	// Verify that the client is alive
	if(!IsPlayerAlive(view_as<int>(cBasePlayer)))
	{
		return ACTION_CONTINUE;
	}
	
	// Verify that the client is zombie and skills are avalible
	if(cBasePlayer->m_bZombie && !cBasePlayer->m_bNemesis && !cBasePlayer->m_bSkill && !cBasePlayer->m_nSkillCountDown)
	{
		// Block skills for some zombies without data
		if(!ZombieGetSkillDuration(cBasePlayer->m_nZombieClass) && !ZombieGetSkillCountDown(cBasePlayer->m_nZombieClass))
		{
			return ACTION_HANDLED;
		}
		
		// Call forward
		Action iResult = APIOnSkillUsed(cBasePlayer);
		
		// Block skill usage
		if(iResult == ACTION_HANDLED)
		{
			return ACTION_HANDLED;
		}

		// Set skill
		cBasePlayer->m_bSkill = true;
		
		// Set timer for removing skill usage
		EndTimer(Task_ZombieSkill[view_as<int>(cBasePlayer)]);
		Task_ZombieSkill[view_as<int>(cBasePlayer)] = CreateTimer(float(ZombieGetSkillDuration(cBasePlayer->m_nZombieClass)), EventSkillZombie, view_as<int>(cBasePlayer));
		
		// Block command
		return ACTION_HANDLED;
	}
	
	// Allow command
	return ACTION_CONTINUE;
}

/**
 * Timer for remove skill usage.
 *
 * @param hTimer		The timer handle.
 * @param clientIndex	The client index.
 **/
public Action EventSkillZombie(Handle hTimer, any clientIndex)
{
	// Get real player index from event key 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);
	
	// Clear timer
	Task_ZombieSkill[view_as<int>(cBasePlayer)] = NULL;
	
	// Verify that the client is connected and alive
	if(!IsPlayerExist(view_as<int>(cBasePlayer)))
	{
		return ACTION_STOP;
	}
	
	// Call forward
	APIOnSkillOver(cBasePlayer);

	// Remove skill using and set countdown
	cBasePlayer->m_bSkill = false;
	cBasePlayer->m_nSkillCountDown = ZombieGetSkillCountDown(cBasePlayer->m_nZombieClass);
	
	// Create counter
	CreateTimer(1.0, EventZombieCountDown, view_as<int>(cBasePlayer), TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
	
	// Destroy timer
	return ACTION_STOP;
}

/**
 * Timer for show skill countdown.
 *
 * @param hTimer		The timer handle.
 * @param clientIndex	The client index.
 **/
public Action EventZombieCountDown(Handle hTimer, any clientIndex)
{
	// Get real player index from event key 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);
	
	// Verify that the client is connected and zombie
	if(IsPlayerExist(view_as<int>(cBasePlayer)) && cBasePlayer->m_bZombie && !cBasePlayer->m_bNemesis)
	{
		// Reset counter
		cBasePlayer->m_nSkillCountDown--;
		
		// If counter is over
		if(!cBasePlayer->m_nSkillCountDown)
		{
			// Show info
			TranslationPrintHintText(view_as<int>(cBasePlayer), "Skill ready");

			// Destroy timer
			return ACTION_STOP;
		}

		// Show counter
		TranslationPrintHintText(view_as<int>(cBasePlayer), "Countdown", cBasePlayer->m_nSkillCountDown);
		
		// Allow counter
		return ACTION_CONTINUE;
	}
	
	// Destroy timer
	return ACTION_STOP;
}