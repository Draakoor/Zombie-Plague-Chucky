/*
 * ============================================================================
 *
 *  Zombie Plague Mod #3 Generation
 *
 *  File:           paramparser.cpp
 *  Type:           Core 
 *  Description:    Provides functions for parsing single line strings, 
 *					and parameters in key="value" format.
 *
 *
 *                  Examle raw string:
 *                  key1 = "value1"
 *                  key2 = "value2"
 *
 *  Copyright (C) 2015-2016  Nikita Ushakov (Ireland, Dublin)
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
 * @section Limit settings.
 **/
#define PARAM_VALUE_MAXPARTS 32		 /** Maximum sub parts of value string. */	
#define PARAM_NAME_MAXLEN    64      /** Maximum length of key name. */
#define PARAM_VALUE_MAXLEN   512    /** Maximum length of value string. */
/**
 * @endsection
 **/

/**
 * @section Parsing error codes.
 **/
#define PARAM_ERROR_NO               	-1  /** No errors. */
#define PARAM_ERROR_EMPTY               0   /** Source string is empty. */
#define PARAM_ERROR_UNEXPECTED_KEY      1   /** Unexpected key name. */
#define PARAM_ERROR_UNEXPECTED_END      2   /** Unexpected end of source string. */
#define PARAM_ERROR_MISSING_EQUATION    3   /** Could not find a equation sign (=) after previous key name. */
#define PARAM_ERROR_UNKNOWN             4   /** Unknown error. The parser got a invalid result from a search function it couldn't handle. */
#define PARAM_ERROR_FULL                5   /** Destination array is full. */
/**
 * @endsection
 **/

/**
 * Modes for what to do and expect when parsing. White space characters between
 * modes are ignored.
 **/
enum ParamModes
{
    ParamMode_Equal,	/** Expect a equation sign. */
    ParamMode_Key,		/** Expect a key name. */
    ParamMode_Value,	/** Expect a value string. */
	ParamMode_Finish	/** Finish parsing. */
}

/**
 * Structure for storing a key/value pair.
 **/
enum ParamParseResult
{
    String:Param_Name[PARAM_NAME_MAXLEN],   /** Key or flag name. */
    String:Param_Value[PARAM_VALUE_MAXLEN]  /** Value. Only used if a key. */
}


/**************************************
 *                                    *
 *       PARAMETER FUNCTIONS          *
 *                                    *
 **************************************/

 
/**
 * Parses a parameter string in key = "value" format and store the result in a ParamParseResult array.
 *
 * @param iBuffer		A ParamParseResult array to store results.
 * @param sParamString	The source string to parse. String is trimmed before parsing.
 * @param iMaxLen       Maximum number of keys that can be stored (first dimension of buffer).
 * @param iKeys			Opional output: Number of array.
 * @return 				Returns error code if parsing error.
 **/
stock int ParamParseString(iBuffer[][ParamParseResult], char[] sParamString, int iMaxLen, int &iKeys = 0)
{
	/*
	 *  VALIDATION OF INPUT AND BUFFERS
	 */

	// Trim string
	TrimString(sParamString);

	// Get string length
	int iLen = strlen(sParamString);

	// Check if string is empty
	if (!iLen)
	{
		return PARAM_ERROR_EMPTY;
	}

	// Check if there's space left in the destination buffer
	if (iMaxLen > PARAM_VALUE_MAXLEN || iLen > PARAM_VALUE_MAXLEN)
	{
		// Exit loop. No more parameters can be parsed
		return PARAM_ERROR_FULL;
	}
	
	/*
	 *  PARSE LOOP
	 */

	// Initialize. Expect a equation sign
	ParamModes iMode = ParamMode_Equal;

	// Buffers for temp values
	int  iStartPos;
	int  iEndPos;
	int  iEquationPos;
	char sValue[PARAM_VALUE_MAXLEN];

	// Loop through all string
	while(iMode < ParamMode_Finish)
	{
		/*
		 *  MODE CHECK
		 */

		// Check mode for deciding what to do
		switch (iMode)
		{
			case ParamMode_Equal :
			{
				// Position of equal character
				iEquationPos = FindCharInString(sParamString, '=', false);

				// Parse error
				if(iEquationPos == -1)
				{
					return PARAM_ERROR_MISSING_EQUATION;
				}

				// Update end position of key character. Substract by one to include 
				// the current character in next mode
				iEndPos = iEquationPos - 1;

				// Expect a key name
				iMode = ParamMode_Key;
			}

			case ParamMode_Key :
			{
				// Extract key name
				StrExtract(sValue, sParamString, iStartPos, iEndPos);

				// Check if string is empty
				if (!strlen(sValue))
				{
					return PARAM_ERROR_UNEXPECTED_KEY;
				}

				// Copy key name to destination buffer
				strcopy(iBuffer[iKeys][Param_Name], PARAM_NAME_MAXLEN, sValue);

				// Change mode to expect a value at next position.
				iMode = ParamMode_Value;
			}

			case ParamMode_Value :
			{
				// Find start position of first non white space character
				iStartPos = iEquationPos + 1;

				// Extract value string
				StrExtract(sParamString, sParamString, iStartPos, iLen);

				// Check if string is empty
				if (!strlen(sParamString))
				{
					return PARAM_ERROR_UNEXPECTED_END;
				}

				// Copy value string to destination buffer
				strcopy(iBuffer[iKeys][Param_Value], PARAM_VALUE_MAXLEN, sParamString);

				// Successful parsing
				iMode = ParamMode_Finish;
			}
		}
	}

	return PARAM_ERROR_NO;
}

/**
 * Finds the first key index in a parameter array matching the specified key.
 *
 * @param iParams		A ParamParseResult array to search through.
 * @param iMaxKeys		Max amount of keys on the 2D array.
 * @param sKey			Key to find.
 * @param caseSensitive	Specifies whether the search is case sensitive or not (default).
 * @return				Index of the key if found, -1 otherwise.
 **/
stock int ParamFindKey(iBuffer[][ParamParseResult], int iMaxKeys, const char[] sKey, bool caseSensitive = false)
{
    // Loop through all parameters
    for (int iIndex = 0; iIndex < iMaxKeys; iIndex++)
    {
        // Match key name
        if (StrEqual(iBuffer[iIndex][Param_Name], sKey, caseSensitive))
        {
            // Key found, return the key index
            return iIndex;
        }
    }
    
    return -1;
}

/**************************************
 *                                    *
 *         HELPER FUNCTIONS           *
 *                                    *
 **************************************/
 
/**
 * Extracts a area in a string between two positions.
 *
 * @param sBuffer	Destination string buffer.
 * @param sSource	Source string to extract from.
 * @param startPos	Start position of string to extract.
 * @param endPos	End position of string to extract.
 * @return			Number of cells written.
 **/
stock int StrExtract(char[] sBuffer, const char[] sSource, int startPos, int endPos)
{
    // Calculate string length. Also add space for null terminator
    int iMaxLen = endPos - startPos + 1;
    
    // Validate length
    if (iMaxLen < 0)
    {
        return 0;
    }
    
    // Extract string and store it in the buffer
    return strcopy(sBuffer, iMaxLen, sSource[startPos]);
}

/**
 * Unescapes a string (replaces "\n" with newlines, etc.).
 *
 * @param sBuffer	String to unescape.
 **/
stock void StrUnescape(char[] sBuffer)
{
    ReplaceString(sBuffer, PARAM_VALUE_MAXLEN, "\\n", "\n");
    ReplaceString(sBuffer, PARAM_VALUE_MAXLEN, "\\r", "\r");
    ReplaceString(sBuffer, PARAM_VALUE_MAXLEN, "\\t", "\t");
    ReplaceString(sBuffer, PARAM_VALUE_MAXLEN, "\\\"", "\"");
    ReplaceString(sBuffer, PARAM_VALUE_MAXLEN, "\\\\", "\\");
}