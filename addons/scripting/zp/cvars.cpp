/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          cvars.cpp
 *  Type:          Main 
 *  Description:   Config creation and cvar control.
 *
 *  Copyright (C) 2015-2016 Nikita Ushakov (Ireland  Dublin)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation  either version 3 of the License  or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not  see <http://www.gnu.org/licenses/>.
 *
 * ============================================================================
 **/
 
/**
 * @section Locked cvar value defines.
 **/
#define CVARS_AUTOTEAMBALANCE_LOCKED 	0
#define CVARS_LIMITTEAMS_LOCKED 		0
#define CVARS_CASHAWARD_LOCKED 			1
#define CVARS_FRIENDLYFIRE_LOCKED 		0
#define CVARS_BUYANYWHERE_LOCKED 		0
/**
 * @endsection
 **/

/**
 * List of cvars used by the plugin.
 **/
enum CvarsList
{
	ConVar:CVAR_ROUND_START,
	ConVar:CVAR_HUD_HEALTH_MESSAGES,
	ConVar:CVAR_BLOCK_BUY_ROUND_START,
	ConVar:CVAR_REMOVE_DROPPED,
	ConVar:CVAR_LVL_SYSTEM,
	ConVar:CVAR_REMOVE_DOORS,
	ConVar:CVAR_AMMOPACK_SAVE,
	ConVar:CVAR_HITGROUPS,
	ConVar:CVAR_CUSTOM_MODELS,
	ConVar:CVAR_CONFIG_PATH_MODELS,
	ConVar:CVAR_CONFIG_PATH_DOWNLOADS,
	ConVar:CVAR_CONFIG_PATH_SOUNDS,
	ConVar:CVAR_CONFIG_PATH_WEAPONS,
	ConVar:CVAR_CONFIG_PATH_HITGROUPS,
	ConVar:CVAR_LOG,
    ConVar:CVAR_LOG_MODULE_FILTER,
    ConVar:CVAR_LOG_IGNORE_CONSOLE,
    ConVar:CVAR_LOG_ERROR_OVERRIDE,
    ConVar:CVAR_LOG_PRINT_CHAT,
	ConVar:CVAR_KNOCKBACK_BOOST,
	ConVar:CVAR_KNOCKBACK_AIR,
	ConVar:CVAR_KNOCKBACK_BOOST_AMOUNT,
	ConVar:CVAR_KNOCKBACK_BOOST_MAX,
	ConVar:CVAR_JUMPBOOST_ENABLE,
	ConVar:CVAR_JUMPBOOST_MULTIPLIER,
	ConVar:CVAR_JUMPBOOST_MAX,
	ConVar:CVAR_MODE_MULTI,
	ConVar:CVAR_MODE_MULTI_CHANCE,
	ConVar:CVAR_MODE_MULTI_MIN,
	ConVar:CVAR_MODE_MULTI_RATIO,
	ConVar:CVAR_MODE_SWARM,
	ConVar:CVAR_MODE_SWARM_CHANCE,
	ConVar:CVAR_MODE_SWARM_MIN,
	ConVar:CVAR_MODE_SWARM_RATIO,
	ConVar:CVAR_MODE_NEMESIS,
	ConVar:CVAR_MODE_NEMESIS_CHANCE,
	ConVar:CVAR_MODE_NEMESIS_MIN,
	ConVar:CVAR_MODE_SURVIVOR,
	ConVar:CVAR_MODE_SURVIVOR_CHANCE,
	ConVar:CVAR_MODE_SURVIVOR_MIN,
	ConVar:CVAR_MODE_ARMAGEDDON,
	ConVar:CVAR_MODE_ARMAGEDDON_CHANCE,
	ConVar:CVAR_MODE_ARMAGEDDON_MIN,
	ConVar:CVAR_HUMAN_ARMOR_PROTECT,
	ConVar:CVAR_HUMAN_DAMAGE_REWARD,
	ConVar:CVAR_HUMAN_LEVEL_RATIO,
	ConVar:CVAR_HUMAN_HEALTH,
	ConVar:CVAR_HUMAN_SPEED,
	ConVar:CVAR_HUMAN_GRAVITY,
	ConVar:CVAR_HUMAN_KNIFE_KNOCKBACK,
	ConVar:CVAR_HUMAN_ARMOR,
	ConVar:CVAR_HUMAN_INF_AMMO,
	ConVar:CVAR_SURVIVOR_SPEED,
	ConVar:CVAR_SURVIVOR_GRAVITY,
	ConVar:CVAR_SURVIVOR_HEALTH,
	ConVar:CVAR_SURVIVOR_DAMAGE,
	ConVar:CVAR_SURVIVOR_DAMAGE_REWARD,
	ConVar:CVAR_SURVIVOR_INF_AMMO,
	ConVar:CVAR_SURVIVOR_GLOW,
	ConVar:CVAR_SURVIVOR_GLOW_COLOR,
	ConVar:CVAR_SURVIVOR_PLAYER_MODEL,
	ConVar:CVAR_SURVIVOR_M249_SHOOT,
	ConVar:CVAR_SURVIVOR_M249_V_MODEL,
	ConVar:CVAR_SURVIVOR_M249_W_MODEL,
	ConVar:CVAR_SURVIVOR_PISTOL_SHOOT,
	ConVar:CVAR_SURVIVOR_PISTOL_V_MODEL,
	ConVar:CVAR_SURVIVOR_PISTOL_W_MODEL,
	ConVar:CVAR_ZOMBIE_FISRT_HEALTH,
	ConVar:CVAR_ZOMBIE_NIGHT_VISION,
	ConVar:CVAR_ZOMBIE_XRAY,
	ConVar:CVAR_ZOMBIE_FOV,
	ConVar:CVAR_ZOMBIE_LEVEL_RATIO,
	ConVar:CVAR_ZOMBIE_BLEEDING,
	ConVar:CVAR_ZOMBIE_RESTORE,
	ConVar:CVAR_ZOMBIE_RESTORE_TIME,
	ConVar:CVAR_ZOMBIE_RESTORE_AMOUNT,
	ConVar:CVAR_ZOMBIE_SILENT,
	ConVar:CVAR_NEMESIS_SPEED,
	ConVar:CVAR_NEMESIS_GRAVITY,
	ConVar:CVAR_NEMESIS_DAMAGE,
	ConVar:CVAR_NEMESIS_HEALTH,
	ConVar:CVAR_NEMESIS_GLOW,
	ConVar:CVAR_NEMESIS_GLOW_COLOR,
	ConVar:CVAR_NEMESIS_PLAYER_MODEL,
	ConVar:CVAR_NEMESIS_KNIFE_MODEL,
	ConVar:CVAR_LEAP_ZOMBIE, 			
	ConVar:CVAR_LEAP_ZOMBIE_FORCE, 			
	ConVar:CVAR_LEAP_ZOMBIE_COUNTDOWN, 		
	ConVar:CVAR_LEAP_NEMESIS, 				
	ConVar:CVAR_LEAP_NEMESIS_FORCE, 			
	ConVar:CVAR_LEAP_NEMESIS_COUNTDOWN, 		
	ConVar:CVAR_LEAP_SURVIVOR, 				
	ConVar:CVAR_LEAP_SURVIVOR_FORCE,		
	ConVar:CVAR_LEAP_SURVIVOR_COUNTDOWN,
	ConVar:CVAR_BONUS_HEALTH,
	ConVar:CVAR_BONUS_INFECT,
	ConVar:CVAR_BONUS_KILL,
	ConVar:CVAR_RESPAWN_DEATHMATCH,
	ConVar:CVAR_RESPAWN_SUICIDE,
	ConVar:CVAR_RESPAWN_AMOUNT,
	ConVar:CVAR_RESPAWN_TIME,
	ConVar:CVAR_RESPAWN_WORLD,
	ConVar:CVAR_RESPAWN_LAST,
	ConVar:CVAR_RESPAWN_INFETION,
	ConVar:CVAR_RESPAWN_ALLOW_NEMESIS,
	ConVar:CVAR_RESPAWN_ALLOW_SURVIVOR,
	ConVar:CVAR_RESPAWN_ALLOW_SWARM,
	ConVar:CVAR_RESPAWN_ZOMBIE,
	ConVar:CVAR_RESPAWN_HUMAN,
	ConVar:CVAR_RESPAWN_NEMESIS,
	ConVar:CVAR_RESPAWN_SURVIVOR,
	ConVar:CVAR_AMMOPACKS_FOR_NEW_PLAYER,
	ConVar:CVAR_AMMOPACKS_ZOMBIE_WIN,
	ConVar:CVAR_AMMOPACKS_ZOMBIE_FAIL,
	ConVar:CVAR_AMMOPACKS_HUMAN_WIN,
	ConVar:CVAR_AMMOPACKS_HUMAN_FAIL,
	ConVar:CVAR_VEFFECTS_EXPLOSION,
	ConVar:CVAR_VEFFECTS_SPLASH,
	ConVar:CVAR_VEFFECTS_SHAKE,
    ConVar:CVAR_INFECT_SHAKE_AMP,
    ConVar:CVAR_INFECT_SHAKE_FREQUENCY,
    ConVar:CVAR_INFECT_SHAKE_DURATION,
	ConVar:CVAR_VEFFECTS_LIGHTSTYLE,
    ConVar:CVAR_VEFFECTS_LIGHTSTYLE_VALUE,
    ConVar:CVAR_VEFFECTS_SKY,
    ConVar:CVAR_VEFFECTS_SKY_PATH,
    ConVar:CVAR_VEFFECTS_SUN_DISABLE,
    ConVar:CVAR_VEFFECTS_FOG,
    ConVar:CVAR_VEFFECTS_FOG_COLOR,
    ConVar:CVAR_VEFFECTS_FOG_DENSITY,
    ConVar:CVAR_VEFFECTS_FOG_STARTDIST,
    ConVar:CVAR_VEFFECTS_FOG_ENDDIST,
    ConVar:CVAR_VEFFECTS_FOG_FARZ,
    ConVar:CVAR_VEFFECTS_RAGDOLL_REMOVE,
    ConVar:CVAR_VEFFECTS_RAGDOLL_DISSOLVE,
    ConVar:CVAR_VEFFECTS_RAGDOLL_DELAY,
	ConVar:CVAR_GRENADE_DAMAGE_HEGRENADE,
	ConVar:CVAR_GRENADE_KNOCKBACK_HEGRENADE,
	ConVar:CVAR_GRENADE_DAMAGE_MOLOTOV,
	ConVar:CVAR_GRENADE_IGNITTING,
	ConVar:CVAR_GRENADE_FREEZING,
	ConVar:CVAR_GRENADE_FREEZING_DAMAGE,
	ConVar:CVAR_GRENADE_FREEZING_RADIUS,
	ConVar:CVAR_GRENADE_LIGHT_RADIUS,
	ConVar:CVAR_GRENADE_LIGHT_DISTANCE,
	ConVar:CVAR_GRENADE_LIGHT_DURATION,
	ConVar:CVAR_GRENADE_LIGHT_COLOR,
	ConVar:CVAR_HUD_ZOMBIE_WIN,
	ConVar:CVAR_HUD_HUMAN_WIN,
	ConVar:CVAR_HUD_LEVELS,
	ConVar:CVAR_URL_MOTD,
	ConVar:CVAR_HELP_MESSAGES
};

/**
 * Array to store cvar data in.
 **/
ConVar gCvarList[CvarsList];

/**
 * @section Global cvar handles.
 **/
ConVar gHolidayMode;
ConVar gRoundTime;
ConVar gRoundTimeCS;
ConVar gRoundTimeDE;
ConVar gTeamBalance;
ConVar gLimitTeams;
ConVar gCashAward;
ConVar gFriendlyFire;
ConVar gRestartGame;
ConVar gBuyAnyWhere;
/**
 * @endsection
 **/

/**
 * Cvars module init function.
 **/
void CvarInit(/*void*/)
{
	// Retrieve handles of CS:GO cvars
	gHolidayMode  = FindConVar("sv_holiday_mode");
	gRoundTime    = FindConVar("mp_roundtime");
	gRoundTimeCS  = FindConVar("mp_roundtime_hostage");
	gRoundTimeDE  = FindConVar("mp_roundtime_defuse");
	gTeamBalance  = FindConVar("mp_autoteambalance"); 
	gLimitTeams   = FindConVar("mp_limitteams");
	gCashAward    = FindConVar("mp_playercashawards");
	gFriendlyFire = FindConVar("mp_friendlyfire");
	gRestartGame  = FindConVar("mp_restartgame");
	gBuyAnyWhere  = FindConVar("mp_buy_anywhere");
	
	// Create zombieplague cvars
	CvarCreate();
	
	// Hook cvars
	CvarHook();
	
	#if defined ADD_VERSION_INFO
        CreateConVar("zombieplague_revision", PLUGIN_VERSION, "Revision number for this plugin in source code repository.", FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_UNLOGGED|FCVAR_DONTRECORD|FCVAR_REPLICATED|FCVAR_NOTIFY);
    #endif
	
	// Forward event to modules
    VEffectsOnCvarInit();
}

/**
 * Create all cvars for plugin.
 **/
void CvarCreate(/*void*/)
{
	
	//*********************************************************************
	//*           AUTOMATIC CREATION OF THE CONFIGURATION FILE            *
	//*           FILE RELATIVE TO CONFIG SOURCEMOD REPOSITORY            *
	//*********************************************************************
	
	
	// =========================== //
    // 		General Purpose		   //
    // =========================== //
	gCvarList[CVAR_ROUND_START]	 		 	    = CreateConVar("zp_delay_time",  					"30");
	gCvarList[CVAR_HUD_HEALTH_MESSAGES]	 	    = CreateConVar("zp_damage_messages",  				"1"); 
	gCvarList[CVAR_BLOCK_BUY_ROUND_START]	    = CreateConVar("zp_block_buy",  					"0"); 
	gCvarList[CVAR_REMOVE_DROPPED]				= CreateConVar("zp_remove_dropped",  				"0.0"); 
	gCvarList[CVAR_LVL_SYSTEM]	 			    = CreateConVar("zp_level_system",  					"1"); 
	gCvarList[CVAR_REMOVE_DOORS]	 		    = CreateConVar("zp_remove_doors",  					"0"); 
	gCvarList[CVAR_AMMOPACK_SAVE]				= CreateConVar("zp_ammopacks_save",  				"1");
	gCvarList[CVAR_HITGROUPS]                   = CreateConVar("zp_hitgroups",   					"1");
	gCvarList[CVAR_CUSTOM_MODELS]				= CreateConVar("zp_custom_models",   				"1");
	
	// =========================== //
    // 			Configs			   //
    // =========================== //
	gCvarList[CVAR_CONFIG_PATH_MODELS]        	= CreateConVar("zp_config_path_models",         	"zombieplague/models.ini");
	gCvarList[CVAR_CONFIG_PATH_DOWNLOADS]     	= CreateConVar("zp_config_path_downloads",      	"zombieplague/downloads.ini");
	gCvarList[CVAR_CONFIG_PATH_SOUNDS]        	= CreateConVar("zp_config_path_sounds",  			"zombieplague/sounds.ini");
	gCvarList[CVAR_CONFIG_PATH_WEAPONS]       	= CreateConVar("zp_config_path_weapons",        	"zombieplague/weapons.ini");
	gCvarList[CVAR_CONFIG_PATH_HITGROUPS]     	= CreateConVar("zp_config_path_hitgroups",      	"zombieplague/hitgroups.ini");

	// =========================== //
    // 			Logs			   //
    // =========================== //
	gCvarList[CVAR_LOG]                       	= CreateConVar("zp_log",                   	 		"1");
	gCvarList[CVAR_LOG_MODULE_FILTER]         	= CreateConVar("zp_log_module_filter",      		"0");
	gCvarList[CVAR_LOG_IGNORE_CONSOLE]        	= CreateConVar("zp_log_ignore_console",     		"0");
	gCvarList[CVAR_LOG_ERROR_OVERRIDE]        	= CreateConVar("zp_log_error_override",     		"1");
	gCvarList[CVAR_LOG_PRINT_CHAT]            	= CreateConVar("zp_log_print_chat",         		"0");

	// =========================== //
    // 			Knockback 		   //
    // =========================== //
	gCvarList[CVAR_KNOCKBACK_BOOST]             = CreateConVar("zp_knockback_boost",  				"1");
	gCvarList[CVAR_KNOCKBACK_AIR]               = CreateConVar("zp_knockback_boost_air",  			"0.5");
	gCvarList[CVAR_KNOCKBACK_BOOST_AMOUNT]      = CreateConVar("zp_knockback_boost_push",  			"251.0");
	gCvarList[CVAR_KNOCKBACK_BOOST_MAX]         = CreateConVar("zp_knockback_boost_max",  			"350.0");

	// =========================== //
	// 		   Jump boost		   //
	// =========================== //
	gCvarList[CVAR_JUMPBOOST_ENABLE] 			= CreateConVar("zp_jumpboost_enable",     			"1");
	gCvarList[CVAR_JUMPBOOST_MULTIPLIER]		= CreateConVar("zp_jumpboost_multiplier",  			"1.1");
	gCvarList[CVAR_JUMPBOOST_MAX]				= CreateConVar("zp_jumpboost_max",   	   			"300.0");

	// =========================== //
    // 			Multi Mode		   //
    // =========================== //
	gCvarList[CVAR_MODE_MULTI] 				    = CreateConVar("zp_multi_mode_enabled",  			"1");
	gCvarList[CVAR_MODE_MULTI_CHANCE] 		    = CreateConVar("zp_multi_mode_chance",  			"20"); 			 
	gCvarList[CVAR_MODE_MULTI_MIN] 			    = CreateConVar("zp_multi_mode_min_players",  		"0"); 	 
	gCvarList[CVAR_MODE_MULTI_RATIO] 		    = CreateConVar("zp_multi_mode_ratio",  				"0.125"); 	 
	
	// =========================== //
    // 		   Swarm Mode		   //
    // =========================== //
	gCvarList[CVAR_MODE_SWARM] 			 	    = CreateConVar("zp_swarm_mode_enabled",  			"1"); 
	gCvarList[CVAR_MODE_SWARM_CHANCE]		    = CreateConVar("zp_swarm_mode_chance",  			"20"); 
	gCvarList[CVAR_MODE_SWARM_MIN]			    = CreateConVar("zp_swarm_mode_min_players",  		"0"); 
	gCvarList[CVAR_MODE_SWARM_RATIO]		    = CreateConVar("zp_swarm_mode_ratio",  				"2.0"); 

	// =========================== //
    //     Armageddon Mode		   //
    // =========================== //
	gCvarList[CVAR_MODE_ARMAGEDDON] 	 	    = CreateConVar("zp_armageddon_mode_enabled",  		"1");
	gCvarList[CVAR_MODE_ARMAGEDDON_CHANCE]      = CreateConVar("zp_armageddon_mode_chance",  		"20"); 
	gCvarList[CVAR_MODE_ARMAGEDDON_MIN]		    = CreateConVar("zp_armageddon_mode_min_players",  	"0"); 
	
	// =========================== //
    // 			Humans			   //
    // =========================== //
	gCvarList[CVAR_HUMAN_ARMOR_PROTECT] 	    = CreateConVar("zp_human_armor_protect",  			"1"); 
	gCvarList[CVAR_HUMAN_DAMAGE_REWARD] 		= CreateConVar("zp_human_damage_reward",  			"500"); 
	gCvarList[CVAR_HUMAN_LEVEL_RATIO]			= CreateConVar("zp_human_level_multiplier",  		"0.3"); 
	gCvarList[CVAR_HUMAN_HEALTH] 			    = CreateConVar("zp_human_health",  					"100"); 
	gCvarList[CVAR_HUMAN_SPEED] 			    = CreateConVar("zp_human_speed",  					"1.0"); 
	gCvarList[CVAR_HUMAN_GRAVITY] 			    = CreateConVar("zp_human_gravity", 					"0.9"); 
	gCvarList[CVAR_HUMAN_KNIFE_KNOCKBACK] 	    = CreateConVar("zp_human_knife_knockback",  		"600.0"); 
	gCvarList[CVAR_HUMAN_ARMOR] 		 	    = CreateConVar("zp_human_armor", 					"100"); 
	gCvarList[CVAR_HUMAN_INF_AMMO] 				= CreateConVar("zp_human_unlimited_ammo", 			"0");
	
	// =========================== //
    // 			Survivor		   //
    // =========================== //
	gCvarList[CVAR_MODE_SURVIVOR]			    = CreateConVar("zp_survivor_mode_enabled",  		"1"); 
	gCvarList[CVAR_MODE_SURVIVOR_CHANCE]        = CreateConVar("zp_survivor_mode_chance",  			"20"); 
	gCvarList[CVAR_MODE_SURVIVOR_MIN]		    = CreateConVar("zp_survivor_mode_min_players",  	"0"); 
	gCvarList[CVAR_SURVIVOR_SPEED] 			    = CreateConVar("zp_survivor_speed",  				"1.3"); 
	gCvarList[CVAR_SURVIVOR_GRAVITY] 		    = CreateConVar("zp_survivor_gravity",  				"0.8"); 
	gCvarList[CVAR_SURVIVOR_HEALTH] 		    = CreateConVar("zp_survivor_health",  				"2000"); 
	gCvarList[CVAR_SURVIVOR_DAMAGE] 		    = CreateConVar("zp_survivor_damage",  				"5.0"); 
	gCvarList[CVAR_SURVIVOR_DAMAGE_REWARD] 	    = CreateConVar("zp_survivor_damage_reward",  		"2000"); 
	gCvarList[CVAR_SURVIVOR_INF_AMMO]			= CreateConVar("zp_survivor_unlimited_ammo",  		"1");
	gCvarList[CVAR_SURVIVOR_GLOW] 		 	    = CreateConVar("zp_survivor_glow",  				"1"); 
	gCvarList[CVAR_SURVIVOR_GLOW_COLOR] 	  	= CreateConVar("zp_survivor_glow_color",  			"0 0 255 255"); 
	gCvarList[CVAR_SURVIVOR_PLAYER_MODEL] 		= CreateConVar("zp_survivor_model",  				"models/player/custom_player/legacy/tm_phoenix_heavy.mdl");
	gCvarList[CVAR_SURVIVOR_M249_SHOOT] 		= CreateConVar("zp_survivor_sound_mach",  			"weapons/m60.mp3");  
	gCvarList[CVAR_SURVIVOR_M249_V_MODEL] 		= CreateConVar("zp_survivor_model_v_mach",  		"models/weapons/v_mach_m60.mdl");  
	gCvarList[CVAR_SURVIVOR_M249_W_MODEL] 		= CreateConVar("zp_survivor_model_w_mach",  		"models/weapons/w_mach_m60para.mdl");  
	gCvarList[CVAR_SURVIVOR_PISTOL_SHOOT] 		= CreateConVar("zp_survivor_sound_pist",  			"weapons/infi.mp3"); 
	gCvarList[CVAR_SURVIVOR_PISTOL_V_MODEL] 	= CreateConVar("zp_survivor_model_v_pist",  		"models/weapons/v_pist_dualinfinity.mdl");  
	gCvarList[CVAR_SURVIVOR_PISTOL_W_MODEL] 	= CreateConVar("zp_survivor_model_w_pist",  		"models/weapons/w_inf_elite.mdl"); 
	
	// =========================== //
    // 			Zombies			   //
    // =========================== //
	gCvarList[CVAR_ZOMBIE_FISRT_HEALTH] 	    = CreateConVar("zp_zombie_additional_health",  		"10000"); 
	gCvarList[CVAR_ZOMBIE_NIGHT_VISION] 	    = CreateConVar("zp_zombie_nvg_give",  				"1"); 
	gCvarList[CVAR_ZOMBIE_XRAY]				    = CreateConVar("zp_zombie_xray_give",  				"1"); 
	gCvarList[CVAR_ZOMBIE_SILENT]			    = CreateConVar("zp_zombie_silent",  				"0"); 
	gCvarList[CVAR_ZOMBIE_FOV]					= CreateConVar("zp_zombie_fov",  					"90");
	gCvarList[CVAR_ZOMBIE_LEVEL_RATIO]			= CreateConVar("zp_zombie_level_ratio",  			"0.03");
	gCvarList[CVAR_ZOMBIE_BLEEDING]				= CreateConVar("zp_zombie_bleeding",  				"1");
	gCvarList[CVAR_ZOMBIE_RESTORE]				= CreateConVar("zp_zombie_restore",  				"0");   
	gCvarList[CVAR_ZOMBIE_RESTORE_TIME]			= CreateConVar("zp_zombie_restore_time",  			"3.0"); 
	gCvarList[CVAR_ZOMBIE_RESTORE_AMOUNT]		= CreateConVar("zp_zombie_restore_amount",  		"200");
	
	// =========================== //
    // 			Nemesis			   //
    // =========================== //
	gCvarList[CVAR_MODE_NEMESIS] 			    = CreateConVar("zp_nemesis_mode_enabled",  			"1"); 
	gCvarList[CVAR_MODE_NEMESIS_CHANCE]		    = CreateConVar("zp_nemesis_mode_chance",  			"20"); 
	gCvarList[CVAR_MODE_NEMESIS_MIN]		    = CreateConVar("zp_nemesis_mode_min_players",  		"0"); 
	gCvarList[CVAR_NEMESIS_SPEED] 			    = CreateConVar("zp_nemesis_speed",  				"1.5"); 
	gCvarList[CVAR_NEMESIS_GRAVITY] 		    = CreateConVar("zp_nemesis_gravity",  				"0.8"); 
	gCvarList[CVAR_NEMESIS_DAMAGE]			    = CreateConVar("zp_nemesis_slash_damage",  			"499.0"); 
	gCvarList[CVAR_NEMESIS_HEALTH] 			    = CreateConVar("zp_nemesis_health_ratio",  			"2000"); 
	gCvarList[CVAR_NEMESIS_GLOW] 			    = CreateConVar("zp_nemesis_glow",  					"1"); 
	gCvarList[CVAR_NEMESIS_GLOW_COLOR] 			= CreateConVar("zp_nemesis_glow_color",  			"255 0 0 255");
	gCvarList[CVAR_NEMESIS_PLAYER_MODEL] 		= CreateConVar("zp_nemesis_model",  				"models/player/kuristaja/walker/walker.mdl"); 
	gCvarList[CVAR_NEMESIS_KNIFE_MODEL] 		= CreateConVar("zp_nemesis_model_knife",  			"models/zombie/normalhost/hand/hand_zombie_normalhost.mdl"); 
	
	// =========================== //
    // 		   Leap jump		   //
    // =========================== //
	gCvarList[CVAR_LEAP_ZOMBIE] 				= CreateConVar("zp_leap_zombies", 					"0");
	gCvarList[CVAR_LEAP_ZOMBIE_FORCE] 			= CreateConVar("zp_leap_zombies_force", 			"500.0");
	gCvarList[CVAR_LEAP_ZOMBIE_COUNTDOWN] 		= CreateConVar("zp_leap_zombies_cooldown", 			"5.0");
	gCvarList[CVAR_LEAP_NEMESIS] 				= CreateConVar("zp_leap_nemesis", 					"1");
	gCvarList[CVAR_LEAP_NEMESIS_FORCE] 			= CreateConVar("zp_leap_nemesis_force", 			"500.0");
	gCvarList[CVAR_LEAP_NEMESIS_COUNTDOWN] 		= CreateConVar("zp_leap_nemesis_cooldown", 			"5.0");
	gCvarList[CVAR_LEAP_SURVIVOR] 				= CreateConVar("zp_leap_survivor", 					"0");
	gCvarList[CVAR_LEAP_SURVIVOR_FORCE] 		= CreateConVar("zp_leap_survivor_force", 			"500.0");
	gCvarList[CVAR_LEAP_SURVIVOR_COUNTDOWN] 	= CreateConVar("zp_leap_survivor_cooldown", 		"5.0");
	
	// =========================== //
    // 			Bonuses			   //
    // =========================== //
	gCvarList[CVAR_BONUS_HEALTH] 			    = CreateConVar("zp_bonus_health",  					"500"); 
	gCvarList[CVAR_BONUS_INFECT] 			    = CreateConVar("zp_bonus_infect",  					"1"); 
	gCvarList[CVAR_BONUS_KILL] 				    = CreateConVar("zp_bonus_kill",						"2"); 

	// =========================== //
    // 		  Deathmatch		   //
    // =========================== //
	gCvarList[CVAR_RESPAWN_DEATHMATCH] 			= CreateConVar("zp_deathmatch", 					"0");
	gCvarList[CVAR_RESPAWN_SUICIDE] 			= CreateConVar("zp_suicide", 						"0");
	gCvarList[CVAR_RESPAWN_AMOUNT] 			    = CreateConVar("zp_respawn_amount",  				"5"); 
	gCvarList[CVAR_RESPAWN_TIME] 			    = CreateConVar("zp_respawn_time",  					"5.0"); 
	gCvarList[CVAR_RESPAWN_WORLD] 				= CreateConVar("zp_respawn_on_suicide", 			"1");
	gCvarList[CVAR_RESPAWN_LAST] 				= CreateConVar("zp_respawn_after_last_human", 		"1");
	gCvarList[CVAR_RESPAWN_INFETION] 			= CreateConVar("zp_infection_allow_respawn", 		"1");
	gCvarList[CVAR_RESPAWN_ALLOW_NEMESIS]  		= CreateConVar("zp_nemesis_allow_respawn", 			"0"); 
	gCvarList[CVAR_RESPAWN_ALLOW_SURVIVOR] 		= CreateConVar("zp_survivor_allow_respawn", 		"0"); 
	gCvarList[CVAR_RESPAWN_ALLOW_SWARM]			= CreateConVar("zp_swarm_allow_respawn", 			"0"); 
	gCvarList[CVAR_RESPAWN_ZOMBIE] 				= CreateConVar("zp_respawn_zombies", 				"1");
	gCvarList[CVAR_RESPAWN_HUMAN] 				= CreateConVar("zp_respawn_humans", 				"0"); 
	gCvarList[CVAR_RESPAWN_NEMESIS]				= CreateConVar("zp_respawn_nemesis", 				"1"); 
	gCvarList[CVAR_RESPAWN_SURVIVOR] 			= CreateConVar("zp_respawn_survivor", 				"1");
	
	// =========================== //
    // 			Ammopacks		   //
    // =========================== //
	gCvarList[CVAR_AMMOPACKS_FOR_NEW_PLAYER]    = CreateConVar("zp_ammopacks_new",  				"30"); 
	gCvarList[CVAR_AMMOPACKS_ZOMBIE_WIN] 	    = CreateConVar("zp_ammopacks_zombie_win",  			"2"); 
	gCvarList[CVAR_AMMOPACKS_ZOMBIE_FAIL] 	    = CreateConVar("zp_ammopacks_zombie_fail",  		"1"); 
	gCvarList[CVAR_AMMOPACKS_HUMAN_WIN] 	    = CreateConVar("zp_ammopacks_human_win",  			"2"); 
	gCvarList[CVAR_AMMOPACKS_HUMAN_FAIL] 	    = CreateConVar("zp_ammopacks_human_fail",  			"1"); 
	
	// =========================== //
    // 			Effects			   //
    // =========================== //
	gCvarList[CVAR_VEFFECTS_EXPLOSION] 		    = CreateConVar("zp_veffects_smoke_explosion",    	"1");
	gCvarList[CVAR_VEFFECTS_SPLASH] 		 	= CreateConVar("zp_veffects_splash",  				"1"); 
	gCvarList[CVAR_VEFFECTS_SHAKE] 			 	= CreateConVar("zp_veffects_shake",  	  			"1"); 
	gCvarList[CVAR_INFECT_SHAKE_AMP]         	= CreateConVar("zp_veffects_shake_amp",             "15.0");
	gCvarList[CVAR_INFECT_SHAKE_FREQUENCY]   	= CreateConVar("zp_veffects_shake_frequency",       "1.0");
	gCvarList[CVAR_INFECT_SHAKE_DURATION]    	= CreateConVar("zp_veffects_shake_duration",        "5.0"); 

	// =========================== //
    // 			  Sky			   //
    // =========================== //
	gCvarList[CVAR_VEFFECTS_LIGHTSTYLE]     	= CreateConVar("zp_veffects_lightstyle",         	"1");
	gCvarList[CVAR_VEFFECTS_LIGHTSTYLE_VALUE]   = CreateConVar("zp_veffects_lightstyle_value",   	"b");
	gCvarList[CVAR_VEFFECTS_SKY]                = CreateConVar("zp_veffects_sky",                	"1");
	gCvarList[CVAR_VEFFECTS_SKY_PATH]           = CreateConVar("zp_veffects_sky_path",           	"jungle");
	gCvarList[CVAR_VEFFECTS_SUN_DISABLE]     	= CreateConVar("zp_veffects_sun_disable",        	"1");
	
	// =========================== //
    // 			 Fog			   //
    // =========================== //
	gCvarList[CVAR_VEFFECTS_FOG]             	= CreateConVar("zp_veffects_fog",                	"1");
	gCvarList[CVAR_VEFFECTS_FOG_COLOR]     	    = CreateConVar("zp_veffects_fog_color",          	"200 200 200");
	gCvarList[CVAR_VEFFECTS_FOG_DENSITY]     	= CreateConVar("zp_veffects_fog_density",        	"0.2");
	gCvarList[CVAR_VEFFECTS_FOG_STARTDIST]      = CreateConVar("zp_veffects_fog_startdist",     	"300");
	gCvarList[CVAR_VEFFECTS_FOG_ENDDIST]        = CreateConVar("zp_veffects_fog_enddist",        	"1200");
	gCvarList[CVAR_VEFFECTS_FOG_FARZ]           = CreateConVar("zp_veffects_fog_farz",           	"4000");
	
	// =========================== //
    // 			Ragdoll			   //
    // =========================== //
	gCvarList[CVAR_VEFFECTS_RAGDOLL_REMOVE]     = CreateConVar("zp_veffects_ragdoll_remove",     	"1");
	gCvarList[CVAR_VEFFECTS_RAGDOLL_DISSOLVE]   = CreateConVar("zp_veffects_ragdoll_dissolve",   	"-1");
	gCvarList[CVAR_VEFFECTS_RAGDOLL_DELAY]      = CreateConVar("zp_veffects_ragdoll_delay",      	"0.5");
	
	// =========================== //
    // 			Grenades		   //
    // =========================== //
	gCvarList[CVAR_GRENADE_DAMAGE_HEGRENADE] 	= CreateConVar("zp_grenade_explosion_damage",  		"6.0"); 
	gCvarList[CVAR_GRENADE_KNOCKBACK_HEGRENADE] = CreateConVar("zp_grenade_explosion_knockback",  	"500.0"); 
	gCvarList[CVAR_GRENADE_DAMAGE_MOLOTOV]  	= CreateConVar("zp_grenade_igniting_damage",  		"7.0");
	gCvarList[CVAR_GRENADE_IGNITTING] 		    = CreateConVar("zp_grenade_igniting_time",  		"5.0");
	gCvarList[CVAR_GRENADE_FREEZING]   	 	    = CreateConVar("zp_grenade_freeze_time",  			"4.0");
	gCvarList[CVAR_GRENADE_FREEZING_DAMAGE]		= CreateConVar("zp_grenade_freeze_damage_on",  		"0"); 			
	gCvarList[CVAR_GRENADE_FREEZING_RADIUS]     = CreateConVar("zp_grenade_freeze_radius",  		"200.0"); 		
	gCvarList[CVAR_GRENADE_LIGHT_RADIUS]      	= CreateConVar("zp_grenade_light_radius",  			"150.0"); 
	gCvarList[CVAR_GRENADE_LIGHT_DISTANCE]      = CreateConVar("zp_grenade_light_distance",  		"2000.0"); 
	gCvarList[CVAR_GRENADE_LIGHT_DURATION]      = CreateConVar("zp_grenade_light_duration",  		"60.0");  		
	gCvarList[CVAR_GRENADE_LIGHT_COLOR]        	= CreateConVar("zp_grenade_light_color",  			"255 255 255 255");
	
	// =========================== //
    // 			Resources		   //
    // =========================== //
	gCvarList[CVAR_HUD_ZOMBIE_WIN]              = CreateConVar("zp_overlay_zombie_win",     		"overlays/zp/zg_zombies_win"); 
	gCvarList[CVAR_HUD_HUMAN_WIN]               = CreateConVar("zp_overlay_human_win",     			"overlays/zp/zg_humans_win");
	gCvarList[CVAR_HUD_LEVELS]               	= CreateConVar("zp_overlay_levels",     			"overlays/zp/lvl/lvl_@_hud");
	
	// =========================== //
    // 			Help			   //
    // =========================== //
	gCvarList[CVAR_URL_MOTD]	 			    = CreateConVar("zp_help_motd_url", 					"http://yourdomain.com/help.html"); 
	gCvarList[CVAR_HELP_MESSAGES]				= CreateConVar("zp_help_messages",   				"1");	
	
	// =========================== //
    //   Create configure file	   //
    // =========================== //
	AutoExecConfig(true, "zombieplague");
}

/**
 * Hook cvar changes.
 **/
void CvarHook(/*void*/)
{
	// Set locked cvars to their locked value
	SetConVarInt(gTeamBalance, 	   CVARS_AUTOTEAMBALANCE_LOCKED);
	SetConVarInt(gLimitTeams, 		   CVARS_LIMITTEAMS_LOCKED);
	SetConVarInt(gCashAward, 		   CVARS_CASHAWARD_LOCKED);
	SetConVarInt(gFriendlyFire, 	   CVARS_FRIENDLYFIRE_LOCKED);
	SetConVarInt(gBuyAnyWhere, 	   	   CVARS_BUYANYWHERE_LOCKED);

	// Hook cvar to prevent it from changing
	HookConVarChange(gTeamBalance, CvarsHookLocked);
	HookConVarChange(gLimitTeams, 	   CvarsHookLocked);
	HookConVarChange(gCashAward, 	   CvarsHookLocked);
	HookConVarChange(gFriendlyFire,    CvarsHookLocked);
	HookConVarChange(gRoundTimeCS,     CvarsHookLocked);
	HookConVarChange(gRoundTimeDE,     CvarsHookLocked);
	HookConVarChange(gHolidayMode,     CvarsHookLocked);
	HookConVarChange(gBuyAnyWhere,     CvarsHookLocked);
	HookConVarChange(gRestartGame, 	   CvarsHookRestartGame);
}

/**
 * Cvar hook callback
 * Prevents changes of the some cvars.
 * 
 * @param iConVar	The cvar handle.
 * @param oldValue	The value before the attempted change.
 * @param newValue	The new value.
 **/
public void CvarsHookLocked(ConVar iConVar, const char[] oldValue, const char[] newValue)
{
    // If cvar is mp_autoteambalance, then continue
    if (iConVar == gTeamBalance)
    {
        // Revert to locked value
        SetConVarInt(gTeamBalance, CVARS_AUTOTEAMBALANCE_LOCKED);
    }
    // If cvar is mp_limitteams, then continue
    else if (iConVar == gLimitTeams)
    {
        // Revert to locked value
        SetConVarInt(gLimitTeams, CVARS_LIMITTEAMS_LOCKED);
    }
	// If cvar is mp_teamcashawards, then continue
    else if (iConVar == gCashAward)
    {
        // Revert to locked value
        SetConVarInt(gCashAward, CVARS_CASHAWARD_LOCKED);
    }
	// If cvar is mp_friendlyfire, then continue
    else if (iConVar == gFriendlyFire)
    {
        // Revert to locked value
        SetConVarInt(gFriendlyFire, CVARS_FRIENDLYFIRE_LOCKED);
    }
	// If cvar is mp_roundtime_hostage, then continue
    else if (iConVar == gRoundTimeCS)
    {
        // Revert to locked value
        SetConVarFloat(gRoundTimeCS, GetConVarFloat(gRoundTime));
    }
	// If cvar is mp_roundtime_defuse, then continue
    else if (iConVar == gRoundTimeDE)
    {
        // Revert to locked value
        SetConVarFloat(gRoundTimeDE, GetConVarFloat(gRoundTime));
    }
	// If cvar is sv_holiday_mode, then continue
    else if (iConVar == gHolidayMode)
    {
        // Revert to locked value
        SetConVarInt(gHolidayMode, GetCurrentHoliday());
    }
	// If cvar is mp_limitteams, then continue
    else if (iConVar == gBuyAnyWhere)
    {
        // Revert to locked value
        SetConVarInt(gBuyAnyWhere, CVARS_BUYANYWHERE_LOCKED);
    }
}

/**
 * Cvar hook callback (mp_restartgame)
 * Stops restart and just ends the round.
 * 
 * @param iConVar	The cvar handle.
 * @param oldValue	The value before the attempted change.
 * @param newValue	The new value.
 **/
public void CvarsHookRestartGame(ConVar iConVar, const char[] oldValue, const char[] newValue)
{
	// Prevent round restart
	SetConVarInt(iConVar, 0);

	// If value was invalid or 0, then stop
	float flDelay = StringToFloat(newValue);
	if (flDelay <= 0.0)
	{
		return;
	}
	
	// Reset number of round
	gRoundNum = 0;

	// Terminate the round with restart time as delay
	CS_TerminateRound(flDelay, CSRoundEnd_GameStart, false);
}