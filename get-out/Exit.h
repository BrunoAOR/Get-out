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
	Exit(int id, const std::string& name, const std::string& description, Direction direction, bool isLocked, const std::string& lockedDescription, Room* targetRoom);
	virtual ~Exit();

public:
	Exit(const Exit& source) = delete;
	Exit& operator= (const Exit& source) = delete;

	Direction getDirection() const;
	bool isLocked() const;
	std::string getLockedDescription() const;
	Room* getTargetRoom() const;
	void lock();
	void unlock();

private:
	Direction m_direction;
	bool m_isLocked;
	std::string m_lockedDescription;
	Room* m_targetRoom = nullptr;

	// Entity overrides
	virtual bool canAddChild(const Entity* child) const override;
};


#endif // !H_EXIT
