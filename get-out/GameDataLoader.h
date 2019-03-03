#ifndef H_GAME_DATA_LOADER
#define H_GAME_DATA_LOADER

#include <string>
#include "json_fwd.hpp"
struct GameData;

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

	Player* loadGameData(const GameData& aGameData);
	const std::string& getWelcomeMessage() const;
	const std::string& getExitMessage() const;

private:
	std::string mWelcomeMessage;
	std::string mExitMessage;
	EntityFactory* mEntityFactory = nullptr;
	ActionFactory* mActionFactory = nullptr;
};


#endif // !H_GAME_DATA_LOADER
