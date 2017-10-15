#include "Player.h"
#include "globals.h"
#include "EntityType.h" 
#include "Instruction.h"
#include "ActionType.h"

Player::Player(EntityType type, std::string name, std::string description, unsigned int maxItems)
	: Entity(type, name, description), m_maxItems(maxItems)
{
}


Player::~Player()
{
}


void Player::executeInstruction(const Instruction * instruction)
{
	switch (instruction->actionType)
	{
	case ActionType::LOOK:
		look();
		break;
		/*
	case ActionType::GO:
		go(instruction->direction);
		break;
	case ActionType::TAKE:
		take(instruction->entity);
		break;
	case ActionType::DROP:
		drop(instruction->entity);
		break;
	case ActionType::INSPECT:
		inspect(instruction->entity);
		break;
	case ActionType::OPEN:
		open(instruction->interactableOpen);
		break;
	case ActionType::USE:
		use(instruction->itemUse);
		break;
	case ActionType::PUT:
		put(instruction->itemPut);
		break;
	*/
	default:
		break;
	}
	
}

bool Player::canAddChild(Entity * child)
{
	return child->getType() == EntityType::ITEM;
}

void Player::look()
{
	if (m_parent == nullptr)
	{
		return;
	}
	consoleLog(m_parent->getDescription());
}
