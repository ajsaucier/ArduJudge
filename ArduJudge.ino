// See: https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/
#include <Arduboy2.h>

#include "globals.h"
#include "game.h"

void setup() {
  // arduboy.begin();
  arduboy.boot();
  arduboy.setFrameRate(30);

  // initialize things here
  arduboy.initRandomSeed();

}

void loop() {
  if (!arduboy.nextFrame())
    return;

  arduboy.clear();

  arduboy.pollButtons();

  switch (gameStatus) {

    case GameStatus::Reset:
      resetGame();
      gameStatus = GameStatus::PlayGame;
    
    case GameStatus::Introduction:
      arduboy.setCursor(0, 0);
      arduboy.print(F("Intro"));
      break;

    case GameStatus::PlayGame:
      playGame();
      break;

    case GameStatus::GameOver:
      arduboy.setCursor(0, 0);
      arduboy.print(F("Game Over"));
      break;
      
  }
  

  arduboy.display();
}
