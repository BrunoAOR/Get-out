#include "EntityFactory.h"

#include <assert.h>
#include "Entity.h"
#include "EntityInfo.h"
#include "EntityType.h"
#include "Player.h"
#include "Room.h"
#include "Exit.h"
#include "Item.h"
#include "Interactable.h"
#include "globals.h"


EntityFactory::EntityFactory()
{
}


EntityFactory::~EntityFactory()
{
}


void EntityFactory::close()
{
	for (auto lEntry : mEntitiesById)
	{
		delete lEntry.second;
	}
	mEntitiesById.clear();
}


Entity* EntityFactory::createEntity(EntityInfo aInfo)
{
	assert(mEntitiesById.count(aInfo.mId) == 0);
	Entity* lEntity = nullptr;
	switch (aInfo.mType)
	{
	case EntityType::PLAYER:
	{
		Entity* lStartingRoom = getEntity(aInfo.mPlayer.mStartingRoomId);
		assert(lStartingRoom && lStartingRoom->getType() == EntityType::ROOM);
		lEntity = new Player(aInfo.mId, aInfo.mName, aInfo.mDescription, aInfo.mPlayer.mMaxItems, static_cast<Room*>(lStartingRoom));
		break;
	}
	case EntityType::ITEM:
		lEntity = new Item(aInfo.mId, aInfo.mName, aInfo.mDescription, aInfo.mAditionalDescription, aInfo.mItem.mIsVisibleInDark, aInfo.mItem.mHasLight);
		break;
	case EntityType::INTERACTABLE:
		lEntity = new Interactable(aInfo.mId, aInfo.mName, aInfo.mDescription, aInfo.mAditionalDescription, aInfo.mInteractable.mIsVisibleInDark);
		break;
	case EntityType::ROOM:
		lEntity = new Room(aInfo.mId, aInfo.mName, aInfo.mDescription, aInfo.mRoom.mIsDark);
		break;
	case EntityType::EXIT:
	{
		Entity* lTargetRoom = getEntity(aInfo.mExit.mTargetRoomId);
		assert(lTargetRoom && lTargetRoom->getType() == EntityType::ROOM);
		lEntity = new Exit(aInfo.mId, aInfo.mName, aInfo.mDescription, aInfo.mExit.mDirection, aInfo.mExit.mIsLocked, aInfo.mAditionalDescription, static_cast<Room*>(lTargetRoom));
		break;
	}

	}
	if (lEntity != nullptr)
	{
		if (aInfo.mParentId >= 0)
		{
			Entity* lParent = getEntity(aInfo.mParentId);
			assert(lParent);
			bool lSuccess = lEntity->setParent(lParent);
			assert(lSuccess);
		}
		mEntitiesById[aInfo.mId] = lEntity;
	}
	return lEntity;
}


Entity* EntityFactory::getEntity(int aId) const
{
	if (mEntitiesById.count(aId) != 0)
	{
		return mEntitiesById.at(aId);
	}
	return nullptr;
}
