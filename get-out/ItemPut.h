#ifndef H_ITEM_PUT
#define H_ITEM_PUT

#include<string>
class Item;


struct ItemPut
{
public:
	Item* item;
	Item* container;
	std::string m_description;
};

#endif // !H_ITEM_PUT
