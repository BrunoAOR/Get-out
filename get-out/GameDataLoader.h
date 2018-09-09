#ifndef H_GAME_DATA_LOADER
#define H_GAME_DATA_LOADER

#include <string>
#include "json_fwd.hpp"
class ActionFactory;
class EntityFactory;
class Player;

typedef nlohmann::json Json;


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
	Json* loadJson(const char* path);
	bool loadMessages(Json* json);
	Player* hardcodedMethod(EntityFactory* entityFactory, ActionFactory* actionFactory);

	std::string welcomeMessage;
	std::string gameEndMessage;
	std::string exitMessage;
};


#endif // !H_GAME_DATA_LOADER
