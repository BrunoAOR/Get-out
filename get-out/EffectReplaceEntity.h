#ifndef H_EFFECT_REPLACE_ENTITY
#define H_EFFECT_REPLACE_ENTITY

#include "ActionEffect.h"
class Entity;


class EffectReplaceEntity :
	public ActionEffect
{
public:
	EffectReplaceEntity(const std::string& effectDescription, Entity* entityToRemove, Entity* entityToAdd);
	~EffectReplaceEntity();

	// Inherited via ActionEffect
	virtual void doEffect() override;

private:
	Entity* m_entityToRemove = nullptr;
	Entity* m_entityToAdd = nullptr;
};



#endif // !H_EFFECT_REPLACE_ENTITY