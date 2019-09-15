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
  arduboy.setCursor(0, 0);
  arduboy.fillRect(0, 0, 16, 16, BLACK);
  arduboy.setTextSize(1);
  arduboy.print(playerScore);
}

void showEnemyScore() {
  arduboy.setCursor(WIDTH - 16, 0);
  arduboy.fillRect(WIDTH - 16, 0, 16, 16, BLACK);
  arduboy.setTextSize(1);
  arduboy.print(enemyScore);
}

void resetTimer() {
  resetCardNumbers();
  mainTimer = 3;
  mainNumber = 1000;
}

int countDown() {
  if (mainNumber > 0) {
    --mainNumber;
  }
  
  if (mainNumber % 50 == 0 && mainTimer > 0)
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

void swingHammer(Entity entity) {
  entity.didSwingHammer = true;
  entity.didDodgeHammer = false;
  // Draw sprite with hammer swung
}

void dodgeHammer(Entity entity) {
  entity.didDodgeHammer = true;
  entity.didDodgeHammer = false;
  // Draw sprite with hammer swung
}

void drawCards() {
  player.showCard();
  enemy.showCard();
  player.showCardNumber();
  enemy.showCardNumber();
}

void mainAction() {
  if (player.cardNumber > enemy.cardNumber || player.cardNumber == enemy.cardNumber) {
    // Enemy AI
    // if (random(0, 3) == 1) {
    //   dodgeHammer(enemy);
    //   enemyScore += 2;
    // } else {
    //   swingHammer(enemy);
    //   playerScore += 2;
    // }
    // Player control
    if (arduboy.justPressed(A_BUTTON)) {
      swingHammer(player);
      playerScore += 3;
      inGame = false;
      resetTimer();
      countDown();
    } else if (arduboy.justPressed(B_BUTTON)) {
      dodgeHammer(player);
      enemyScore += 2;
      inGame = false;
      resetTimer();
      countDown();
    }
  } else {
    // Enemy AI
    // if (random(0, 3) == 1) {
    //   swingHammer(enemy);
    //   enemyScore += 3;
    // } else {
    //   dodgeHammer(enemy);
    //   playerScore += 3;
    // }
    // Player control
    if (arduboy.justPressed(A_BUTTON)) {
      swingHammer(player);
      enemyScore += 3;
      inGame = false;
      resetTimer();
      countDown();
    } else if (arduboy.justPressed(B_BUTTON)) {
      dodgeHammer(player);
      playerScore += 2;
      inGame = false;
      resetTimer();
      countDown();
    }
  }
}

void playGame() {
  showPlayerScore();
  showEnemyScore();
  countDown();
  showCountdown();
  drawPlayer();
  drawEnemy();
  if (mainTimer == 0) {
    inGame = true;
    drawCards();
  }
  if (inGame) {
    mainAction();
  }

  // if (arduboy.justPressed(A_BUTTON)) {
  //   resetCardNumbers();
  //   mainTimer = 3;
  //   mainNumber = 1000;
  //   countDown();
  //   if (mainTimer == 0) 
  //     drawCards();
  // }
}

#endif
