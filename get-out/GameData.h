#ifndef H_GAME_DATA
#define H_GAME_DATA

#include "globals.h"
#include <vector>
#include "EntityInfo.h"
struct ActionInfo;

struct GameData
{
public:
	using entity_iterator = std::vector< EntityInfo >::const_iterator;
	using action_iterator = std::vector< ActionInfo >::const_iterator;

	void LoadFromJson(const char* aPath);

	const std::string& getWelcomeMessage() const { return mWelcomeMessage; }
	const std::string& getExitMessage() const { return mExitMessage; }
	
	const EntityInfo& getPlayerInfo() const { return mPlayerInfo; }
	entity_iterator entitiesBegin() const { return mEntityInfos.begin(); }
	entity_iterator entitiesEnd() const { return mEntityInfos.end(); }
	action_iterator actionsBegin() const { return mActionInfos.begin(); }
	action_iterator actionsEnd() const { return mActionInfos.end(); }

private:
	EntityInfo mPlayerInfo;
	std::vector< EntityInfo > mEntityInfos;
	std::vector< ActionInfo > mActionInfos;
	std::string mWelcomeMessage;
	std::string mExitMessage;
};


#endif // !H_GAME_DATA
