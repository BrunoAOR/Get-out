#ifndef H_EFFECT_PLACE_ITEM_IN_ITEM
#define H_EFFECT_PLACE_ITEM_IN_ITEM

#include "ActionEffect.h"
class Item;


class EffectPlaceItemInItem :
	public ActionEffect
{
public:
	EffectPlaceItemInItem(const std::string& aEffectDescription, Item* aItem, Item* aContainer);
	~EffectPlaceItemInItem();

	// Inherited via ActionEffect
	virtual void doEffect() const override;

private:
	Item* mItem = nullptr;
	Item* mContainer = nullptr;
};


#endif // !H_EFFECT_PLACE_ITEM_IN_ITEM
