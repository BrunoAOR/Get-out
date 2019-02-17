#include "Exit.h"

#include <assert.h>
#include "EntityType.h"


Exit::Exit(int aId, const std::string& aName, const std::string& aDescription, Direction aDirection, bool aIsLocked, const std::string& aLockedDescription, Room* aTargetRoom)
	: Entity(aId, EntityType::EXIT, aName, aDescription, true), mDirection(aDirection), mIsLocked(aIsLocked), mLockedDescription(aLockedDescription), mTargetRoom(aTargetRoom)
{
	assert(mTargetRoom);
}


Exit::~Exit()
{
}


Direction Exit::getDirection() const
{
	return mDirection;
}


bool Exit::isLocked() const
{
	return mIsLocked;
}


std::string Exit::getLockedDescription() const
{
	return mLockedDescription;
}


Room* Exit::getTargetRoom() const
{
	return mTargetRoom;
}


void Exit::lock()
{
	mIsLocked = true;
}


void Exit::unlock()
{
	mIsLocked = false;
}


bool Exit::canAddChild(const Entity* aChild) const
{
	return false;
}
