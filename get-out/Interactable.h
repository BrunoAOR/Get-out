#ifndef H_INTERACTABLE
#define H_INTERACTABLE

#include "Entity.h"
#include "EntityFactory.h"


class Interactable :
	public Entity
{
	friend Entity* EntityFactory::createEntity(EntityInfo);
private:
	Interactable(int aId, const std::string& aName, const std::string& aDescription, const std::string& aInspectDescription, bool aIsVisibleInDark = false);
	virtual ~Interactable();

public:
	Interactable(const Interactable& aSource) = delete;
	Interactable& operator=(const Interactable& aSource) = delete;

	// Entity overrides
	virtual std::string getDetailedDescription() const override;

private:
	std::string mInspectDescription;
	bool mIsVisibleInDark;

	// Entity overrides
	virtual bool canAddChild(const Entity* aChild) const override;
};


#endif // !H_INTERACTABLE
