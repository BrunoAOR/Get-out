#ifndef H_PLAYER
#define H_PLAYER

#include "Entity.h"
#include "EntityFactory.h"
class Room;
class ActionFactory;
struct EntityInfo;
struct Instruction;


class Player :
	public Entity
{
	friend Entity * EntityFactory::createEntity(EntityInfo);
private:
	Player(int id, const std::string& name, const std::string& description, int maxItems, Room* startingRoom);
	virtual ~Player();

public:
	Player(const Player& source) = delete;
	Player& operator=(const Player& source) = delete;

	void setActionFactory(ActionFactory* actionFactory);
	void executeInstruction(const Instruction& instruction);

private:
	Room* m_location;
	int m_maxItems = -1;
	bool m_hasLight = false;
	ActionFactory* m_actionFactory;

	// Entity overrides
	virtual bool canAddChild(const Entity* child) const override;
	virtual void addChild(Entity* child);
	virtual void removeChild(const Entity* entity);
	
	// Instructions:
	void look();
	void inventory();
	bool go(const Instruction& instruction); // Ask room if there's an exit in that direkol,ction -> Room::getExit
	bool take(const Instruction& instruction); // Must check that type == Item && there’s room in children (inheritted from Entity)
	bool drop(const Instruction& instruction); // Must check that target is in children
	bool inspect(const Instruction& instruction);	// Item or interactable
	bool open(const Instruction& instruction); //Must check type & presence in room
	bool use(const Instruction& instruction); // Must verify that item can be used in target (done by item internally)
	bool put(const Instruction& instruction); // Place an item inside another item (if allowed)

	// Helper methods
	void updateLightStatus();
};


#endif // !H_PLAYER
