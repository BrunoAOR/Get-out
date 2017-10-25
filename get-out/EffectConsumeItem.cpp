#include "EffectConsumeItem.h"
#include "Item.h"
#include <assert.h>


EffectConsumeItem::EffectConsumeItem(const std::string& effectDescription, Item * itemToConsume)
	: ActionEffect(effectDescription), m_item(itemToConsume)
{
	assert(m_item);
}


EffectConsumeItem::~EffectConsumeItem()
{
}


void EffectConsumeItem::doEffect()
{
	m_item->setParent(nullptr);
}
