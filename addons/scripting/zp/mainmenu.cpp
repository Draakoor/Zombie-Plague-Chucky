/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          mainmenu.cpp
 *  Type:          Module 
 *  Description:   Menu with shop of weapons and extraitems.
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
 * Sends main menu to client.
 *
 * @param cBasePlayer	The client index.
 * @param bPageNext		The next page, if menu was already opened.
 **/
void OpenMainMenu(CBasePlayer* cBasePlayer, bool bPageNext = false)
{
	// Switch to next page, if client is press button, when menu is already open
	if(bPageNext && GetClientMenu(view_as<int>(cBasePlayer), INVALID_HANDLE) != MenuSource_None)
	{
		FakeClientCommandEx(view_as<int>(cBasePlayer), "menuselect 8");
		return;
	}

	// Sets the language to target
	SetGlobalTransTarget(view_as<int>(cBasePlayer));
	
	// Create menu handle
	Menu iMenu = CreateMenu(ServerMenuMainHandle);

	// Set title
	SetMenuTitle(iMenu, "%t", GetConVarBool(gCvarList[CVAR_LVL_SYSTEM]) ? "Level" : "Menu", (cBasePlayer->m_iLevel));
	
	// Initialize chars variables
	char szText[NORMAL_LINE_LENGTH];

	// Show menu
	FormatEx(szText, NORMAL_LINE_LENGTH, "%t", "Buy weapons");
	AddMenuItem(iMenu, "1", szText);
	
	FormatEx(szText, NORMAL_LINE_LENGTH, "%t", "Buy extraitems");
	AddMenuItem(iMenu, "2", szText);
	
	FormatEx(szText, NORMAL_LINE_LENGTH, "%t", "Choose zombieclass");
	AddMenuItem(iMenu, "3", szText);
	
	FormatEx(szText, NORMAL_LINE_LENGTH, "%t", "Help motd");
	AddMenuItem(iMenu, "4", szText);
	
	FormatEx(szText, NORMAL_LINE_LENGTH, "%t", "Unstuck");
	AddMenuItem(iMenu, "5", szText);
	
	FormatEx(szText, NORMAL_LINE_LENGTH, "%t", "Admin menu");
	AddMenuItem(iMenu, "6", szText, MenuGetItemDraw(IsClientPrivileged(view_as<int>(cBasePlayer))));

	// Set exit and back button
	SetMenuExitButton(iMenu, true);

	// Set options and display it
	SetMenuOptionFlags(iMenu, MENUFLAG_BUTTON_EXIT);
	DisplayMenu(iMenu, view_as<int>(cBasePlayer), MENU_TIME_FOREVER); 
}

/**
 * Called when client selects option in the main menu, and handles it.
 *  
 * @param iMenu			Handle of the menu being used.
 * @param mAction		The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex	The client index.
 * @param mSlot			The slot index selected (starting from 0).
 **/ 
public int ServerMenuMainHandle(Menu iMenu, MenuAction mAction, int clientIndex, int mSlot)
{
	// Get real player index from event key 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);
	
	// Client hit "Exit" button
	if (mAction == MenuAction_End)
	{
		CloseHandle(iMenu);
	}
	
    // Client selected an option
	if (mAction == MenuAction_Select)
    {
		switch(mSlot)
		{
			// Shop menu case
			case 0 : OpenShopMenu(cBasePlayer);
			
			// Extra item menu case
			case 1 : OpenSubShopMenu(cBasePlayer);
			
			// Zombie class menu case
			case 2 : OpenZClassMenu(cBasePlayer);
			
			// Help menu case
			case 3 : OpenMotDMenu(cBasePlayer);

			// UnStuck menu case
			case 4 : SpawnOnClientStuck(cBasePlayer);
			
			// Admin menu case
			case 5 : OpenAdminMenu(cBasePlayer); 
		}
    }
}

/**
 * Create shop menu.
 *
 * @param cBasePlayer	The client index.
 **/
void OpenShopMenu(CBasePlayer* cBasePlayer) 
{
	// Verify that the client is alive
	if(!IsPlayerExist(view_as<int>(cBasePlayer)))
	{
		return;
	}
	
	// If client is survivor, nemesis or zombie, then stop 
	if(cBasePlayer->m_bSurvivor || cBasePlayer->m_bNemesis || cBasePlayer->m_bZombie)
	{
		// Show info
		TranslationPrintHintText(view_as<int>(cBasePlayer), "You can not buy it");
		return;
	}
	
	// Sets the language to target
	SetGlobalTransTarget(view_as<int>(cBasePlayer));

	// Create menu handle
	Menu iMenu = CreateMenu(ServerShopMainHandle);

	// Set title
	SetMenuTitle(iMenu, "%t", GetConVarBool(gCvarList[CVAR_LVL_SYSTEM]) ? "Level" : "Menu", (cBasePlayer->m_iLevel));
	
	// Initialize char
	char sBuffer[NORMAL_LINE_LENGTH];

	// Show pistol list
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Buy pistols");
	AddMenuItem(iMenu, "1", sBuffer);
	
	// Show shotgun list
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Buy shothuns");
	AddMenuItem(iMenu, "2", sBuffer);
	
	// Show rifle list
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Buy rifles");
	AddMenuItem(iMenu, "3", sBuffer);
	
	// Show sniper list
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Buy snipers");
	AddMenuItem(iMenu,	"4", sBuffer);
	
	// Show machinegun list
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Buy machinehuns");
	AddMenuItem(iMenu, "5", sBuffer);

	// Set exit and back button
	SetMenuExitBackButton(iMenu, true);

	// Set options and display it
	SetMenuOptionFlags(iMenu, MENUFLAG_BUTTON_EXIT|MENUFLAG_BUTTON_EXITBACK);
	DisplayMenu(iMenu, view_as<int>(cBasePlayer), MENU_TIME_FOREVER); 
}

/**
 * Called when client selects option in the shop menu, and handles it.
 *  
 * @param iMenu			Handle of the menu being used.
 * @param mAction		The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex	The client index.
 * @param mSlot			The slot index selected (starting from 0).
 **/ 
public int ServerShopMainHandle(Menu iMenu, MenuAction mAction, int clientIndex, int mSlot)
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
		if (mSlot == MenuCancel_ExitBack)
		{
			// Open new menu
			OpenMainMenu(cBasePlayer);
		}
	}
	
    // Client selected an option
	if (mAction == MenuAction_Select)
    {
		// Create sub menu 
		OpenSubShopMenu(cBasePlayer, mSlot);
    }
}

/**
 * Used for creating sub part of shop menu and extra items menu.
 *  
 * @param cBasePlayer	The client index.
 * @param mSlot			The slot index selected (starting from 0).
 **/ 
void OpenSubShopMenu(CBasePlayer* cBasePlayer, int mSlot = -1)
{
	// Verify that the client is connected and alive
	if(!IsPlayerExist(view_as<int>(cBasePlayer)))
	{
		return;
	}
	
	// If client is survivor or nemesis, then stop 
	if(cBasePlayer->m_bSurvivor || cBasePlayer->m_bNemesis)
	{
		// Show info
		TranslationPrintHintText(view_as<int>(cBasePlayer), "You can not buy it");
		return;
	}
	
	// Initialize chars variables
	char sBuffer[BIG_LINE_LENGTH];
	char sName[NORMAL_LINE_LENGTH];
	char sInfo[SMALL_LINE_LENGTH];
	
	// Create menu
	Menu iMenu;
	
	// Sets the language to target
	SetGlobalTransTarget(view_as<int>(cBasePlayer));
	
	// Extra items menu
	if(mSlot == -1 || cBasePlayer->m_bZombie)	
	{
		// Create extra items menu handle
		iMenu = CreateMenu(ServerMenuItemsHandle);

		// i = Extra item number
		for(int i = 0; i < gExtraItemsNum; i++)
		{
			// Skip some extra items, if team is not equal
			if(cBasePlayer->m_bZombie && ItemsGetTeam(i) == 0 || !cBasePlayer->m_bZombie && ItemsGetTeam(i) == 1)
				continue;
		
			// Get extra item name
			ItemsGetName(i, sName, sizeof(sName));
			
			// Formatex some chars for showing in menu
			// Is string contains @, name will be translate
			FormatEx(sBuffer, sizeof(sBuffer), (sName[0] == '@') ? "%t    %t" : "%s    %t", sName, "Ammopacks", ItemsGetCost(i));

			// Show option
			IntToString(i, sInfo, sizeof(sInfo));
			AddMenuItem(iMenu, sInfo, sBuffer, MenuGetItemDraw((cBasePlayer->m_iLevel) < ItemsGetLevel(i) || fnGetPlaying() < ItemsGetOnline(i) || (ItemsGetLimit(i) != 0 && ItemsGetLimit(i) <= gExtraBuyLimit[view_as<int>(cBasePlayer)][i]) || cBasePlayer->m_nAmmoPacks < ItemsGetCost(i)) ? false : true);
		}
	}
	// Weapons menu
	else
	{
		// If mode is started, then stop 
		if(GetConVarBool(gCvarList[CVAR_BLOCK_BUY_ROUND_START]) && bModeStarted)
		{
			// Show info
			TranslationPrintHintText(view_as<int>(cBasePlayer), "You can not buy it");
			return;
		}
	
		// Create menu handle
		iMenu = CreateMenu(ServerMenuSubShopHandle);
		
		// i = Array index
		int iSize = GetArraySize(arrayWeapons);
		for(int i = 0; i < iSize; i++)
		{
			// Skip some weapons, if slot is not equal
			if(WeaponsGetSlot(i) != mSlot)
				continue;
		
			// Create weapon name
			WeaponsGetEntity(i, sName, sizeof(sName));
			WeaponsEntityToDisplay(sName, sizeof(sName));
			
			// Formatex some chars for showing in menu
			FormatEx(sBuffer, sizeof(sBuffer), (WeaponsGetCost(i)) ? "%s    %t" : "%s", sName, "Ammopacks", WeaponsGetCost(i));

			// Show option
			IntToString(i, sInfo, sizeof(sInfo));
			AddMenuItem(iMenu, sInfo, sBuffer, MenuGetItemDraw(((cBasePlayer->m_iLevel) < WeaponsGetLevel(i) || fnGetPlaying() < WeaponsGetOnline(i) || cBasePlayer->m_nAmmoPacks < WeaponsGetCost(i)) ? false : true));
		}
	}
	
	// Set title
	SetMenuTitle(iMenu, "%t", GetConVarBool(gCvarList[CVAR_LVL_SYSTEM]) ? "Level" : "Menu", (cBasePlayer->m_iLevel));
	
	// Set exit and back button
	SetMenuExitBackButton(iMenu, true);
	
	// Set options and display it
	SetMenuOptionFlags(iMenu, MENUFLAG_BUTTON_EXIT|MENUFLAG_BUTTON_EXITBACK);
	DisplayMenu(iMenu, view_as<int>(cBasePlayer), MENU_TIME_FOREVER);
}

/**
 * Called when client selects option in the extra items menu, and handles it.
 *  
 * @param iMenu			Handle of the menu being used.
 * @param mAction		The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex	The client index.
 * @param mSlot			The slot index selected (starting from 0).
 **/ 
public int ServerMenuItemsHandle(Menu iMenu, MenuAction mAction, int clientIndex, int mSlot)
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
		if (mSlot == MenuCancel_ExitBack)
		{
			// Open new menu
			OpenMainMenu(cBasePlayer);
		}
	}

	// Client selected an option
	if (mAction == MenuAction_Select)
	{
		// Verify that the client is connected and alive
		if(!IsPlayerExist(view_as<int>(cBasePlayer)))
		{
			return;
		}

		// If client is survivor or nemesis, then stop
		if(cBasePlayer->m_bSurvivor || cBasePlayer->m_bNemesis)
		{
			// Show message of buying
			TranslationPrintHintText(view_as<int>(cBasePlayer), "You can not buy it");
			return;
		}

		// Initialize char
		char sItemName[NORMAL_LINE_LENGTH];

		// Get ID of the extra item
		GetMenuItem(iMenu, mSlot, sItemName, sizeof(sItemName));
		int iItem = StringToInt(sItemName);
		
		// Call forward
		Action iResult = APIOnBuyingExtraItem(cBasePlayer, iItem);
		
		// Validate, that buying is non successful
		if(iResult == ACTION_HANDLED)
		{
			// Show message of false buying
			TranslationPrintHintText(view_as<int>(cBasePlayer), "You can not buy it");
		}
		else
		{
			// If help messages enabled 
			if(GetConVarBool(gCvarList[CVAR_HELP_MESSAGES]))
			{
				// Get extra item name
				ItemsGetName(iItem, sItemName, sizeof(sItemName));
				
				// Remove translation symbol, if it exist
				ReplaceString(sItemName, sizeof(sItemName), "@", "");
				
				// Get client name
				char sClientName[NORMAL_LINE_LENGTH];
				GetClientName(view_as<int>(cBasePlayer), sClientName, sizeof(sClientName));
				
				// Show message of successful buying
				TranslationPrintToChatAll("Buy extraitem", sClientName, sItemName);
			}
			
			// If item don't have price
			if(ItemsGetCost(iItem))
			{
				// Remove ammo and store it for returning if player will be first zombie
				cBasePlayer->m_nAmmoPacks -= ItemsGetCost(iItem);
				cBasePlayer->m_nLastBoughtAmount += ItemsGetCost(iItem);
				
				// Update limit of buying
				if(ItemsGetLimit(iItem))
				{
					// Increment count
					gExtraBuyLimit[view_as<int>(cBasePlayer)][iItem]++;
				}
			}
		}
	}
}

/**
 * Called when client selects option in the shop menu, and handles it.
 *  
 * @param iMenu			Handle of the menu being used.
 * @param mAction		The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex	The client index.
 * @param mSlot			The slot index selected (starting from 0).
 **/ 
public int ServerMenuSubShopHandle(Menu iMenu, MenuAction mAction, int clientIndex, int mSlot)
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
		if (mSlot == MenuCancel_ExitBack)
		{
			// Open new menu
			OpenShopMenu(cBasePlayer);
		}
	}

	// Client selected an option
	if (mAction == MenuAction_Select)
	{
		// Verify that the client is connected and alive
		if(!IsPlayerExist(view_as<int>(cBasePlayer)))
		{
			return;
		}

		// If client is zombie, survivor or nemesis, or just round is started, then stop
		if(cBasePlayer->m_bZombie || cBasePlayer->m_bSurvivor || cBasePlayer->m_bNemesis || GetConVarBool(gCvarList[CVAR_BLOCK_BUY_ROUND_START]) && bModeStarted)
		{
			// Show message of buying
			TranslationPrintHintText(view_as<int>(cBasePlayer), "You can not buy it");
			return;
		}
		
		// Initialize char
		char sWeaponEntity[SMALL_LINE_LENGTH];

		// Get ID of the weapon
		GetMenuItem(iMenu, mSlot, sWeaponEntity, sizeof(sWeaponEntity));
		int weaponIndex = StringToInt(sWeaponEntity);

		// Get weapon alias
		WeaponsGetEntity(weaponIndex, sWeaponEntity, sizeof(sWeaponEntity));
		
		// If client have this weapon
		if(WeaponsHasEntity(view_as<int>(cBasePlayer), sWeaponEntity))
		{
			// Show message of buying
			TranslationPrintHintText(view_as<int>(cBasePlayer), "You can not buy it");
			return;
		}
		
		// Force client to drop weapon
		if(!StrEqual(sWeaponEntity, "weapon_taser"))
		{
			// Get weapon index from slot
			int iSlot = GetPlayerWeaponSlot(view_as<int>(cBasePlayer), (!WeaponsGetSlot(weaponIndex)) ? WEAPON_SLOT_SECONDARY : WEAPON_SLOT_PRIMARY);

			// If weapon is invalid, then stop
			if (iSlot != WEAPON_SLOT_INVALID)
			{
				CS_DropWeapon(view_as<int>(cBasePlayer), iSlot, true, false);
			}
		}
		
		// Give weapon for player
		cBasePlayer->CItemMaterialize(sWeaponEntity);

		// If weapon don't have price
		if(WeaponsGetCost(weaponIndex))
		{
			// Remove ammo and store it for returning if player will be first zombie
			cBasePlayer->m_nAmmoPacks -= WeaponsGetCost(weaponIndex);
			cBasePlayer->m_nLastBoughtAmount += WeaponsGetCost(weaponIndex);
		}
	}
}

/**
 * Create a zombie class menu.
 *
 * @param cBasePlayer	The client index.
 **/
void OpenZClassMenu(CBasePlayer* cBasePlayer) 
{
	// Verify that the client is connected
	if(!IsPlayerExist(view_as<int>(cBasePlayer), false))
	{
		return;
	}

	// Initialize chars variables
	char sBuffer[BIG_LINE_LENGTH];
	char sZombieName[NORMAL_LINE_LENGTH];
	char sInfo[SMALL_LINE_LENGTH];

	// Sets the language to target
	SetGlobalTransTarget(view_as<int>(cBasePlayer));

	// Open zombie class menu
	Menu iMenu = CreateMenu(ServerMenuZombieHandle);

	// Add formatted options to menu
	SetMenuTitle(iMenu, "%t", "Choose zombieclass");

	// i = Zombie class number
	for(int i = 0; i < gZombieClassNum; i++)
	{
		// Get zombie class name
		ZombieGetName(i, sZombieName, sizeof(sZombieName));
		
		// Formatex some chars for showing in menu
		// Is string contains @, name will be translate
		FormatEx(sBuffer, sizeof(sBuffer), (sZombieName[0] == '@') ? "%t    %s" : "%s    %s", sZombieName, ZombiesIsVIP(i) ? "[VIP]" : "");
		
		// Show option
		IntToString(i, sInfo, sizeof(sInfo));
		AddMenuItem(iMenu, sInfo, sBuffer, MenuGetItemDraw(((!IsClientPrivileged(view_as<int>(cBasePlayer), OperationType_VIP) && ZombiesIsVIP(i)) || cBasePlayer->m_nZombieNext == i) ? false : true));
	}
	
	// Set exit and back button
	SetMenuExitBackButton(iMenu, true);

	// Set options and display it
	SetMenuOptionFlags(iMenu, MENUFLAG_BUTTON_EXIT|MENUFLAG_BUTTON_EXITBACK);
	DisplayMenu(iMenu, view_as<int>(cBasePlayer), MENU_TIME_FOREVER); 
}

/**
 * Called when client selects option in the zombie class menu, and handles it.
 *  
 * @param iMenu			Handle of the menu being used.
 * @param mAction		The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex	The client index.
 * @param mSlot			The slot index selected (starting from 0).
 **/ 
public int ServerMenuZombieHandle(Menu iMenu, MenuAction mAction, int clientIndex, int mSlot)
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
		if (mSlot == MenuCancel_ExitBack)
		{
			// Open new menu
			OpenMainMenu(cBasePlayer);
		}
	}

	// Client selected an option
	if (mAction == MenuAction_Select)
	{
		// Verify that the client is connected
		if(!IsPlayerExist(view_as<int>(cBasePlayer), false))
		{
			return;
		}
		
		// Sets the language to target
		SetGlobalTransTarget(view_as<int>(cBasePlayer));
		
		// Initialize char
		char sZombieName[NORMAL_LINE_LENGTH];

		// Get ID of zombie class
		GetMenuItem(iMenu, mSlot, sZombieName, sizeof(sZombieName));
		int iClass = StringToInt(sZombieName);
		
		// Showing double instead float
		char sFloat[2][4];

		// Set next zombie class
		cBasePlayer->m_nZombieNext = iClass;

		// Get Zombie Info and convert it to string
		FloatToString(ZombieGetSpeed(iClass), sFloat[0], sizeof(sFloat[]));
		FloatToString(ZombieGetGravity(iClass), sFloat[1], sizeof(sFloat[]));
		
		// Get zombie name
		ZombieGetName(iClass, sZombieName, sizeof(sZombieName));
		
		// Remove translation symbol, if it exist
		ReplaceString(sZombieName, sizeof(sZombieName), "@", "");

		// Show class info
		TranslationPrintToChat(view_as<int>(cBasePlayer), "Zombie info", sZombieName, ZombieGetHealth(iClass), sFloat[0], sFloat[1]);
	}
}

/**
 * Create a motd window for client.
 *
 * @param cBasePlayer	The client index.
 **/
void OpenMotDMenu(CBasePlayer* cBasePlayer)
{
	// Get URL to motd
	char sURL[BIG_LINE_LENGTH];
	GetConVarString(gCvarList[CVAR_URL_MOTD], sURL, sizeof(sURL));
	
	// Open motd window
	ShowMOTDPanel(view_as<int>(cBasePlayer), "Help", sURL, MOTDPANEL_TYPE_URL);
}