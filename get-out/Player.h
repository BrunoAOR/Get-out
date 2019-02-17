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
	Player(int aId, const std::string& aName, const std::string& aDescription, int aMaxItems, Room* aStartingRoom);
	virtual ~Player();

public:
	Player(const Player& aSource) = delete;
	Player& operator=(const Player& aSource) = delete;

	void setActionFactory(ActionFactory* aActionFactory);
	void executeInstruction(const Instruction& aInstruction);

protected:
	// Entity overrides
	virtual bool canAddChild(const Entity* aChild) const override;
	virtual void addChild(Entity* aChild) override;
	virtual void removeChild(const Entity* aEntity) override;

private:
	Room* mLocation = nullptr;
	int mMaxItems = -1;
	bool mHasLight = false;
	ActionFactory* mActionFactory = nullptr;
	
	// Instructions:
	void look() const;
	void inventory() const;
	bool go(const Instruction& aInstruction);
	bool take(const Instruction& aInstruction);
	bool drop(const Instruction& aInstruction);
	bool inspect(const Instruction& aInstruction) const;
	bool open(const Instruction& aInstruction) const;
	bool use(const Instruction& aInstruction);
	bool put(const Instruction& aInstruction);

	// Helper methods
	void updateLightStatus();
};


#endif // !H_PLAYER
