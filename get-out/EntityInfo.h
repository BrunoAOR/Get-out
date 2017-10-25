#ifndef H_ENTITY_INFO
#define H_ENTITY_INFO

#include <string>
enum class EntityType;
enum class Direction;


struct EntityInfo
{
	static EntityInfo createPlayerInfo(int id, EntityType type, int parentId, const std::string& name, const std::string& description, int maxItems, int startingRoomId);
	static EntityInfo createRoomInfo(int id, EntityType type, int parentId, const std::string& name, const std::string& description, bool isDark);
	static EntityInfo createExitInfo(int id, EntityType type, int parentId, const std::string& name, const std::string& description, Direction direction, bool isLocked, const char* lockedDescription, int targetRoomId);
	static EntityInfo createItemInfo(int id, EntityType type, int parentId, const std::string& name, const std::string& description, const char* inspectDescription, bool isVisibleInDark, bool hasLight = false);
	static EntityInfo createInteractableInfo(int id, EntityType type, int parentId, const std::string& name, const std::string& description, const char* inspectDescription, bool isVisibleInDark);

	int id;
	EntityType type;
	int parentId;
	std::string name;
	std::string description;
	union
	{
		struct
		{
			int maxItems;
			int startingRoomId;
		} player;
		struct
		{
			bool isDark;
		} room;
		struct
		{
			Direction direction;
			bool isLocked;
			const char* lockedDescription;
			int targetRoomId;
		} exit;
		struct
		{
			const char* inspectDescription;
			bool isVisibleInDark;
			bool hasLight;
		} item;
		struct
		{
			const char* inspectDescription;
			bool isVisibleInDark;
		} interactable;
	};
};


#endif // !H_ENTITY_INFO
