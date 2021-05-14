#ifndef GAME_ELEMENT_H
#define GAME_ELEMENT_H
#include <memory>

#include "cpputils/graphics/image.h"

class GameElement {
 public:
  GameElement()
      : x_(0), y_(0), width_(50), height_(50) {}  // default constructor
  GameElement(int x, int y, int width, int height)
      : x_(x),
        y_(y),
        width_(width),
        height_(height) {}  // nondefault constructors

  void SetX(int x) { x_ = x; }
  void SetY(int y) { y_ = y; }

  virtual void Draw(graphics::Image &gamescreen) = 0;

  virtual void Move(const graphics::Image &gamescreen) = 0;

  bool IntersectsWith(GameElement *game);

  int GetX() const { return x_; }
  int GetY() const { return y_; }
  int GetWidth() const { return width_; }
  int GetHeight() const { return height_; }

  bool GetIsActive() const { return active_; }
  void SetIsActive(bool active) { active_ = active; }

  bool IsOutOfBounds(const graphics::Image &gamescreen);

 protected:
  bool active_ = true;
  int x;
  int y;
  int x_, y_, height_, width_;
};

#endif
