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
      resetGame();
      resetCardNumbers();
      gameState = GameState::Introduction;
    
    case GameState::Introduction:
      introduction();
      break;

    case GameState::PlayGame:
      playGame();
      break;
      
    case GameState::AfterRound:
      afterRoundState();
      break;

    case GameState::GameOver:
      gameOver();
      break;
      
  }

  arduboy.display();
}
