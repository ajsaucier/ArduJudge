#ifndef GAME_H
#define GAME_H

#include "globals.h"

void resetGame() {
  player.cardNumber = random(1, 10);
  enemy.cardNumber = random(1, 10);
}

int secondCountdown() {
  timer = millis();
  seconds = (timer/1000);
  
  if ((tcounter - seconds) > 3 || (tcounter - seconds) == 0) {
    timer = 0;
    seconds = 0;
    tcounter = 0;
  }
  
  return (tcounter - seconds);
}

void showPlayerScore() {
  
}

void showEnemyScore() {
  
}

void showCountdown() {
  arduboy.setCursor((WIDTH / 2) - 4, 8);
  arduboy.print(secondCountdown());
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
}

void playGame() {
  showCountdown();
  drawPlayer();
  drawEnemy();
  if (!secondCountdown())
    drawCards();
}

#endif
