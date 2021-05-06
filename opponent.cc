#include "opponent.h"

void Opponent::Draw(graphics::Image &gamescreen) {
  // Draw Opponent
  gamescreen.DrawCircle(x_ + kWidth_ / 2, y_ + kHeight_ / 2, 25, 138, 79, 17);
}

void Opponent::Move(const graphics::Image &gamescreen) {
  if ((GameElement::IsOutOfBounds(gamescreen))) {
    active_ = false;
  } else {
    GameElement::SetY(GameElement::GetY() + 1);
    GameElement::SetX(GameElement::GetX() + 1);
  }
}
std::unique_ptr<OpponentProjectile> Opponent::LaunchProjectile() {
  if (counter == 10) {
    std::unique_ptr<OpponentProjectile> new_ptr_proj_opp =
        std::make_unique<OpponentProjectile>(Opponent::GetX(),
                                             Opponent::GetY());
    counter = 0;
    return new_ptr_proj_opp;
  } else {
    counter++;
    return nullptr;
  }
}

void OpponentProjectile::Draw(graphics::Image &gamescreen) {
  // Draw Opponent's projectile
  gamescreen.DrawCircle(x_ + kWidth_ / 2, y_ + kHeight_ / 2, 1, 133, 94, 17);
}

void OpponentProjectile::Move(const graphics::Image &gamescreen) {
  if ((IsOutOfBounds(gamescreen))) {
    SetIsActive(false);
  } else {
    GameElement::SetY(GetY() + 3);
    GameElement::SetX(GetX() + 3);
  }
}
