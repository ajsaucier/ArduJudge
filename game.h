#ifndef GAME_H
#define GAME_H

#include "globals.h"

void resetGame() {
  player.cardNumber = random(1, 10);
  enemy.cardNumber = random(1, 10);
}

void resetCardNumbers() {
  player.cardNumber = random(1, 10);
  enemy.cardNumber = random(1, 10);
}

void showPlayerScore() {
  
}

void showEnemyScore() {
  
}

int countDown() {
  if (mainNumber > 0) {
    --mainNumber;
  }
  
  if (mainNumber % randomMod == 0 && mainTimer > 0)
    --mainTimer;
  
  return mainTimer;
}

void showScores() {
  
}

void showCountdown() {
  arduboy.setCursor((WIDTH / 2) - 4, 8);
  Sprites::drawPlusMask((WIDTH / 2) - 4, 8, counterArrows_plus_mask, countDown());
}

void drawPlayer() {
  Sprites::drawSelfMasked(player.x, player.y, player.image, 0);
}

void drawEnemy() {
  Sprites::drawSelfMasked(enemy.x, enemy.y, enemy.image, 0);
}

void drawCards() {
  player.showCard();
  enemy.showCard();
  player.showCardNumber();
  enemy.showCardNumber();
  
 if (player.cardNumber > enemy.cardNumber) {
    arduboy.setCursor(0, 0);
    arduboy.print(F("player!"));
  } else {
    arduboy.print(F("enemy!"));
  }
  
 if (player.cardNumber == enemy.cardNumber) {
    arduboy.setCursor(0, 0);
    arduboy.print(F("uh oh!"));
  }
}

void playGame() {
  countDown();
  showCountdown();
  drawPlayer();
  drawEnemy();
  if (mainTimer == 0) {
    drawCards();
  }
  if (arduboy.justPressed(A_BUTTON)) {
    resetCardNumbers();
    mainTimer = 3;
    mainNumber = 1000;
    randomMod = random(40, 51);
    countDown();
    if (mainTimer == 0) 
      drawCards();
  }
}

#endif
