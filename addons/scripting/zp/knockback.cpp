/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          knockback.cpp
 *  Type:          Game 
 *  Description:   Hurt event.
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
 
/** Client has been hurt.
 *
 * @param iVictim		The client index. (zombie)
 * @param iAttacker		The attacker index. (human)
 * @param weaponID		The weapon id.
 * @param iHitgroup		Hitgroup attacker has damaged. 
 * @param iDamage		Damage done.
 **/
Action KnockbackOnClientHurt(int iVictim, int iAttacker, CSWeaponID weaponID, int iHitgroup, int iDamage)
{
	// Get real player index from event key
	CBasePlayer* cBaseVictim = view_as<CBasePlayer>(iVictim);
	CBasePlayer* cBaseAttacker = view_as<CBasePlayer>(iAttacker);

	// Verify that the client is connected and alive
	if (!IsPlayerExist(view_as<int>(cBaseAttacker)) || !IsPlayerExist(view_as<int>(cBaseVictim)))
	{
		return ACTION_HANDLED;
	}

	// If victim is zombie
	if(cBaseVictim->m_bZombie)
	{
		// Initialize vectors
		float flClientLoc[3];
		float flAttackerLoc[3];
		
		// Get victim position
		cBaseVictim->m_flGetOrigin(flClientLoc);

		// Get attackers eye angles
		float flAttackeRang[3];
		float flKnockBack;
		
		// Calculate knockback velocity
		switch(weaponID)
		{
			// Check if a grenade was thrown
			case CSWeapon_HEGRENADE :
			{
				flKnockBack = GetConVarFloat(gCvarList[CVAR_GRENADE_KNOCKBACK_HEGRENADE]);
				
				// Get the location of the grenade
				if (KnockbackFindExplodingGrenade(flAttackerLoc) == -1)
				{
					// If the grenade wasn't found, then stop
					return ACTION_HANDLED;
				}
			}
			// Check if a victim was hit by knife
			case CSWeapon_KNIFE :
			{
				flKnockBack = GetConVarFloat(gCvarList[CVAR_HUMAN_KNIFE_KNOCKBACK]);
			}
			// Otherelse
			default :
			{
				// Get attackers eye position
				GetClientEyePosition(view_as<int>(cBaseAttacker), flAttackerLoc);
				
				// Get head position of attacker
				cBaseAttacker->m_flGetEyeAngles(flAttackeRang);

				// Calculate knockback end-vector
				TR_TraceRayFilter(flAttackerLoc, flAttackeRang, MASK_ALL, RayType_Infinite, IsValidPlayer);
				TR_GetEndPosition(flClientLoc);
				
				// Get weapon alias
				char sAlias[SMALL_LINE_LENGTH];
				CS_WeaponIDToAlias(weaponID, sAlias, sizeof(sAlias));
				
				// Apply weapon knockback multiplier by weapon alias, for setting knockback multiplier 
				int iIndex = WeaponsEntityToIndex(sAlias);
				if (iIndex != -1)
				{
					flKnockBack = WeaponsGetKnockBack(iIndex);
				}
			}
		}
		
		// If hitgrops enabled
		if (GetConVarBool(gCvarList[CVAR_HITGROUPS]))
		{
			int iHitIndex = HitgroupToIndex(iHitgroup);
			if (iHitIndex != -1)
			{
				// Apply hitgroup knockback multiplier
				flKnockBack *= HitgroupsGetKnockback(iHitIndex);
			}
		}

		// Apply damage knockback multiplier
		flKnockBack *= iDamage;

		// Apply multiplier if client on air
		if(GetEntPropEnt(view_as<int>(cBaseVictim), Prop_Send, "m_hGroundEntity") == -1) flKnockBack *= GetConVarFloat(gCvarList[CVAR_KNOCKBACK_AIR]);
		
		// Apply knockback
		KnockbackSetVelocity(view_as<int>(cBaseVictim), flAttackerLoc, flClientLoc, flKnockBack);
	}

	// Allow damage
	return ACTION_CONTINUE;
}

/**
 * Set velocity to victim.
 *  
 * @param clientIndex	The client index.
 * @param flStartPoint	The starting coordinate to push from.
 * @param flEndPoint	The ending coordinate to push towards.
 * @param flMagniTude	Magnitude of the push.
 **/
void KnockbackSetVelocity(int clientIndex, float flStartPoint[3], float flEndPoint[3], float flMagniTude)
{
	// Get real player index from event key 
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);
	
	// Create vector from the given starting and ending points
	float flVector[3];
	MakeVectorFromPoints(flStartPoint, flEndPoint, flVector);

	// Normalize the vector (equal magnitude at varying distances)
	NormalizeVector(flVector, flVector);

	// Apply the magnitude by scaling the vector (multiplying each of its components)
	ScaleVector(flVector, flMagniTude);

	// Apply knock back boost if enabled
	if(GetConVarBool(gCvarList[CVAR_KNOCKBACK_BOOST]))
	{
		// Initialize velocity vector
		float flVelocity[3];
		ToolsGetVelosity(view_as<int>(cBasePlayer), flVelocity);

		// Get amount of boost
		float flKnockBackBoost = GetConVarFloat(gCvarList[CVAR_KNOCKBACK_BOOST_AMOUNT]);
		float flKnockBackBoostMax = GetConVarFloat(gCvarList[CVAR_KNOCKBACK_BOOST_MAX]);
		
		// Remove boost if current velocity is too high
		if (flVelocity[2] > flKnockBackBoostMax)
		{
			// Don't add extra boost
			flVector[2] = 0.0;
		}
		else if (cBasePlayer->m_iFlags & FL_ONGROUND && flVector[2] < flKnockBackBoost)
		{
			// Apply minimum boost required to push player off the ground
			flVector[2] = flKnockBackBoost;
		}
	}

	// Knock back victim
	cBasePlayer->m_iTeleportPlayer(NULL_VECTOR, NULL_VECTOR, flVector);
}

/**
 * Find the location of an exploding grenade (currently inflicting damage in player_hurt).
 *  
 * @param flGrenadeLoc	The location of the exploding grenade.
 *
 * @return				The entity index of the grenade. 
 **/  
int KnockbackFindExplodingGrenade(float flGrenadeLoc[3])
{
	// Initialize char
    char sClassname[NORMAL_LINE_LENGTH];
    
    // Find max entities and loop through all of them
    int nGetMaxEnt = GetMaxEntities();
	
    for (int nEntity = MaxClients; nEntity <= nGetMaxEnt; nEntity++)
    {
        // If entity is invalid, then stop
        if (!IsValidEdict(nEntity))
        {
            continue;
        }
        
        // If entity isn't a grenade, then stop
        GetEdictClassname(nEntity, sClassname, sizeof(sClassname));
		
        if (!StrEqual(sClassname, "hegrenade_projectile", false))
        {
            continue;
        }
        
        // If m_takedamage is set to 0, we found our grenade
        int iTakeDamage = GetEntProp(nEntity, Prop_Data, "m_takedamage");
		
        if (!iTakeDamage)
        {
            // Return its location
            GetEntPropVector(nEntity, Prop_Send, "m_vecOrigin", flGrenadeLoc);
            
            // Return its entity index
            return nEntity;
        }
    }
    
    // Didn't find the grenade
    return -1;
}

/**
 * Trace Ray forward, used as a filter to continue tracing if told so. (See sdktools_trace.inc)
 *  
 * @param iEntity			The entity index.
 * @param iContentsMask		The contents mask.
 * @return					True to allow hit, false to continue tracing. 
 **/
public bool IsValidPlayer(int iEntity, int iContentsMask)
{
	return !(1 <= iEntity <= MaxClients);
}