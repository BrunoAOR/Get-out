#ifndef H_EFFECT_ADD_ENTITIES_TO_ROOM
#define H_EFFECT_ADD_ENTITIES_TO_ROOM

#include "ActionEffect.h"
#include <vector>
class Entity;
class Room;


class EffectAddEntitiesToRoom :
	public ActionEffect
{
public:
	EffectAddEntitiesToRoom(const std::string& aEffectDescription, const std::vector<Entity*>& aEntitiesToAdd, Room* aTargetRoom);
	~EffectAddEntitiesToRoom();

	// Inherited via ActionEffect
	virtual void doEffect() const override;

private:
	std::vector<Entity*> mEntitiesToAdd;
	Room* mTargetRoom = nullptr;
};


#endif // !H_EFFECT_ADD_ENTITIES_TO_ROOM
