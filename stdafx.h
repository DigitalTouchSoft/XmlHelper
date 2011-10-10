/**
*
*    Filename: stdafx.h
*
* Description: pre-compiled header.
*
*      Author: Dennis McMahon
*
*/

#pragma once

#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#define STRICT
#define WINVER       0x500
#define _WIN32_WINNT 0x501
#define _WIN32_IE    0x501   // set the target ie version to 5 and above.

#include <windows.h>
#include <commctrl.h>
#include <objbase.h>
#include <list>
#include <map>

/**
* the outer declaration header for this module...
*/

#include <XMLHelper.h>
#include "ShellXml.h"

#ifdef _DEBUG
static void DebugOut (char *string,...)
{
va_list valist;

    if (string)
      {
        wchar_t *inString = new wchar_t [strlen (string) + 4];
        if (inString)
          {
            wchar_t outString[1024];
            // copy the string...
			for (int pos=0; string[pos]; ++pos) { inString[pos] = (wchar_t)string[pos]; inString[pos] = 0; }
            // sprint the string...
            va_start (valist,string); 
            int len = wvsprintf (outString,inString,valist);
            va_end (valist);
            outString[len++] = '\r';
            outString[len++] = '\n';
            outString[len] = 0;
            OutputDebugString (outString);
            // delete the temp buffer...
            delete inString;
          }
      }
}
#else
#define DebugOut __noop 
#endif

/* eof */