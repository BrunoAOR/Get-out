#ifndef H_GAME_DATA_LOADER
#define H_GAME_DATA_LOADER

#include <string>
class ActionFactory;
class EntityFactory;
class Player;


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
	Player* hardcodedMethod(EntityFactory* entityFactory, ActionFactory* actionFactory);

	std::string welcomeMessage;
	std::string gameEndMessage;
	std::string exitMessage;
};


#endif // !H_GAME_DATA_LOADER
