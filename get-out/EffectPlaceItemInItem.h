#ifndef H_EFFECT_PLACE_ITEM_IN_ITEM
#define H_EFFECT_PLACE_ITEM_IN_ITEM

#include "ActionEffect.h"
class Item;


class EffectPlaceItemInItem :
	public ActionEffect
{
public:
	EffectPlaceItemInItem(const std::string& effectDescription, Item* item, Item* container);
	~EffectPlaceItemInItem();

	// Inherited via ActionEffect
	virtual void doEffect() override;

private:
	Item* m_item;
	Item* m_container;
};


#endif // !H_EFFECT_PLACE_ITEM_IN_ITEM
