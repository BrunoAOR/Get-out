#include "EffectPlaceItemInItem.h"

#include "globals.h"
#include "Item.h"


EffectPlaceItemInItem::EffectPlaceItemInItem(const std::string& aEffectDescription, Item* aItem, Item* aContainer)
	: ActionEffect(aEffectDescription), mItem(aItem), mContainer(aContainer)
{
	ASSERT(mItem && mContainer);
}


EffectPlaceItemInItem::~EffectPlaceItemInItem()
{
}


void EffectPlaceItemInItem::doEffect() const
{
	bool lSuccess = mItem->setParent(mContainer);
	ASSERT(lSuccess);
}
