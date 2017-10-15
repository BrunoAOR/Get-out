#ifndef H_ROOM
#define H_ROOM

#include "Entity.h"
class Exit;
class Item;
class Interactable;
enum class Direction;


class Room :
	public Entity
{
public:
	Room(EntityType type, std::string name, std::string description, bool isDark);
	~Room();

	Exit* getExit(Direction direction);
	bool hasItem(Item* item);
	bool hasInteractable(Interactable* Interactable);
	virtual std::string getDescription() const override;

private:
	bool m_isDark;

	virtual bool canAddChild(Entity* child) override;
};


#endif // !H_ROOM
