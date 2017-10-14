#ifndef H_INSTRUCTION
#define H_INSTRUCTION

#include <string>
enum class ActionType;
enum class Direction;
class Entity;
struct InteractableOpen;
struct ItemUse;
struct ItemPut;


struct Instruction
{
public:
	ActionType actionType;
	std::string errorDescription;
	union
	{
		Direction direction;
		Entity* entity;
		InteractableOpen* interactableOpen;
		ItemUse* itemUse;
		ItemPut* itemPut;
	};
};


#endif // !H_INSTRUCTION
