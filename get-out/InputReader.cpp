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
	char key;
	
	if (shouldClear)
	{
		shouldClear = false;
		input.clear();
	}

	if (_kbhit())
	{
		key = _getch();

		// Handle forbidden characters (any non-alphanumeric) and special sequences (like arrows, function keys, home, end, etc...)
		if (!isAllowed(key))
		{
			std::string message = "INPUT: Ignoring special character/sequence: ";
			if (key == '\0')
			{
				message.append("[NUL (code: 0)]");
			}
			else
			{
				message += "[" + std::string(1, key) + " (code: " + std::to_string((int)key) + ")]";
			}
			
			// drop all chars after invalid character (if present)
			
			while (_kbhit())
			{
				message += " + ";
				key = _getch();
				if (key == '\0')
				{
					message.append("[NUL (code: 0)]");
				}
				else
				{
					message += "[" + std::string(1, key) + " (code: " + std::to_string((int)key) + ")]";
				}
			}
			message += "!";
			OutputLog(message.c_str());
		}
		// Handle backspace
		else if (key == '\b')
		{
			if (input.length() > 0) {
				input.pop_back();
				consoleLog('\b');
				consoleLog(" ");
				consoleLog('\b');
			}
		}
		// Handle any key other than Enter
		else if (key != '\r')
		{
			input.push_back(key);
			consoleLog(key);
		}
		// Handle Enter key
		else
		{
			shouldClear = true;
			if (input.length() > 0)
			{
				consoleLog('\n');
				return input;
			}
		}
	}
	
	return "";
}

// Allowed characters are alphanumeric, space, backspace and enter
bool InputReader::isAllowed(char c) const
{
	return c > 0 && c <= 255 &&  isalnum(c) || c == '\b' || c == '\r' || c == 32;
}
