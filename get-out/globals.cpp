#include "globals.h"

#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

bool gIsGameEndRequested = false;


namespace {

	static std::string sAssertionString;

	void outputLog(const char* aFile, int aLine, const char* aFormat, va_list aArgs)
	{
		static char lTtmp_string[4096];
		static char lTmp_string2[4096];
		// Construct the string from variable arguments
		vsprintf_s(lTtmp_string, 4096, aFormat, aArgs);
		sprintf_s(lTmp_string2, 4096, "--%s (Ln. %d)\n----%s\n", aFile, aLine, lTtmp_string);
		OutputDebugString(lTmp_string2);
	}

}//	anonymous

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
	static va_list  lArgs;
	va_start(lArgs, aFormat);
	outputLog(aFile, aLine, aFormat, lArgs);
}


void assertLog(bool expression, const char* aExpressionText, const char* aFile, int aLine)
{
	if (!expression)
	{
		outputLog(aFile, aLine, "ASSERTION ERROR!\n----Assert expression: %s", aExpressionText);
		abort();
	}
}


void assertMsg(bool expression, const char* aFormat, const char* aExpressionText, const char* aFile, int aLine, ...)
{
	if (!expression)
	{
		sAssertionString.clear();
		sAssertionString = "ASSERTION ERROR: ";
		sAssertionString.append(aFormat);
		sAssertionString.append("\n----Assert expression: ");
		sAssertionString.append(aExpressionText);
		const char* lNewFormat = sAssertionString.c_str();
		static va_list  lArgs;
		va_start(lArgs, aLine);
		outputLog(aFile, aLine, lNewFormat, lArgs);
		va_end(lArgs);
		abort();
	}
}
	

bool caselessEquals(const std::string& aS1, const std::string& aS2)
{
	return _stricmp(aS1.c_str(), aS2.c_str()) == 0;
}
