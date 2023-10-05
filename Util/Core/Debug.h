//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Debug
//------------------------------------------------------------------------------
#ifdef _DEBUG
void LogDebugArgs(const char* text, ...);
void LogDebug(const char* text);
void LogDebugArgsW(const wchar_t* text, ...);
void LogDebugW(const wchar_t* text);
#else
#define LogDebug(...) {}
#define LogDebugArgs(...) {}
#define LogDebugW(...) {}
#define LogDebugArgsW(...) {}
#endif