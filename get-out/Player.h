#ifndef H_PLAYER
#define H_PLAYER

#include "Entity.h"
#include "EntityFactory.h"
class ActionFactory;
class Room;
struct EntityInfo;
struct Instruction;


class Player :
	public Entity
{
	friend Entity* EntityFactory::createEntity(EntityInfo);
private:
	Player(int id, const std::string& name, const std::string& description, int maxItems, Room* startingRoom);
	virtual ~Player();

public:
	Player(const Player& source) = delete;
	Player& operator=(const Player& source) = delete;

	void setActionFactory(ActionFactory* actionFactory);
	void executeInstruction(const Instruction& instruction);

protected:
	// Entity overrides
	virtual bool canAddChild(const Entity* child) const override;
	virtual void addChild(Entity* child) override;
	virtual void removeChild(const Entity* entity) override;

private:
	Room* m_location = nullptr;
	int m_maxItems = -1;
	bool m_hasLight = false;
	ActionFactory* m_actionFactory = nullptr;
	
	// Instructions:
	void look() const;
	void inventory() const;
	bool go(const Instruction& instruction);
	bool take(const Instruction& instruction);
	bool drop(const Instruction& instruction);
	bool inspect(const Instruction& instruction) const;
	bool open(const Instruction& instruction) const;
	bool use(const Instruction& instruction);
	bool put(const Instruction& instruction);

	// Helper methods
	void updateLightStatus();
};


#endif // !H_PLAYER
