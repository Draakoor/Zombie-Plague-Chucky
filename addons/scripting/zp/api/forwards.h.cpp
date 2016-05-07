/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          natives.h.cpp
 *  Type:          API 
 *  Description:   Forwards handlers for the ZP API.
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
 * @section: Global forward handles.
 **/
Handle gForwardOnClientInfected 		= NULL;
Handle gForwardOnModStarted 			= NULL;
Handle gForwardOnBuyingExtraItem 		= NULL;
Handle gForwardOnClientHeroed 			= NULL;
Handle gForwardOnSkillUsed				= NULL;
Handle gForwardOnSkillOver				= NULL;
/**
 * @endsection
 **/

/**
 * Initializes all natives and forwards related to infection.
 **/
void APIForwardsInit(/*void*/)
{
    // Load Forwards
	gForwardOnClientInfected 	= CreateGlobalForward("ZP_OnClientInfected", 	ET_Ignore, Param_Cell, Param_Cell);
	gForwardOnModStarted 		= CreateGlobalForward("ZP_OnZombieModStarted",  ET_Ignore, Param_Cell);
	gForwardOnBuyingExtraItem	= CreateGlobalForward("ZP_OnExtraBuyCommand", 	ET_Hook,   Param_Cell, Param_Cell);
	gForwardOnClientHeroed		= CreateGlobalForward("ZP_OnClientHeroed", 		ET_Ignore, Param_Cell);
	gForwardOnSkillUsed			= CreateGlobalForward("ZP_OnSkillUsed", 		ET_Hook,   Param_Cell);
	gForwardOnSkillOver			= CreateGlobalForward("ZP_OnSkillOver", 		ET_Ignore, Param_Cell);
}

/**
 * Called when a client is about to become a zombie.
 * 
 * @param cBaseVictim      	 	The client index.
 * @param cBaseAttacker     	The attacker index.
 *
 **/
void APIOnClientInfected(CBasePlayer* cBaseVictim, CBasePlayer* cBaseAttacker)
{
    // Start forward call
	Call_StartForward(gForwardOnClientInfected);

	// Push the parameters
	Call_PushCell(view_as<int>(cBaseVictim));
	Call_PushCell(view_as<int>(cBaseAttacker));
	
	// Finish the call
	Call_Finish();
}

/**
 * Called after a zombie round is started.
 * 
 * @param iGameMode             The type of round mode. See enum ZPModeType.
 *
 **/
void APIOnModStarted(int iGameMode)
{
	// Start forward call
	Call_StartForward(gForwardOnModStarted);

	// Push the parameters
	Call_PushCell(iGameMode);

	// Finish the call
	Call_Finish();
}

/**
 * Called after select an extraitem in equipment menu.
 * 
 * @param cBasePlayer           The client index.
 * @param iExtraItemIndex       Index of extra item from ZP_RegisterExtraItem() native.
 *
 * @return                  	Plugin_Handled to block purhase. Anything else
 *                          				(like Plugin_Continue) to allow purhase and taking ammopacks.
 **/
Action APIOnBuyingExtraItem(CBasePlayer* cBasePlayer, int iExtraItemIndex)
{
	// Initialize future result
	Action iResult;
	
	// Start forward call
	Call_StartForward(gForwardOnBuyingExtraItem);
	
	// Push the parameters
	Call_PushCell(view_as<int>(cBasePlayer));
	Call_PushCell(iExtraItemIndex);
	
	// Finish the call
	Call_Finish(iResult);
	
	// Return result
	return iResult;
}

/**
 * Called when a client is about to become a survivor.
 * 
 * @param cBasePlayer        	The client index.
 *
 **/
void APIOnClientHeroed(CBasePlayer* cBasePlayer)
{
	// Start forward call
	Call_StartForward(gForwardOnClientHeroed);

	// Push the parameters
	Call_PushCell(view_as<int>(cBasePlayer));

	// Finish the call
	Call_Finish();
}

/**
 * Called when a zombie press E button. Activation of skill.
 * 
 * @param cBasePlayer           The client index.
 *
 * @return                  	Plugin_Handled to block using skill. Anything else
 *                          				(like Plugin_Continue) to allow use.
 *
 **/
Action APIOnSkillUsed(CBasePlayer* cBasePlayer)
{
	// Initialize future result
	Action iResult;

	// Start forward call
	Call_StartForward(gForwardOnSkillUsed);

	// Push the parameters
	Call_PushCell(view_as<int>(cBasePlayer));

	// Finish the call
	Call_Finish(iResult);
	
	// Return result
	return iResult;
}

/**
 * Called when a zombie skill duration is over, for reset data.
 * 
 * @param cBasePlayer           The client index.
 *
 **/
void APIOnSkillOver(CBasePlayer* cBasePlayer)
{
	// Start forward call
	Call_StartForward(gForwardOnSkillOver);

	// Push the parameters
	Call_PushCell(view_as<int>(cBasePlayer));
	
	// Finish the call
	Call_Finish();
}