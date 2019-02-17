#ifndef H_EXIT
#define H_EXIT

#include "Entity.h"
#include "EntityFactory.h"
class Room;
enum class Direction;


class Exit :
	public Entity
{
	friend Entity * EntityFactory::createEntity(EntityInfo);
private:
	Exit(int aId, const std::string& aName, const std::string& aDescription, Direction aDirection, bool aIsLocked, const std::string& aLockedDescription, Room* aTargetRoom);
	virtual ~Exit();

public:
	Exit(const Exit& aSource) = delete;
	Exit& operator= (const Exit& aSource) = delete;

	Direction getDirection() const;
	bool isLocked() const;
	std::string getLockedDescription() const;
	Room* getTargetRoom() const;
	void lock();
	void unlock();

private:
	Direction mDirection;
	bool mIsLocked;
	std::string mLockedDescription;
	Room* mTargetRoom = nullptr;

	// Entity overrides
	virtual bool canAddChild(const Entity* aChild) const override;
};


#endif // !H_EXIT
