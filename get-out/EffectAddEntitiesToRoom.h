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
	EffectAddEntitiesToRoom(std::string effectDescription, std::vector<Entity*> entitiesToAdd, Room* targetRoom);
	~EffectAddEntitiesToRoom();

	// Inherited via ActionEffect
	virtual void doEffect() override;

private:
	std::vector<Entity*> m_entitiesToAdd;
	Room* m_targetRoom = nullptr;
};


#endif // !H_EFFECT_ADD_ENTITIES_TO_ROOM
