/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          visualeffects.cpp
 *  Type:          Module 
 *  Description:   Visual effects.
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
  * Load other visual effect modules
  */
#include "zp/visualeffects/visualambience.cpp"
#include "zp/visualeffects/ragdoll.cpp"

 
/**
 * @section Explosion flags.
 **/
#define EXP_NODAMAGE               1
#define EXP_REPEATABLE             2
#define EXP_NOFIREBALL             4
#define EXP_NOSMOKE                8
#define EXP_NODECAL               16
#define EXP_NOSPARKS              32
#define EXP_NOSOUND               64
#define EXP_RANDOMORIENTATION    128
#define EXP_NOFIREBALLSMOKE      256
#define EXP_NOPARTICLES          512
#define EXP_NODLIGHTS           1024
#define EXP_NOCLAMPMIN          2048
#define EXP_NOCLAMPMAX          4096
/**
 * @endsection
 **/

/**
 * Number of valid models.
 **/
int gSmoke;
int gBloodDecal;

/**
 * Visual effect loading.
 */
void VEffectsLoad(/*void*/)
{
	// Precache smoke model
	gSmoke = PrecacheModel("sprites/steam1.vmt");
	
	// Precache blood decals
	gBloodDecal = PrecacheDecal("decals/bloodstain_001.vtf");
	
	// Forward event to sub-modules
	VAmbienceLoad();
}

/**
 * Plugin has just finished creating/hooking cvars.
 **/
void VEffectsOnCvarInit(/*void*/)
{
    // Hook zp_veffects_* cvars
    VAmbienceCvarsHook();
}

/**
 * The map is ending.
 **/
void VEffectsOnMapEnd(/*void*/)
{
    // Forward event to sub-modules.
    VAmbienceOnMapEnd();
}
 
/**
 * Create infect effect.
 *
 * @param clientIndex	The client index.
 **/
void VEffectInfectEffect(int clientIndex)
{
	// Initialize vector variables
	float flOrigin[3];
	
	// Get client's position
	GetClientAbsOrigin(clientIndex, flOrigin);
	flOrigin[2] += 30;

	// Create smoke explosion at client's origin
	if(GetConVarBool(gCvarList[CVAR_VEFFECTS_EXPLOSION])) 
	{
		SetupSmokeFunction(flOrigin);
	}
	
	// Create energy splash effect
	if(GetConVarBool(gCvarList[CVAR_VEFFECTS_SPLASH]))
	{
		SetupEnergySplashFunction(flOrigin);
	}
	
	// Shake client's screen
	VEffectsShakeClientScreen(clientIndex);
}

/**
 * Create fire effect.
 *
 * @param clientIndex	The client index.
 **/
void VEffectFireEffect(int clientIndex)
{
	// Initialize vector variable
	float flOrigin[3];
	
	// Get client's position
	GetClientAbsOrigin(clientIndex, flOrigin);
	
	// Create an fire entity
	int iEntity = CreateEntityByName("info_particle_system", -1);
	
	// If fire entity isn't valid, then stop
	if (iEntity)
	{
		// Give name to the entity 
		DispatchKeyValue(iEntity, "effect_name", "env_fire_large");
		
		// Set the origin of the explosion
		DispatchKeyValueVector(iEntity, "origin", flOrigin);
		
		// Spawn the entity into the world
		DispatchSpawn(iEntity);
		
		// Get and modify flags on fired
		SetVariantString("!activator");
		
		// Set parent to entity
		AcceptEntityInput(iEntity, "SetParent", clientIndex);
		
		// Activate the enity
		ActivateEntity(iEntity);
		AcceptEntityInput(iEntity, "Start");
		
		// Set modified flags on entity
		SetVariantString("OnUser1 !self:kill::1.5:1");
		AcceptEntityInput(iEntity, "AddOutput");
		AcceptEntityInput(iEntity, "FireUser1");
	}
}

/**
 * Shake a client's screen with specific parameters.
 * 
 * @param clientIndex	The client index.
 **/
void VEffectsShakeClientScreen(int clientIndex)
{
	// If screen shake disabled
	if(!GetConVarBool(gCvarList[CVAR_VEFFECTS_SHAKE])) 
	{
		return;
	}
	
	// Create message
	Handle iShake = StartMessageOne("Shake", clientIndex);

	// Write shake information to message handle
	PbSetInt(iShake,   "command", 0);
	PbSetFloat(iShake, "local_amplitude", GetConVarFloat(gCvarList[CVAR_INFECT_SHAKE_AMP]));
	PbSetFloat(iShake, "frequency", GetConVarFloat(gCvarList[CVAR_INFECT_SHAKE_FREQUENCY]));
	PbSetFloat(iShake, "duration", GetConVarFloat(gCvarList[CVAR_INFECT_SHAKE_DURATION]));

	// End usermsg and send to client
	EndMessage();
}

/**
 * Create a light dynamic entity.
 * 
 * @param flOrigin			Vector for origin of entity.
 * @param sColorLight		The string will color. (RGBA)
 * @param flDistanceLight	The distance of light.
 * @param flRadiusLight		The radius of light.
 * @param flDurationLight	The duration of light.
 * @param bAttach			If true, entity will be attach to client.
 * @param clientIndex	 	The client index.
 **/
void VEffectLightDynamic(float flOrigin[3] = 0.0, char[] sColorLight, float flDistanceLight, float flRadiusLight, float flDurationLight, bool bAttach = false, int clientIndex = 0)
{
	// Create an light_dynamic entity
	int iLight = CreateEntityByName("light_dynamic", -1);
	
	// If light_dynamic entity isn't valid, then stop
	if(iLight)
	{
		// Set the inner (bright) angle
		DispatchKeyValue(iLight, "inner_cone", "0");
		
		// Set the outer (fading) angle
		DispatchKeyValue(iLight, "cone", "80");
		
		// Set the light brightness
		DispatchKeyValue(iLight, "brightness", "1");
		
		// Used instead of Pitch Yaw Roll's value for reasons unknown
		DispatchKeyValue(iLight, "pitch", "90");
		
		// Change the lightstyle (see Appearance field for possible values)
		DispatchKeyValue(iLight, "style", "1");
		
		// Set the light's render color (R G B)
		DispatchKeyValue(iLight, "_light", sColorLight);
		
		// Set the maximum light distance
		DispatchKeyValueFloat(iLight, "distance", flDistanceLight);
		
		// Set the radius of the spotlight at the end point
		DispatchKeyValueFloat(iLight, "spotlight_radius", flRadiusLight);

		// Spawn the entity
		DispatchSpawn(iLight);

		// Activate the enity
		AcceptEntityInput(iLight, "TurnOn");
		
		// Update vectors of player
		if(bAttach)
		{
			// Get origin vector
			GetClientAbsOrigin(clientIndex, flOrigin);
		}

		// Teleport entity
		TeleportEntity(iLight, flOrigin, NULL_VECTOR, NULL_VECTOR);
		
		// Attach light to player
		if(bAttach)
		{
			// Set parent
			SetVariantString("!activator"); 
			AcceptEntityInput(iLight, "SetParent", clientIndex, iLight, 0); 
			SetEntPropEnt(iLight, Prop_Data, "m_pParent", clientIndex);
		}
		else
		{
			// Emit the sound
			EmitSoundToAll("items/nvg_on.wav", iLight, SNDCHAN_WEAPON);
		}
		
		// Initialize time
		char sTime[SMALL_LINE_LENGTH];
		Format(sTime, sizeof(sTime), "OnUser1 !self:kill::%f:1", flDurationLight);
		
		// Set modified flags on entity
		SetVariantString(sTime);
		AcceptEntityInput(iLight, "AddOutput");
		AcceptEntityInput(iLight, "FireUser1");
	}
}

/**
 * Delete a light dynamic entity from the client.
 * 
 * @param clientIndex	The client index.
 **/
void VEffectRemoveLightDynamic(int clientIndex)
{
	// Initialize char
	char sClassname[NORMAL_LINE_LENGTH];
	
	// Get max amount of entities
	int nGetMaxEnt = GetMaxEntities();
	
	// nEntity = entity index
	for (int nEntity = 0; nEntity <= nGetMaxEnt; nEntity++)
	{
		// If entity isn't valid, then stop
		if(!IsValidEdict(nEntity))
		{
			continue;	
		}
		
		// Get valid edict's classname
		GetEdictClassname(nEntity, sClassname, sizeof(sClassname));
		
		// If entity is light dymanic
		if(StrEqual(sClassname, "light_dynamic"))
		{
			if (GetEntPropEnt(nEntity, Prop_Data, "m_pParent") == clientIndex)
			{
				AcceptEntityInput(nEntity, "Kill");
			}
		}
	}
}

/**
 * Create sparks splash effect.
 *
 * @param flOrigin		Position of the sparks.
 * @param iMagnitude	Sparks size.
 * @param iTrailLength	Trail length of the sparks.
 * @param bMetal		Is the metal sparks?
 **/
void SetupSparksFunction(float flOrigin[3], int iMagnitude = 0, int iTrailLength = 0, bool bMetal = false)
{
	if(bMetal) 
	{
		TE_SetupMetalSparks(flOrigin, NULL_VECTOR);
	}
	else	
	{
		TE_SetupSparks(flOrigin, NULL_VECTOR, iMagnitude, iTrailLength);
	}
	
	TE_SendToAll();
}

/**
 * Create smoke explosion effect.
 *
 * @param flOrigin	Position of the smoke.
 **/
void SetupSmokeFunction(float flOrigin[3])
{
	TE_SetupSmoke(flOrigin, gSmoke, 130.0, 10);
	TE_SendToAll();
}

/**
 * Create dust effect.
 *
 * @param flOrigin	Position of the dust.
 **/
void SetupDustFunction(float flOrigin[3])
{
	TE_SetupDust(flOrigin, NULL_VECTOR, 10.0, 1.0);
	TE_SendToAll();
}

/**
 * Create energy splash effect.
 *
 * @param flOrigin	Position of the splash.
 **/
void SetupEnergySplashFunction(float flOrigin[3])
{
	TE_SetupEnergySplash(flOrigin, NULL_VECTOR, true);
	TE_SendToAll();
}

/**
 * Create blood decal.
 *
 * @param clientIndex	The client index.
 **/
void CreateBloodDecalFunction(int clientIndex)
{
	// Initialize origin vectors
	float flOrigin[3];

	// Get client's position
	GetClientAbsOrigin(clientIndex, flOrigin);
	
	// Get foot position
	flOrigin[1] -= 36.0;
		
	// Spawn 
	TE_Start("World Decal");
	TE_WriteVector("m_vecOrigin", flOrigin);
	TE_WriteNum("m_nIndex", gBloodDecal);
	TE_SendToAll();
}