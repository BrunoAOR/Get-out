#ifndef H_ROOM
#define H_ROOM

#include "Entity.h"
#include "EntityFactory.h"
class Exit;
class Item;
class Interactable;
struct EntityInfo;
enum class Direction;


class Room :
	public Entity
{	
	friend Entity * EntityFactory::createEntity(EntityInfo);
private:
	Room(int id, const std::string& name, const std::string& description, bool isDark);
	virtual ~Room();

public:
	Room(const Room& source) = delete;
	Room& operator=(const Room& source) = delete;

	Exit* getExit(Direction direction);
	std::string getDescriptionInDarkness() const;
	Entity* getChildInDarkness(const std::string& entityName, bool searchInChildren = false) const;

	// Entity overrides
	virtual std::string getDescription() const override;

private:
	bool m_isDark;
	std::vector<Exit*> m_exits;

	// Entity overrides
	virtual bool canAddChild(const Entity* child) const override;
	virtual void addChild(Entity* child) override;
	virtual void removeChild(const Entity* entity) override;
};


#endif // !H_ROOM
