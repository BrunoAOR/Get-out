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
	GameDataLoader(EntityFactory* entityFactory, ActionFactory* actionFactory);
	~GameDataLoader();
	GameDataLoader(const GameDataLoader& source) = delete;
	GameDataLoader operator=(const GameDataLoader& source) = delete;

	Player* loadGameData(const char* path);
	const std::string& getWelcomeMessage() const;
	const std::string& getExitMessage() const;

private:
	using entityLoaderFunc = bool(GameDataLoader::*)(const Json&, std::vector<EntityInfo>&);

	Json loadJson(const char* path);
	bool loadMessages(const Json& json);
	
	Player* loadAndCreateEntities(const Json& json);
	bool loadEntitiesByKey(const Json& jsonEntityInfos, std::vector<EntityInfo>& entityInfos, const std::string& key, entityLoaderFunc loaderfunc);
	bool loadRoomInfos(const Json& jsonRooms, std::vector<EntityInfo>& entityInfos);
	bool loadExitInfos(const Json& jsonExits, std::vector<EntityInfo>& entityInfos);
	bool loadInteractableInfos(const Json& jsonInteractables, std::vector<EntityInfo>& entityInfos);
	bool loadItemInfos(const Json& jsonItems, std::vector<EntityInfo>& entityInfos);

	bool loadAndCreateActions(const Json& jsonActions);
	bool loadAction(const Json& jsonAction, int actionIndex);
	bool loadActionEffects(const Json& jsonEffects, std::vector<ActionEffect*>& effects, int actionIndex);
	ActionEffect* loadActionEffect(const Json& jsonEffect, ActionEffectType effectType, int actionIndex, int effectIndex);

	std::string m_welcomeMessage;
	std::string m_exitMessage;
	EntityFactory* m_entityFactory = nullptr;
	ActionFactory* m_actionFactory = nullptr;
};


#endif // !H_GAME_DATA_LOADER
