/**
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:          translation.cpp
 *  Type:          Core 
 *  Description:   Translation parsing functions.
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
 * @section Max length of different message formats.
 **/
#define TRANSLATION_MAX_LENGTH_CHAT 192
#define TRANSLATION_MAX_LENGTH_CONSOLE 1024
/**
 * @endsection
 **/

/**
 * Prefix on all messages printed from the plugin.
 **/
#define TRANSLATION_PHRASE_PREFIX 		"[ZP]"

/**
 * @section Text color chars.
 **/
#define TRANSLATION_TEXT_COLOR_DEFAULT 	"\x01"
#define TRANSLATION_TEXT_COLOR_RED 		"\x02"
#define TRANSLATION_TEXT_COLOR_LGREEN 	"\x03"
#define TRANSLATION_TEXT_COLOR_GREEN 	"\x04"
/**
 * @endsection
 **/

/**
 * Load translations file here.
 **/
void TranslationInit()
{
	// Load translations phrases used by plugin
	LoadTranslations("zombieplaguemod.pharses");
	LoadTranslations("zombieplagueitems.pharses");
	LoadTranslations("zombieplagueclasses.pharses");
}

/**
 * Format the string to the plugin's style.
 * 
 * @param sText      Text to format.
 * @param iMaxlen    Maximum length of the formatted text.
 **/
stock void TranslationPluginFormatString(char[] sText, int iMaxlen, bool bColor = true)
{
	if (bColor)
	{
		// Format prefix onto the string
		Format(sText, iMaxlen, " @green%s @default%s", TRANSLATION_PHRASE_PREFIX, sText);

		// Replace color tokens with CS:GO color chars
		ReplaceString(sText, iMaxlen, "@default", TRANSLATION_TEXT_COLOR_DEFAULT);
		ReplaceString(sText, iMaxlen, "@red", TRANSLATION_TEXT_COLOR_RED);
		ReplaceString(sText, iMaxlen, "@lgreen", TRANSLATION_TEXT_COLOR_LGREEN);
		ReplaceString(sText, iMaxlen, "@green", TRANSLATION_TEXT_COLOR_GREEN);
	}
	else
	{
		// Format prefix onto the string
		Format(sText, iMaxlen, "%s %s", TRANSLATION_PHRASE_PREFIX, sText);
	}
}

/**
 * Print center text to all clients.
 *
 * @param ...      Formatting parameters.
 **/
stock void TranslationPrintHintTextAll(any ...)
{
	// i = client index
	for (int i = 1; i <= MaxClients; i++)
	{
		// Verify that the client is connected and alive
		if(!IsPlayerExist(i, false))
			continue;

		// Set translation target
		SetGlobalTransTarget(i);
		
		 // Translate phrase
		char sTranslation[TRANSLATION_MAX_LENGTH_CHAT];
		VFormat(sTranslation, TRANSLATION_MAX_LENGTH_CHAT, "%t", 1);
		
		// Print translated phrase to server or client's chat/console
		PrintHintText(i, sTranslation);
	}
}

/**
 * Print HUD text to client.
 * 
 * @param clientIndex    The client index.
 * @param ...            Formatting parameters.
 **/
stock void TranslationPrintHintText(int clientIndex, any ...)
{
	// Set translation target
	SetGlobalTransTarget(clientIndex);

	// Translate phrase
	char sTranslation[TRANSLATION_MAX_LENGTH_CHAT];
	VFormat(sTranslation, TRANSLATION_MAX_LENGTH_CHAT, "%t", 2);

	// Print translated phrase to server or client's chat/console
	PrintHintText(clientIndex, sTranslation);
}

/**
 * Print chat text to client.
 * 
 * @param clientIndex    The client index.
 * @param ...            Formatting parameters. 
 **/
stock void TranslationPrintToChat(int clientIndex, any ...)
{
   // Translate phrase
	char sTranslation[TRANSLATION_MAX_LENGTH_CHAT];
	VFormat(sTranslation, TRANSLATION_MAX_LENGTH_CHAT, "%t", 2);
	
	// Format string to create plugin style
	TranslationPluginFormatString(sTranslation, TRANSLATION_MAX_LENGTH_CHAT);
	
	// Print translated phrase to server or client's chat/console
	PrintToChat(clientIndex, sTranslation);
}

/**
 * Print center text to all clients.
 *
 * @param ...      Formatting parameters.
 **/
stock void TranslationPrintToChatAll(any ...)
{
	// i = client index
	for (int i = 1; i <= MaxClients; i++)
	{
		// Verify that the client is connected and alive
		if(!IsPlayerExist(i, false))
			continue;

		// Set translation target
		SetGlobalTransTarget(i);
		
		 // Translate phrase
		char sTranslation[TRANSLATION_MAX_LENGTH_CHAT];
		VFormat(sTranslation, TRANSLATION_MAX_LENGTH_CHAT, "%t", 1);
		
		// Format string to create plugin style
		TranslationPluginFormatString(sTranslation, TRANSLATION_MAX_LENGTH_CHAT);
		
		// Print translated phrase to server or client's chat/console
		PrintToChat(i, sTranslation);
	}
}

/**
 * Print into console for client.
 * 
 * @param clientIndex    The client index.
 * @param ...            Formatting parameters. 
 **/
stock void TranslationReplyToCommand(int clientIndex, any ...)
{
	// Translate phrase
	char sTranslation[TRANSLATION_MAX_LENGTH_CONSOLE];
	VFormat(sTranslation, TRANSLATION_MAX_LENGTH_CONSOLE, "%t", 2);

	// Format string to create plugin style.
	TranslationPluginFormatString(sTranslation, TRANSLATION_MAX_LENGTH_CONSOLE, false);

	// Print translated phrase to server or client's chat/console
	ReplyToCommand(clientIndex, sTranslation);
}