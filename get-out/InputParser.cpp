#include "InputParser.h"

#include "Instruction.h"
#include "InstructionType.h"
#include "globals.h"


InputParser::InputParser()
{
}


InputParser::~InputParser()
{
}


Instruction InputParser::parse(const std::string& userInput) const
{
	std::vector<std::string> inputVector = parseToVector(userInput);
	return parseToInstruction(inputVector);
}


std::vector<std::string> InputParser::parseToVector(const std::string& userInput) const
{
	std::vector<std::string> inputVector;
	bool inWord = false;
	unsigned int start = 0;
	unsigned int inputLength = userInput.length();
	for (unsigned int i = 0; i < inputLength; ++i)
	{
		if (userInput[i] == ' ')
		{
			if (inWord)
			{
				inputVector.push_back(userInput.substr(start, i - start));
			}
			start = i + 1;
			inWord = false;
		}
		else
		{
			inWord = true;
		}
	}
	if (inWord)
	{
		inputVector.push_back(userInput.substr(start, inputLength - start));
	}
	return inputVector;
}


Instruction InputParser::parseToInstruction(const std::vector<std::string>& input) const
{
	Instruction instruction;
	// We'll assume an error unless proven otherwise (this is the default value of InstructionType anyway)
	instruction.instructionType = InstructionType::ERROR;

	int inputSize = input.size();
	// Step 1: Determine if we actually got some input (the used could have just written spaces)
	if (inputSize == 0)
	{
		instruction.param1 = "Input rejected: You might want to write something other than spaces before pushing ENTER.";
		return instruction;
	}

	// Step 2: Determine if we have a valid action and if we have the right amount of words for that action
	InstructionType action = getInstructionFromString(input[0]);
	const std::string actionString = getStringFromInstruction(action);
	const int actionExpectedLength = getInstructionExpectedLength(action);
	if (action == InstructionType::ERROR)
	{
		instruction.param1 = "Input rejected: I can't understand the word " + input[0] + ".";
		return instruction;
	}
	else if (actionExpectedLength != inputSize)
	{
		instruction.param1 = "Input rejected: The amount of words written differs from what's expected for the " + actionString + " action.";
		return instruction;
	}

	// Step 3: Determine if we have the right preposition (index 2) for the action (if applicable -> inputSize == 4)
	if (inputSize == 4 && !caselessEquals(getInstructionPreposition(action), input[2]))
	{
		instruction.param1 = "Input rejected: The right preposition for the " + actionString + " action is not " + input[2] + ".";
		return instruction;
	}

	// Step 4: If we got this far, the input's format is valid, so we populate the instruction
	instruction.instructionType = action;
	if (inputSize == 2)
	{
		instruction.param1 = input[1];
	}
	if (inputSize == 4)
	{
		instruction.param1 = input[1];
		instruction.param2 = input[3];
	}
	return instruction;
}
