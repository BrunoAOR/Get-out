#ifndef H_WORLD_CONFIG
#define H_WORLD_CONFIG

#include <string>
class Player;
class EntityFactory;
class ActionFactory;


Player* setUpWorld(EntityFactory* entityFactory, ActionFactory* actionFactory);
std::string getWelcomeMessage();
std::string getGameEndMessage();


#endif // !H_WORLD_CONFIG
