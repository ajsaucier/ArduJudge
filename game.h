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
  player.isHoldingCard = false;
  enemy.isHoldingCard = false;
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

void resetTimers() {
  resetCardNumbers();
  mainTimerSeconds = 3;
  mainCountdownNumber = 1000;
  inGameTimer = 32;
  playerSwungHammer = false;
  enemySwungHammer = false;
  playerDodgedHammer = false;
  enemyDodgedHammer= false;
}

int countDown() {
  if (mainCountdownNumber > 0) {
    --mainCountdownNumber;
  }
  
  if (mainCountdownNumber % 50 == 0 && mainTimerSeconds > 0)
    --mainTimerSeconds;
  
  return mainTimerSeconds;
}

void showCountdown() {
  arduboy.setCursor((WIDTH / 2) - 4, 8);
  Sprites::drawPlusMask((WIDTH / 2) - 4, 8, counterArrows_plus_mask, countDown());
}

void drawPlayer() {
  arduboy.setCursor(0, 16);
  arduboy.print(inGameTimer);
  Sprites::drawOverwrite(player.x, player.y, player.image, 0);
  if (player.isHoldingCard) {
    Sprites::drawOverwrite(player.x, player.y, characterHold, 0);
  }
  if (playerSwungHammer) {
    Sprites::drawOverwrite(player.x, player.y, characterSwing, 0);
  }
  if (enemySwungHammer) {
    Sprites::drawOverwrite(player.x, player.y, characterHit, 0);
  }
}

void drawEnemy() {
  Sprites::drawOverwrite(enemy.x, enemy.y, enemy.image, 0);
  if (enemy.isHoldingCard) {
    Sprites::drawOverwrite(enemy.x, enemy.y, characterFlippedHold, 0);
  }
  if (enemySwungHammer) {
    Sprites::drawOverwrite(enemy.x, enemy.y, characterFlippedSwing, 0);
  }
  if (playerSwungHammer) {
    Sprites::drawOverwrite(enemy.x, enemy.y, characterFlippedHit, 0);
  }
}

void drawCards() {
  player.showCard();
  enemy.showCard();
  player.showCardNumber();
  enemy.showCardNumber();
  player.isHoldingCard = true;
  enemy.isHoldingCard = true;
}

void mainAction() {
  if (player.cardNumber > enemy.cardNumber || player.cardNumber == enemy.cardNumber) {
    --inGameTimer;
    // Enemy AI
    if (!didPressButton) {
      if (inGameTimer == 2) {
        if (random(0, 2) == 1) {
          enemyDodgedHammer = true;
          enemyScore += 2;
        } else {
          enemySwungHammer = true;
          playerScore += 2;
        }
      }
    }
    // Player control
    if (arduboy.justPressed(A_BUTTON)) {
      playerSwungHammer = true;
      playerScore += 3;
      didPressButton = true;
    } else if (arduboy.justPressed(B_BUTTON)) {
      playerDodgedHammer = true;
      enemyScore += 2;
      didPressButton = true;
    }
  } else {
    --inGameTimer;
    // Enemy AI
    if (!didPressButton) {
      if (inGameTimer == 2) {
        if (random(0, 2) == 1) {
          enemySwungHammer = true;
          enemyScore += 3;
        } else {
          enemyDodgedHammer = true;
          playerScore += 3;
        }
      }
    }
    // Player control
    if (arduboy.justPressed(A_BUTTON)) {
      playerSwungHammer = true;
      enemyScore += 3;
      didPressButton = true;
    } else if (arduboy.justPressed(B_BUTTON)) {
      playerDodgedHammer = true;
      playerScore += 2;
      didPressButton = true;
    }
  }
  // If the time is up and player hasn't hit either button
  // have the enemy make a decision and restart all timers
  if (inGameTimer == 0) {
    inGame = false;
    delay(3000);
    resetTimers();
    countDown();
    didPressButton = false;
  }
}

void playGame() {
  showPlayerScore();
  showEnemyScore();
  countDown();
  showCountdown();
  drawPlayer();
  drawEnemy();
  if (mainTimerSeconds == 0) {
    inGame = true;
  }
  if (inGame) {
    drawCards();
    mainAction();
  }

  // if (arduboy.justPressed(A_BUTTON)) {
  //   resetCardNumbers();
  //   mainTimerSeconds = 3;
  //   mainCountdownNumber = 1000;
  //   countDown();
  //   if (mainTimerSeconds == 0) 
  //     drawCards();
  // }
}

#endif
