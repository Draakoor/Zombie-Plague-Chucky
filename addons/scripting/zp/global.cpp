/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          global.cpp
 *  Type:          Main 
 *  Description:   General plugin functions.
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
 
/*
 * Note: See global.h for header types and defines.
 */

#include "zp/global.h.cpp"
 
/***************************************************************************************
 *                                													   *
 *   							  METHOD MAP CONSTRUCTOR	  	  					   *		
 *                                													   *
 ***************************************************************************************/

methodmap CBasePlayer
{
	// Health
    property int m_iHealth
    {
        public get() 
		{ 
			return GetClientHealth(view_as<int>(this)); 
		}
        public set(int iValue) 
		{ 
			SetEntProp(view_as<int>(this), Prop_Send, "m_iHealth", iValue, 4); 
		}
    }
	
	// Armor
    property int m_iArmorValue
    {
        public get() 
		{ 
			return GetClientArmor(view_as<int>(this)); 
		}
        public set(int iValue) 
		{ 
			SetEntProp(view_as<int>(this), Prop_Send, "m_ArmorValue", iValue, 1); 
		}
    }
	
	// Speed
	property float m_flLaggedMovementValue
	{
		public get() 
		{ 
			return GetEntPropFloat(view_as<int>(this), Prop_Data, "m_flLaggedMovementValue"); 
		}
		public set(float flValue) 
		{ 
			SetEntPropFloat(view_as<int>(this), Prop_Data, "m_flLaggedMovementValue", flValue); 
		}
	}
	
	// Gravity
	property float m_flGravity
	{
		public get() 
		{ 
			return GetEntityGravity(view_as<int>(this)); 
		}
		public set(float flValue)
		{ 
			SetEntityGravity(view_as<int>(this), flValue); 
		}
	}
	
	// Flags
	property int m_iFrags
	{
		public get() 
		{ 
			return GetEntProp(view_as<int>(this), Prop_Data, "m_iFrags"); 
		}
		public set(int iValue)
		{ 
			SetEntProp(view_as<int>(this), Prop_Data, "m_iFrags", iValue); 
		}
	}
	
	// Deathes
	property int m_iDeaths
	{
		public get() 
		{ 
			return GetEntProp(view_as<int>(this), Prop_Data, "m_iDeaths");
		}
		public set(int iValue) 
		{ 
			SetEntProp(view_as<int>(this), Prop_Data, "m_iDeaths", iValue); 
		}
	}
	
	// Model index
	property int m_nModelIndex
	{
		public get() 
		{ 
			return gViewModelIndex[this]; 
		}
		public set(int iValue) 
		{ 
			gViewModelIndex[this] = iValue; 
		}
	}
	
	// Team
	property int m_iTeamNum
	{
		public get() 
		{ 
			return GetClientTeam(view_as<int>(this)); 
		}
		public set(int iValue) 
		{ 
			CS_SwitchTeam(view_as<int>(this), iValue);
		}
	}
	
	// Zombie
    property bool m_bZombie
    {
        public get() 
		{ 
			return gZombie[this]; 
		}
        public set(bool bValue) 
		{ 
			gZombie[this] = bValue; 
		}
    }
	
	// Survivor
	property bool m_bSurvivor
    {
        public get() 
		{ 
			return gSurvivor[this]; 
		}
        public set(bool bValue)
		{ 
			gSurvivor[this] = bValue;
		}
    }
	
	// Nemesis
	property bool m_bNemesis
    {
        public get() 
		{ 
			return gNemesis[this]; 
		}
        public set(bool bValue) 
		{ 
			gNemesis[this] = bValue; 
		}
    }
	
	// Zombie class
	property int m_nZombieClass
    {
        public get() 
		{ 
			return gZombieClass[this]; 
		}
        public set(int iValue) 
		{ 
			gZombieClass[this] = iValue; 
		}
    }
	
	// Next zombie class
	property int m_nZombieNext
    {
        public get() 
		{ 
			return gZombieClassNext[this]; 
		}
        public set(int iValue) 
		{ 
			gZombieClassNext[this] = iValue;
		}
    }
	
	// Team on respawn
	property int m_bRespawn
    {
        public get() 
		{ 
			return gRespawn[this]; 
		}
        public set(int bValue) 
		{ 
			gRespawn[this] = bValue; 
		}
    }
	
	// Amount of respawnings
	property int m_nRespawnTimes
    {
        public get() 
		{ 
			return gRespawnTimes[this]; 
		}
        public set(int iValue) 
		{ 
			gRespawnTimes[this] = iValue;
		}
    }

	// Ammopacks
	property int m_nAmmoPacks
    {
        public get() 
		{ 
			return gAmmoPacks[this]; 
		}
        public set(int iValue) 
		{
			if(iValue > 65000)
			{
				iValue = 65000; 
			}

			gAmmoPacks[this] = iValue; 
		}
    }
	
	// Amount of spended amopacks
	property int m_nLastBoughtAmount
    {
        public get() 
		{ 
			return gLastBought[this]; 
		}
        public set(int iValue)
		{ 
			gLastBought[this] = iValue;
		}
    }
	
	// Applied damage
	property int m_nDamageFilter
    {
        public get() 
		{ 
			return gDamadeDealt[this]; 
		}
        public set(int iValue) 
		{ 
			gDamadeDealt[this] = iValue;
		}
    }
	
	// Skill
	property bool m_bSkill
    {
        public get() 
		{ 
			return gIsUsedSkill[this]; 
		}
        public set(bool bValue) 
		{ 	
			gIsUsedSkill[this] = bValue; 
		}
    }
	
	// Skill countdown
	property int m_nSkillCountDown
    {
        public get() 
		{ 
			return gTimeWait[this]; 
		}
        public set(int iValue) 
		{ 
			gTimeWait[this] = iValue; 
		}
    }
	
	// Level
	property int m_iLevel
    {
        public get() 
		{ 
			return gRoundLevel[this]; 
		}
        public set(int iValue) 
		{ 
			gRoundLevel[this] = iValue; 
		}
    }
	
	// Health restore countdown
	property float m_flHealthRegen
    {
        public get() 
		{ 
			return gRestoreHealth[this]; 
		}
        public set(float flValue) 
		{ 
			gRestoreHealth[this] = flValue; 
		}
    }
	
	// Leap jump countdown
	property float m_flLeapCountDown
    {
        public get() 
		{ 
			return gLeapTimeWait[this]; 
		}
        public set(float flValue) 
		{ 
			gLeapTimeWait[this] = flValue; 
		}
    }
	
	// Tools countdown
	property float m_flToolCountDown
    {
        public get() 
		{ 
			return gToolTimeWait[this]; 
		}
        public set(float flValue) 
		{ 
			gToolTimeWait[this] = flValue; 
		}
    }
	
	// Nightvision
	property int m_bNightVisionOn
	{
		public get() 
		{ 
			return GetEntProp(view_as<int>(this), Prop_Send, "m_bNightVisionOn"); 
		}
        public set(int iValue) 
		{ 
			SetEntProp(view_as<int>(this), Prop_Send, "m_bNightVisionOn", iValue); 
		}
	}
	
	// Flags
	property int m_iFlags
	{
		public get() 
		{ 
			return GetEntityFlags(view_as<int>(this));
		}
        public set(int iFlags) 
		{ 
			SetEntityFlags(view_as<int>(this), iFlags);
		}
	}
	
	// Movetype
	property MoveType m_iMoveType
	{
		public get() 
		{ 
			return GetEntityMoveType(view_as<int>(this));
		}
        public set(MoveType iValue) 
		{ 
			SetEntityMoveType(view_as<int>(this), iValue);
		}
	}
	
	// Money HUD
	public void m_iAccount(int iValue)
	{
		SetEntProp(view_as<int>(this), Prop_Send, "m_iAccount", iValue );
	}
	
	// Fov distance
	public void m_iFOV(int iValue)
	{
		SetEntProp(view_as<int>(this), Prop_Send, "m_iFOV", iValue);
		SetEntProp(view_as<int>(this), Prop_Send, "m_iDefaultFOV", iValue);
	}
	
	// Render color
	public void m_iRender(int iRed, int iGreen, int iBlue)
	{
		SetEntityRenderMode(view_as<int>(this), RENDER_TRANSCOLOR);
		SetEntityRenderColor(view_as<int>(this), iRed, iGreen, iBlue, 255);
	}
	
	// Origin vector
	public void m_flGetOrigin(float flValue[3])
	{
		GetClientAbsOrigin(view_as<int>(this), flValue);
	}
	
	// Eye angle vector
	public void m_flGetEyeAngles(float flValue[3])
	{
		GetClientEyeAngles(view_as<int>(this), flValue);
	}
	
	// Teleport
	public void m_iTeleportPlayer(float flOrigin[3] = NULL_VECTOR, float flAngles[3] = NULL_VECTOR, float flVelocity[3] = NULL_VECTOR)
	{
		TeleportEntity(view_as<int>(this), flOrigin, flAngles, flVelocity);
	}

	// Respawn
	public void m_iRespawnPlayer()
	{
		CS_RespawnPlayer(view_as<int>(this));
	}
	
	// Flashlight
	public void m_bFlashLightOn(bool bSwitch)
	{
		// Need to be switch
		if(bSwitch)
		{
			SetEntProp(view_as<int>(this), Prop_Send, "m_fEffects", GetEntProp(view_as<int>(this), Prop_Send, "m_fEffects") ^ 4); 
		}
		// Need to be just turn off ?
		else
		{
			SetEntProp(view_as<int>(this), Prop_Send, "m_fEffects", 4 ^ 4); 
		}
	}
	
	// Set glowing
	public void m_bSetGlow(bool bValue)
	{
		SetEntPropFloat(view_as<int>(this), Prop_Send, "m_flDetectedByEnemySensorTime", bValue ? (GetGameTime() + 9999.0) : 0.0);
	}
	
	// Give weapon or item
	public void CItemMaterialize(char[] sClassname)
	{
		// Create item
		int iEntity = GivePlayerItem(view_as<int>(this), view_as<char>(sClassname));
		
		// Try to call deploy
		FakeClientCommandEx(view_as<int>(this), "use %s", view_as<char>(sClassname));
		
		// If entity is valid, switch to it
		if(IsValidEdict(iEntity))
		{
			SetEntPropEnt(view_as<int>(this), Prop_Data, "m_hActiveWeapon", iEntity);
			ChangeEdictState(view_as<int>(this), FindDataMapOffs(view_as<int>(this), "m_hActiveWeapon"));
		}
	}
	
	// Emit sound
	public void InputEmitAISound(int iChannel, int iLevel, char[] sSound, bool bDynamic = false)
	{
		// Initialize char
		char sBuffer[PLATFORM_MAX_PATH];
		
		// If round sound ?
		if(bDynamic)
		{
			// Select one from global sound array
			SoundsGetRandomSound(sBuffer, sizeof(sBuffer), sSound);
			Format(sBuffer, sizeof(sBuffer), "*/%s", sBuffer);
		}
		else
		{
			// Else static one
			Format(sBuffer, sizeof(sBuffer), "*/%s", sSound);
		}

		// Emit sound to all
		EmitSoundToAll(sBuffer, view_as<int>(this), iChannel, iLevel);
	}
	
	// Set model
	public void m_ModelName(char[] sModel)
    {
		SetEntityModel(view_as<int>(this), sModel);
    }
}