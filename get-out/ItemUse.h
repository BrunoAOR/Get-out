#ifndef H_ITEM_USE
#define H_ITEM_USE

#include<string>
class Item;
class Interactable;
class ActionEffect;


struct ItemUse
{
public:
	Item* item;
	Interactable* interactable;
	std::string m_description;
	ActionEffect* effect;
};


#endif // !H_ITEM_USE
