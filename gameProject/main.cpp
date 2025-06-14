#include <iostream>
#include <thread>
#include <sstream> 
#include <iomanip> 

#include "Game/game.h"

#include "Game/gameRender/gameRender.h"

#include "Game/gameControls/keybindHandler/keybindHandler.h"

#include "Game/gameObjects/gameState/inPreMainMenuState/inPreMainMenuState.h"

#include "Game/Managers/gameResourceManager/gameResourceManager.h"
#include "Game/Managers/gameStateManagers/gameStateManager.h"

#include "Game/Handlers/shadersHandler/shadersHandler.h"

#include "Globals/Globals.h"
#include "Utils/Log/Log.h"
#include "Utils/utils.h"

#include <raylib/raylib.h>
#include <raylib/raymath.h>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

Game game;

void getWindowSize( ) {
	std::pair<int , int> screenSize = utils::Get( ).getScreenSize( );
	Globals::Get( ).screenWidth = screenSize.first;
	Globals::Get( ).screenHeight = screenSize.second;
	Log::Print( "[Globals] Screen size: %dx%d" , screenSize.first , screenSize.second );
}

int main( void ) {
#if _DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#else

#endif
	Log::Print( "[Render] Initialized window!" );
	getWindowSize( );

	SetConfigFlags( FLAG_VSYNC_HINT );
	//SetConfigFlags( FLAG_FULLSCREEN_MODE ); 
	InitWindow( Globals::Get( ).screenWidth , Globals::Get( ).screenHeight , "Janela Fullscreen" ); // Tamanhos ignorados no fullscreen
	InitAudioDevice( );      // Initialize audio device
	DisableCursor( );

	gameStateManager gameStateManager_;

	//
	BeginDrawing( );
	ClearBackground( BLACK );
	DrawText( "Loading" , Globals::Get( ).screenWidth / 2 - MeasureText( "Loading" , 20 ) / 2 , Globals::Get( ).screenHeight / 2 - 10 , 20 , WHITE );
	EndDrawing( );

	Log::Print( "[main] starting game!" );
	if ( !game.start( ) ) {
		Log::Print( "[main] failed to start game!" );
	}
	else {
		while ( !game.isResourcesLoaded( ) ) {
			std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
		}

		shaderHandler::Get( ).initializeRenderTexture( );
		shaderHandler::Get( ).preLoadAll( );

		gameStateManager_.ChangeState( std::make_unique<inPreMainMenuState>( ) );

		while ( true )
		{
			_gameResourceManager.getSoundManager( )->setVolume( Globals::Get( ).getGameSettings( )->getSoundVolume( ) );
			_gameResourceManager.getMusicManager( )->setMusicVolume( Globals::Get( ).getGameSettings( )->getMusicVolume( ) );

			Vector2 mousePos = GetMousePosition( );
			Globals::Get( ).mousePosX = mousePos.x;
			Globals::Get( ).mousePosY = mousePos.y;

			// --- Entrada ---
			gameStateManager_.HandleInput( );

			// --- Atualização ---
			float deltaTime = GetFrameTime( );
			gameStateManager_.Update( deltaTime );

			keybindHandler::Get( ).update( );
			gameRender::Get( ).processSoundEvents( );
			shaderHandler::Get( ).updateAll( );

			BeginDrawing( );
			ClearBackground( WHITE );

			// --- Renderização ---
			gameStateManager_.Render( ); // O estado atual desenha o que precisa

			gameRender::Get( ).renderCustomCursor( );

			//Log::Print( "[main] current game time: %f" , Globals::Get( ).getGame( )->getCurrentGameTime( ) );

			EndDrawing( );
		}
		CloseWindow( );
	}
	system( "pause" );

	return 1;
}