#include "globals.h"

#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


bool gIsGameEndRequested = false;


void consoleLog(const std::string& aMessage)
{
	if (aMessage.length() == 0)
	{
		return;
	}
	std::cout << "\n*****\n" << aMessage << "\n*****\n\n";
}


void consoleLog(char aCharacter)
{
	std::cout << aCharacter;
}


void outputLog(const char* aFile, int aLine, const char* aFormat, ...)
{
	static char lTtmp_string[4096];
	static char lTmp_string2[4096];
	static va_list  ap;
	// Construct the string from variable arguments
	va_start(ap, aFormat);
	vsprintf_s(lTtmp_string, 4096, aFormat, ap);
	va_end(ap);
	sprintf_s(lTmp_string2, 4096, "--%s(%d)\n----%s\n", aFile, aLine, lTtmp_string);
	OutputDebugString(lTmp_string2);
}


bool caselessEquals(const std::string& aS1, const std::string& aS2)
{
	return _stricmp(aS1.c_str(), aS2.c_str()) == 0;
}
