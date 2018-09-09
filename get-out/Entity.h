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
	Entity(int id, EntityType type, const std::string& name, const std::string& description, bool isVisibleInDark);
	virtual ~Entity() = 0;

public:
	Entity(const Entity& source) = delete;
	Entity& operator=(const Entity& source) = delete;

	// Getters and setters
	EntityType getType() const;
	const std::string& getName() const;
	virtual std::string getDescription() const;
	bool isVisibleInDark() const;
	virtual std::string getDetailedDescription() const;

	bool hasChild(Entity* entity) const;
	Entity* getChild(const std::string& entityName, bool searchInChildren=false) const;
	Entity* getParent() const;
	bool setParent(Entity* parent);

protected:
	int m_id;
	EntityType m_type;
	std::string m_name;
	std::string m_description;
	bool m_isVisibleInDark;
	Entity* m_parent = nullptr;
	std::vector<Entity*> m_children;

	virtual bool canAddChild(const Entity* child) const;
	virtual void addChild(Entity* child);
	virtual void removeChild(const Entity* entity);
};


#endif // !H_ENTITY
