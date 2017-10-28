#ifndef H_INTERACTABLE
#define H_INTERACTABLE

#include "Entity.h"
#include "EntityFactory.h"


class Interactable :
	public Entity
{
	friend Entity * EntityFactory::createEntity(EntityInfo);
private:
	Interactable(int id, const std::string& name, const std::string& description, const std::string& inspectDescription, bool isVisibleInDark = false);
	virtual ~Interactable();

public:
	Interactable(const Interactable& source) = delete;
	Interactable& operator=(const Interactable& source) = delete;

	// Entity overrides
	virtual std::string getDetailedDescription() const override;

private:
	std::string m_inspectDescription;
	bool m_isVisibleInDark;

	// Entity overrides
	virtual bool canAddChild(const Entity* child) const override;
};


#endif // !H_INTERACTABLE
