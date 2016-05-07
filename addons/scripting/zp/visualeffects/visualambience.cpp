/*
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          visualambience.cpp
 *  Type:          Module
 *  Description:   Fog, light style, sky, sun rendering, etc
 *
 *  Copyright (C) 2015-2016  Greyscale, Richard Helgeby
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
 */

/**
 * Default sky of current map.
 */
char gVAmbienceDefaultSky[PLATFORM_MAX_PATH];

/**
 * The map is ending.
 **/
void VAmbienceOnMapEnd(/*void*/)
{
    // Reset the default sky cache to empty, to be re-cached for the next map
    gVAmbienceDefaultSky[0] = 0;
}

/**
 * Hook zp_veffects_* cvar changes.
 * 
 */
void VAmbienceCvarsHook(/*void*/)
{
    // Hook lightstyle cvars
    HookConVarChange(gCvarList[CVAR_VEFFECTS_LIGHTSTYLE], VAmbienceCvarsHookLightStyle);
    HookConVarChange(gCvarList[CVAR_VEFFECTS_LIGHTSTYLE_VALUE], VAmbienceCvarsHookLightStyle);
    
    // Hook sky cvars
    HookConVarChange(gCvarList[CVAR_VEFFECTS_SKY], VAmbienceCvarsHookSky);
    HookConVarChange(gCvarList[CVAR_VEFFECTS_SKY_PATH], VAmbienceCvarsHookSky);
    
    // Hook sun cvars
    HookConVarChange(gCvarList[CVAR_VEFFECTS_SUN_DISABLE], VAmbienceCvarsHookSunDisable);
    
    // Hook fog cvars
    HookConVarChange(gCvarList[CVAR_VEFFECTS_FOG], VAmbienceCvarsHookFog);
    HookConVarChange(gCvarList[CVAR_VEFFECTS_FOG_COLOR], VAmbienceCvarsHookFog);
    HookConVarChange(gCvarList[CVAR_VEFFECTS_FOG_DENSITY], VAmbienceCvarsHookFog);
    HookConVarChange(gCvarList[CVAR_VEFFECTS_FOG_STARTDIST], VAmbienceCvarsHookFog);
    HookConVarChange(gCvarList[CVAR_VEFFECTS_FOG_ENDDIST], VAmbienceCvarsHookFog);
    HookConVarChange(gCvarList[CVAR_VEFFECTS_FOG_FARZ], VAmbienceCvarsHookFog);
}

/**
 * Cvar hook callback (zp_veffects_lightstyle, zp_veffects_lightstyle_value)
 * Updated server to cvar values.
 * 
 * @param convar    The cvar handle.
 * @param oldvalue  The value before change.
 * @param newvalue  The new value.
 **/
public void VAmbienceCvarsHookLightStyle(ConVar iConVar, const char[] oldValue, const char[] newValue)
{
    // If lightstyle is disabled, then disable
    bool bLightStyle = GetConVarBool(gCvarList[CVAR_VEFFECTS_LIGHTSTYLE]);
    
    // Apply light style.
    VAmbienceApplyLightStyle(!bLightStyle);
}

/**
 * Cvar hook callback (zp_veffects_sky, zp_veffects_sky_path)
 * Updated server to cvar values.
 * 
 * @param convar    The cvar handle.
 * @param oldvalue  The value before change.
 * @param newvalue  The new value.
 **/
public void VAmbienceCvarsHookSky(ConVar iConVar, const char[] oldValue, const char[] newValue)
{
    // If sky is disabled, then disable
    bool bSky = GetConVarBool(gCvarList[CVAR_VEFFECTS_SKY]);
    
    // Apply new sky
    VAmbienceApplySky(!bSky);
}

/**
 * Cvar hook callback (zp_veffects_sun_disable)
 * Updated server to cvar values.
 * 
 * @param convar    The cvar handle.
 * @param oldvalue  The value before change.
 * @param newvalue  The new value.
 **/
public void VAmbienceCvarsHookSunDisable(ConVar iConVar, const char[] oldValue, const char[] newValue)
{
    // If fog is disabled, then disable
    bool bSun = GetConVarBool(gCvarList[CVAR_VEFFECTS_SUN_DISABLE]);
    
    // Apply sun
    VAmbienceApplySunDisable(!bSun);
}

/**
 * Cvar hook callback (zp_veffects_fog_*)
 * Updated server to cvar values.
 * 
 * @param convar    The cvar handle.
 * @param oldvalue  The value before change.
 * @param newvalue  The new value.
 **/
public void VAmbienceCvarsHookFog(ConVar iConVar, const char[] oldValue, const char[] newValue)
{
    // Apply fog
    VAmbienceApplyFog();
}

/**
 * Apply all cvar values on server.
 **/
void VAmbienceLoad(/*void*/)
{
    // If lightstyle is disabled, then disable
    bool bLightstyle = GetConVarBool(gCvarList[CVAR_VEFFECTS_LIGHTSTYLE]);
    
    // Apply light style
    VAmbienceApplyLightStyle(!bLightstyle);
    
    // If sky is disabled, then disable
    bool bSky = GetConVarBool(gCvarList[CVAR_VEFFECTS_SKY]);
    
    // Apply new sky
    VAmbienceApplySky(!bSky);
    
    // Apply fog
    VAmbienceApplyFog();
}

/**
 * Apply light style on server.
 **/
void VAmbienceApplyLightStyle(bool bDisable = false)
{
	// If default, then set to normal light style
	if (bDisable)
	{
		// Set light style
		SetLightStyle(0, "n");

		return;
	}

	// Get light value
	char sLightStyleValue[4];
	GetConVarString(gCvarList[CVAR_VEFFECTS_LIGHTSTYLE_VALUE], sLightStyleValue, sizeof(sLightStyleValue));

	// If light value contants 'a', render of textures will be remove
	if(StrContains(sLightStyleValue, "a") != -1)
	{
		// Set darkest light style
		SetLightStyle(0, "b");

		return;
	}

	// Set light style
	SetLightStyle(0, sLightStyleValue);
}

/**
 * Apply sky on server.
 **/
void VAmbienceApplySky(bool bDisable = false)
{
    // If we can't find the sv_skyname cvar, then stop
    ConVar hSkyname = FindConVar("sv_skyname");
    if (hSkyname == NULL)
    {
        return;
    }
    
    // Store map's default sky before applying new one
    if (!gVAmbienceDefaultSky[0])
    {
        GetConVarString(hSkyname, gVAmbienceDefaultSky, sizeof(gVAmbienceDefaultSky));
    }
    
    // If default, then set to default sky
    if (bDisable)
    {
        if (gVAmbienceDefaultSky[0])
        {
            // Set default sky on all clients
            SetConVarString(hSkyname, gVAmbienceDefaultSky, true);
        }
        
        return;
    }
    
    // Get sky path
    char sSkyPath[PLATFORM_MAX_PATH];
    GetConVarString(gCvarList[CVAR_VEFFECTS_SKY_PATH], sSkyPath, sizeof(sSkyPath));
    
    // Set new sky on all clients
    SetConVarString(hSkyname, sSkyPath, true);
}

/**
 * Apply sun on server.
 **/
void VAmbienceApplySunDisable(bool bDisable = false)
{
    // Find sun entity
    int iSun = FindEntityByClassname(-1, "env_sun");
    
    // If sun is invalid, then stop
    if (iSun == -1)
    {
        return;
    }
    
    // If default, then re-enable sun rendering
    if (bDisable)
    {
        // Turn on sun rendering
        AcceptEntityInput(iSun, "TurnOn");
        
        return;
    }
    
    // Turn off sun rendering
    AcceptEntityInput(iSun, "TurnOff");
}

/**
 * Apply fog on server.
 **/
void VAmbienceApplyFog(/*void*/)
{
	// If fog is disabled, then stop.
	bool bFog = GetConVarBool(gCvarList[CVAR_VEFFECTS_FOG]);

	if (!bFog)
	{
		return;
	}

	// Searching fog controlling entity
	int iFogControllerIndex = FindEntityByClassname(-1, "env_fog_controller");

	if(iFogControllerIndex == -1)
	{
		iFogControllerIndex = CreateEntityByName("env_fog_controller");
		DispatchSpawn(iFogControllerIndex);
		return;
	}

	// Set density of the fog
	DispatchKeyValueFloat(iFogControllerIndex, "fogmaxdensity", GetConVarFloat(gCvarList[CVAR_VEFFECTS_FOG_DENSITY]));

	// Set start distance of the fog
	SetVariantInt(GetConVarInt(gCvarList[CVAR_VEFFECTS_FOG_STARTDIST]));
	AcceptEntityInput(iFogControllerIndex, "SetStartDist");

	// Set end distance of the fog
	SetVariantInt(GetConVarInt(gCvarList[CVAR_VEFFECTS_FOG_ENDDIST]));
	AcceptEntityInput(iFogControllerIndex, "SetEndDist");

	// Set plain distance of the fog
	SetVariantInt(GetConVarInt(gCvarList[CVAR_VEFFECTS_FOG_FARZ]));
	AcceptEntityInput(iFogControllerIndex, "SetFarZ");

	// Get color
	char sFogColor[16];
	GetConVarString(gCvarList[CVAR_VEFFECTS_FOG_COLOR], sFogColor, sizeof(sFogColor));
	
	// Set main color
	SetVariantString(sFogColor);
	AcceptEntityInput(iFogControllerIndex, "SetColor");

	// Set secondary color
	SetVariantString(sFogColor);
	AcceptEntityInput(iFogControllerIndex, "SetColorSecondary");
}
