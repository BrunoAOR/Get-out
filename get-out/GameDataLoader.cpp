#include "GameDataLoader.h"

#include "globals.h"
#include "ActionEffect.h"
#include "ActionEffectType.h"
#include "ActionFactory.h"
#include "ActionType.h"
#include "Direction.h"
#include "EffectAddEntitiesToRoom.h"
#include "EffectGameEnd.h"
#include "EffectLockExit.h"
#include "EffectPlaceItemInItem.h"
#include "EffectRemoveEntities.h"
#include "EffectReplaceEntity.h"
#include "EffectUnlockExit.h"
#include "EntityFactory.h"
#include "EntityInfo.h"
#include "EntityType.h"
#include "Exit.h"
#include "Item.h"
#include "Player.h"
#include "Room.h"

#include "ActionEffectInfo.h"
#include "ActionEffectInfoIO.h"
#include "AddEntitiesInfoIO.h"
#include "GameEndInfoIO.h"
#include "LockExitInfoIO.h"
#include "PlaceItemInItemInfoIO.h"
#include "RemoveEntitiesInfoIO.h"
#include "ReplaceEntityInfoIO.h"
#include "UnlockExitInfoIO.h"
#include "ActionInfo.h"
#include "GameData.h"


namespace
{
	ActionEffect* loadActionEffect(const ActionEffectInfo& aInfo, EntityFactory* aEntityFactory, int aActionIndex, int aEffectIndex)
	{
		ActionEffectType lEffectType = ActionEffectInfoIO::getType(aInfo);
		ASSERT(lEffectType != ActionEffectType::_UNDEFINED);

		ActionEffect* lActionEffect = nullptr;

		// Now we can switch over lEffectType and load effects appropriately
		switch (lEffectType)
		{

		case ActionEffectType::ADD_ENTITIES:
		{
			AddEntitiesInfoIO lInfoReader(aInfo);

			bool lSuccess = true;

			std::vector< Entity* > lEntitiesToAdd;
			size_t lIdx = 0;
			for (ActionEffectInfo::entities_iterator lIt = lInfoReader.entitiesToAddBegin(), lEnd = lInfoReader.entitiesToAddEnd(); lIt != lEnd; ++lIt, ++lIdx)
			{
				Entity* lEntity = aEntityFactory->getEntity(*lIt);
				if (lEntity)
				{
					lEntitiesToAdd.push_back(lEntity);
				}
				else
				{
					OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'entitiesToAdd' at index %i that doesn't point to any existing Entity!", aEffectIndex, aActionIndex, lIdx);
					lSuccess = false;
				}
			}

			Room* lTargetRoom = static_cast<Room*>(aEntityFactory->getEntity(lInfoReader.getTargetRoomId()));
			if (!lTargetRoom)
			{
				OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'targetRoomId' that doesn't point to any existing Room!", aEffectIndex, aActionIndex);
				lSuccess = false;
			}

			if (lSuccess)
			{
				// We continue
				lActionEffect = new EffectAddEntitiesToRoom(lInfoReader.getDescription(), lEntitiesToAdd, lTargetRoom);
			}
			break;
		}

		case ActionEffectType::GAME_END:
		{
			GameEndInfoIO lInfoReader(aInfo);

			lActionEffect = new EffectGameEnd(lInfoReader.getDescription());
			break;
		}

		case ActionEffectType::LOCK_EXIT:
		{
			LockExitInfoIO lInfoReader(aInfo);

			Exit* lExitToLock = static_cast<Exit*>(aEntityFactory->getEntity(lInfoReader.getExitToLockId()));
			if (lExitToLock)
			{
				lActionEffect = new EffectLockExit(lInfoReader.getDescription(), lExitToLock);
			}
			else
			{
				OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'exitToLockId' that does't point to any existing Exit!", aEffectIndex, aActionIndex);
			}
			break;
		}

		case ActionEffectType::PLACE_ITEM_IN_ITEM:
		{
			PlaceItemInItemInfoIO lInfoReader(aInfo);

			Item* lItem = static_cast<Item*>(aEntityFactory->getEntity(lInfoReader.getItemId()));
			Item* lContainer = static_cast<Item*>(aEntityFactory->getEntity(lInfoReader.getContainerId()));
			if (lItem && lContainer)
			{
				lActionEffect = new EffectPlaceItemInItem(lInfoReader.getDescription(), lItem, lContainer);
			}
			else
			{
				if (!lItem)
				{
					OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'itemId' that doesn't point to any existing Entity!", aEffectIndex, aActionIndex);
				}
				if (!lContainer)
				{
					OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'containerId' that doesn't point to any existing Entity!", aEffectIndex, aActionIndex);
				}
			}
			break;
		}

		case ActionEffectType::REMOVE_ENTITIES:
		{
			RemoveEntitiesInfoIO lInfoReader(aInfo);

			bool lSuccess = true;

			std::vector< Entity* > lEntitiesToRemove;
			size_t lIdx = 0;
			for (ActionEffectInfo::entities_iterator lIt = lInfoReader.entitiesToRemoveBegin(), lEnd = lInfoReader.entitiesToRemoveEnd(); lIt != lEnd; ++lIt, ++lIdx)
			{
				Entity* lEntity = aEntityFactory->getEntity(*lIt);
				if (lEntity)
				{
					lEntitiesToRemove.push_back(lEntity);
				}
				else
				{
					OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'entitiesToAdd' at index %i that does't point to any existing Entity!", aEffectIndex, aActionIndex, lIdx);
					lSuccess = false;
				}
			}

			if (lSuccess)
			{
				// We continue
				lActionEffect = new EffectRemoveEntities(lInfoReader.getDescription(), lEntitiesToRemove);
			}
			break;
		}

		case ActionEffectType::REPLACE_ENTITY:
		{
			ReplceEntityInfoIO lInfoReader(aInfo);

			Entity* lEntityToRemove = aEntityFactory->getEntity(lInfoReader.getEntityToRemoveId());
			Entity* lEntityToAdd = aEntityFactory->getEntity(lInfoReader.getEntityToAddId());
			if (lEntityToRemove && lEntityToAdd)
			{
				lActionEffect = new EffectReplaceEntity(lInfoReader.getDescription(), lEntityToRemove, lEntityToAdd);
			}
			else
			{
				if (!lEntityToRemove)
				{
					OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'entityToRemoveId' that doesn't point to any existing Entity!", aEffectIndex, aActionIndex);
				}
				if (!lEntityToAdd)
				{
					OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'entityToAdd' that doesn't point to any existing Entity!", aEffectIndex, aActionIndex);
				}
			}
			break;
		}

		case ActionEffectType::UNLOCK_EXIT:
		{
			UnlockExitInfoIO lInfoReader(aInfo);

			Exit* lExitToUnlock = static_cast<Exit*>(aEntityFactory->getEntity(lInfoReader.getExitToUnlockId()));
			if (lExitToUnlock)
			{
				lActionEffect = new EffectUnlockExit(lInfoReader.getDescription(), lExitToUnlock);
			}
			else
			{
				OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'exitToUnlockId' that does't point to any existing Exit!", aEffectIndex, aActionIndex);
			}
			break;
		}

		}// switch (lEffectType)

		return lActionEffect;
	}
}// anonymous


GameDataLoader::GameDataLoader(EntityFactory* aEntityFactory, ActionFactory* aActionFactory)
	:mEntityFactory(aEntityFactory), mActionFactory(aActionFactory)
{
	ASSERT(mEntityFactory && mActionFactory);
}


GameDataLoader::~GameDataLoader()
{
}


Player* GameDataLoader::loadGameData(const GameData& aGameData)
{
	ASSERT(aGameData.isValid());
	bool lSuccess = true;
	Player* lPlayer = nullptr;

	// Messages
	mWelcomeMessage = aGameData.getWelcomeMessage();
	mExitMessage = aGameData.getExitMessage();

	// Entities (not player)
	for (GameData::entity_iterator lIt = aGameData.entitiesBegin(), lEnd = aGameData.entitiesEnd(); lIt != lEnd; ++lIt)
	{
		mEntityFactory->createEntity(*lIt);
	}

	// Player entity
	lPlayer = static_cast<Player*>(mEntityFactory->createEntity(aGameData.getPlayerInfo()));

	// Actions
	size_t lActionIndex = 0;
	for (GameData::action_iterator lIt = aGameData.actionsBegin(), lEnd = aGameData.actionsEnd(); lIt != lEnd; ++lIt, ++lActionIndex)
	{
		const ActionInfo& lActionInfo = *lIt;
		std::vector< ActionEffect* > lActionEffects;

		size_t lActionEffectIndex = 0;
		for (ActionInfo::actionEffects_iterator lIt2 = lActionInfo.actionEffectInfosBegin(), lEnd2 = lActionInfo.actionEffectInfosEnd(); lIt2 != lEnd2; ++lIt2, ++lActionEffectIndex)
		{
			ActionEffect* lActionEffect = ::loadActionEffect(*lIt2, mEntityFactory, lActionIndex, lActionEffectIndex);
			if (lActionEffect)
			{
				lActionEffects.push_back(lActionEffect);
			}
			else
			{
				OUTPUT_LOG("ERROR: Failed to load ActionEffect at index %i in Action at index %i!", lActionEffectIndex, lActionIndex);
				lSuccess = false;
			}
		}

		if (lSuccess)
		{
			mActionFactory->createAction(lActionInfo.mType, lActionInfo.mDescription, lActionEffects, lActionInfo.mShouldDestroy, lActionInfo.mFirstEntity, lActionInfo.mSecondEntity);
		}
	}

	if (!lSuccess)
	{
		lPlayer = nullptr;
		mEntityFactory->close();
		mActionFactory->close();
	}

	return lPlayer;
}


const std::string& GameDataLoader::getWelcomeMessage() const
{
	return mWelcomeMessage;
}


const std::string& GameDataLoader::getExitMessage() const
{
	return mExitMessage;
}
