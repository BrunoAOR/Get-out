#ifndef H_ENTITY
#define H_ENTITY

#include <string>
#include <vector>
enum class EntityType;

class Entity
{
public:
	Entity(EntityType type, std::string name, std::string description);
	virtual ~Entity() = 0;

	virtual void update() {}

	// Getters, setters and list-modification
	EntityType getType() const;
	std::string getName() const;
	virtual std::string getDescription() const;
	virtual std::string getDetailedDescription() const;
	bool hasChild(Entity* entity) const;
	Entity* getChild(const std::string& entityName);
	
	Entity* getParent() const;
	bool setParent(Entity* parent); // Set parent MUST call removeChild on the former parent and addChild on the new parent (provided that they are different)

protected:
	EntityType m_type;
	std::string m_name;
	std::string m_description;
	Entity* m_parent = nullptr;
	std::vector<Entity*> m_children;

private:
	virtual bool canAddChild(Entity* child);
	virtual void addChild(Entity* child);
	virtual void removeChild(Entity* entity);
};


#endif // !H_ENTITY
