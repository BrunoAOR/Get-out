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


Instruction * InputParser::parse(const std::string & userInput) const
{
	Instruction* instruction = new Instruction();

	std::vector<std::string> inputVector = parseToVector(userInput);

	checkInputValidity(inputVector, instruction);

	return instruction;
}


std::vector<std::string> InputParser::parseToVector(const std::string & userInput) const
{
	std::vector<std::string> inputVector;
	bool inWord = false;
	unsigned int start = 0;
	for (unsigned int i = 0; i < userInput.length(); ++i)
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
		inputVector.push_back(userInput.substr(start, userInput.length() - start));
	}
	return inputVector;
}


bool InputParser::checkInputValidity(const std::vector<std::string>& input, Instruction * instruction) const
{
	// We'll assume an error unless proven otherwise
	instruction->instructionType = InstructionType::ERROR;

	int inputSize = input.size();
	// Step 1: Determine if we actually got some input (the used could have just written spaces)
	if (inputSize == 0)
	{	
		instruction->errorDescription = "Input rejected: You might want to write something other than spaces before pushing ENTER.";
		return false;
	}

	// Step 2: Determine if we have a valid action and if we have the right amount of words for that action
	InstructionType action = getInstructionFromString(input[0]);
	const std::string actionString = getStringFromInstruction(action);
	const int actionExpectedLength = getInstructionExpectedLength(action);
	if (action == InstructionType::ERROR)
	{
		instruction->errorDescription = "Input rejected: I can't understand the word " + input[0] + ".";
		return false;
	}
	else if (actionExpectedLength != inputSize)
	{
		instruction->errorDescription = "Input rejected: The amount of words written differs from what's expected for the " + actionString + " action.";
		return false;
	}

	// Step 3: Determine if we have the right preposition (index 2) for the action (if applicable -> inputSize == 4)
	if (inputSize == 4 && !caselessEquals(getInstructionPreposition(action), input[2]))
	{
		instruction->errorDescription = "Input rejected: The right preposition for the " + actionString + " action is not " + input[2] + ".";
		return false;
	}
	
	// Step 4: If we got this far, the input's format is valid, so we populate the instruction
	instruction->instructionType = action;
	if (inputSize == 2)
	{
		instruction->param1 = input[1];
	}
	if (inputSize == 4)
	{
		instruction->param1 = input[1];
		instruction->param2 = input[3];
	}
	return true;
}
