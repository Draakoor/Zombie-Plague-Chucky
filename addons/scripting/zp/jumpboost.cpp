/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          jumpboost.cpp
 *  Type:          Game 
 *  Description:   Modified jump vector magnitudes.
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
 * Client is jumping.
 * 
 * @param clientIndex	The client index.
 **/
void JumpBoostOnClientJump(int clientIndex)
{ 
	// If jump boost disabled
	if(!GetConVarBool(gCvarList[CVAR_JUMPBOOST_ENABLE]))
	{
		return;
	}
	
	// Get real player index from event key
	CBasePlayer* cBasePlayer = view_as<CBasePlayer>(clientIndex);

	// Creates a single use next frame hook
	RequestFrame(view_as<RequestFrameCallback>(JumpBoostOnClientJumpPost), cBasePlayer);
}  

/**
 * Client is jumping. *(Post)
 *
 * @param cBasePlayer	The client index.
 **/
public void JumpBoostOnClientJumpPost(CBasePlayer* cBasePlayer)
{
	// Verify that the client is connected and alive
	if(!IsPlayerExist(view_as<int>(cBasePlayer)))
	{
		return;
	}

	// Initialize velocity vector
	float flVelocity[3];
	
	// Only apply horizontal multiplier if it's not a bhop
	if (ToolsGetVelosity(view_as<int>(cBasePlayer), flVelocity) < GetConVarFloat(gCvarList[CVAR_JUMPBOOST_MAX]))
	{
		// Apply horizontal multipliers to jump vector
		flVelocity[0] *= GetConVarFloat(gCvarList[CVAR_JUMPBOOST_MULTIPLIER]);
		flVelocity[1] *= GetConVarFloat(gCvarList[CVAR_JUMPBOOST_MULTIPLIER]);
	}

	// Apply height multiplier to jump vector
	flVelocity[2] *= GetConVarFloat(gCvarList[CVAR_JUMPBOOST_MULTIPLIER]);

	// Toss client
	cBasePlayer->m_iTeleportPlayer(NULL_VECTOR, NULL_VECTOR, flVelocity);
}