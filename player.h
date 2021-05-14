#ifndef PLAYER_H
#define PLAYER_H
#include <memory>

#include "game_element.h"
#include "opponent.h"
class Player : public GameElement {
 public:
  Player() : GameElement() {}

  Player(int x, int y) : GameElement(x, y, 50, 50) {}

  void Draw(graphics::Image &gamescreen) override;

  void Move(const graphics::Image &gamescreen) override;
};

class PlayerProjectile : public GameElement {
 public:
  PlayerProjectile() : GameElement() {}

  PlayerProjectile(int x, int y) : GameElement(x, y, 5, 5) {}

  void Draw(graphics::Image &gamescreen) override;

  void Move(const graphics::Image &gamescreen) override;
};
#endif
