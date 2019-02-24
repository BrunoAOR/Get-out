#ifndef H_ENTITY
#define H_ENTITY

#include <string>
#include <vector>
#include "EntityFactory.h"
enum class EntityType;


class Entity
{
	friend void EntityFactory::close();
protected:
	Entity(int aId, EntityType aType, const std::string& aName, const std::string& aDescription, bool aIsVisibleInDark);
	virtual ~Entity() = 0;

public:
	Entity(const Entity& aSource) = delete;
	Entity& operator=(const Entity& aSource) = delete;

	// Getters and setters
	EntityType getType() const;
	const std::string& getName() const;
	virtual std::string getDescription() const;
	bool isVisibleInDark() const;
	virtual std::string getDetailedDescription() const;

	bool hasChild(Entity* aEntity) const;
	Entity* getChild(const std::string& aEntityName, bool aSearchInChildren=false) const;
	Entity* getParent() const;
	bool setParent(Entity* aParent);

protected:
	int mId;
	EntityType mType;
	std::string mName;
	std::string mDescription;
	bool mIsVisibleInDark;
	Entity* mParent = nullptr;
	std::vector< Entity* > mChildren;

	virtual bool canAddChild(const Entity* aChild) const;
	virtual void addChild(Entity* aChild);
	virtual void removeChild(const Entity* aEntity);
};


#endif // !H_ENTITY
