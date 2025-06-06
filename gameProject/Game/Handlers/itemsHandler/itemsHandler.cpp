#include "itemsHandler.h"

#include "../../../Utils/utils.h"
#include "../../../Utils/Log/Log.h"

#include "../entitiesHandler/entitiesHandler.h"

#include "../../Managers/collisionManager/collisionManager.h"

void itemsHandler::updateItems( ) {

	std::lock_guard<std::mutex> lock( handlerMutex );
	CPlayerEntity * localPlayer = entitiesHandler::Get( ).getLocalPlayer( );
	if ( localPlayer == nullptr )
		return;

	if ( this->spawnedItems.empty( ) ) {
		return;
	}

	auto it = spawnedItems.begin( );
	while ( it != spawnedItems.end( ) ) {
		CBaseItem * item = it->get( );
		if ( !item || !item->getEntityAnimations( ) ) {
			it = spawnedItems.erase( it );
			continue;
		}

		if ( item->isActive( ) ) {
			if ( CollisionManager::Get( ).checkCollision( item , localPlayer , item->getEntityPosition( ) ) ) {
				item->applyEffect( );
				item->Deactive( );
				// n�o removemos aqui, esperamos que a pr�xima itera��o remova
				++it;
				continue;
			}
		}
		else {
			it = spawnedItems.erase( it );
			continue;
		}

		item->getEntityAnimations( )->updateAnimation( );
		++it;
	}

}

std::vector<std::unique_ptr<CBaseItem>> * itemsHandler::getSpawnedItems( )
{
	std::lock_guard<std::mutex> lock( handlerMutex );
	return &spawnedItems;
}

void itemsHandler::addSpawnableItem( std::unique_ptr<CBaseItem> item )
{
	std::lock_guard<std::mutex> lock( handlerMutex );
	spawnableItems.emplace( item->getItemType( ) , std::move( item ) );
}

void itemsHandler::spawnItem( ItemType itemType ) {
	std::lock_guard<std::mutex> lock( handlerMutex );

	auto it = spawnableItems.find( itemType );
	if ( it == spawnableItems.end( ) )
		return;

	if ( it->second.get( ) != nullptr ) {
		std::unique_ptr<CBaseItem> itemClone = it->second->ItemClone( );
		Log::Print( "Copy complete, trying to active item" );
		itemClone->Active( );
		Log::Print( "Item activated, trying to push to vector" );
		spawnedItems.push_back( std::move( itemClone ) );
	}
}

std::unique_ptr<CBaseItem> * itemsHandler::getSpawnableItem( ItemType itemType )
{
	std::lock_guard<std::mutex> lock( handlerMutex );
	auto it = spawnableItems.find( itemType );
	if ( it != spawnableItems.end( ) ) {
		return &it->second;
	}
	return nullptr;
}