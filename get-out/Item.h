#ifndef H_ITEM
#define H_ITEM

#include "Entity.h"
#include "EntityFactory.h"


class Item :
	public Entity
{
	friend Entity* EntityFactory::createEntity(EntityInfo);
private:
	Item(int aId, const std::string& aName, const std::string& aDescription, const std::string& aInspectDescription, bool aIsVisibleInDark = false, bool aHasLight = false);
	virtual ~Item();

public:
	Item(const Item& aSource) = delete;
	Item& operator=(const Item& aSource) = delete;

	// Entity overrides
	virtual std::string getDescription() const override;
	virtual std::string getDetailedDescription() const override;

	bool hasLight() const;

private:
	std::string mInspectDescription;
	bool mHasLight;

	// Entity overrides
	virtual bool canAddChild(const Entity* aChild) const override;
};


#endif // !H_ITEM
