#ifndef H_EXIT
#define H_EXIT

#include "Entity.h"
#include "EntityFactory.h"
enum class Direction;
class Room;

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
	void unlock();

private:
	Direction m_direction;
	bool m_isLocked;
	std::string m_lockedDescription;
	Room* m_targetRoom = nullptr;

	virtual bool canAddChild(Entity* child) override;
};


#endif // !H_EXIT
