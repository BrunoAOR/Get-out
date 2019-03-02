#ifndef H_ENTITY_INFO
#define H_ENTITY_INFO

#include <string>
enum class EntityType;
enum class Direction;


struct EntityInfo
{
public:
	static EntityInfo createPlayerInfo(int aId, EntityType aType, int aParentId, const std::string& aName, const std::string& aDescription, int aMaxItems, int aStartingRoomId);
	static EntityInfo createRoomInfo(int aId, EntityType aType, int aParentId, const std::string& aName, const std::string& aDescription, bool aIsDark);
	static EntityInfo createExitInfo(int aId, EntityType aType, int aParentId, const std::string& aName, const std::string& aDescription, Direction aDirection, bool aIsLocked, const std::string& aLockedDescription, int aTargetRoomId);
	static EntityInfo createItemInfo(int aId, EntityType aType, int aParentId, const std::string& aName, const std::string& aDescription, const std::string& aInspectDescription, bool aIsVisibleInDark, bool aHasLight = false);
	static EntityInfo createInteractableInfo(int aId, EntityType aType, int aParentId, const std::string& aName, const std::string& aDescription, const std::string& aInspectDescription, bool aIsVisibleInDark);
	
	EntityInfo();
	bool isValid() const;

private:
	EntityInfo(int aId, EntityType aType, int aParentId, const std::string& aName, const std::string& aDescription);

public:
	int mId;
	EntityType mType;
	int mParentId;
	std::string mName;
	std::string mDescription;
	std::string mAditionalDescription;
	union
	{
		struct
		{
			int mMaxItems;
			int mStartingRoomId;
		} mPlayer;
		struct
		{
			bool mIsDark;
		} mRoom;
		struct
		{
			Direction mDirection;
			bool mIsLocked;
			int mTargetRoomId;
		} mExit;
		struct
		{
			bool mIsVisibleInDark;
			bool mHasLight;
		} mItem;
		struct
		{
			bool mIsVisibleInDark;
		} mInteractable;
	};
};


#endif // !H_ENTITY_INFO
