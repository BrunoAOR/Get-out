#include "EffectPlaceItemInItem.h"

#include <assert.h>
#include "Item.h"


EffectPlaceItemInItem::EffectPlaceItemInItem(const std::string& aEffectDescription, Item* aItem, Item* aContainer)
	: ActionEffect(aEffectDescription), mItem(aItem), mContainer(aContainer)
{
	assert(mItem && mContainer);
}


EffectPlaceItemInItem::~EffectPlaceItemInItem()
{
}


void EffectPlaceItemInItem::doEffect() const
{
	bool lSuccess = mItem->setParent(mContainer);
	assert(lSuccess);
}
