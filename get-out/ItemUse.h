#ifndef H_ITEM_USE
#define H_ITEM_USE

#include "Action.h"
#include <string>
#include <vector>
class Item;
class Interactable;
class ActionEffect;


class ItemUse :
	public Action
{
public:
	ItemUse(std::string description, std::vector<ActionEffect*> effects, bool shouldDestroy, Item* item, Interactable* interactable);
	~ItemUse();
};


#endif // !H_ITEM_USE
