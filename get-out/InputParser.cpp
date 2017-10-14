#include "InputParser.h"

#include "globals.h"
#include "World.h"
#include "Instruction.h"
#include "ActionType.h"
#include "Direction.h"
#include "NamesInfo.h"


InputParser::InputParser()
{
}


InputParser::~InputParser()
{
}


Instruction * InputParser::parse(const std::string & userInput, const NamesInfo & namesInfo) const
{
	Instruction* instruction = new Instruction();

	std::vector<std::string> inputVector = parseToVector(userInput);

	if (checkInputValidity(inputVector, namesInfo, instruction))
	{
		populateInstructionUnion(inputVector, instruction);
	}
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


bool InputParser::checkInputValidity(const std::vector<std::string>& input, const NamesInfo & namesInfo, Instruction * instruction) const
{
	// We'll assume an error unless proven otherwise
	instruction->actionType = ActionType::ERROR;

	int inputSize = input.size();
	// Step 1: Determine if we actually got some input (the used could have just written spaces)
	if (inputSize == 0)
	{	
		instruction->errorDescription = "Input rejected: You might want to write something other than spaces before pushing ENTER.";
		return false;
	}

	// Step 2: Determine if we have a valid action and if we have the right amount of words for that action
	ActionType action = getActionFromString(input[0]);
	const std::string actionString = getStringFromAction(action);
	const int actionExpectedLength = getActionExpectedLength(action);
	if (action == ActionType::_UNDEFINED || action == ActionType::ERROR)
	{
		instruction->errorDescription = "Input rejected: I can't understand the word " + input[0] + ".";
		return false;
	}
	else if (actionExpectedLength != inputSize)
	{
		instruction->errorDescription = "Input rejected: The amount of words written differs from what's expected for the " + actionString + " action.";
		return false;
	}

	// Step 3: Verify if our action requires no parameters (single-word actions)
	if (actionExpectedLength == 1)
	{
		instruction->actionType = action;
		return true;
	}

	// Step 4: Determine if we have the right preposition (index 2) for the action (if applicable -> inputSize == 4)
	if (inputSize == 4 && !caselessEquals(getActionPreposition(action), input[2]))
	{
		instruction->errorDescription = "Input rejected: The right preposition for the " + actionString + " action is not " + input[2] + ".";
		return false;
	}

	// Step 5: Determine if we have the right parameter at index 1 for the action
	if (!verifyActionParameterType(getActionParameterType(action, 1), input[1], namesInfo, instruction))
	{
		return false;
	}

	// Step 6: Verify if our action requires no further parameters (two-word action)
	if (actionExpectedLength == 2)
	{
		instruction->actionType = action;
		return true;
	}

	// Step 7: Determine if we have the right parameter at index 3
	if (!verifyActionParameterType(getActionParameterType(action, 3), input[3], namesInfo, instruction))
	{
		return false;
	}
	
	// Step 8: If we got this far, the input is also valid (there were some early valid exits)
	instruction->actionType = action;
	return true;
}


bool InputParser::populateInstructionUnion(const std::vector<std::string>& validatedInput, Instruction * instruction) const
{
	switch (instruction->actionType)
	{
	case ActionType::ERROR:
	case ActionType::QUIT:
	case ActionType::HELP:
	case ActionType::LOOK:
		// In this cases, nothing should be added to the instruction
		break;
	case ActionType::GO:
		instruction->direction = getDirectionFromString(validatedInput[1]);
		break;
	case ActionType::TAKE:
	case ActionType::DROP:
	case ActionType::INSPECT:
		instruction->entity = world->getEntity(validatedInput[1]);
		break;
	case ActionType::OPEN:
		instruction->interactableOpen = world->getInteractableOpen(validatedInput[1]);
		break;
	case ActionType::USE:
		instruction->itemUse = world->getItemUse(validatedInput[1], validatedInput[3]);
		break;
	case ActionType::PUT:
		instruction->itemPut = world->getItemPut(validatedInput[1], validatedInput[3]);
		break;
	default:
		break;
	}

	return true;
}


bool InputParser::verifyActionParameterType(ActionParameterType actionParameterType, const std::string & parameter, const NamesInfo& namesInfo, Instruction* instruction) const
{
	switch (actionParameterType)
	{
	case ActionParameterType::DIRECTION:
		if (getDirectionFromString(parameter) == Direction::_UNDEFINED)
		{
			instruction->errorDescription = "Input rejected: " + parameter + " is not a valid direction.";
			return false;
		}
		break;
	case ActionParameterType::ITEM:
		if (!isCaselessStringInVector(namesInfo.items, parameter))
		{
			instruction->errorDescription = "Input rejected: " + parameter + " does not quite fit in your instruction.";
			return false;
		}
		break;
	case ActionParameterType::INTERACTABLE:
		if (!isCaselessStringInVector(namesInfo.interactables, parameter))
		{
			instruction->errorDescription = "Input rejected: " + parameter + " does not quite fit in your instruction.";
			return false;
		}
		break;
	case ActionParameterType::ITEM_OR_INTERACTABLE:
		if (!isCaselessStringInVector(namesInfo.items, parameter) && !isCaselessStringInVector(namesInfo.interactables, parameter))
		{
			instruction->errorDescription = "Input rejected: " + parameter + " does not quite fit in your instruction.";
			return false;
		}
		break;
	default:
		// This case should never be hit (including NONE), but just in case, we'll have a message
		instruction->errorDescription = "Input rejected: Why? Who knows...";
		return false;
	}

	// No errors found.
	return true;
}


bool InputParser::isCaselessStringInVector(const std::vector<std::string>& vector, const std::string string) const
{
	for (const std::string& s : vector)
	{
		if (caselessEquals(s, string))
		{
			return true;
		}
	}
	return false;
}

