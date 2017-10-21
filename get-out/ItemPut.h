#ifndef H_ITEM_PUT
#define H_ITEM_PUT

#include "Action.h"
#include<string>
class Item;


class ItemPut :
	public Action
{
public:
	ItemPut(std::string description, std::vector<ActionEffect*> effects, bool shouldDestroy, Item* item, Item* container);
	~ItemPut();

private:
	Item* m_item;
	Item* m_container;
};

#endif // !H_ITEM_PUT
