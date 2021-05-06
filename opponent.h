#ifndef OPPONENT_H
#define OPPONENT_H
#include "game_element.h"
#include <memory>

class OpponentProjectile : public GameElement {
public:
  OpponentProjectile() : GameElement() {}
  OpponentProjectile(int x, int y) : GameElement(x, y, 5, 5) {}

  void Draw(graphics::Image &gamescreen) override;

  void Move(const graphics::Image &gamescreen) override;

private:
  int kWidth_ = 5;
  int kHeight_ = 5;
};

class Opponent : public GameElement {
public:
  Opponent() : GameElement() {}

  Opponent(int x, int y) : GameElement(x, y, 50, 50) {}

  void Draw(graphics::Image &gamescreen) override;

  void Move(const graphics::Image &gamescreen) override;

  std::unique_ptr<OpponentProjectile> LaunchProjectile();

private:
  int counter = 0;
  int kWidth_ = 50;
  int kHeight_ = 50;
};

#endif
