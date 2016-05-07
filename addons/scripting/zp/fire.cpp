/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          fire.cpp
 *  Type:          Game 
 *  Description:   Fire on weapon event.
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
 * Weapon has been fired.
 *
 * @param clientIndex	The client index.
 * @param weaponIndex	The weapon index.
 **/
void WeaponOnWeaponFire(int clientIndex, int weaponIndex) 
{ 
	// Get real player index from event key
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);
	
	// Change zombie attack sounds
	if(cBasePlayer->m_bZombie) 
	{
		cBasePlayer->InputEmitAISound(SNDCHAN_ITEM, SNDLEVEL_NORMAL, (ZombieIsFemale(cBasePlayer->m_nZombieClass)) ? "ZOMBIE_FEMALE_ATTACK_SOUNDS" : "ZOMBIE_ATTACK_SOUNDS", true);
	}
	else
	{
		// If weapon isn't valid, then stop
		if(!IsValidEdict(weaponIndex))
		{
			return;
		}

		// Get current mode of infinity ammo
		switch(cBasePlayer->m_bSurvivor ? GetConVarInt(gCvarList[CVAR_SURVIVOR_INF_AMMO]) : GetConVarInt(gCvarList[CVAR_HUMAN_INF_AMMO]))
		{
			case 0 : return;
			case 1 : SetEntProp(weaponIndex, Prop_Send, "m_iPrimaryReserveAmmoCount", 200);
			case 2 : SetEntProp(weaponIndex, Prop_Data, "m_iClip1", GetEntProp(weaponIndex, Prop_Data, "m_iClip1") + 1);
		}
	}
}  

/**
 * Bullet has been fired.
 *
 * @param clientIndex	The client index.
 * @param weaponID		The weapon id.
 **/
void WeaponOnWeaponBullet(int clientIndex, CSWeaponID weaponID) 
{
	// Get real player index from event key
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);
	
	// Change survivor attack sounds
	if(cBasePlayer->m_bSurvivor)
	{
		// Play custom weapon sounds
		switch(weaponID)
		{
			// Survivor's M249
			case CSWeapon_M249  :
			{
				cBasePlayer->InputEmitAISound(SNDCHAN_WEAPON, SNDLEVEL_ROCKET,  sMachinegunSound);
				cBasePlayer->InputEmitAISound(SNDCHAN_AUTO, SNDLEVEL_NORMAL, sMachinegunSound);
			}
			// Survivor's Dual Elite
			case CSWeapon_ELITE :
			{
				cBasePlayer->InputEmitAISound(SNDCHAN_WEAPON, SNDLEVEL_ROCKET,  sPistolSound);
				cBasePlayer->InputEmitAISound(SNDCHAN_AUTO, SNDLEVEL_NORMAL, sPistolSound);
			}
		}
	}
}