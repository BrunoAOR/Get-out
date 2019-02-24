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
	friend Entity* EntityFactory::createEntity(EntityInfo);
private:
	Room(int aId, const std::string& aName, const std::string& aDescription, bool aIsDark);
	virtual ~Room();

public:
	Room(const Room& aSource) = delete;
	Room& operator=(const Room& aSource) = delete;

	Exit* getExit(Direction aDirection);
	std::string getDescriptionInDarkness() const;
	Entity* getChildInDarkness(const std::string& aEntityName, bool aSearchInChildren = false) const;

	// Entity overrides
	virtual std::string getDescription() const override;

private:
	bool mIsDark;
	std::vector< Exit* > mExits;

	// Entity overrides
	virtual bool canAddChild(const Entity* aChild) const override;
	virtual void addChild(Entity* aChild) override;
	virtual void removeChild(const Entity* aEntity) override;
};


#endif // !H_ROOM
