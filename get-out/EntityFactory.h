#ifndef H_ENTITY_FACTORY
#define H_ENTITY_FACTORY

#include <string>
#include <vector>
#include <map>
class Entity;
struct EntityInfo;


class EntityFactory
{
public:
	EntityFactory();
	~EntityFactory();
	EntityFactory(const EntityFactory& source) = delete;
	EntityFactory& operator=(const EntityFactory& source) = delete;

	void close();

	Entity* createEntity(EntityInfo info);
	Entity* getEntity(const std::string& name);
	Entity* getEntity(int id);

private:
	std::vector<Entity*> m_entities;
	std::map<int, Entity*> m_entitiesById;
};


#endif // !H_ENTITY_FACTORY
