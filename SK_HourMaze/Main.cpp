//  ===================================================================================================================
//  Includes
//  ===================================================================================================================
#include <allegro.h>
#include "Globals.h"
#include "Game.h"

//  ===================================================================================================================
//  Main Entry Point
//  ===================================================================================================================
int main(void)
{
  using namespace core;

  // Initiate game
  Game* game = new Game();
  if (game != NULL)
  {
    game->init();

    // Game loop
    while(game->gameIsPlaying())
    {
      game->updateDisplay();
      game->updateCatchInput();
    }    
  }
  delete game;
  

  return 0;
}
END_OF_MAIN();