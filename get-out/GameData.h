#ifndef H_GAME_DATA
#define H_GAME_DATA

#include "globals.h"
#include <vector>
struct ActionInfo;
struct EntityInfo;

struct GameData
{
public:
	using entity_iterator = std::vector< EntityInfo >::const_iterator;
	using action_iterator = std::vector< ActionInfo >::const_iterator;

	void LoadFromJson(const char* aPath);

	entity_iterator EntitiesBegin() { return mEntityInfos.begin(); }
	entity_iterator EntitiesEnd() { return mEntityInfos.end(); }
	action_iterator ActionsBegin() { return mActionInfos.begin(); }
	action_iterator ActionsEnd() { return mActionInfos.end(); }

private:
	std::vector< EntityInfo > mEntityInfos;
	std::vector< ActionInfo > mActionInfos;
};


#endif // !H_GAME_DATA
