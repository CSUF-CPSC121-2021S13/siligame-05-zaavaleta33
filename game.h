#ifndef GAME_H
#define GAME_H
#include "cpputils/graphics/image.h"
#include "cpputils/graphics/image_event.h"
#include "opponent.h"
#include "player.h"
#include <memory>
#include <vector>
class Game : public graphics::AnimationEventListener,
             public graphics::MouseEventListener {
public:
  Game() : gamescreen(800, 600) {}

  Game(int width, int height) : gamescreen(width, height) {}

  void CreateOpponents() {
    std::unique_ptr<Opponent> opp;//(200, 200);
    opp_list.push_back(std::move(opp));
  }

  void Init();

  void UpdateScreen();

  void Start();

  graphics::Image &GetGameScreen() { return gamescreen; }

  std::vector<std::unique_ptr<Opponent>> &GetOpponents() { return opp_list; }

  std::vector<std::unique_ptr<OpponentProjectile>> &GetOpponentProjectiles() {
    return opponent_projectiles;
  }
  std::vector<std::unique_ptr<PlayerProjectile>> &GetPlayerProjectiles() {
    return lazer;
  }

  Player &GetPlayer() { return play; } // gets the object

  void MoveGameElements();

  void FilterIntersections();

  void OnAnimationStep() override;

  void OnMouseEvent(const graphics::MouseEvent &event) override;

  int GetScore() { return player_score; }

  bool HasLost() { return lost_game; }

  void LaunchProjectiles();

  void RemoveInactive();

private:
  int player_score = 0;
  bool lost_game = false;
  int width_;
  int height_;
  graphics::Image gamescreen;
  std::vector<std::unique_ptr<Opponent>> opp_list;
  std::vector<std::unique_ptr<OpponentProjectile>> opponent_projectiles;
  std::vector<std::unique_ptr<PlayerProjectile>> lazer;
  Player play;
};

#endif
