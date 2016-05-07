/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          damage.cpp
 *  Type:          Game 
 *  Description:   Modify damage values here.
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
 * @section Counter Strike: Global Offensive specific damage flags.
 **/
#define DMG_CSGO_FALL        (DMG_FALL)      /** Client was damaged by falling.	 **/
#define DMG_CSGO_BLAST       (DMG_BLAST)     /** Client was damaged by explosion.**/
#define DMG_CSGO_BURN        (DMG_BURN)    	 /** Client was damaged by inferno.	 **/
#define DMG_CSGO_FIRE        (DMG_DIRECT)    /** Client was damaged by fire.	 **/
#define DMG_CSGO_BULLET      (DMG_NEVERGIB)  /** Client was shot or knifed. 	 **/
#define DMG_CSGO_DROWN    	 (DMG_DROWN)     /** Client was damaged by water. 	 **/
/**
 * @endsection
 **/
 
/**
 * @section m_nWaterLevel defines.
 **/
#define CSGO_WLEVEL_DRY 0
#define CSGO_WLEVEL_FEET 1
#define CSGO_WLEVEL_HALF 2
#define CSGO_WLEVEL_FULL 3
/**
 * @endsection
 **/
 
/**
 * Hook: OnTraceAttack
 * Called right before the bullet enters a client.
 * 
 * @param iVictim		The victim index.
 * @param iAttacker		The attacker index.
 * @param Inflictor		The inflictor index.
 * @param pDamageBits	The type of damage inflicted.
 * @param iAmmoType		The ammo type of the attacker's weapon.
 * @param iHitBox		The hitbox index.  
 * @param iHitGroup		The hitgroup index.  
 **/
public Action DamageOnTraceAttack(int iVictim, int &iAttacker, int &Inflictor, float &flDamage, int &pDamageBits, int &iAmmoType, int iHitBox, int iHitGroup)
{
	// Get real player index from event key 
	CBasePlayer* cBaseVictim = view_as<CBasePlayer>(iVictim);
	CBasePlayer* cBaseAttacker = view_as<CBasePlayer>(iAttacker);
	
	// If mode doesn't started yet, then stop
	if (bNewRound || bEndRound)
	{
		// Stop trace
		return ACTION_HANDLED;
	}
	
	// Verify that the clients are connected and alive
	if (!IsPlayerExist(view_as<int>(cBaseAttacker)) || !IsPlayerExist(view_as<int>(cBaseVictim)))
	{
		// Stop trace
		return ACTION_HANDLED;
	}

	// If clients have same team, then allow damage
	if (cBaseVictim->m_iTeamNum == cBaseAttacker->m_iTeamNum)
	{
		// Stop trace
		return ACTION_HANDLED;
	}

	// If damage hitgroups cvar is disabled, then allow damage
	if (!GetConVarBool(gCvarList[CVAR_HITGROUPS]))
	{
		// Allow trace
		return ACTION_CONTINUE;
	}

	// Get hitgroup index
	int iIndex = HitgroupToIndex(iHitGroup);

	// If index can't be found, then allow damage
	if (iIndex == -1)
	{
		// Allow trace
		return ACTION_CONTINUE;
	}

	// If damage is disabled for this hitgroup, then stop
	if (!HitgroupsCanDamage(iIndex))
	{
		// Stop trace
		return ACTION_HANDLED;
	}

	// Allow trace
	return ACTION_CONTINUE;
}

/**
 * Hook: OnTakeDamage
 * Called right before damage is done.
 * 
 * @param iVictim		The victim index.
 * @param iAttacker		The attacker index.
 * @param iInflicter	The inflicter index.
 * @param flDamage		The amount of damage inflicted.
 * @param pDamageBits	The type of damage inflicted.
 **/
public Action DamageOnTakeDamage(int iVictim, int &iAttacker, int &iInflicter, float &flDamage, int &pDamageBits)
{
	// If mode doesn't started yet, then stop
	if (bNewRound || bEndRound)
	{
		return ACTION_HANDLED;
	}

	// If client is attacking himself, then stop
	if(iVictim == iAttacker)
	{
		return ACTION_HANDLED;
	}

	// Get class name of the inflicter
	char sClassname[NORMAL_LINE_LENGTH];
	GetEdictClassname(iInflicter, sClassname, sizeof(sClassname));

	// If entity is a trigger, then allow damage (Map is damaging client)
	if(StrContains(sClassname, "trigger") > -1)
	{
		return ACTION_CONTINUE;
	}

	// Get real player index from event key 
	CBasePlayer* cBaseVictim = view_as<CBasePlayer>(iVictim);

	// Verify that the victim is connected and alive
	if (!IsPlayerExist(view_as<int>(cBaseVictim)))
	{
		return ACTION_HANDLED;
	}

	// Forward this hook to another stock an return (or not) amount of damage
	float flFire = NapalmOnTakeDamage(cBaseVictim, pDamageBits, flDamage, cBaseVictim->m_iMoveType);
	
	// Client was burned
	if(flFire > 0.0)
	{
		flDamage = flFire;
	}
	// Client was damaged by inferno or fire
	else if (pDamageBits & DMG_CSGO_BULLET)
    {
		// Get real player index from event key 
		CBasePlayer* cBaseAttacker = view_as<CBasePlayer>(iAttacker);
	
		// Verify that the attacker is connected and alive
		if (!IsPlayerExist(view_as<int>(cBaseAttacker)))
		{
			return ACTION_HANDLED;
		}
		
		// If client and attacker are on the same team
		if (cBaseVictim->m_bZombie == cBaseAttacker->m_bZombie)
		{
			return ACTION_HANDLED;
		}
		
		// Get damage of nemesis
		float flNemesisDamage = GetConVarFloat(gCvarList[CVAR_NEMESIS_DAMAGE]);
		
		// If victim survivor or nemesis
		if(cBaseVictim->m_bSurvivor || cBaseVictim->m_bNemesis)
		{
			// Apply damage without pain shock
			return PainShockOnTakeDamage(cBaseVictim, cBaseAttacker->m_bNemesis ? flNemesisDamage : flDamage);
		}
		
		// Client is about to be infected
		if(cBaseAttacker->m_bZombie && (gMode == MODE_INFECTION || gMode == MODE_MULTI))
		{
			// Make a zombie
			return ArmorOnTakeDamage(cBaseVictim, cBaseAttacker, flDamage);
		}
		else
		{
			// If attacker is a zombie
			if(cBaseAttacker->m_bZombie)
			{
				// Disable flash light for fix : No such sound Player.FlashlightOff
				cBaseVictim->m_bFlashLightOn(false);  
				
				// Change damage for nemesis
				flDamage = cBaseAttacker->m_bNemesis ? flNemesisDamage : flDamage;
			}
			else
			{
				// Change amount of damage
				if(cBaseVictim->m_iMoveType != MOVETYPE_NONE)
				{
					flDamage *= cBaseAttacker->m_bSurvivor ? GetConVarFloat(gCvarList[CVAR_SURVIVOR_DAMAGE]) : (float(cBaseAttacker->m_iLevel) * (GetConVarBool(gCvarList[CVAR_LVL_SYSTEM]) ? GetConVarFloat(gCvarList[CVAR_HUMAN_LEVEL_RATIO]) : 1.0));
				}
				else return GetConVarBool(gCvarList[CVAR_GRENADE_FREEZING_DAMAGE]) ? ACTION_CONTINUE : ACTION_HANDLED; // Block damage if victim freezed ?
				
				// Show applied damage for attacker
				MessagesOnTakeDamage(cBaseVictim, cBaseAttacker, flDamage);
				
				// Reward ammopacks
				RewardOnTakeDamage(cBaseAttacker, flDamage);

				// Play hurt sound
				cBaseVictim->InputEmitAISound(SNDCHAN_BODY, SNDLEVEL_SCREAMING, cBaseVictim->m_bNemesis ? "ZOMBIE_NEMESIS_HURT_SOUNDS" : ((ZombieIsFemale(cBaseVictim->m_nZombieClass)) ? "ZOMBIE_FEMALE_HURT_SOUNDS" : "ZOMBIE_HURT_SOUNDS"), true);
			}
		}
	}
	// Client was damaged by explosion
	else if (pDamageBits & DMG_CSGO_BLAST)
    {
		flDamage *= GetConVarFloat(gCvarList[CVAR_GRENADE_DAMAGE_HEGRENADE]);
	}
	
	// Allow damage
	return ACTION_CHANGED;
}

/**
 * Hook: OnTakeDamage
 * Forwarded from the damage module to check if we should extinguish any flames.
 * 
 * @param cBaseVictim	The victim index.
 * @param pDamageBits	The type of damage inflicted.
 * @param flDamage		The amount of damage inflicted.
 * @param iMoveType		Movetype, see enum in entity_prop_stocks.inc
 *
 * @return				Return amount of damage.
 **/
float NapalmOnTakeDamage(CBasePlayer* cBaseVictim, int &pDamageBits, float &flDamage, MoveType iMoveType)
{
    // Client was damaged by inferno or fire
    if (pDamageBits & DMG_CSGO_BURN || pDamageBits & DMG_CSGO_FIRE)
    {
		// If victim is zombie
        if (cBaseVictim->m_bZombie && !cBaseVictim->m_bNemesis)
        {
            // If the client water-level is equal or higher than the given, then we want to extinguish the flame
            if (iMoveType != MOVETYPE_NONE && GetEntProp(view_as<int>(cBaseVictim) , Prop_Data, "m_nWaterLevel") <= CSGO_WLEVEL_FEET)
            {
                // Put the fire in
				if(pDamageBits & DMG_CSGO_BURN) IgniteEntity(view_as<int>(cBaseVictim) , GetConVarFloat(gCvarList[CVAR_GRENADE_IGNITTING]));
                
				// Randomly play burn sound
				if(Math_GetRandomInt(0, 1)) cBaseVictim->InputEmitAISound(SNDCHAN_BODY, SNDLEVEL_SCREAMING, (ZombieIsFemale(cBaseVictim->m_nZombieClass)) ? "ZOMBIE_FEMALE_BURN_SOUNDS" : "ZOMBIE_BURN_SOUNDS", true);
			
				// Change damage
				flDamage *= GetConVarFloat(gCvarList[CVAR_GRENADE_DAMAGE_MOLOTOV]);

				// Allow damage
				return flDamage;
            }
			else
			{
				// This works, compare to ExtinguishEntity();
				int iFire = GetEntPropEnt(view_as<int>(cBaseVictim), Prop_Data, "m_hEffectEntity");
				if (IsValidEntity(iFire))
				{
					// Make sure the entity is a flame, so we can extinguish it
					char sClassname[SMALL_LINE_LENGTH];
					GetEdictClassname(iFire, sClassname, sizeof(sClassname));
					if (StrEqual(sClassname, "entityflame", false))
					{
						SetEntPropFloat(iFire, Prop_Data, "m_flLifetime", 0.0);
					}
				}
			}
        }
    }
	
    // Let the damage module continue as usual
    return 0.0;
}

/**
 * Hook: OnTakeDamage
 * Reducing armor and infecting victim.
 *
 * @param cBaseVictim		The victim index.
 * @param cBaseAttacker		The attacker index.
 * @param flDamage			The amount of damage inflicted. 
 **/
Action ArmorOnTakeDamage(CBasePlayer* cBaseVictim, CBasePlayer* cBaseAttacker, float &flDamage)
{
	// Human armor need to be reduced before infecting ?
	if(GetConVarBool(gCvarList[CVAR_HUMAN_ARMOR_PROTECT]))
	{
		// Get amount of armor
		int vArmor = cBaseVictim->m_iArmorValue;

		// Get victim armor
		if (vArmor > 0)
		{
			vArmor -= RoundFloat(flDamage);
		}
		
		// Allow armor damage
		if (vArmor > 0)
		{
			cBaseVictim->m_iArmorValue = vArmor;
			return ACTION_HANDLED;
		}
	}

	// Make a zombie
	InfectHumanToZombie(view_as<int>(cBaseVictim), view_as<int>(cBaseAttacker));
	
	// Block damage
	return ACTION_HANDLED;
}

/**
 * Hook: OnTakeDamage
 * Show applied damage HUD.
 *
 * @param cBaseVictim		The victim index.
 * @param cBaseAttacker		The attacker index.
 * @param flDamage			The amount of damage inflicted. 
 **/
void MessagesOnTakeDamage(CBasePlayer* cBaseVictim, CBasePlayer* cBaseAttacker, float &flDamage)
{
	// If hud disabled
	if(!GetConVarBool(gCvarList[CVAR_HUD_HEALTH_MESSAGES]))
	{
		return;
	}
	
	// Initialize char
	char sInfo[BIG_LINE_LENGTH];
	
	// Count damage
	int iHealth = cBaseVictim->m_iHealth - RoundFloat(flDamage);

	// Format message
	Format(sInfo, BIG_LINE_LENGTH, "<font color='#FFFFFF'>HP:</font> <font color='#FF0000'>%i</font>", (iHealth < 0) ? 0 : iHealth);
	
	// Sent hint message
	Handle hMessage = StartMessageOne("HintText", view_as<int>(cBaseAttacker));
	PbSetString(hMessage, "text", sInfo);
	EndMessage();
}

/**
 * Hook: OnTakeDamage
 * Reward ammopacks for applied damage.
 *
 * @param cBaseAttacker		The attacker index.
 * @param flDamage			The amount of damage inflicted. 
 **/
void RewardOnTakeDamage(CBasePlayer* cBaseAttacker, float &flDamage)
{
	// Add damage to filter
	cBaseAttacker->m_nDamageFilter += RoundFloat(flDamage);

	// Counting amount of reward
	int nReward = cBaseAttacker->m_bSurvivor ? GetConVarInt(gCvarList[CVAR_SURVIVOR_DAMAGE_REWARD]) : GetConVarInt(gCvarList[CVAR_HUMAN_DAMAGE_REWARD]);
	
	// Reward ammo packs for every [ammo damage] dealt
	while (cBaseAttacker->m_nDamageFilter > nReward)
	{
		// Give reward for attacker
		cBaseAttacker->m_nAmmoPacks++;

		// Reset damage filter
		cBaseAttacker->m_nDamageFilter -= nReward;
	}
}

/**
 * Hook: OnTakeDamage
 * Damage without pain shock.
 *
 * @param cBaseVictim		The victim index.
 * @param flDamage			The amount of damage applying.
 **/
Action PainShockOnTakeDamage(CBasePlayer* cBaseVictim, float flDamage)
{
	// Get health
	int vHealth = cBaseVictim->m_iHealth;
	
	// If health more than zero
	if(vHealth > 0)
	{
		vHealth -= RoundFloat(flDamage);
	}
	
	// Allow damage without painshock
	if(vHealth > 0)
	{
		cBaseVictim->m_iHealth = vHealth;
		return ACTION_HANDLED;
	}
	else
		return ACTION_CONTINUE;
}