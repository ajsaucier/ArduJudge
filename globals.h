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
const uint8_t scoreToWin { 99 };

uint8_t playerNumber { 0 }; // 1 through 9
uint8_t enemyNumber { 0 };

uint16_t tcounter { 3 }; // 3 second countdown
uint16_t seconds { 0 };
unsigned long timer;

bool inGame { false };

// Amount of time for player to press a button
uint8_t inGameTimer { 25 };

// 
// Set up repreating countdowns
// 

// Amount of time for 3...2...1 countdown
// Increase for longer time betwen counts
uint16_t mainCountdownNumber { 1000 };
uint8_t mainTimerSeconds { 3 };

// Increase to slow down time between 3...2...1
uint8_t timeBetweenCounts { 40 };

// Amount of time to pause between rounds
uint8_t afterRoundTimer { 75 };

// uint8_t randomMod { random(30, 51) };

// Check for a pressed button and keep count
bool didPressButton { false };

enum class GameState : uint8_t {
  Reset,
  Introduction,
  PlayGame,
  AfterRound,
  GameOver,
};

GameState gameState {GameState::Reset}; // for production
// GameState GameState {GameState::PlayGame}; // for testing

struct EntityImages
{
  const uint8_t *defaultImage;
  const uint8_t *holdImage;
  const uint8_t *swingImage;
  const uint8_t *hitImage;
  const uint8_t *dodgeImage;
};

struct Entity 
{
  uint8_t x;
  uint8_t y;
  uint8_t score; // max 99
  uint8_t cardNumber;
  bool isHoldingCard;
  bool didSwingHammer;
  bool didDodgeHammer;
  EntityImages images;
  
  void showCard() {
    Sprites::drawOverwrite(x + (getImageWidth(characterHold) / 2) - (getImageWidth(numberCard) / 2), y - 32, numberCard, 0);
  }
  
  void hideCard() {
    Sprites::drawErase(x + (getImageWidth(characterHold) / 2) - (getImageWidth(numberCard) / 2), y - 32, numberCard, 0);
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
  0,
  0,
  false,
  false,
  false,
  {
    character,
    characterHold,
    characterSwing,
    characterHit,
    characterDodge
  }
};

Entity enemy {
  static_cast<uint8_t>((WIDTH - getImageWidth(characterHold)) - 8),
  32,
  0,
  0,
  false,
  false,
  false,
    {
    characterFlipped,
    characterFlippedHold,
    characterFlippedSwing,
    characterFlippedHit,
    characterFlippedDodge
  }
};

#endif
