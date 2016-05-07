/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          admintools.cpp
 *  Type:          Core 
 *  Description:   Functions for admin privileges.
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
 * Admin commands.
 **/
enum AdminMenuMode
{
	AdminMode_Human = 0,
	AdminMode_Zombie,
	AdminMode_Respawn,
	AdminMode_Nemesis,
	AdminMode_Survivor,
	AdminMode_Ammopack,
	AdminMode_Armageddon,
	AdminMode_Multi,
	AdminMode_Swarm
};

/**
 * List of operation types to specify the category of a admin operation.
 **/
enum OperationTypes
{
	OperationType_Invalid = -1,     /** Invalid operation type. */
	OperationType_Generic,    		/** Generic admin; required for admins. */
	OperationType_Config,    		/** Execute config files. */
	OperationType_Reservation,		/** Reserved slot access. */
	OperationType_Kick,				/** Kick other players. */
	OperationType_Slay,				/** Slay/harm other players. */  
	OperationType_Changemap,		/** Change the map or major gameplay features. */
	OperationType_VIP,    			/** Custom Group 1 */
	OperationType_Custom2,			/** Custom Group 2 */
	OperationType_Custom3,			/** Custom Group 3 */
	OperationType_Custom4,			/** Custom Group 4 */
	OperationType_Custom5,			/** Custom Group 5 */
	OperationType_Custom6			/** Custom Group 6 */
}

/**
 * Returns whether a player is allowed to do a certain operation or not.
 *
 * @param clientIndex	The client index.
 * @return				True if allowed, false otherwise.
 **/
stock bool IsClientPrivileged(int clientIndex, OperationTypes operationType = OperationType_Generic)
{
	// Verify that the client is connected
	if(!IsPlayerExist(clientIndex, false))
	{
		return false;
	}

	/*********************************
	 *                               *
	 *   FLAG BASED AUTHENTICATION   *
	 *                               *
	 *********************************/
	 
	AdminFlag iFlag;
	
	// Check operation type
	switch (operationType)
	{
		case OperationType_Generic :
		{
			iFlag = Admin_Generic;
		}
		case OperationType_Config :
		{
			iFlag = Admin_Config;
		}
		case OperationType_Reservation :
		{
			iFlag = Admin_Reservation;
		}
		case OperationType_Kick :
		{
			iFlag = Admin_Kick;
		}
		case OperationType_Slay :
		{
			iFlag = Admin_Slay;
		}
		case OperationType_Changemap :
		{
			iFlag = Admin_Changemap;
		}
		case OperationType_VIP :
		{
			iFlag = Admin_Custom1;
		}
		case OperationType_Custom2 :
		{
			iFlag = Admin_Custom2;
		}
		case OperationType_Custom3 :
		{
			iFlag = Admin_Custom3;
		}
		case OperationType_Custom4 :
		{
			iFlag = Admin_Custom4;
		}
		case OperationType_Custom5 :
		{
			iFlag = Admin_Custom5;
		}
		case OperationType_Custom6 :
		{
			iFlag = Admin_Custom6;
		}
		default :
		{
			// Invalid operation type
			return false;
		}
	}

	// Retrieves a client's AdminId
	AdminId iD = GetUserAdmin(clientIndex);

	// Validate id
	if (iD == INVALID_ADMIN_ID)
	{
		return false;
	}
	
	return GetAdminFlag(iD, iFlag);
}

/**
 * Returns whether a player is in a spesific group or not.
 *
 * @param clientIndex	The client index.
 * @param sGroup     	SourceMod group name to check.
 * @return              True if in the group, false otherwise.
 **/
stock bool IsClientInGroup(int clientIndex, const char[] sGroup)
{
	// Retrieves a client's AdminId
    AdminId iD = GetUserAdmin(clientIndex);
    
    // Validate id
    if (iD == INVALID_ADMIN_ID)
    {
        return false;
    }
    
    // Get number of groups
    int iGroupNum = GetAdminGroupCount(iD);
    char sGroupName[64];
    
    // Validate number of groups
    if (iGroupNum > 0)
    {
        // Loop through each group
        for (int i = 0; i < iGroupNum; i++)
        {
            // Get group name
            GetAdminGroup(iD, i, sGroupName, sizeof(sGroupName));
            
            // Compare names
            if (StrEqual(sGroup, sGroupName, false))
            {
                return true;
            }
        }
    }
    
    // No groups or no match
    return false;
}

/**
 * Create an admin menu.
 *
 * @param cBasePlayer	The client index.
 **/
void OpenAdminMenu(CBasePlayer* cBasePlayer) 
{
	// Verify that the client admin
	if(!IsClientPrivileged(view_as<int>(cBasePlayer)))
	{
		return;
	}
	
	// Initialize char
	char sBuffer[NORMAL_LINE_LENGTH];

	// Sets the global language target
	SetGlobalTransTarget(view_as<int>(cBasePlayer));

	// Create menu handle
	Menu iMenu = CreateMenu(ServerMenuAdminHandle);

	// Add formatted options to menu
	SetMenuTitle(iMenu, "%t", "Admin menu");

	// Make human
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Make human");
	AddMenuItem(iMenu, "1", sBuffer, MenuGetItemDraw(IsClientPrivileged(view_as<int>(cBasePlayer), OperationType_Reservation)));
	
	// Make zombie
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Make zombie");
	AddMenuItem(iMenu, "2", sBuffer, MenuGetItemDraw(IsClientPrivileged(view_as<int>(cBasePlayer), OperationType_Kick)));
	
	// Respawn player
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Respawn");
	AddMenuItem(iMenu, "3", sBuffer, MenuGetItemDraw(IsClientPrivileged(view_as<int>(cBasePlayer), OperationType_Slay)));
	
	// Make nemesis
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Make nemesis");
	AddMenuItem(iMenu, "4", sBuffer, MenuGetItemDraw(IsClientPrivileged(view_as<int>(cBasePlayer), OperationType_Changemap)));
	
	// Make survivor
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Make survivor");
	AddMenuItem(iMenu, "5", sBuffer, MenuGetItemDraw(IsClientPrivileged(view_as<int>(cBasePlayer), OperationType_Custom2)));
	
	// Give ammopacks
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Give ammopacks");
	AddMenuItem(iMenu, "6", sBuffer, MenuGetItemDraw(IsClientPrivileged(view_as<int>(cBasePlayer), OperationType_Custom3)));
	
	// Start armageddon
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Start armageddon");
	AddMenuItem(iMenu, "7", sBuffer, MenuGetItemDraw(IsClientPrivileged(view_as<int>(cBasePlayer), OperationType_Custom4)));
	
	// Start multi mode
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Start multi mode");
	AddMenuItem(iMenu, "8", sBuffer, MenuGetItemDraw(IsClientPrivileged(view_as<int>(cBasePlayer), OperationType_Custom5)));
	
	// Start swarm mode
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Start swarm mode");
	AddMenuItem(iMenu, "9", sBuffer, MenuGetItemDraw(IsClientPrivileged(view_as<int>(cBasePlayer), OperationType_Custom6)));

	// Set exit and back button
	SetMenuExitBackButton(iMenu, true);
	
	// Set options and display it
	SetMenuOptionFlags(iMenu, MENUFLAG_BUTTON_EXIT|MENUFLAG_BUTTON_EXITBACK);
	DisplayMenu(iMenu, view_as<int>(cBasePlayer), MENU_TIME_FOREVER); 
}

/**
 * Called when client selects option in the admin menu, and handles it.
 *  
 * @param iMenu			Handle of the menu being used.
 * @param mAction		The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex	The client index.
 * @param nSlot			The slot index selected (starting from 0).
 **/ 
public int ServerMenuAdminHandle(Menu iMenu, MenuAction mAction, int clientIndex, int nSlot)
{
	// Get real player index from event key 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);

	// Client hit "Exit" button
	if (mAction == MenuAction_End)
	{
		CloseHandle(iMenu);
	}
	// Client hit "Back" button
	else if (mAction == MenuAction_Cancel)
	{
		if (nSlot == MenuCancel_ExitBack)
		{
			// Open new menu
			OpenMainMenu(cBasePlayer);
		}
	}
	
	// Client selected an option
	if (mAction == MenuAction_Select)
	{
		AdminSubMenu(cBasePlayer, view_as<AdminMenuMode>(nSlot));
	}
}

/**
 * Used for creating sub part of admin menu by choosing mode.
 *
 * @param cBasePlayer	The client index.
 * @param iMode			The mode of the command.
 **/
void AdminSubMenu(CBasePlayer* cBasePlayer, AdminMenuMode iMode)
{
	// Verify that the client is connected
	if(!IsPlayerExist(view_as<int>(cBasePlayer), false))
	{
		return;
	}

	// Sets the global language target
	SetGlobalTransTarget(view_as<int>(cBasePlayer));
	
	// Get client name
	char sAdminName[NORMAL_LINE_LENGTH];
	GetClientName(view_as<int>(cBasePlayer), sAdminName, sizeof(sAdminName));
	
	Menu iMenu;
	
	// Select Mode
	switch(iMode)
	{
		//!									   			   Menu Handle				  Title				Alive?      Dead?        Skip Zombies?      Skip Humans?    Skip Nemesises?    	Skip Survivors?
		case AdminMode_Human    : iMenu = CreatePlayerList(ServerMakeHumanHandle, 	 "Make human", 		true, 		false, 		 false, 	 		true, 		 	false, 	 			true);
		case AdminMode_Zombie   : iMenu = CreatePlayerList(ServerMakeZombieHandle, 	 "Make zombie",		true, 		false, 		 true, 	 			false, 	 	 	true, 	 	 		false);
		case AdminMode_Respawn  : iMenu = CreatePlayerList(ServerMakeAliveHandle, 	 "Respawn", 		false,   	true, 		 false, 		 	false, 		 	false, 		 		false);	
		case AdminMode_Nemesis  : iMenu = CreatePlayerList(ServerMakeNemesisHandle,  "Make nemesis", 	true, 		false, 		 false, 	 	 	false, 	 	 	true, 		 		false); 		
		case AdminMode_Survivor : iMenu = CreatePlayerList(ServerMakeSurvivorHandle, "Make survivor", 	true, 		false, 		 false, 	 		false, 		 	false, 	 			true);
		case AdminMode_Ammopack : iMenu = CreatePlayerList(ServerMakeAmmopackHandle, "Give ammopacks",  false, 		false, 		 false, 	 		false, 		 	false, 	 			false);
		default : 
		{
			// If round started, stop
			if(!bNewRound)
			{
				return;
			}
			
			// Select Mode
			switch(iMode)
			{
				case AdminMode_Armageddon :
				{
					// Start mode
					Make_A_Zombie(MODE_ARMAGEDDON);
					
					// Log event
					LogEvent(false, LogType_Command, LOG_GAME_EVENTS, LogModule_Admin, "Admin Command", "[%s] started armageddon mode", sAdminName);
					
					// Print event
					TranslationPrintToChatAll("Admin started armageddon", sAdminName);
				}
				case AdminMode_Multi :
				{
					// Start mode
					Make_A_Zombie(MODE_MULTI);
		
					// Log event
					LogEvent(false, LogType_Command, LOG_GAME_EVENTS, LogModule_Admin, "Admin Command", "[%s] started multi mode", sAdminName);
					
					// Print event
					TranslationPrintToChatAll("Admin started multi mode", sAdminName);
				}
				case AdminMode_Swarm :
				{
					// Start mode
					Make_A_Zombie(MODE_SWARM);
		
					// Log event
					LogEvent(false, LogType_Command, LOG_GAME_EVENTS, LogModule_Admin, "Admin Command", "[%s] started swarm mode", sAdminName);
					
					// Print event
					TranslationPrintToChatAll("Admin started swarm mode", sAdminName);
				}
			}
			return;
		}
	}

	// Set exit and back button
	SetMenuExitBackButton(iMenu, true);
	
	// Set options and display it
	SetMenuOptionFlags(iMenu, MENUFLAG_BUTTON_EXIT|MENUFLAG_BUTTON_EXITBACK);
	DisplayMenu(iMenu, view_as<int>(cBasePlayer), MENU_TIME_FOREVER); 
}

/**
 * Shows a list of all clients to a client, different handlers can be used for this, as well as title.
 * 
 * @param sHandler	The menu handler.
 * @param sTitle	Set menu title to the translated phrase.
 * @param bAlive    If true, only clients that are alive will be displayed.
 * @param bDead     If true, only clients that are dead will be displayed. 
 * @param bZombie   If true, only clients on a zombie will be skipped.
 * @param bHuman    If true, only clients on a human will be skipped.
 * @param bNemesis  If true, only clients on a nemesis will be skipped.
 * @param bSurvivor	If true, only clients on a survivor will be skipped.
 * @return     		Return menu index.
 **/
Menu CreatePlayerList(MenuHandler sHandler, char[] sTitle, bool bAlive = false, bool bDead = false, bool bZombie = false, bool bHuman = false, bool bNemesis = false, bool bSurvivor = false)
{
	// Creates a new, empty menu using the default style
	Menu iMenu = CreateMenu(sHandler);
	
	// Initialize chars variables
	char sBuffer[NORMAL_LINE_LENGTH];
	char sInfo[SMALL_LINE_LENGTH];
	
	// Add formatted options to menu
	SetMenuTitle(iMenu, "%t", sTitle);
	
	// Get amount of connected players
	int iMaxPlayers = fnGetPlaying();
	
	int iCount;
	
	// i = Client index
	for(int i = 1; i < iMaxPlayers; i++)
	{
		// Verify that a client is connected
		if(!IsPlayerExist(i, false))
		{
			continue;
		}

		// If client is dead, then stop
		if (bAlive && !IsPlayerAlive(i))
		{
			continue;
		}

		// If client is alive, then stop
		if (bDead && IsPlayerAlive(i))
		{
			continue;
		}

		// If client is zombie, then stop
		if (bZombie && gZombie[i])
		{
			continue;
		}
		
		// If client is human, then stop
		if(bHuman && !gZombie[i])
		{
			continue;
		}
		
		// If client is nemesis, then stop
		if (bNemesis && gNemesis[i])
		{
			continue;
		}

		// If client is survivor, then stop
		if (bSurvivor && gSurvivor[i])
		{
			continue;
		}

		// Format some chars for showing in menu
		FormatEx(sBuffer, sizeof(sBuffer), "%N", i);
		
		//Strips a quote pair and whitespaces off a string 
		StripQuotes(sBuffer);
		TrimString(sBuffer);
		
		// Show option
		IntToString(i, sInfo, sizeof(sInfo));
		AddMenuItem(iMenu, sInfo, sBuffer);

		// Increment count
		iCount++;
	}
	
	// If there are no clients, add an "(Empty)" line
	if (iCount == 0)
	{
		char sEmpty[SMALL_LINE_LENGTH];
		Format(sEmpty, SMALL_LINE_LENGTH, "Menu empty");

		AddMenuItem(iMenu, "empty", sEmpty, ITEMDRAW_DISABLED);
	}

	// Return a new menu Handle
	return iMenu;
}			

/**
 * Called when client selects option in the sub admin menu, and handles it.
 *  
 * @param iMenu			Handle of the menu being used.
 * @param mAction		The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex	The client index.
 * @param nSlot			The slot index selected (starting from 0).
 **/ 
public int ServerMakeHumanHandle(Menu iMenu, MenuAction mAction, int clientIndex, int nSlot)
{
	AdminCommand(iMenu, mAction, clientIndex, nSlot, AdminMode_Human);
}

/**
 * Called when client selects option in the sub admin menu, and handles it.
 *  
 * @param iMenu			Handle of the menu being used.
 * @param mAction		The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex	The client index.
 * @param nSlot			The slot index selected (starting from 0).
 **/ 
public int ServerMakeZombieHandle(Menu iMenu, MenuAction mAction, int clientIndex, int nSlot)
{
	AdminCommand(iMenu, mAction, clientIndex, nSlot, AdminMode_Zombie);
}

/**
 * Called when client selects option in the sub admin menu, and handles it.
 *  
 * @param iMenu			Handle of the menu being used.
 * @param mAction		The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex	The client index.
 * @param nSlot			The slot index selected (starting from 0).
 **/ 
public int ServerMakeAliveHandle(Menu iMenu, MenuAction mAction, int clientIndex, int nSlot)
{
	AdminCommand(iMenu, mAction, clientIndex, nSlot, AdminMode_Respawn);
}

/**
 * Called when client selects option in the sub admin menu, and handles it.
 *  
 * @param iMenu			Handle of the menu being used.
 * @param mAction		The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex	The client index.
 * @param nSlot			The slot index selected (starting from 0).
 **/ 
public int ServerMakeNemesisHandle(Menu iMenu, MenuAction mAction, int clientIndex, int nSlot)
{
	AdminCommand(iMenu, mAction, clientIndex, nSlot, AdminMode_Nemesis);
}

/**
 * Called when client selects option in the sub admin menu, and handles it.
 *  
 * @param iMenu			Handle of the menu being used.
 * @param mAction		The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex	The client index.
 * @param nSlot			The slot index selected (starting from 0).
 **/ 
public int ServerMakeSurvivorHandle(Menu iMenu, MenuAction mAction, int clientIndex, int nSlot)
{
	AdminCommand(iMenu, mAction, clientIndex, nSlot, AdminMode_Survivor);
}

/**
 * Called when client selects option in the sub admin menu, and handles it.
 *  
 * @param iMenu			Handle of the menu being used.
 * @param mAction		The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex	The client index.
 * @param nSlot			The slot index selected (starting from 0).
 **/ 
public int ServerMakeAmmopackHandle(Menu iMenu, MenuAction mAction, int clientIndex, int nSlot)
{
	AdminCommand(iMenu, mAction, clientIndex, nSlot, AdminMode_Ammopack);
}

/**
 * Create sub admin menu.
 *
 * @param iMenu			Handle of the menu being used.
 * @param mAction		The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex	The client index.
 * @param nSlot			The slot index selected (starting from 0).
 * @param CMode			The mode of the command.
 **/
void AdminCommand(Menu iMenu, MenuAction mAction, int clientIndex, int nSlot, AdminMenuMode CMode)
{
	// Get real player index from event key
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);
	
	// Client hit "Exit" button
	if (mAction == MenuAction_End)
	{
		CloseHandle(iMenu);
	}
	// Client hit "Back" button
	else if (mAction == MenuAction_Cancel)
	{
		if (nSlot == MenuCancel_ExitBack)
		{
			// Open new menu
			OpenAdminMenu(cBasePlayer);
		}
	}

	// Client selected an option
	if (mAction == MenuAction_Select)
	{
		// If round ended
		if(bEndRound)
		{
			return;
		}
		
		// Verify that the client is connected
		if(!IsPlayerExist(view_as<int>(cBasePlayer), false))
		{
			return;
		}

		// Initialize char
		char sInfo[SMALL_LINE_LENGTH];

		// Get index of the player in the menu
		GetMenuItem(iMenu, nSlot, sInfo, sizeof(sInfo));
		int iSelect = StringToInt(sInfo);
		
		// Get real player index from event key
		CBasePlayer* cBaseVictim  = view_as<CBasePlayer>(iSelect);
		
		// Verify that the select client is connected
		if(!IsPlayerExist(view_as<int>(cBaseVictim), false))
		{
			TranslationPrintHintText(view_as<int>(cBasePlayer), "Player didn't find");
			return;
		}
		
		// Get client names
		char sAdminName[NORMAL_LINE_LENGTH], sClientName[NORMAL_LINE_LENGTH];
		GetClientName(view_as<int>(cBasePlayer), sAdminName, sizeof(sAdminName));
		GetClientName(view_as<int>(cBaseVictim), sClientName, sizeof(sClientName));
		
		// Get mode
		switch(CMode)
		{
			// Make a humans
			case AdminMode_Human :
			{
				// Verify that the chosen client is alive and a zombie
				if(IsPlayerAlive(view_as<int>(cBaseVictim)) && cBaseVictim->m_bZombie)
				{
					// If new round or last zombie
					if(bNewRound || fnGetZombies() <= 1)
					{
						TranslationPrintHintText(view_as<int>(cBasePlayer), "Can not do it");
						return;
					}
					
					// Make a human
					MakeZombieToHuman(view_as<int>(cBaseVictim));
					
					// Log event
					LogEvent(false, LogType_Command, LOG_GAME_EVENTS, LogModule_Admin, "Admin Command", "[%s] give antidot for : [%s]", sAdminName, sClientName);
					
					// Print event
					TranslationPrintToChatAll("Admin give antidot", sAdminName, sClientName);
				}
				// Player is dead or human
				else 
				{
					TranslationPrintHintText(view_as<int>(cBasePlayer), "Can not do it");
				}
			}
			// Make a zombies
			case AdminMode_Zombie :	
			{
				// Verify that the chosen client is alive and not zombie
				if(IsPlayerAlive(view_as<int>(cBaseVictim)) && !cBaseVictim->m_bZombie)
				{
					// Start mode
					if(bNewRound)
					{
						Make_A_Zombie(MODE_INFECTION, view_as<int>(cBaseVictim));
					}
					else
					{
						// If last human
						if(fnGetHumans() <= 1)
						{
							TranslationPrintHintText(view_as<int>(cBasePlayer), "Can not do it");
							return;
						}
						
						// Make zombie
						InfectHumanToZombie(view_as<int>(cBaseVictim));
					}
					
					// Log event
					LogEvent(false, LogType_Command, LOG_GAME_EVENTS, LogModule_Admin, "Admin Command", "[%s] infect : [%s]", sAdminName, sClientName);
					
					// Print event
					TranslationPrintToChatAll("Admin infect player", sAdminName, sClientName);
				}
				// Player is dead or zombie
				else 
				{
					TranslationPrintHintText(view_as<int>(cBasePlayer), "Can not do it");
				}
			}
			// Respawn players
			case AdminMode_Respawn :
			{
				// Verify that the chosen client is dead
				if(!IsPlayerAlive(view_as<int>(cBaseVictim)))
				{
					// Respawn
					ToolsRespawn(view_as<int>(cBaseVictim));
					
					// Log event
					LogEvent(false, LogType_Command, LOG_GAME_EVENTS, LogModule_Admin, "Admin Command", "[%s] respawned : [%s]", sAdminName, sClientName);
					
					// Print event
					TranslationPrintToChatAll("Admin respawned player", sAdminName, sClientName);
				}
				// Player is alive
				else
				{
					TranslationPrintHintText(view_as<int>(cBasePlayer), "Can not do it");
				}
			}	
			// Make a nemesis
			case AdminMode_Nemesis :	
			{
				// Verify that the chosen client is alive
				if(IsPlayerAlive(view_as<int>(cBaseVictim)))
				{
					// Start mode
					if(bNewRound)
					{
						Make_A_Zombie(MODE_NEMESIS, view_as<int>(cBaseVictim));	
					}
					else
					{
						// If last human
						if(fnGetHumans() <= 1 && !cBaseVictim->m_bZombie)
						{
							TranslationPrintHintText(view_as<int>(cBasePlayer), "Can not do it");
							return;
						}
						
						// Make nemesis
						InfectHumanToZombie(view_as<int>(cBaseVictim), _, _, true);
					}
					
					// Log event
					LogEvent(false, LogType_Command, LOG_GAME_EVENTS, LogModule_Admin, "Admin Command", "[%s] give a nemesis for : [%s]", sAdminName, sClientName);
					
					// Print event
					TranslationPrintToChatAll("Admin give a nemesis to player", sAdminName, sClientName);
				}
				// Player is dead
				else 
				{
					TranslationPrintHintText(view_as<int>(cBasePlayer), "Can not do it");
				}
			}
			// Make a survivor
			case AdminMode_Survivor :	
			{
				// Verify that the chosen client is alive
				if(IsPlayerAlive(view_as<int>(cBaseVictim)))
				{
					// Start mode
					if(bNewRound)
					{
						Make_A_Zombie(MODE_SURVIVOR, view_as<int>(cBaseVictim));	
					}
					else
					{
						// If last zombie
						if(fnGetZombies() <= 1 && cBaseVictim->m_bZombie)
						{
							TranslationPrintHintText(view_as<int>(cBasePlayer), "Can not do it");
							return;
						}
						
						// Make survivor
						MakeHumanIntoSurvivor(view_as<int>(cBaseVictim));
					}
					
					// Log event
					LogEvent(false, LogType_Command, LOG_GAME_EVENTS, LogModule_Admin, "Admin Command", "[%s] give a survivor for : [%s]", sAdminName, sClientName);
					
					// Print event
					TranslationPrintToChatAll("Admin give a survivor to player", sAdminName, sClientName);
				}
				// Player is dead
				else 
				{
					TranslationPrintHintText(view_as<int>(cBasePlayer), "Can not do it");
				}
			}
			// Give a ammopack
			case AdminMode_Ammopack :	
			{
				// Give ammopacks
				cBaseVictim->m_nAmmoPacks += 100;

				// Log event
				LogEvent(false, LogType_Command, LOG_GAME_EVENTS, LogModule_Admin, "Admin Command", "[%s] give ammopacks to : [%s]", sAdminName, sClientName);
				
				// Print event
				TranslationPrintToChatAll("Admin give 100 ammopacks to player", sAdminName, sClientName);
			}
		}
		
		// Update menu
		AdminSubMenu(cBasePlayer, CMode);
	}
}