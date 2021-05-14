#include "game_element.h"

bool GameElement::IntersectsWith(GameElement *game) {
  return !(x_ > game->GetX() + game->GetWidth() ||
           game->GetX() > GetX() + GetWidth() ||
           y_ > game->GetY() + game->GetHeight() ||
           game->GetY() > GetY() + GetHeight());
}
bool GameElement::IsOutOfBounds(const graphics::Image &gamescreen) {
  if (x_ + width_ > gamescreen.GetWidth() ||
      y_ + height_ > gamescreen.GetHeight() || x_ < 0 || y_ < 0) {
    return true;
  } else {
    return false;
  }
}
