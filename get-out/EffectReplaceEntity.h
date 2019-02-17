#ifndef H_EFFECT_REPLACE_ENTITY
#define H_EFFECT_REPLACE_ENTITY

#include "ActionEffect.h"
class Entity;


class EffectReplaceEntity :
	public ActionEffect
{
public:
	EffectReplaceEntity(const std::string& aEffectDescription, Entity* aEntityToRemove, Entity* aEntityToAdd);
	~EffectReplaceEntity();

	// Inherited via ActionEffect
	virtual void doEffect() const override;

private:
	Entity* mEntityToRemove = nullptr;
	Entity* mEntityToAdd = nullptr;
};


#endif // !H_EFFECT_REPLACE_ENTITY
