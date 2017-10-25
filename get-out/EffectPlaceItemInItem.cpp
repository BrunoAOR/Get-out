#include "EffectPlaceItemInItem.h"
#include "Item.h"
#include <assert.h>


EffectPlaceItemInItem::EffectPlaceItemInItem(const std::string& effectDescription, Item * item, Item * container)
	: ActionEffect(effectDescription), m_item(item), m_container(container)
{
	assert(m_item && m_container);
}

EffectPlaceItemInItem::~EffectPlaceItemInItem()
{
}

void EffectPlaceItemInItem::doEffect()
{
	bool success = m_item->setParent(m_container);
	assert(success);
}
