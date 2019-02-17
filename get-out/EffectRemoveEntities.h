#ifndef H_EFFECT_REMOVE_ENTITIES
#define H_EFFECT_REMOVE_ENTITIES

#include "ActionEffect.h"
#include <vector>
class Entity;


class EffectRemoveEntities :
	public ActionEffect
{
public:
	EffectRemoveEntities(const std::string& aEffectDescription, const std::vector<Entity*>& aEntitiesToRemove);
	~EffectRemoveEntities();

	// Inherited via ActionEffect
	virtual void doEffect() const override;

private:
	std::vector<Entity*> mEntitiesToRemove;
};


#endif // !H_EFFECT_REMOVE_ENTITIES
