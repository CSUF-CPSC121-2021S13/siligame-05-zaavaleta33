#include "player.h"

#include "opponent.h"

void Player::Draw(graphics::Image &gamescreen) {
  // draw player
  gamescreen.DrawRectangle(x_ + 10, y_ + 10, 30, 30, 255, 0,
                           0);  // Draw rectangle
  gamescreen.DrawPolygon({x_ + 25, y_, x_, y_ + 10, x_ + 40, y_ + 10}, 47, 158,
                         132);  // Draw polygone at top
}
void Player::Move(const graphics::Image &gamescreen) {}

void PlayerProjectile::Draw(graphics::Image &gamescreen) {
  // Drawing player project
  gamescreen.DrawRectangle(x_ + 1, y_ + 1, 4, 4, 32, 220,
                           227);  // projectiles
                                  // Rectangle
}

void PlayerProjectile::Move(const graphics::Image &gamescreen) {
  if ((GameElement::IsOutOfBounds(gamescreen))) {
    SetIsActive(false);
  } else {
    GameElement::SetY(GetY() + 3);
    GameElement::SetX(GetX() + 3);
  }
}
