#ifndef H_GAME_DATA_LOADER
#define H_GAME_DATA_LOADER

#include <string>
#include "json_fwd.hpp"
class ActionFactory;
class EntityFactory;
class Player;
struct EntityInfo;

using Json = nlohmann::json;

class GameDataLoader
{
public:
	GameDataLoader();
	~GameDataLoader();
	GameDataLoader(const GameDataLoader& source) = delete;
	GameDataLoader operator=(const GameDataLoader& source) = delete;

	Player* loadGameData(EntityFactory* entityFactory, ActionFactory* actionFactory);
	const std::string& getWelcomeMessage();
	const std::string& getGameEndMessage();
	const std::string& getExitMessage();

private:
	using entityLoaderFunc = bool(GameDataLoader::*)(const Json&, std::vector<EntityInfo>&);

	Json loadJson(const char* path);
	bool loadMessages(const Json& json);
	Player* loadAndCreateEntities(const Json& json, EntityFactory* entityFactory);
	bool loadEntitiesByKey(const Json& jsonEntityInfos, std::vector<EntityInfo>& entityInfos, const std::string& key, entityLoaderFunc loaderfunc);
	bool loadRoomInfos(const Json& jsonRooms, std::vector<EntityInfo>& entityInfos);
	bool loadExitInfos(const Json& jsonExits, std::vector<EntityInfo>& entityInfos);
	bool loadInteractableInfos(const Json& jsonInteractables, std::vector<EntityInfo>& entityInfos);
	bool loadItemInfos(const Json& jsonItems, std::vector<EntityInfo>& entityInfos);
	void hardcodedMethod(EntityFactory* entityFactory, ActionFactory* actionFactory);

	std::string welcomeMessage;
	std::string gameEndMessage;
	std::string exitMessage;
};


#endif // !H_GAME_DATA_LOADER
