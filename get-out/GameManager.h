#ifndef H_GAME_MANAGER
#define H_GAME_MANAGER

#include <string>
#include "globals.h"
class ActionFactory;
class EntityFactory;
class InputParser;
class InputReader;
class Player;
enum class LoopStatus;


class GameManager
{
public:
	GameManager();
	~GameManager();
	GameManager(const GameManager& source) = delete;
	GameManager& operator= (const GameManager& source) = delete;

	LoopStatus init();
	LoopStatus update();
	LoopStatus close();

private:
	Player* player = nullptr;
	InputReader * m_inputReader = nullptr;
	InputParser* m_inputParser = nullptr;
	EntityFactory* m_entityFactory = nullptr;
	ActionFactory* m_actionFactory = nullptr;
	LoopStatus m_loopStatus = LoopStatus::CONTINUE;

	LoopStatus processInput(const std::string& userInput);
	void logHelpMessage() const;
};

#endif // !H_GAME_MANAGER
