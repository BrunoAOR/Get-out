#ifndef H_INSTRUCTION
#define H_INSTRUCTION

#include <string>
enum class InstructionType;


struct Instruction
{
public:
	InstructionType mInstructionType;
	std::string mParam1;
	std::string mParam2;
};


#endif // !H_INSTRUCTION
