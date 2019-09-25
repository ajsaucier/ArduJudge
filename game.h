#ifndef GAME_H
#define GAME_H

#include "globals.h"

void resetCardNumbers() 
{
  player.cardNumber = random(1, 10);
  enemy.cardNumber = random(1, 10);
  player.isHoldingCard = false;
  enemy.isHoldingCard = false;
}

void showPlayerScore() 
{
  arduboy.setCursor(0, 0);
  arduboy.fillRect(0, 0, 16, 16, BLACK);
  arduboy.setTextSize(1);
  arduboy.print(player.score);
}

void showEnemyScore() 
{
  arduboy.setCursor(WIDTH - 16, 0);
  arduboy.fillRect(WIDTH - 16, 0, 16, 16, BLACK);
  arduboy.setTextSize(1);
  arduboy.print(enemy.score);
}

void drawEntity(Entity entity)
{
  Sprites::drawOverwrite(entity.x, entity.y, entity.images.defaultImage, 0);

  if (entity.isHoldingCard)
    Sprites::drawOverwrite(entity.x, entity.y, entity.images.holdImage, 0);

  if (entity.didSwingHammer)
    Sprites::drawOverwrite(entity.x, entity.y, entity.images.swingImage, 0);
    
  if (entity.wasHit)
    Sprites::drawOverwrite(entity.x, entity.y, entity.images.hitImage, 0);

  if (entity.didDodgeHammer)
    Sprites::drawOverwrite(entity.x, entity.y, entity.images.dodgeImage, 0);
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

class ResetGameState
{
public: 
  void update()
  {
    player.score = 0;
    enemy.score = 0;
    resetCardNumbers();
  }
};

ResetGameState resetGameState;

class IntroductionState
{
public:
  void update()
  {
    if (arduboy.justPressed(A_BUTTON))
    {
      gameState = GameState::PlayGame;
    }

    if (arduboy.justPressed(B_BUTTON))
    {
      arduboy.audio.toggle();
      arduboy.audio.saveOnOff();
    }
  }
  
  void draw()
  {
    arduboy.setCursor(8, 8);
    arduboy.setTextSize(2);
    arduboy.print(F("ArduJudge"));

    arduboy.setCursor(32, 32);
    arduboy.setTextSize(1);
    arduboy.print(F("(A) Start"));

    arduboy.setCursor(32, 42);
    arduboy.print(F("(B) Sound "));
    
    if (arduboy.audio.enabled())
    {
      arduboy.print(F("on"));
    }
    else
    {
      arduboy.print(F("off"));
    }

    arduboy.setCursor(18, 54);
    arduboy.print(F("@raspberrybrain"));
  }
};

IntroductionState introductionState;

class PlayGameState
{
private:
  int countDown() {
    if (mainCountdownNumber > 0) 
    {
      if (mainCountdownNumber == 1000) 
      {
        sound.tone(NOTE_E5, 50);
      }
      --mainCountdownNumber;
    }
    
    if (mainCountdownNumber % timeBetweenCounts == 0 && mainTimerSeconds > 0) 
    {
      --mainTimerSeconds;
      sound.tone(NOTE_E5, 50);
      if (mainTimerSeconds == 0)
        sound.tone(NOTE_G5, 50);
    }
    
    return mainTimerSeconds;
  }
  
  void showCountdown() 
  {
    arduboy.setCursor((WIDTH / 2) - 4, 8);
    Sprites::drawPlusMask((WIDTH / 2) - 4, 8, counterArrows_plus_mask, countDown());
  }
  // Updated function credit goes to @Pharap
  
  void processMainAction()
  {
    if (inGameTimer > 0)
    {
      // Player control
      if (arduboy.justPressed(A_BUTTON))
      {
        player.didSwingHammer = true;
        
        if (player.cardNumber >= enemy.cardNumber)
          player.score += 3;
        else
          enemy.score += 3;
          
        enemy.wasHit = true;
        sound.tone(NOTE_G5, 50);
        gameState = GameState::AfterRound;
      }
      else if (arduboy.justPressed(B_BUTTON))
      {
        player.didDodgeHammer = true;
        
        if (player.cardNumber < enemy.cardNumber)
          player.score += 2;
        else
          enemy.score += 2;
          
        sound.tone(NOTE_G5, 50);
        gameState = GameState::AfterRound;
      }
    }
    // If the time is up and player hasn't hit either button have the enemy make a decision
    else
    {
      // Enemy AI
      if (player.cardNumber > enemy.cardNumber)
      {
        enemy.didDodgeHammer = true;
        enemy.score += 2;
      }
      else
      {
        enemy.didSwingHammer = true;
        player.wasHit = true;
        enemy.score += 3;
      }
      
      sound.tone(NOTE_G5, 50);
      gameState = GameState::AfterRound;
    }
  }
  
  void mainAction()
  {
    processMainAction();
  
    if (inGameTimer > 0)
    {
      --inGameTimer;
    }
    else
    {
      gameState = GameState::AfterRound;
    }
  }
  
public:
  void update() 
  {
    
    if (mainTimerSeconds == 0) {
      drawCards();
      mainAction();
    }
    if (player.score >= scoreToWin || enemy.score >= scoreToWin) {
      gameState = GameState::GameOver;
    }
  }
  
  void draw()
  {
    showPlayerScore();
    showEnemyScore();
    showCountdown();
    drawEntity(player);
    drawEntity(enemy);
  }
};

PlayGameState playGameState;

class AfterRoundState
{
private:
  void resetTimers() 
  {
    mainTimerSeconds = 3;
    mainCountdownNumber = 1000;
    inGameTimer = 25;
    afterRoundTimer = 70;
  }
  
  void resetEntities() {
    player.didSwingHammer = false;
    enemy.didSwingHammer = false;
    player.didDodgeHammer = false;
    enemy.didDodgeHammer= false;
    player.wasHit = false;
    enemy.wasHit= false;
  }

public:
  void update() 
  {
    showPlayerScore();
    showEnemyScore();
    drawEntity(player);
    drawEntity(enemy);
    drawCards();
    --afterRoundTimer;
    if (afterRoundTimer == 0)
    {
      resetCardNumbers();
      resetTimers();
      resetEntities();
      gameState = GameState::PlayGame;
    }
  }
  
  void draw() 
  {
    if (afterRoundTimer % 14 == 0)
    {
      // Player flickering
      if (enemy.didDodgeHammer)
      {
        Sprites::drawErase(player.x, player.y, player.images.holdImage, 0);
        player.hideCard();
      }
      else if (enemy.didSwingHammer)
      {
        Sprites::drawErase(player.x, player.y, player.images.hitImage, 0);
        player.hideCard();
      }
      
      // Everything else
      if (player.didSwingHammer)
      {
        if (player.cardNumber >= enemy.cardNumber)
        {
          Sprites::drawErase(enemy.x, enemy.y, enemy.images.hitImage, 0);
          enemy.hideCard();
        }
        else 
        {
          Sprites::drawErase(player.x, player.y, player.images.swingImage, 0);
          player.hideCard();
        }
      }
      
      if (player.didDodgeHammer)
      {
        if (player.cardNumber < enemy.cardNumber)
        {
          Sprites::drawErase(enemy.x, enemy.y, enemy.images.holdImage, 0);
          enemy.hideCard();
        }
        else 
        {
          Sprites::drawErase(player.x, player.y, player.images.dodgeImage, 0);
          player.hideCard();
        }
      }
    }
  }
};

AfterRoundState afterRoundState;

class GameOverState
{
public:
  void update()
  {
    if (arduboy.justPressed(A_BUTTON))
    {
      gameState = GameState::Reset;
    }
  }
  
  void draw()
  {
    arduboy.setCursor(8, 8);
    arduboy.setTextSize(2);
    arduboy.print(F("Game Over"));
    
    arduboy.setTextSize(1);
    if (player.score >= scoreToWin)
    {
      arduboy.setCursor(32, 32);
      arduboy.print(F("You Won!"));
    }
    else
    {
      arduboy.setCursor(32, 32);
      arduboy.print(F("You lost!"));
    }
    
    arduboy.setCursor(32, 42);
    arduboy.print(F("(A) Start Over"));
  }
};

GameOverState gameOverState;

#endif
