// See: https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/
#include <Arduboy2.h>

#include "globals.h"
#include "game.h"

void setup() {
  arduboy.begin();
  // arduboy.boot();
  arduboy.setFrameRate(30);

  // initialize things here
  arduboy.initRandomSeed();

}

void loop() {
  if (!arduboy.nextFrame())
    return;

  arduboy.clear();

  arduboy.pollButtons();

  switch (gameState) {

    case GameState::Reset:
      resetGameState.update();
      gameState = GameState::Introduction;
    
    case GameState::Introduction:
      introductionState.update();
      introductionState.draw();
      break;

    case GameState::PlayGame:
      playGameState.update();
      playGameState.draw();
      break;
      
    case GameState::AfterRound:
      afterRoundState.update();
      afterRoundState.draw();
      break;

    case GameState::GameOver:
      gameOverState.update();
      gameOverState.draw();
      break;
  }

  arduboy.display();
}
