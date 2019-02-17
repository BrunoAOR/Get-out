#ifndef H_GAME_MANAGER
#define H_GAME_MANAGER

#include <string>
#include "globals.h"
class ActionFactory;
class EntityFactory;
class GameDataLoader;
class InputParser;
class InputReader;
class Player;
enum class LoopStatus;


class GameManager
{
public:
	GameManager();
	~GameManager();
	GameManager(const GameManager& aSource) = delete;
	GameManager& operator= (const GameManager& aSource) = delete;

	LoopStatus init();
	LoopStatus update();
	LoopStatus close();

private:
	Player* mPlayer = nullptr;
	InputReader * mInputReader = nullptr;
	InputParser* mInputParser = nullptr;
	EntityFactory* mEntityFactory = nullptr;
	ActionFactory* mActionFactory = nullptr;
	GameDataLoader* mGameDataLoader = nullptr;
	LoopStatus mLoopStatus = LoopStatus::CONTINUE;

	LoopStatus processInput(const std::string& aUserInput);
	void logHelpMessage() const;
};


#endif // !H_GAME_MANAGER
