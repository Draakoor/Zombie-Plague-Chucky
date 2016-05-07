/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          events.cpp
 *  Type:          Game 
 *  Description:   Event hooking and forwarding.
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
 * Init function for event module.
 **/
void EventInit(/*void*/)
{
	EventHook();
}

/**
 * Hook events used by plugin.
 * 
 * @param bUnhook    If true, then unhook all events, if false, then hook.
 **/ 
void EventHook(bool bUnhook = false)
{
	// If unhook is true, then continue
	if (bUnhook)
	{
		// Unhook all events
		UnhookEvent("round_prestart", 		 EventRoundPreStart);
		UnhookEvent("round_start", 			 EventRoundStart);
		UnhookEvent("round_end", 			 EventRoundEnd);
		UnhookEvent("cs_win_panel_round", 	 EventRoundPanel);
		UnhookEvent("player_spawn",      	 EventPlayerSpawn);
		UnhookEvent("player_death", 		 EventPlayerDeath);
		UnhookEvent("player_hurt", 			 EventPlayerHurt);
		UnhookEvent("player_team", 			 EventPlayerTeam);
		UnhookEvent("player_jump", 			 EventPlayerJump);
		UnhookEvent("weapon_fire",			 EventPlayerFire);
		UnhookEvent("decoy_firing", 		 EventEntityDecoy);
		UnhookEvent("smokegrenade_detonate", EventEntitySmoke);
		RemoveTempEntHook("Shotgun Shot",    EventEntityFireBullets); 
		
		// Stop after unhooking events
		return;
	}

	// Hook server events
	HookEvent("round_prestart", 		EventRoundPreStart,  		EventHookMode_Pre);
	HookEvent("round_start", 			EventRoundStart, 			EventHookMode_Post);
	HookEvent("round_end", 				EventRoundEnd, 	 			EventHookMode_Pre);
	HookEvent("cs_win_panel_round", 	EventRoundPanel, 	 		EventHookMode_Pre);

	// Hook player events
	HookEvent("player_spawn",      		EventPlayerSpawn, 			EventHookMode_Post);
	HookEvent("player_death", 			EventPlayerDeath, 	 		EventHookMode_Post);
	HookEvent("player_hurt", 			EventPlayerHurt, 			EventHookMode_Post);
	HookEvent("player_team", 			EventPlayerTeam,			EventHookMode_Pre);
	HookEvent("player_jump", 			EventPlayerJump,			EventHookMode_Post);
	HookEvent("weapon_fire",			EventPlayerFire,		 	EventHookMode_Pre);

	// Hook entity events
	HookEvent("decoy_firing", 			EventEntityDecoy, 	 		EventHookMode_Post);
	HookEvent("smokegrenade_detonate", 	EventEntitySmoke, 	 		EventHookMode_Post);
	
	// Hook temp entity
	AddTempEntHook("Shotgun Shot", 		EventEntityFireBullets);
}

/**
 * Player events.
 **/

/**
 * Client has just connected to the server.
 *
 * @param clientIndex	The client index.
 **/
public void OnClientConnected(int clientIndex)
{
	// Forward event to modules
	ToolsResetVars(clientIndex);
}

/**
 * Client is leaving the server.
 *
 * @param clientIndex	The client index.
 **/
public void OnClientDisconnect(int clientIndex)
{
	// Store ammopacks in the SQL Base
	SQLBaseInsert(clientIndex);

	// Forward event to modules
	ToolsKillAllTimers(clientIndex);
	ToolsResetVars(clientIndex);
	RoundEndGetRoundCheck();
}

/**
 * Called when a client receives an auth ID.
 *
 * @param clientIndex	The client index.
 * @param sSteamID 		Client Steam2 id, if available, else engine auth id.
 **/
public void OnClientPostAdminCheck(int clientIndex)
{
	// Load ammopacks from the SQL Base
	SQLBaseExtract(clientIndex);
	
	// Turn on Player Hooks
	SDKHook(clientIndex, SDKHook_OnTakeDamage,	   DamageOnTakeDamage);
	SDKHook(clientIndex, SDKHook_TraceAttack,	   DamageOnTraceAttack);
	SDKHook(clientIndex, SDKHook_WeaponCanUse,     WeaponOnCanUse);
	SDKHook(clientIndex, SDKHook_WeaponSwitchPost, WeaponOnDeployPost);
	SDKHook(clientIndex, SDKHook_WeaponDropPost,   WeaponOnDropPost);
}

/**
 * Called when a clients movement buttons are being processed.
 *  
 * @param clientIndex		The client index.
 * @param iButtons          Copyback buffer containing the current commands (as bitflags - see entity_prop_stocks.inc).
 * @param iImpulse          Copyback buffer containing the current impulse command.
 * @param flVelocity        Players desired velocity.
 * @param flAngles 			Players desired view angles.	
 * @param weaponIndex		Entity index of the new weapon if player switches weapon, 0 otherwise.
 * @param iSubType			Weapon subtype when selected from a menu.
 * @param iCmdNum			Command number. Increments from the first command sent.
 * @param iTickCount		Tick count. A client's prediction based on the server's GetGameTickCount value.
 * @param iSeed				Random seed. Used to determine weapon recoil, spread, and other predicted elements.
 * @param iMouse			Mouse direction (x, y).
 **/ 
public Action OnPlayerRunCmd(int clientIndex, int &iButtons, int &iImpulse, float flVelocity[3], float flAngles[3], int &weaponIndex, int &iSubType, int &iCmdNum, int &iTickCount, int &iSeed, int iMouse[2])
{
	// Validate client
	if(!IsPlayerExist(clientIndex, false))
	{
		return ACTION_CONTINUE;
	}
	
	// Forward event to modules
	return RunCmdOnPlayerRunCmd(clientIndex, iButtons);
}

/**
 * Global player events.
 **/

/**
 * Event callback (round_prestart)
 * The round is starting.
 * 
 * @param gEventHook       The event handle.
 * @param gEventName       Name of the event.
 * @param iDontBroadcast   If true, event is broadcasted to all clients, false if not.
 **/
public Action EventRoundPreStart(Event gEventHook, const char[] gEventName, bool iDontBroadcast) 
{
	// Forward event to modules
	RoundStartOnRoundPreStart();
}

/**
 * Event callback (round_start)
 * The round is starting.
 * 
 * @param gEventHook       The event handle.
 * @param gEventName       Name of the event.
 * @param iDontBroadcast   If true, event is broadcasted to all clients, false if not.
 **/
public Action EventRoundStart(Event gEventHook, const char[] gEventName, bool iDontBroadcast) 
{
	// Forward event to modules
	RoundStartOnRoundStart();
}

/**
 * Event callback (round_end)
 * The round is ending.
 * 
 * @param gEventHook       The event handle.
 * @param gEventName       Name of the event.
 * @param iDontBroadcast   If true, event is broadcasted to all clients, false if not.
 **/
public Action EventRoundEnd(Event gEventHook, const char[] gEventName, bool iDontBroadcast) 
{
	// Forward event to modules
	RoundEndBalanceTeams();
	RoundEndOnRoundEnd();
}

/**
 * Event callback (cs_win_panel_round)
 * The panel is about to open.
 * 
 * @param gEventHook       The event handle.
 * @param gEventName       Name of the event.
 * @param iDontBroadcast   If true, event is broadcasted to all clients, false if not.
 **/
public Action EventRoundPanel(Event gEventHook, const char[] gEventName, bool iDontBroadcast) 
{
	// Sets whether an event's broadcasting will be disabled
	if(!iDontBroadcast) 
	{
		// Disable broadcasting
		SetEventBroadcast(gEventHook, true); 
	}
}

/**
 * Event callback (player_spawn)
 * Client is spawning into the game.
 * 
 * @param gEventHook       The event handle.
 * @param gEventName       Name of the event.
 * @param iDontBroadcast   If true, event is broadcasted to all clients, false if not.
 **/
public Action EventPlayerSpawn(Event gEventHook, const char[] gEventName, bool iDontBroadcast) 
{
	// Get all required event info
	int clientIndex = GetClientOfUserId(GetEventInt(gEventHook, "userid"));

	// Validate client
	if (!IsPlayerExist(clientIndex))
	{
		return;
	}

	// Forward event to modules
	ToolsKillAllTimers(clientIndex);
	SpawnOnClientSpawn(clientIndex);
}

/**
 * Event callback (player_hurt)
 * Client is being hurt.
 * 
 * @param gEventHook       The event handle.
 * @param gEventName       Name of the event.
 * @param iDontBroadcast   If true, event is broadcasted to all clients, false if not.
 **/
public Action EventPlayerHurt(Event gEventHook, const char[] gEventName, bool iDontBroadcast) 
{
	// Get all required event info
	int clientIndex = GetClientOfUserId(GetEventInt(gEventHook, "userid"));
	int attackerIndex = GetClientOfUserId(GetEventInt(gEventHook, "attacker"));
	int iHitgroup = GetEventInt(gEventHook, "hitgroup");
	int iDamage = GetEventInt(gEventHook, "dmg_health");

	// Get weapon alias
	char sClassname[WEAPONS_MAX_LENGTH];
	GetEventString(gEventHook, "weapon", sClassname, sizeof(sClassname));

	// Forward event to modules
	return KnockbackOnClientHurt(clientIndex, attackerIndex, CS_AliasToWeaponID(sClassname), iHitgroup, iDamage);
}

/**
 * Event callback (player_death)
 * Client has been killed.
 * 
 * @param gEventHook       The event handle.
 * @param gEventName       Name of the event.
 * @param iDontBroadcast   If true, event is broadcasted to all clients, false if not.
 **/
public Action EventPlayerDeath(Event gEventHook, const char[] gEventName, bool iDontBroadcast) 
{
	// Get the weapon name
	char sClassname[WEAPONS_MAX_LENGTH];
	GetEventString(gEventHook, "weapon", sClassname, sizeof(sClassname));

	// If client is being infected, then stop
	if (StrEqual(sClassname, "weapon_zombie_claws"))
	{
		return ACTION_HANDLED;
	}

	// Get all required event info
	int clientIndex = GetClientOfUserId(GetEventInt(gEventHook, "userid"));
	int attackerIndex = GetClientOfUserId(GetEventInt(gEventHook, "attacker"));
	
	// Validate client
	if (!IsPlayerExist(clientIndex, false))
	{
		// If the client isn't a player, a player really didn't die now. Some
		// other mods might sent this event with bad data.
		return ACTION_HANDLED;
	}

	// Forward event to modules
	ToolsKillAllTimers(clientIndex);
	RagdollOnClientDeath(clientIndex);
	DeathOnClientDeath(clientIndex, attackerIndex);
	RoundEndGetRoundValidate();
	
	return ACTION_CONTINUE;
}

/**
 * Event callback (player_team)
 * The player is about to change team.
 * 
 * @param gEventHook       The event handle.
 * @param gEventName       Name of the event.
 * @param iDontBroadcast   If true, event is broadcasted to all clients, false if not.
 **/
public Action EventPlayerTeam(Event gEventHook, const char[] gEventName, bool iDontBroadcast) 
{
	// Sets whether an event's broadcasting will be disabled
	if(!iDontBroadcast) 
	{
		// Disable broadcasting
		SetEventBroadcast(gEventHook, true); 
	}
}

/**
 * Event callback (player_jump)
 * Client is jumping.
 * 
 * @param gEventHook       The event handle.
 * @param gEventName       Name of the event.
 * @param iDontBroadcast   If true, event is broadcasted to all clients, false if not.
 **/
public Action EventPlayerJump(Event gEventHook, const char[] gEventName, bool iDontBroadcast) 
{
	// Get all required event info
	int clientIndex = GetClientOfUserId(GetEventInt(gEventHook, "userid"));

	// Forward event to modules
	JumpBoostOnClientJump(clientIndex);
}

/**
 * Event callback (weapon_fire)
 * The player is about to shot.
 * 
 * @param gEventHook       The event handle.
 * @param gEventName       Name of the event.
 * @param iDontBroadcast   If true, event is broadcasted to all clients, false if not.
 **/
public Action EventPlayerFire(Event gEventHook, const char[] gEventName, bool iDontBroadcast) 
{
	// Get all required event info
	int clientIndex = GetClientOfUserId(GetEventInt(gEventHook, "userid"));

	// Validate client
	if (!IsPlayerExist(clientIndex))
	{
		return;
	}
	
	// Get active weapon index
	int weaponIndex = GetEntPropEnt(clientIndex, Prop_Data, "m_hActiveWeapon");

	// Forward event to modules
	WeaponOnWeaponFire(clientIndex, weaponIndex);
}

/**
 * Event callback (decoy_firing)
 * The decoy nade is fired.
 * 
 * @param gEventHook       The event handle.
 * @param gEventName       Name of the event.
 * @param iDontBroadcast   If true, event is broadcasted to all clients, false if not.
 **/
public Action EventEntityDecoy(Event gEventHook, const char[] gEventName, bool iDontBroadcast) 
{
	// Initialize vector variables
	float flOrigin[3];

	// Get entity origin
	int nEntity = GetEventInt(gEventHook, "entityid");
	flOrigin[0] = GetEventFloat(gEventHook, "x"); 
	flOrigin[1] = GetEventFloat(gEventHook, "y"); 
	flOrigin[2] = GetEventFloat(gEventHook, "z");

	// If entity isn't valid, then stop
	if(!IsValidEdict(nEntity))
	{
		return;
	}

	// Forward event to modules
	GrenadeOnDecoyDetonate(nEntity, flOrigin);
}

/**
 * Event callback (smokegrenade_detonate)
 * The flashbang nade is exployed.
 * 
 * @param gEventHook       The event handle.
 * @param gEventName       Name of the event.
 * @param iDontBroadcast   If true, event is broadcasted to all clients, false if not.
 **/
public Action EventEntitySmoke(Event gEventHook, const char[] gEventName, bool iDontBroadcast) 
{
	// Initialize vector variables
	float flOrigin[3];

	// Get entity origin
	int nEntity = GetEventInt(gEventHook, "entityid");
	flOrigin[0] = GetEventFloat(gEventHook, "x"); 
	flOrigin[1] = GetEventFloat(gEventHook, "y"); 
	flOrigin[2] = GetEventFloat(gEventHook, "z");

	// If entity isn't valid, then stop
	if(!IsValidEdict(nEntity))
	{
		return;
	}

	// Forward event to modules
	GrenadeOnSmokeDetonate(nEntity, flOrigin);
}

/**
 * Event callback (Shotgun Shot)
 * The weapon is about to shoot.
 * 
 * @param sTEName       Temp name.
 * @param iPlayers      Array containing target player indexes.
 * @param numClients    Number of players in the array.
 * @param flDelay   	Delay in seconds to send the TE.
 **/
public Action EventEntityFireBullets(const char[] sTEName, const int[] iPlayers, int numClients, float flDelay)
{
    // Get all required event info
	int clientIndex = TE_ReadNum("m_iPlayer") + 1;

	// Validate client
	if (!IsPlayerExist(clientIndex))
	{
		return;
	}
	
	// Get weapon alias
	char sClassname[NORMAL_LINE_LENGTH];
	GetClientWeapon(clientIndex, sClassname, sizeof(sClassname));
	WeaponsGetAlias(sClassname, sizeof(sClassname));

	// Forward event to modules
	WeaponOnWeaponBullet(clientIndex, CS_AliasToWeaponID(sClassname));
}