#include "EntityFactory.h"
#include "Entity.h"
#include "EntityInfo.h"
#include "EntityType.h"
#include "Player.h"
#include "Room.h"
#include "Exit.h"
#include "Item.h"
#include "Interactable.h"
#include "globals.h"
#include <assert.h>


EntityFactory::EntityFactory()
{
}


EntityFactory::~EntityFactory()
{
	if (m_entities.size() > 0)
	{
		close();
	}
}


void EntityFactory::close()
{
	for (Entity* entity : m_entities)
	{
		delete entity;
	}
	m_entities.clear();
	m_entitiesById.clear();
}


Entity * EntityFactory::createEntity(EntityInfo info)
{
	assert(m_entitiesById.count(info.id) == 0);
	Entity * entity = nullptr;
	switch (info.type)
	{
	case EntityType::PLAYER:
	{
		Entity* startingRoom = getEntity(info.player.startingRoomId);
		assert(startingRoom && startingRoom->getType() == EntityType::ROOM);
		entity = new Player(info.id, info.name, info.description, info.player.maxItems, static_cast<Room*>(startingRoom));
		break;
	}
	case EntityType::ITEM:
		entity = new Item(info.id, info.name, info.description, info.item.inspectDescription, info.item.isVisibleInDark, info.item.hasLight);
		break;
	case EntityType::INTERACTABLE:
		entity = new Interactable(info.id, info.name, info.description, info.interactable.inspectDescription, info.interactable.isVisibleInDark);
		break;
	case EntityType::ROOM:
		entity = new Room(info.id, info.name, info.description, info.room.isDark);
		break;
	case EntityType::EXIT:
	{
		Entity* targetRoom = getEntity(info.exit.targetRoomId);
		assert(targetRoom && targetRoom->getType() == EntityType::ROOM);
		entity = new Exit(info.id, info.name, info.description, info.exit.direction, info.exit.isLocked, info.exit.lockedDescription, static_cast<Room*>(targetRoom));
		break; 
	}
	default:
		break;
	}
	if (entity != nullptr)
	{
		if (info.parentId >= 0)
		{
			Entity* parent = getEntity(info.parentId);
			assert(parent);
			bool success = entity->setParent(parent);
			assert(success);
		}
		m_entitiesById[info.id] = entity;
		m_entities.push_back(entity);
	}
	return entity;
}


Entity * EntityFactory::getEntity(const std::string & name)
{
	for (Entity* entity : m_entities)
	{
		if (caselessEquals(name, entity->getName()))
		{
			return entity;
		}
	}

	return nullptr;
}


Entity * EntityFactory::getEntity(int id)
{
	if (m_entitiesById.count(id) != 0)
	{
		return m_entitiesById[id];
	}
	return nullptr;
}