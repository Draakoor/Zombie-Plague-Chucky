/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          global.h.cpp
 *  Type:          Main 
 *  Description:   General plugin defines.
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
 * @section: Similarly as with plain data structures, 
 * 			 the members of an object can be accessed directly from a pointer by using the arrow operator.
 **/
#define cBaseAttacker->					cBaseAttacker.
#define cBaseVictim->					cBaseVictim.
#define cBasePlayer->					cBasePlayer.
#define CBasePlayer*					CBasePlayer
/**
 * @endsection
 **/
 
/**
 * @section: Core static macroses.
 **/
#define ACTION_CONTINUE     			Plugin_Continue
#define ACTION_CHANGED    		  		Plugin_Changed
#define ACTION_HANDLED      			Plugin_Handled
#define ACTION_STOP						Plugin_Stop

#define SMALL_LINE_LENGTH 				32
#define NORMAL_LINE_LENGTH 				64
#define BIG_LINE_LENGTH 				128

#define UNVALID_MESSAGE_ID				0
#define NULL							INVALID_HANDLE

#define ZP_TEAM_ZOMBIE					CS_TEAM_T
#define ZP_TEAM_HUMAN					CS_TEAM_CT
/**
 * @endsection
 **/
 
/**
 * @section: List of all global variables and arrays.
 **/
bool   bNewRound;  	
bool   bEndRound;		
bool   bModeStarted;		
int    gZombieCount;
int    gRoundNum;
int    gMode;
bool   gZombie[MAXPLAYERS+1];
bool   gSurvivor[MAXPLAYERS+1];
bool   gNemesis[MAXPLAYERS+1];
int    gZombieClass[MAXPLAYERS+1];
int    gZombieClassNext[MAXPLAYERS+1];
int    gRespawn[MAXPLAYERS+1];
int    gRespawnTimes[MAXPLAYERS+1];
int    gAmmoPacks[MAXPLAYERS+1];
int    gLastBought[MAXPLAYERS+1];
int    gDamadeDealt[MAXPLAYERS+1];
int    gViewModelIndex[MAXPLAYERS+1];
int    gRoundLevel[MAXPLAYERS+1];
int    gTimeWait[MAXPLAYERS+1];
bool   gIsUsedSkill[MAXPLAYERS+1];
float  gLeapTimeWait[MAXPLAYERS+1];
float  gToolTimeWait[MAXPLAYERS+1];
float  gRestoreHealth[MAXPLAYERS+1];
Handle Task_ZombieRound 					= NULL;
Handle Task_ZombieSkill[MAXPLAYERS+1] 		= NULL;
Handle Task_ZombieRespawn[MAXPLAYERS+1] 	= NULL;
Handle Task_ZombieFreezed[MAXPLAYERS+1] 	= NULL;
/**
 * @endsection
 **/