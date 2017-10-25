#ifndef H_EFFECT_CONSUME_ITEM
#define H_EFFECT_CONSUME_ITEM

#include "ActionEffect.h"
class Item;


class EffectConsumeItem :
	public ActionEffect
{
public:
	EffectConsumeItem(const std::string& effectDescription, Item* itemToConsume);
	~EffectConsumeItem();

	// Inherited via ActionEffect
	virtual void doEffect() override;

private:
	Item* m_item;
};


#endif // !H_EFFECT_CONSUME_ITEM
