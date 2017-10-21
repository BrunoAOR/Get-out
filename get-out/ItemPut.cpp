#include "ItemPut.h"
#include "ActionType.h"
#include "Item.h"
#include <assert.h>

ItemPut::ItemPut(std::string description, std::vector<ActionEffect*> effects, bool shouldDestroy, Item * item, Item * container)
	: Action(ActionType::ItemPut, description, effects, shouldDestroy, item, container)
{
	assert(item && container);
}


ItemPut::~ItemPut()
{
}
