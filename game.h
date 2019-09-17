#ifndef GAME_H
#define GAME_H

#include "globals.h"

void resetGame() {
  player.score = 0;
  enemy.score = 0;
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
  arduboy.print(player.score);
}

void showEnemyScore() {
  arduboy.setCursor(WIDTH - 16, 0);
  arduboy.fillRect(WIDTH - 16, 0, 16, 16, BLACK);
  arduboy.setTextSize(1);
  arduboy.print(enemy.score);
}

void resetTimers() {
  resetCardNumbers();
  mainTimerSeconds = 3;
  mainCountdownNumber = 1000;
  inGameTimer = 25;
  player.didSwingHammer = false;
  enemy.didSwingHammer = false;
  player.didDodgeHammer = false;
  enemy.didDodgeHammer= false;
}

int countDown() {
  if (mainCountdownNumber > 0) {
    if (mainCountdownNumber == 1000) {
      sound.tone(NOTE_E5, 50);
    }
    --mainCountdownNumber;
  }
  
  if (mainCountdownNumber % timeBetweenCounts == 0 && mainTimerSeconds > 0) {
    --mainTimerSeconds;
    sound.tone(NOTE_E5, 50);
    if (mainTimerSeconds == 0)
      sound.tone(NOTE_G5, 50);
  }
  
  return mainTimerSeconds;
}

void showCountdown() {
  arduboy.setCursor((WIDTH / 2) - 4, 8);
  Sprites::drawPlusMask((WIDTH / 2) - 4, 8, counterArrows_plus_mask, countDown());
}

void drawPlayer() {
  Sprites::drawOverwrite(player.x, player.y, player.image, 0);
  if (player.isHoldingCard) {
    Sprites::drawOverwrite(player.x, player.y, characterHold, 0);
  }
  if (player.didSwingHammer) {
    Sprites::drawOverwrite(player.x, player.y, characterSwing, 0);
  }
  if (enemy.didSwingHammer) {
    Sprites::drawOverwrite(player.x, player.y, characterHit, 0);
  }
  if (player.didDodgeHammer) {
    Sprites::drawOverwrite(player.x, player.y, characterDodge, 0);
    player.isHoldingCard = false;
  }
}

void drawEnemy() {
  Sprites::drawOverwrite(enemy.x, enemy.y, enemy.image, 0);
  if (enemy.isHoldingCard) {
    Sprites::drawOverwrite(enemy.x, enemy.y, characterFlippedHold, 0);
  }
  if (enemy.didSwingHammer) {
    Sprites::drawOverwrite(enemy.x, enemy.y, characterFlippedSwing, 0);
  }
  if (player.didSwingHammer) {
    Sprites::drawOverwrite(enemy.x, enemy.y, characterFlippedHit, 0);
  }
  if (enemy.didDodgeHammer) {
    Sprites::drawOverwrite(enemy.x, enemy.y, characterFlippedDodge, 0);
    enemy.isHoldingCard = false;
  }
}

void drawCards() {
  if (!player.didDodgeHammer) {
    player.showCard();
  } else {
    player.hideCard();
  }
  if (!enemy.didDodgeHammer) {
    enemy.showCard();
  } else {
    enemy.hideCard();
  }
  player.showCardNumber();
  enemy.showCardNumber();
  player.isHoldingCard = true;
  enemy.isHoldingCard = true;
}

void mainAction() {
  if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON))
    sound.tone(NOTE_G5, 50);
    
  if (player.cardNumber > enemy.cardNumber) {
    --inGameTimer;
    // Enemy AI
    if (!didPressButton) {
      if (inGameTimer == 2) {
        enemy.didDodgeHammer = true;
        enemy.score += 2;
        sound.tone(NOTE_G5, 50);
      }
    }
    // Player control
    if (arduboy.justPressed(A_BUTTON) && !enemy.didDodgeHammer) {
      player.didSwingHammer = true;
      player.score += 3;
      didPressButton = true;
    } else if (arduboy.justPressed(B_BUTTON) && !enemy.didDodgeHammer) {
      player.didDodgeHammer = true;
      enemy.score += 2;
      didPressButton = true;
    }
  } else if (player.cardNumber == enemy.cardNumber) {
    --inGameTimer;
    // Enemy AI
    if (!didPressButton) {
      if (inGameTimer == 2) {
        enemy.didSwingHammer = true;
        enemy.score += 3;
        sound.tone(NOTE_G5, 50);
      }
    }
    // Player control
    if (arduboy.justPressed(A_BUTTON) && !enemy.didSwingHammer) {
      player.didSwingHammer = true;
      player.score += 3;
      didPressButton = true;
    } else if (arduboy.justPressed(B_BUTTON) && !enemy.didSwingHammer) {
      player.didDodgeHammer = true;
      enemy.score += 2;
      didPressButton = true;
    }
  } else if (player.cardNumber < enemy.cardNumber) {
    --inGameTimer;
    // Enemy AI
    if (!didPressButton) {
      if (inGameTimer == 2) {
        enemy.didSwingHammer = true;
        enemy.score += 3;
        sound.tone(NOTE_G5, 50);
      }
    }
    // Player control
    if (arduboy.justPressed(A_BUTTON) && !enemy.didSwingHammer) {
      player.didSwingHammer = true;
      enemy.score += 3;
      didPressButton = true;
    } else if (arduboy.justPressed(B_BUTTON) && !enemy.didSwingHammer) {
      player.didDodgeHammer = true;
      player.score += 2;
      didPressButton = true;
    }
  }
  // If the time is up and player hasn't hit either button
  // have the enemy make a decision and restart all timers
  if (inGameTimer == 0) {
    inGame = false;
    delay(2000);
    resetTimers();
    countDown();
    didPressButton = false;
  }
}

void introduction() {
  arduboy.setCursor(8, 8);
  arduboy.setTextSize(2);
  arduboy.print(F("ArduJudge"));
  arduboy.setTextSize(1);
  arduboy.setCursor(32, 32);
  arduboy.print(F("(A) Start"));
  arduboy.setCursor(32, 42);
  arduboy.print(F("(B) Sound "));
  if (arduboy.audio.enabled()) {
    arduboy.print(F("on"));
  } else {
    arduboy.print(F("off"));
  }

  arduboy.setCursor(18, 54);
  arduboy.print(F("@raspberrybrain"));
  
  if (arduboy.justPressed(A_BUTTON)) {
    gameStatus = GameStatus::PlayGame;
  }
  
  if (arduboy.justPressed(B_BUTTON)) {
    toggleSoundSettings();
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
  if (player.score >= scoreToWin || enemy.score >= scoreToWin) {
    gameStatus = GameStatus::GameOver;
  }
}

void gameOver() {
  arduboy.setCursor(8, 8);
  arduboy.setTextSize(2);
  arduboy.print(F("Game Over"));
  arduboy.setTextSize(1);
  if (player.score >= scoreToWin) {
    arduboy.setCursor(32, 32);
    arduboy.print(F("You Won!"));
    arduboy.setCursor(32, 42);
    arduboy.print(F("(A) Start Over"));
  } else if (enemy.score >= scoreToWin){
    arduboy.setCursor(32, 32);
    arduboy.print(F("You lost!"));
    arduboy.setCursor(32, 42);
    arduboy.print(F("(A) Start Over"));
  }
  
  if (arduboy.justPressed(A_BUTTON)) {
    gameStatus = GameStatus::Reset;
  }
  
}

#endif
