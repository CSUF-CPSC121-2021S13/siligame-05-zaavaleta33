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
  std::string score = "Score: " + std::to_string(GetScore());
  gamescreen.DrawText(1, 1, score, 16, 0, 0, 0);
  if (play.GetIsActive() == true) {
    play.Draw(gamescreen);
  }
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
  if (lost_game) {
    std::string end_game = "GameOver";
    gamescreen.DrawText(250, 250, end_game, 70, 0, 0, 0);
  }
}

void Game::Start() { gamescreen.ShowUntilClosed(); }

void Game::MoveGameElements() {
  for (int i = 0; i < opp_list.size(); i++) {
    if (opp_list[i]->GetIsActive()) {
      opp_list[i]->Move(gamescreen);
    }
  }
  for (int j = 0; j < opponent_projectiles.size(); j++) {
    if (opponent_projectiles[j]->GetIsActive()) {
      opponent_projectiles[j]->Move(gamescreen);
    }
  }
  for (int x = 0; x < lazer.size(); x++) {
    if (lazer[x]->GetIsActive()) {
      lazer[x]->Move(gamescreen);
    }
  }
}

void Game::FilterIntersections() {
  for (int a = 0; a < opp_list.size(); a++) {
    if (play.IntersectsWith(opp_list[a].get())) {
      lost_game = true;
      opp_list[a]->SetIsActive(false);
      play.SetIsActive(false);
    }
  }
  for (int q = 0; q < lazer.size(); q++) {
    for (int x = 0; x < opp_list.size(); x++) {
      if (lazer[q]->IntersectsWith(opp_list[x].get())) {
        if (play.GetIsActive())
          player_score++;
        opp_list[x]->SetIsActive(false);
        lazer[q]->SetIsActive(false);
      }
    }
  }
  for (int f = 0; f < GetOpponentProjectiles().size(); f++) {
    if (opponent_projectiles[f]->IntersectsWith(&play)) {
      lost_game = true;
      play.SetIsActive(false);
      opponent_projectiles[f]->SetIsActive(false);
    }
  }
}
void Game::OnAnimationStep() {
  if (opp_list.size() == 0) {
    CreateOpponents();
  }
  MoveGameElements();
  LaunchProjectiles();
  FilterIntersections();
  RemoveInactive();
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
  if (event.GetMouseAction() == graphics::MouseAction::kPressed ||
      event.GetMouseAction() == graphics::MouseAction::kDragged) {
    std::unique_ptr<PlayerProjectile> lazer2 =
        std::make_unique<PlayerProjectile>(play.GetX(), play.GetY());
    lazer.push_back(std::move(lazer2));
  }
}

void Game::LaunchProjectiles() {
  for (int i = 0; i < opp_list.size(); i++) {
    std::unique_ptr<OpponentProjectile> small_rock =
        std::make_unique<OpponentProjectile>(opp_list[i]->GetX(),
                                             opp_list[i]->GetY());
    small_rock = opp_list[i]->LaunchProjectile();

    if (small_rock != nullptr) {
      opponent_projectiles.push_back(std::move(small_rock));
    }
  }
}

void Game::RemoveInactive() {
  for (int i = opponent_projectiles.size() - 1; i >= 0; i--) {
    if (opponent_projectiles[i]->GetIsActive() == false) {
      opponent_projectiles.erase(opponent_projectiles.begin() + i);
    }
  }
  for (int x = opp_list.size() - 1; x >= 0; x--) {
    if (opp_list[x]->GetIsActive() == false) {
      opp_list.erase(opp_list.begin() + x);
    }
  }
  for (int f = lazer.size() - 1; f >= 0; f--) {
    if (lazer[f]->GetIsActive() == false) {
      lazer.erase(lazer.begin() + f);
    }
  }
}
