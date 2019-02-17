#ifndef H_INSTRUCTION_TYPE
#define H_INSTRUCTION_TYPE

#include <string>


enum class InstructionType
{
	ERROR,
	QUIT,
	HELP,
	LOOK,
	INVENTORY,
	GO,
	TAKE,
	DROP,
	INSPECT,
	OPEN,
	USE,
	PUT
};


InstructionType getInstructionFromString(const std::string& aText);
std::string getStringFromInstruction(InstructionType aInstructionType);
std::string getInstructionPreposition(InstructionType aInstructionType);
unsigned int getInstructionExpectedLength(InstructionType aInstructionType);


#endif // !H_INSTRUCTION_TYPE
