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
	if (m_shouldClear)
	{
		m_shouldClear = false;
		m_input.clear();
		consoleLog('>');
		consoleLog(' ');
	}

	if (_kbhit())
	{
		char key = _getch();

		// Handle forbidden characters (any non-alphanumeric) and special sequences (like arrows, function keys, home, end, etc...)
		if (!isAllowed(key))
		{
			handleInvalidKeys(key);
		}
		// Handle backspace
		else if (key == '\b')
		{
			if (m_input.length() > 0) {
				m_input.pop_back();
				consoleLog('\b');
				consoleLog(' ');
				consoleLog('\b');
			}
		}
		// Handle any key other than Enter
		else if (key != '\r')
		{
			m_input.push_back(key);
			consoleLog(key);
		}
		// Handle Enter key
		else
		{
			if (m_input.length() > 0)
			{
				m_shouldClear = true;
				consoleLog('\n');
				return m_input;
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


void InputReader::handleInvalidKeys(char key) const
{
	std::string message = "INPUT: Ignoring special character/sequence: ";

	// Drop all chars after invalid character (if present)
	bool allCharsCaptured = false;
	while (!allCharsCaptured)
	{
		if (key == '\0')
		{
			message.append("[NUL (code: 0)]");
		}
		else if (key == '%')
		{
			message.append("[%% (code: 37)]");
		}
		else
		{
			message += "[" + std::string(1, key) + " (code: " + std::to_string((int)key) + ")]";
		}

		if (_kbhit())
		{
			message += " + ";
			key = _getch();
		}
		else
		{
			allCharsCaptured = true;
		}
	}

	message += "!";
	OutputLog(message.c_str());
}


bool InputReader::isAllowed(char c) const
{
	// Allowed characters are those within the basic ASCII table, alphanumeric, backspace, return, space and underscore
	return c > 0
		&& c < 128
		&& (isalnum(c) || c == '\b' || c == '\r' || c == ' ' || c == '_');
}
