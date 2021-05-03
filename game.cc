#include "game.h"
void Game::Init() {
  gamescreen.AddMouseEventListener(*this);
  gamescreen.AddAnimationEventListener(*this);
  play.GameElement::SetY(play.GameElement::GetY() + 3);
  play.GameElement::SetX(play.GameElement::GetX() + 3);
}

void Game::UpdateScreen() {
  // draw white ractangle
  gamescreen.DrawRectangle(0, 0, 800, 600, 255, 255, 255);
  for (int i = 0; i < opp_list.size(); i++) {
    if (opp_list[i]->GetIsActive()) {
      opp_list[i]->Draw(gamescreen);
    }
  }
  for (int j = 0; j < opponent_projectiles.size(); j++) {
    if (opponent_projectiles[j]->GetIsActive()) {
      opponent_projectiles[j]->Draw(gamescreen);
    }
  }
  for (int x = 0; x < lazer.size(); x++) {
    if (lazer[x]->GetIsActive()) {
      lazer[x]->Draw(gamescreen);
    }
  }
  if (play->GetIsActive()) {
    play->Draw(gamescreen);
  }
  gamescreen.DrawText(GetScore());
}

void Game::Start() { gamescreen.ShowUntilClosed(); }

void Game::MoveGameElements() {
  for (int i = 0; i < opp_list.size(); i++) {
    if (opp_list[i]->GetIsActive()) {
      opp_list[i].Move(gamescreen);
    }
  }
  for (int j = 0; j < opponent_projectiles.size(); j++) {
    if (opponent_projectiles[j]->GetIsActive()) {
      opponent_projectiles[j].Move(gamescreen);
    }
  }
  for (int x = 0; x < lazer.size(); x++) {
    if (lazer[x]->GetIsActive()) {
      lazer[x].Move(gamescreen);
    }
  }
}

void Game::FilterIntersections() {
  for (int a = 0; a < opp_list.size(); a++) {
    if (play->IntersectsWith(opp_list[a]) &&
        opp_list[a]->IntersectsWith(play) {
      lost_game = true;
      opp_list[a]->SetIsActive(false);
      play->SetIsActive(false);
    }
  }
  for (int q = 0; q < lazer.size(); q++) {
    for (int x = 0; x < opp_list.size(); x++) {
      if (lazer[q]->IntersectsWith(opp_list[x]) &&
          opp_list[x]->IntersectsWith(lazer[q].get())) {
        player_score++;
        opp_list[x]->SetIsActive(false);
        lazer[q]->SetIsActive(false);
      }
    }
  }
  for (int f = 0; f < GetOpponentProjectiles().size(); f++) {
    if (opponent_projectiles[f]->IntersectsWith(play) &&
        play->IntersectsWith(opponent_projectiles[f])) {
      lost_game = true;
      play->SetIsActive(false);
      opponent_projectiles[f]->SetIsActive(false);
    }
  }
}
void Game::OnAnimationStep() {
  MoveGameElements();
  FilterIntersections();
  UpdateScreen();
  gamescreen.Flush();
}

void Game::OnMouseEvent(const graphics::MouseEvent &event) {
  if (event.GetX() < gamescreen.GetWidth() &&
      event.GetY() < gamescreen.GetHeight() && event.GetX() > 0 &&
      event.GetY() > 0) {
    play.SetX(event.GetX() - 25);
    play.SetY(event.GetY() - 25);
  }
}

void Game::LaunchProjectiles() {
  for (int i = 0; i < opp_list.size(); i++) {
    std::unique_ptr<OpponentProjectile> small_rock =
        opp_list[i]->LaunchProjectile();
    std::unique_ptr<OpponentProjectile> new_rock = std::move(small_rock);
    if (LaunchProjectile() != nullptr) {
      std::push_back(std::move(new_rock))
    } else {
      return nullptr;
    }
  }
}

void Game::RemoveInactive() {
  GameElement game_element;
  for (int i = opp_list.size(); i <= 0; i--) {
    if (!(opp_list[i]->GetIsActive())) {
      opp_list.erase(opp_list.begin())
    }
  }
  for (int i = opponent_projectiles.size(); i <= 0; i--) {
    if (!(opponent_projectiles[i]->GetIsActive())) {
      opponent_projectiles.erase(opponent_projectiles.begin())
    }
  }
  for (int i = lazer.size(); i <= 0; i--) {
    if (!(lazer[i]->GetIsActive())) {
      lazer.erase(lazer.begin())
    }
  }
}
