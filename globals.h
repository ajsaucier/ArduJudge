#ifndef GLOBALS_H
#define GLOBALS_H

Arduboy2 arduboy;
#include <ArduboyTones.h>
ArduboyTones sound(arduboy.audio.enabled);
#include "bitmaps.h"

// Goal is to get to 99 points
// Successful hit = 3
// Successful dodge = 2
// If you mess up, opposite player gets the points
uint8_t playerScore{ 0 }; // max 99
uint8_t enemyScore{ 0 };

uint8_t playerNumber{ 0 }; // 1 through 9
uint8_t enemyNumber{ 0 };

uint16_t tcounter{ 3 }; // 3 second countdown
uint16_t seconds{ 0 };
unsigned long timer;

// Set up repreating countdown
uint16_t mainNumber{ 1000 };
uint8_t mainTimer{ 3 };
uint8_t randomMod{ random(40, 51) };

enum class GameStatus : uint8_t {
  Reset,
  Introduction,
  PlayGame,
  GameOver,
};

GameStatus gameStatus {GameStatus::Reset}; // for production
// GameStatus gameStatus {GameStatus::PlayGame}; // for testing

struct Entity {
  int8_t x;
  int8_t y;
  int8_t cardNumber;
  bool didSwingHammer;
  bool didDodgeHammer;
  const uint8_t *image;
  
  void swingHammer() {
    
  }
  
  void dodgeHammer() {
    
  }
  
  void showCard() {
    Sprites::drawOverwrite(x + (getImageWidth(placeholder) / 2) - (getImageWidth(numberCard) / 2), y - 32, numberCard, 0);
  }
  
  int showCardNumber() {
    arduboy.setCursor(x + (getImageWidth(numberCard)), y - (getImageHeight(numberCard) / 2) - (8));
    arduboy.setTextSize(2);
    arduboy.print(cardNumber);
    return cardNumber;
  }
};

Entity player {
  8,
  32,
  random(1, 10),
  false,
  false,
  placeholder
};

Entity enemy {
  (WIDTH - getImageWidth(placeholder)) - 8,
  32,
  random(1, 10),
  false,
  false,
  placeholder
};

#endif
