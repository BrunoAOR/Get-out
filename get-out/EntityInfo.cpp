#include "EntityInfo.h"


EntityInfo EntityInfo::createPlayerInfo(int aId, EntityType aType, int aParentId, const std::string& aName, const std::string& aDescription, int aMaxItems, int aStartingRoomId)
{
	EntityInfo lInfo{ aId, aType, aParentId, aName, aDescription };
	lInfo.mPlayer.mMaxItems = aMaxItems;
	lInfo.mPlayer.mStartingRoomId = aStartingRoomId;
	return lInfo;
}


EntityInfo EntityInfo::createRoomInfo(int aId, EntityType aType, int aParentId, const std::string& aName, const std::string& aDescription, bool aIsDark)
{
	EntityInfo lInfo{ aId, aType, aParentId, aName, aDescription };
	lInfo.mRoom.mIsDark = aIsDark;
	return lInfo;
}


EntityInfo EntityInfo::createExitInfo(int aId, EntityType aType, int aParentId, const std::string& aName, const std::string& aDescription, Direction aDirection, bool aIsLocked, const std::string& aLockedDescription, int aTargetRoomId)
{
	EntityInfo lInfo{ aId, aType, aParentId, aName, aDescription };
	lInfo.mExit.mDirection = aDirection;
	lInfo.mExit.mIsLocked = aIsLocked;
	lInfo.mAditionalDescription = aLockedDescription;
	lInfo.mExit.mTargetRoomId = aTargetRoomId;
	return lInfo;
}


EntityInfo EntityInfo::createItemInfo(int aId, EntityType aType, int aParentId, const std::string& aName, const std::string& aDescription, const std::string& aInspectDescription, bool aIsVisibleInDark, bool aHasLight)
{
	EntityInfo lInfo{ aId, aType, aParentId, aName, aDescription };
	lInfo.mAditionalDescription = aInspectDescription;
	lInfo.mItem.mIsVisibleInDark = aIsVisibleInDark;
	lInfo.mItem.mHasLight = aHasLight;
	return lInfo;
}


EntityInfo EntityInfo::createInteractableInfo(int aId, EntityType aType, int aParentId, const std::string& aName, const std::string& aDescription, const std::string& aInspectDescription, bool aIsVisibleInDark)
{
	EntityInfo lInfo{ aId, aType, aParentId, aName, aDescription };
	lInfo.mAditionalDescription = aInspectDescription;
	lInfo.mInteractable.mIsVisibleInDark = aIsVisibleInDark;
	return lInfo;
}
