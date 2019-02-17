#ifndef H_ENTITY_FACTORY
#define H_ENTITY_FACTORY

#include <map>
class Entity;
struct EntityInfo;


class EntityFactory
{
public:
	EntityFactory();
	~EntityFactory();
	EntityFactory(const EntityFactory& aSource) = delete;
	EntityFactory& operator=(const EntityFactory& aSource) = delete;

	void close();

	Entity* createEntity(EntityInfo aInfo);
	Entity* getEntity(int aId) const;

private:
	std::map<int, Entity*> mEntitiesById;
};


#endif // !H_ENTITY_FACTORY
