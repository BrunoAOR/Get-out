#include "globals.h"

#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


bool isGameEndRequested = false;


void consoleLog(const std::string& message)
{
	if (message.length() == 0)
	{
		return;
	}
	std::cout << "\n*****\n" << message << "\n*****\n\n";
}


void consoleLog(char character)
{
	std::cout << character;
}


void outputLog(const char* file, int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;
	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "--%s(%d)\n----%s\n", file, line, tmp_string);
	OutputDebugString(tmp_string2);
}


bool caselessEquals(const std::string& s1, const std::string& s2)
{
	return _stricmp(s1.c_str(), s2.c_str()) == 0;
}
