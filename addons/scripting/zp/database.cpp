/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          ammopacks.cpp
 *  Type:          Core 
 *  Description:   SQL dataBase for ammopacks.
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
 * The maximum length of a SteamID
 **/
#define STEAMID_MAX_LENGTH 32
 
/**
 * Name of database section in the database.cfg
 **/
#define AMMOPACK_BLOCK_DATABASE "ammopacks-storage"

/**
 * Create handle for database.
 **/
Handle hDataBase = NULL;

/**
 * Create array for storing SteamID.
 **/
char SteamID[MAXPLAYERS+1][STEAMID_MAX_LENGTH];

/**
 * Create array for storing ID in the SQL base.
 **/
int DataID[MAXPLAYERS+1];

/**
 * Load a SQL database.
 **/
void SQLBaseLoad(/*void*/)
{
	// Close previous handle
	if (hDataBase != NULL)
	{
		CloseHandle(hDataBase);
	}
	
	// Returns if a named configuration is present in databases.cfg.
	if(SQL_CheckConfig(AMMOPACK_BLOCK_DATABASE))
	{
		// Initialize some chars
		char sError[BIG_LINE_LENGTH];
		char sDriver[SMALL_LINE_LENGTH]; 
		char sRequest[PLATFORM_MAX_PATH];
		
		// Creates an SQL connection from a named configuration
		hDataBase = SQL_Connect(AMMOPACK_BLOCK_DATABASE, false, sError, sizeof(sError));

		// If base doesn't exist or mod can't make connection
		if(hDataBase == NULL)
		{
			LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_DataBase, "DataBase", sError);
			return;
		}
		
		// Reads the driver of an opened database
		SQL_ReadDriver(hDataBase, sDriver, sizeof(sDriver)); 
		
		// If driver is a MySQL
		bool MySQLDataBase = StrEqual(sDriver, "mysql", false);
		
		// Block mysql base from other requests
		SQL_LockDatabase(hDataBase);
		
		// Delete existing database if saving disabling
		if(!GetConVarBool(gCvarList[CVAR_AMMOPACK_SAVE]))
		{
			// Format request
			Format(sRequest, sizeof(sRequest), "DROP TABLE IF EXISTS zp_ammopacks;");
			
			// Sent a request
			if(!SQL_FastQuery(hDataBase, sRequest))
			{
				// Get an error, if it exist
				SQL_GetError(hDataBase, sError, sizeof(sError));
				
				// Log error and stop server
				LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_DataBase, "DataBase", "%s in request: \"%s\"", sError, sRequest);
				return;
			}
		}
		
		// Format request
		if(MySQLDataBase)
		{
			Format(sRequest, sizeof(sRequest), "CREATE TABLE IF NOT EXISTS `zp_ammopacks` (`id` int(11) NOT NULL auto_increment, `steam` varchar(31) NOT NULL, `ammopacks` int(11) NOT NULL, PRIMARY KEY  (`id`), UNIQUE KEY `steam` (`steam`))");	
		}
		else
		{
			Format(sRequest, sizeof(sRequest), "CREATE TABLE IF NOT EXISTS zp_ammopacks(id INTEGER PRIMARY KEY AUTOINCREMENT, steam TEXT UNIQUE, ammopacks INTEGER );");
		}
		
		// Sent a request
		if(!SQL_FastQuery(hDataBase, sRequest))
		{
			// Get an error, if it exist
			SQL_GetError(hDataBase, sError, sizeof(sError));
			
			// Log error and stop server
			LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_DataBase, "DataBase", "%s in request: \"%s\"", sError, sRequest);
			return;
		}
		
		// If database loaded
		LogEvent(true, LogType_Normal, LOG_CORE_EVENTS, LogModule_DataBase, "DataBase",  "%s base was successfully load!", MySQLDataBase ? "MySQL" : "SQlite");
		
		// Unlock it
		SQL_UnlockDatabase(hDataBase);
	}
	else
	{
		LogEvent(false, LogType_Fatal, LOG_CORE_EVENTS, LogModule_DataBase, "DataBase", "Section \"%s\" doesn't found in databases.cfg", AMMOPACK_BLOCK_DATABASE);
	}
}

/**
 * SQL functions, which work with the client.
 **/

/**
 * Called when player is connected, extract amount of ammopacks from the SQL base.
 * 
 * @param clientIndex	The client index.
 **/
void SQLBaseExtract(int clientIndex)
{
	// Check if client is a bot
	if (IsFakeClient(clientIndex))
	{
		return;
	}
	
	// Store steam auth to buffer, for next usage
	GetClientAuthId(clientIndex, AuthId_Steam2, SteamID[clientIndex], sizeof(SteamID[]));	

	// Initialize a new player ID in the SQL base
	DataID[clientIndex] = -1;

	// Initialize char
	char sRequest[PLATFORM_MAX_PATH];

	// Format request
	Format(sRequest, sizeof(sRequest), "SELECT id, ammopacks FROM zp_ammopacks WHERE steam = '%s'", SteamID[clientIndex]);

	// Sent a request
	SQL_TQuery(hDataBase, SQLBaseExtract_Callback, sRequest, GetClientUserId(clientIndex));	
}

/**
 * General callback for threaded SQL stuff
 *
 * @param hDriver	Parent object of the handle.
 * @param hResult	Handle to the child object.
 * @param sSQLerror	Error string if there was an error.
 * @param clientID	Data passed in via the original threaded invocation.
 **/
public void SQLBaseExtract_Callback(Handle hDriver, Handle hResult, const char[] sSQLerror, any clientID)
{
	// Get real player index from event key
	int clientIndex = GetClientOfUserId(clientID);

	// Make sure the client didn't disconnect while the thread was running
	if (clientIndex)
	{
		// If invalid query handle
		if (hResult == NULL)
		{
			LogEvent(false, LogType_Error, LOG_CORE_EVENTS, LogModule_DataBase, "DataBase", sSQLerror);
			return;
		}

		// Client not found, add him into the table
		if (!SQL_GetRowCount(hResult)) 
		{
			// Initialize char
			char sRequest[PLATFORM_MAX_PATH];

			// Format request
			Format(sRequest, sizeof(sRequest), "INSERT INTO zp_ammopacks(steam) VALUES('%s')", SteamID[clientIndex]);
			
			// Sent a request
			if(!SQL_FastQuery(hDataBase, sRequest))
			{
				// Initialize char
				char sError[BIG_LINE_LENGTH];
			
				// Get an error, if it exist
				SQL_GetError(hDataBase, sError, sizeof(sError));
				
				// Log error
				LogEvent(false, LogType_Error, LOG_CORE_EVENTS, LogModule_DataBase, "DataBase", "%s in request: \"%s\"", sError, sRequest);
			}
			
			// Set starter client ammopacks
			SQLSetAmmoPacks(clientIndex, GetConVarInt(gCvarList[CVAR_AMMOPACKS_FOR_NEW_PLAYER]));
			return;
		}

		// Client was found in the table
		if(SQL_FetchRow(hResult))
		{
			// Get client ID in SQL base, if it exist
			DataID[clientIndex] = SQL_FetchInt(hResult, 0); 
			
			// Set client loaded amount of ammopacks
			SQLSetAmmoPacks(clientIndex, SQL_FetchInt(hResult, 1)); 
		}
	}
}

/**
 * Called when player is about to disconnect, insert amount of ammopacks in the SQL base.
 *
 * @param clientIndex	The client index.
 **/
void SQLBaseInsert(int clientIndex)
{
	// Check if client is a bot
	if (IsFakeClient(clientIndex))
	{
		return;
	}

	// Initialize char
	char sRequest[PLATFORM_MAX_PATH];

	// Format request
	if(DataID[clientIndex] == -1)
	{
		Format(sRequest, sizeof(sRequest), "UPDATE zp_ammopacks SET ammopacks = %i WHERE steam = '%s'", SQLGetAmmoPacks(clientIndex), SteamID[clientIndex]);
	}
	else
	{
		Format(sRequest, sizeof(sRequest), "UPDATE zp_ammopacks SET ammopacks = %i WHERE id = %i", SQLGetAmmoPacks(clientIndex), DataID[clientIndex]); 
	}

	// Sent a request
	if(!SQL_FastQuery(hDataBase, sRequest))
	{
		// Initialize char
		char sError[BIG_LINE_LENGTH];

		// Get an error, if it exist
		SQL_GetError(hDataBase, sError, sizeof(sError));

		// Log error
		LogEvent(false, LogType_Error, LOG_CORE_EVENTS, LogModule_DataBase, "DataBase", "%s in request: \"%s\"", sError, sRequest);
	}
}

/**
 * SQL data reading API.
 **/

/**
 * Called when request get data from the SQL base.
 *
 * @param clientIndex	The client index.
 * @param ammoPacks		The amount of ammopacks.
 **/
void SQLSetAmmoPacks(int clientIndex, int ammoPacks)
{
	// Get real player index from event key
	CBasePlayer* cBasePlayer  = view_as<CBasePlayer>(clientIndex);
	
	// Set amount of ammopacks
	cBasePlayer->m_nAmmoPacks = ammoPacks;
}

/**
 * Called when request set data to the SQL base.
 *
 * @param clientIndex	The client index.
 **/
int SQLGetAmmoPacks(int clientIndex)
{
	// Get real player index from event key
	CBasePlayer* cBasePlayer  = view_as<CBasePlayer>(clientIndex);
	
	// Get amount of ammopacks
	return cBasePlayer->m_nAmmoPacks;
}