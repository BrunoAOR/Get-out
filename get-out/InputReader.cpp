#include "InputReader.h"

#include <conio.h>
#include "globals.h"


InputReader::InputReader()
{	
}


InputReader::~InputReader()
{
}


std::string InputReader::getInput()
{
	if (mShouldClear)
	{
		mShouldClear = false;
		mInput.clear();
		consoleLog('>');
		consoleLog(' ');
	}

	if (_kbhit())
	{
		char lKey = _getch();

		// Handle forbidden characters (any non-alphanumeric) and special sequences (like arrows, function keys, home, end, etc...)
		if (!isAllowed(lKey))
		{
			handleInvalidKeys(lKey);
		}
		// Handle backspace
		else if (lKey == '\b')
		{
			if (mInput.length() > 0) {
				mInput.pop_back();
				consoleLog('\b');
				consoleLog(' ');
				consoleLog('\b');
			}
		}
		// Handle any lKey other than Enter
		else if (lKey != '\r')
		{
			mInput.push_back(lKey);
			consoleLog(lKey);
		}
		// Handle Enter lKey
		else
		{
			if (mInput.length() > 0)
			{
				mShouldClear = true;
				consoleLog('\n');
				return mInput;
			}
		}
	}

	return "";
}


bool InputReader::getEnter() const
{
	if (_kbhit())
	{
		return (_getch() == '\r');
	}
	return false;
}


void InputReader::handleInvalidKeys(char aKey) const
{
	std::string lMessage = "INPUT: Ignoring special character/sequence: ";

	// Drop all chars after invalid character (if present)
	bool lAllCharsCaptured = false;
	while (!lAllCharsCaptured)
	{
		if (aKey == '\0')
		{
			lMessage.append("[NUL (code: 0)]");
		}
		else if (aKey == '%')
		{
			lMessage.append("[%% (code: 37)]");
		}
		else
		{
			lMessage += "[" + std::string(1, aKey) + " (code: " + std::to_string((int)aKey) + ")]";
		}

		if (_kbhit())
		{
			lMessage += " + ";
			aKey = _getch();
		}
		else
		{
			lAllCharsCaptured = true;
		}
	}

	lMessage += "!";
	OutputLog(lMessage.c_str());
}


bool InputReader::isAllowed(char aChar) const
{
	// Allowed characters are those within the basic ASCII table, alphanumeric, backspace, return, space and underscore
	return aChar > 0
		&& aChar < 128
		&& (isalnum(aChar) || aChar == '\b' || aChar == '\r' || aChar == ' ' || aChar == '_');
}
