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


Instruction InputParser::parse(const std::string& aUserInput) const
{
	std::vector< std::string > lInputVector = parseToVector(aUserInput);
	return parseToInstruction(lInputVector);
}


std::vector< std::string > InputParser::parseToVector(const std::string& aUserInput) const
{
	std::vector< std::string > lInputVector;
	bool lInWord = false;
	unsigned int lStart = 0;
	unsigned int lInputLength = aUserInput.length();
	for (unsigned int i = 0; i < lInputLength; ++i)
	{
		if (aUserInput[i] == ' ')
		{
			if (lInWord)
			{
				lInputVector.push_back(aUserInput.substr(lStart, i - lStart));
			}
			lStart = i + 1;
			lInWord = false;
		}
		else
		{
			lInWord = true;
		}
	}
	if (lInWord)
	{
		lInputVector.push_back(aUserInput.substr(lStart, lInputLength - lStart));
	}
	return lInputVector;
}


Instruction InputParser::parseToInstruction(const std::vector< std::string >& aInput) const
{
	Instruction lInstruction;
	// We'll assume an error unless proven otherwise (this is the default value of InstructionType anyway)
	lInstruction.mInstructionType = InstructionType::ERROR;

	int lInputSize = aInput.size();
	// Step 1: Determine if we actually got some aInput (the used could have just written spaces)
	if (lInputSize == 0)
	{
		lInstruction.mParam1 = "Input rejected: You might want to write something other than spaces before pushing ENTER.";
		return lInstruction;
	}

	// Step 2: Determine if we have a valid lAction and if we have the right amount of words for that lAction
	InstructionType lAction = getInstructionFromString(aInput[0]);
	const std::string lActionString = getStringFromInstruction(lAction);
	const int lActionExpectedLength = getInstructionExpectedLength(lAction);
	if (lAction == InstructionType::ERROR)
	{
		lInstruction.mParam1 = "Input rejected: I can't understand the word " + aInput[0] + ".";
		return lInstruction;
	}
	else if (lActionExpectedLength != lInputSize)
	{
		lInstruction.mParam1 = "Input rejected: The amount of words written differs from what's expected for the " + lActionString + " action.";
		return lInstruction;
	}

	// Step 3: Determine if we have the right preposition (index 2) for the lAction (if applicable -> lInputSize == 4)
	if (lInputSize == 4 && !caselessEquals(getInstructionPreposition(lAction), aInput[2]))
	{
		lInstruction.mParam1 = "Input rejected: The right preposition for the " + lActionString + " action is not " + aInput[2] + ".";
		return lInstruction;
	}

	// Step 4: If we got this far, the aInput's format is valid, so we populate the lInstruction
	lInstruction.mInstructionType = lAction;
	if (lInputSize == 2)
	{
		lInstruction.mParam1 = aInput[1];
	}
	if (lInputSize == 4)
	{
		lInstruction.mParam1 = aInput[1];
		lInstruction.mParam2 = aInput[3];
	}
	return lInstruction;
}
