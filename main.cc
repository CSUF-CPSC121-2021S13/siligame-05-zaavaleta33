#include <iostream>
#include <memory>

#include "game.h"
#include "game_element.h"
#include "opponent.h"
#include "player.h"
int main() {
  Game game;
  game.OnAnimationStep();
  game.Init();
  game.UpdateScreen();
  game.Start();
  return 0;
}
