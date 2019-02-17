#ifndef H_GAME_DATA_LOADER
#define H_GAME_DATA_LOADER

#include <string>
#include "json_fwd.hpp"
class ActionEffect;
class ActionFactory;
class EntityFactory;
class Player;
struct EntityInfo;
enum class ActionEffectType;

using Json = nlohmann::json;

class GameDataLoader
{
public:
	GameDataLoader(EntityFactory* aEntityFactory, ActionFactory* aActionFactory);
	~GameDataLoader();
	GameDataLoader(const GameDataLoader& source) = delete;
	GameDataLoader operator=(const GameDataLoader& source) = delete;

	Player* loadGameData(const char* aPath);
	const std::string& getWelcomeMessage() const;
	const std::string& getExitMessage() const;

private:
	using entityLoaderFunc = bool(GameDataLoader::*)(const Json&, std::vector<EntityInfo>&);

	Json loadJson(const char* aPath);
	bool loadMessages(const Json& aJson);
	
	Player* loadAndCreateEntities(const Json& aJson);
	bool loadEntitiesByKey(const Json& aJonEntityInfos, std::vector<EntityInfo>& aInOutEntityInfos, const std::string& aKey, entityLoaderFunc aLoaderfunc);
	bool loadRoomInfos(const Json& aJsonRooms, std::vector<EntityInfo>& aInOutEntityInfos);
	bool loadExitInfos(const Json& aJsonExits, std::vector<EntityInfo>& aInOutEntityInfos);
	bool loadInteractableInfos(const Json& aJsonInteractables, std::vector<EntityInfo>& aInOutEntityInfos);
	bool loadItemInfos(const Json& aJsonItems, std::vector<EntityInfo>& aInOutEntityInfos);

	bool loadAndCreateActions(const Json& aJsonActions);
	bool loadAction(const Json& aJsonAction, int aActionIndex);
	bool loadActionEffects(const Json& aJsonEffects, std::vector<ActionEffect*>& aInOutEffects, int aActionIndex);
	ActionEffect* loadActionEffect(const Json& aJsonEffect, ActionEffectType aEffectType, int aActionIndex, int aEffectIndex);

	std::string mWelcomeMessage;
	std::string mExitMessage;
	EntityFactory* mEntityFactory = nullptr;
	ActionFactory* mActionFactory = nullptr;
};


#endif // !H_GAME_DATA_LOADER
