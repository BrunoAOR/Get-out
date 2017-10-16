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
	virtual std::string getDescription() const override;

private:
	bool m_isDark;
	std::vector<Exit*> m_exits;

	virtual bool canAddChild(Entity* child) override;
	virtual void addChild(Entity* child) override;
	virtual void removeChild(Entity* entity) override;
};


#endif // !H_ROOM
