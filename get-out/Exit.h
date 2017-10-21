#ifndef H_EXIT
#define H_EXIT

#include "Entity.h"
enum class Direction;
class Room;

class Exit :
	public Entity
{
public:
	Exit(std::string name, std::string m_description, Direction direction, bool isLocked, std::string lockedDescription, Room* targetRoom);
	~Exit();

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
