#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../cpputils/graphics/image.h"
#include "../cppaudit/gtest_ext.h"
#include "../../player.h"
#include "../../opponent.h"
#include "../../game_element.h"
#include "../../game.h"

class TestDerivedClass : public GameElement {
 public:
  TestDerivedClass() : GameElement(0, 0, 50, 50) {}
  TestDerivedClass(int x, int y) : GameElement(x, y, 50, 50) {}
  void Draw(graphics::Image& img) override { }
  void Move(const graphics::Image& img) override { }
};

// GameElement unittests
TEST(GameElement, DefaultConstructor) {
  TestDerivedClass my_game_element;
  ASSERT_EQ(my_game_element.GetX(), 0) << "GameElement objects' default x location should be set to 0.";
  ASSERT_EQ(my_game_element.GetY(), 0) << "GameElement objects' default y location should be set to 0.";
  ASSERT_NE(my_game_element.GetHeight(), 0) << "GameElement objects' height should not be 0.";
  ASSERT_NE(my_game_element.GetWidth(), 0) << "GameElement objects' width should not be 0.";
  ASSERT_TRUE(my_game_element.GetIsActive()) << "GameElement objects should be active by default";
  bool is_abstract = std::is_abstract<GameElement>::value;
  ASSERT_TRUE(is_abstract) << "GameElement should be abstract because Draw and Move are pure virtual functions.";
}

TEST(GameElement, ParametrizedConstructor) {
  TestDerivedClass my_game_element(15, 32);
  ASSERT_EQ(my_game_element.GetX(), 15) << "GameElement objects' x location should be set to the first parameter.";
  ASSERT_EQ(my_game_element.GetY(), 32) << "GameElement objects' y location should be set to the second parameter.";
  ASSERT_NE(my_game_element.GetWidth(), 0) << "GameElement objects' width should be set to the third parameter.";
  ASSERT_NE(my_game_element.GetHeight(), 0) << "GameElement objects' height should be set to the fourth parameter.";
  ASSERT_TRUE(my_game_element.GetIsActive()) << "GameElement objects should be active by default";
  bool is_abstract = std::is_abstract<GameElement>::value;
  ASSERT_TRUE(is_abstract) << "GameElement should be abstract because Draw and Move are pure virtual functions.";
}

TEST(GameElement, IntersectsWithGameElement) {
  TestDerivedClass my_player(40, 100);
  
  TestDerivedClass my_left_opponent;
  my_left_opponent.SetX(40 - (my_left_opponent.GetWidth() - 1));
  my_left_opponent.SetY(100);
  ASSERT_TRUE(my_player.IntersectsWith(&my_left_opponent)) << "A GameElement object must check if another GameElement intersects with it's left side.";

  TestDerivedClass my_right_opponent;
  my_right_opponent.SetX(40 + my_player.GetWidth() - 1);
  my_right_opponent.SetY(100);
  ASSERT_TRUE(my_player.IntersectsWith(&my_right_opponent)) << "A GameElement object must check if another GameElement intersects with it's right side.";

  TestDerivedClass my_top_opponent;
  my_top_opponent.SetX(40);
  my_top_opponent.SetY(100 - (my_top_opponent.GetHeight() - 1));
  ASSERT_TRUE(my_player.IntersectsWith(&my_top_opponent)) << "A GameElement object must check if another GameElement intersects with it's top side.";

  TestDerivedClass my_bottom_opponent;
  my_bottom_opponent.SetX(40);
  my_bottom_opponent.SetY(100 + my_player.GetHeight() - 1);
  ASSERT_TRUE(my_player.IntersectsWith(&my_bottom_opponent)) << "A GameElement object must check if another GameElement intersects with it's top side.";
}

TEST(GameElement, IsOutOfBounds) {
  graphics::Image game_screen(800, 600);
  
  TestDerivedClass left_element;
  left_element.SetX(-10);
  left_element.SetY(10);
  ASSERT_TRUE(left_element.IsOutOfBounds(game_screen)) << "A GameElement at (-10, 10) should be out of bounds.";

  TestDerivedClass top_element;
  top_element.SetX(10);
  top_element.SetY(-10);
  ASSERT_TRUE(top_element.IsOutOfBounds(game_screen)) << "A GameElement at (10, -10) should be out of bounds.";

  TestDerivedClass right_element;
  right_element.SetX(810);
  right_element.SetY(10);
  ASSERT_TRUE(right_element.IsOutOfBounds(game_screen)) << "A GameElement at (810, 10) on an 800 x 600 game screen should be out of bounds.";

  TestDerivedClass bottom_element;
  bottom_element.SetX(10);
  bottom_element.SetY(810);
  ASSERT_TRUE(bottom_element.IsOutOfBounds(game_screen)) << "A GameElement at (10, 810) on an 800 x 600 game screen should be out of bounds.";

}

// Opponent unittests
TEST(Opponent, InheritsFromGameElement) {
  bool is_derived = std::is_base_of<GameElement, Opponent>::value;
  EXPECT_TRUE(is_derived) << "    Opponent should inherit from GameElement";
}

TEST(Opponent, DefaultConstructor) {
  Opponent my_opponent;
  ASSERT_EQ(my_opponent.GetX(), 0) << "Opponent objects' default x location should be set to 0.";
  ASSERT_EQ(my_opponent.GetY(), 0) << "Opponent objects' default y location should be set to 0.";
  ASSERT_NE(my_opponent.GetHeight(), 0) << "Opponent height should not be 0.";
  ASSERT_NE(my_opponent.GetWidth(), 0) << "Opponent width should not be 0.";
}

TEST(Opponent, ParemterizedConstructor) {
  Opponent my_opponent(5, 8);
  ASSERT_EQ(my_opponent.GetX(), 5) << "Opponent objects' x location should be set to the first parameter.";
  ASSERT_EQ(my_opponent.GetY(), 8) << "Opponent objects' y location should be set to the second parameter.";
  ASSERT_NE(my_opponent.GetHeight(), 0) << "Opponent height should not be 0.";
  ASSERT_NE(my_opponent.GetWidth(), 0) << "Opponent width should not be 0.";
}

void OpponentDraw() {
  Opponent my_opponent(10, 10);
  int width = my_opponent.GetWidth();
  int height = my_opponent.GetHeight();
  graphics::Image test_screen(800, 600);
  ASSERT_DURATION_LE(3, {
    my_opponent.Draw(test_screen);
  })
  
  bool has_drawing = false;
  bool has_extraneous_drawing = false;

  graphics::Color white(255, 255, 255);
  for (int y = 0; y < test_screen.GetHeight(); y++) {
    for (int x = 0; x < test_screen.GetWidth(); x++) {
      if (x >= 10 && x <= 10 + width && y >= 10 && y <= 10 + height) {
        if (test_screen.GetColor(x, y) != white) {
          has_drawing = true;
        }
      } else {
        if (test_screen.GetColor(x, y) != white) {
          has_extraneous_drawing = true;
        }
      }
    }
  }
  ASSERT_TRUE(has_drawing) << "The Opponent object's Draw method should draw an image to the given image object.";
  ASSERT_FALSE(has_extraneous_drawing) << "The Opponent object's Draw method should only draw inside it's boundaries.";  
}

TEST(Opponent, Draw) {
  ASSERT_EXIT((OpponentDraw(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
    OpponentDraw();
}

TEST(Opponent, Move) {
  Opponent my_opponent(400, 300);
  graphics::Image my_game_screen(800, 600);
  my_opponent.Move(my_game_screen);
  ASSERT_NE(my_opponent.GetX(), 400) << "An opponent initially at (400, 300) should not be on the same location after calling Move.";
  ASSERT_NE(my_opponent.GetY(), 300) << "An opponent initially at (400, 300) should not be on the same location after calling Move.";
  ASSERT_TRUE(my_opponent.GetIsActive()) << "An opponent should remain active when moving inside the bounds of the game screen.";

  Opponent my_outside_opponent(-99999, -99999);
  my_outside_opponent.Move(my_game_screen);
  ASSERT_FALSE(my_outside_opponent.GetIsActive()) << "An opponent should become inactive when moving outside the bounds of the game screen.";
}

TEST(Opponent, LaunchProjectile) {
  std::unique_ptr<OpponentProjectile> projectile;
  Opponent my_opponent(10, 10);
  bool has_null_ptr = false;
  bool has_projectile = false;
  for (int i = 0; i < 100 && (!has_null_ptr || !has_projectile); i++) {
    projectile = my_opponent.LaunchProjectile();
    if (projectile == nullptr) {
      has_null_ptr = true;
    } else {
      has_projectile = true;
    }
  }
  ASSERT_TRUE(has_null_ptr) << "The Opponent should occasionally return nullptr so it doesn't continually launch projectiles.";
  ASSERT_TRUE(has_projectile) << "The Opponentt should return a projectile.";
}

// OpponentProjectile Unittests

TEST(OpponentProjectile, InheritsFromGameElement) {
  bool is_derived = std::is_base_of<GameElement, OpponentProjectile>::value;
  EXPECT_TRUE(is_derived) << "OpponentProjectile should inherit from GameElement";
}

TEST(OpponentProjectile, DefaultConstructor) {
  OpponentProjectile my_opponentprojectile;
  ASSERT_EQ(my_opponentprojectile.GetX(), 0) << "Opponent objects' default x location should be set to 0.";
  ASSERT_EQ(my_opponentprojectile.GetY(), 0) << "Opponent objects' default y location should be set to 0.";
  ASSERT_NE(my_opponentprojectile.GetHeight(), 0) << "Opponent height should not be 0.";
  ASSERT_NE(my_opponentprojectile.GetWidth(), 0) << "Opponent width should not be 0.";
}

TEST(OpponentProjectile, ParemterizedConstructor) {
  OpponentProjectile my_opponentprojectile(5, 8);
  ASSERT_EQ(my_opponentprojectile.GetX(), 5) << "Opponent objects' x location should be set to the first parameter.";
  ASSERT_EQ(my_opponentprojectile.GetY(), 8) << "Opponent objects' y location should be set to the second parameter.";
  ASSERT_NE(my_opponentprojectile.GetHeight(), 0) << "Opponent height should not be 0.";
  ASSERT_NE(my_opponentprojectile.GetWidth(), 0) << "Opponent width should not be 0.";
}

void OpponentProjectileDraw() {
  OpponentProjectile my_opponentprojectile(10, 10);
  int width = my_opponentprojectile.GetWidth();
  int height = my_opponentprojectile.GetHeight();
  graphics::Image test_screen(800, 600);
  ASSERT_DURATION_LE(3, {
    my_opponentprojectile.Draw(test_screen);    
  })

  bool has_drawing = false;
  bool has_extraneous_drawing = false;

  graphics::Color white(255, 255, 255);
  for (int y = 0; y < test_screen.GetHeight(); y++) {
    for (int x = 0; x < test_screen.GetWidth(); x++) {
      if (x >= 10 && x <= 10 + width && y >= 10 && y <= 10 + height) {
        if (test_screen.GetColor(x, y) != white) {
          has_drawing = true;
        }
      } else {
        if (test_screen.GetColor(x, y) != white) {
          has_extraneous_drawing = true;
        }
      }
    }
  }  
  ASSERT_TRUE(has_drawing) << "The OpponentProjectile object's Draw method should draw an image to the given image object.";
  ASSERT_FALSE(has_extraneous_drawing) << "The OpponentProjectile object's Draw method should only draw inside it's boundaries.";
}

TEST(OpponentProjectile, Draw) {
  ASSERT_EXIT((OpponentProjectileDraw(),exit(0)),::testing::ExitedWithCode(0),".*")
      << "    Should not crash or infinitely loop";
  OpponentProjectileDraw();
}

TEST(OpponentProjectile, Move) {
  OpponentProjectile my_opponent_projectile(388, 220);
  graphics::Image my_game_screen(800, 600);
  my_opponent_projectile.Move(my_game_screen);
  
  ASSERT_TRUE(my_opponent_projectile.GetX() != 388 ||
              my_opponent_projectile.GetY() != 220) << "An opponent projectile initially at (388, 220) should not be on the same location after calling Move.";
  ASSERT_TRUE(my_opponent_projectile.GetIsActive()) << "An opponent projectile should remain active when moving inside the bounds of the game screen.";

  OpponentProjectile my_outside_opponent_projectile(-99999, -99999);
  my_outside_opponent_projectile.Move(my_game_screen);
  ASSERT_FALSE(my_outside_opponent_projectile.GetIsActive()) << "An opponent projectile should become inactive when moving outside the bounds of the game screen.";
}

// Player Unittests

TEST(Player, InheritsFromGameElement) {
  bool is_derived = std::is_base_of<GameElement, Player>::value;
  EXPECT_TRUE(is_derived) << "    Player should inherit from GameElement";
}

TEST(Player, DefaultConstructor) {
  Player my_player;
  ASSERT_EQ(my_player.GetX(), 0) << "Player objects' default x location should be set to 0.";
  ASSERT_EQ(my_player.GetY(), 0) << "Player objects' default y location should be set to 0.";
  ASSERT_NE(my_player.GetHeight(), 0) << "Player height should not be 0.";
  ASSERT_NE(my_player.GetWidth(), 0) << "Player width should not be 0.";
}

TEST(Player, ParemterizedConstructor) {
  Player my_player(5, 8);
  ASSERT_EQ(my_player.GetX(), 5) << "Player objects' x location should be set to the first parameter.";
  ASSERT_EQ(my_player.GetY(), 8) << "Player objects' y location should be set to the second parameter.";
  ASSERT_NE(my_player.GetHeight(), 0) << "Player height should not be 0.";
  ASSERT_NE(my_player.GetWidth(), 0) << "Player width should not be 0.";
}

void PlayerDraw() {
  Player my_player(10, 10);
  int width = my_player.GetWidth();
  int height = my_player.GetHeight();
  graphics::Image test_screen(800, 600);
  ASSERT_DURATION_LE(3, {
    my_player.Draw(test_screen);  
  })
  
  bool has_drawing = false;
  bool has_extraneous_drawing = false;

  graphics::Color white(255, 255, 255);

  for (int y = 0; y < test_screen.GetHeight(); y++) {
    for (int x = 0; x < test_screen.GetWidth(); x++) {
      if (x >= 10 && x <= 10 + width && y >= 10 && y <= 10 + height) {
        if (test_screen.GetColor(x, y) != white) {
          has_drawing = true;
        }
      } else {
        if (test_screen.GetColor(x, y) != white) {
          has_extraneous_drawing = true;
        }
      }
    }
  }
  ASSERT_TRUE(has_drawing) << "The Player object's Draw method should draw an image to the given image object.";
  ASSERT_FALSE(has_extraneous_drawing) << "The Player object's Draw method should only draw inside it's boundaries.";   
}

TEST(Player, Draw) {
  ASSERT_EXIT((PlayerDraw(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  PlayerDraw();
}

TEST(Player, IntersectsWithOpponentsAndProjectiles) {
  Player my_player(50, 110);
  
  OpponentProjectile my_left_opponentprojectile;
  my_left_opponentprojectile.SetX(50 - (my_left_opponentprojectile.GetWidth() - 1));
  my_left_opponentprojectile.SetY(110);
  ASSERT_TRUE(my_player.IntersectsWith(&my_left_opponentprojectile)) << "A Player object must check if an OpponentProjectile intersects with it's left side.";

  Opponent my_right_opponent;
  my_right_opponent.SetX(50 + my_player.GetWidth() - 1);
  my_right_opponent.SetY(110);
  ASSERT_TRUE(my_player.IntersectsWith(&my_right_opponent)) << "A Player object must check if an Opponent intersects with it's right side.";

  Opponent my_top_opponent;
  my_top_opponent.SetX(50);
  my_top_opponent.SetY(110 - (my_top_opponent.GetHeight() - 1));
  ASSERT_TRUE(my_player.IntersectsWith(&my_top_opponent)) << "A Player object must check if an Opponent intersects with it's top side.";

  OpponentProjectile my_bottom_opponentprojectile;
  my_bottom_opponentprojectile.SetX(50);
  my_bottom_opponentprojectile.SetY(110 + my_player.GetHeight() - 1);
  ASSERT_TRUE(my_player.IntersectsWith(&my_bottom_opponentprojectile)) << "A Player object must check if an OpponentProjectile intersects with it's top side.";
}


TEST(Player, Move) {
  Player my_player(400, 300);
  graphics::Image my_game_screen(800, 600);
  my_player.Move(my_game_screen);
  ASSERT_EQ(my_player.GetX(), 400) << "A player initially at (400, 300) should not move when calling its Move function.";
  ASSERT_EQ(my_player.GetY(), 300) << "A player initially at (400, 300) should not move when calling its Move function.";
}

// PlayerProjectile Unittests
TEST(PlayerProjectile, InheritsFromGameElement) {
  bool is_derived = std::is_base_of<GameElement, PlayerProjectile>::value;
  EXPECT_TRUE(is_derived) << "PlayerProjectile should inherit from GameElement";
}

TEST(PlayerProjectile, DefaultConstructor) {
  PlayerProjectile my_playerprojectile;
  ASSERT_EQ(my_playerprojectile.GetX(), 0) << "PlayerProjectile objects' default x location should be set to 0.";
  ASSERT_EQ(my_playerprojectile.GetY(), 0) << "PlayerProjectile objects' default y location should be set to 0.";
  ASSERT_NE(my_playerprojectile.GetHeight(), 0) << "PlayerProjectile height should not be 0.";
  ASSERT_NE(my_playerprojectile.GetWidth(), 0) << "PlayerProjectile width should not be 0.";
}

TEST(PlayerProjectile, ParemterizedConstructor) {
  PlayerProjectile my_playerprojectile(5, 8);
  ASSERT_EQ(my_playerprojectile.GetX(), 5) << "PlayerProjectile objects' x location should be set to the first parameter.";
  ASSERT_EQ(my_playerprojectile.GetY(), 8) << "PlayerProjectile objects' y location should be set to the second parameter.";
  ASSERT_NE(my_playerprojectile.GetHeight(), 0) << "PlayerProjectile height should not be 0.";
  ASSERT_NE(my_playerprojectile.GetWidth(), 0) << "PlayerProjectile width should not be 0.";
}

void PlayerProjectileDraw() {
  PlayerProjectile my_playerprojectile(10, 10);
  int width = my_playerprojectile.GetWidth();
  int height = my_playerprojectile.GetHeight();
  graphics::Image test_screen(800, 600);
  ASSERT_DURATION_LE(3, {
    my_playerprojectile.Draw(test_screen);
  })
  bool has_drawing = false;
  bool has_extraneous_drawing = false;

  graphics::Color white(255, 255, 255);
  for (int y = 0; y < test_screen.GetHeight(); y++) {
    for (int x = 0; x < test_screen.GetWidth(); x++) {
      if (x >= 10 && x <= 10 + width && y >= 10 && y <= 10 + height) {
        if (test_screen.GetColor(x, y) != white) {
          has_drawing = true;
        }
      } else {
        if (test_screen.GetColor(x, y) != white) {
          has_extraneous_drawing = true;
        }
      }
    }
  }
  ASSERT_TRUE(has_drawing) << "The PlayerProjectile object's Draw method should draw an image to the given image object.";
  ASSERT_FALSE(has_extraneous_drawing) << "The PlayerProjectile object's Draw method should only draw inside it's boundaries.";
}

TEST(PlayerProjectile, Draw) {
  ASSERT_EXIT((PlayerProjectileDraw(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  PlayerProjectileDraw();
}

TEST(PlayerProjectile, IntersectsWithOpponent) {
  PlayerProjectile my_playerprojectile(40, 100);
  
  Opponent my_left_opponent;
  my_left_opponent.SetX(40 - (my_left_opponent.GetWidth() - 1));
  my_left_opponent.SetY(100);
  ASSERT_TRUE(my_playerprojectile.IntersectsWith(&my_left_opponent)) << "A PlayerProjectile object must check if an Opponent intersects with it's left side.";

  Opponent my_right_opponent;
  my_right_opponent.SetX(40 + my_playerprojectile.GetWidth() - 1);
  my_right_opponent.SetY(100);
  ASSERT_TRUE(my_playerprojectile.IntersectsWith(&my_right_opponent)) << "A PlayerProjectile object must check if an Opponent intersects with it's right side.";

  Opponent my_top_opponent;
  my_top_opponent.SetX(40);
  my_top_opponent.SetY(100 - (my_top_opponent.GetHeight() - 1));
  ASSERT_TRUE(my_playerprojectile.IntersectsWith(&my_top_opponent)) << "A PlayerProjectile object must check if an Opponent intersects with it's top side.";

  Opponent my_bottom_opponent;
  my_bottom_opponent.SetX(40);
  my_bottom_opponent.SetY(100 + my_playerprojectile.GetHeight() - 1);
  ASSERT_TRUE(my_playerprojectile.IntersectsWith(&my_bottom_opponent)) << "A PlayerProjectile object must check if an Opponent intersects with it's top side.";
}

TEST(PlayerProjectile, Move) {
  PlayerProjectile my_player_projectile(388, 220);
  graphics::Image my_game_screen(800, 600);
  my_player_projectile.Move(my_game_screen);
  ASSERT_TRUE(my_player_projectile.GetX() != 388 ||
              my_player_projectile.GetY() != 220) << "A player projectile initially at (388, 220) should not be on the same location after calling Move.";
  ASSERT_TRUE(my_player_projectile.GetIsActive()) << "A player projectile should remain active when moving inside the bounds of the game screen.";

  PlayerProjectile my_outside_player_projectile(-99999, -99999);
  my_outside_player_projectile.Move(my_game_screen);
  ASSERT_FALSE(my_outside_player_projectile.GetIsActive()) << "A player projectile should become inactive when moving outside the bounds of the game screen.";
}

TEST(Game, DefaultConstructor) {
  Game my_game;
  ASSERT_EQ(my_game.GetGameScreen().GetWidth(), 800) << "The default game width should be 800 pixels.";
  ASSERT_EQ(my_game.GetGameScreen().GetHeight(), 600) << "The default game height should be 600 pixels.";

  bool is_animationeventlistener = std::is_base_of<graphics::AnimationEventListener, Game>::value;
  ASSERT_TRUE(is_animationeventlistener) << "The game class should derive graphics::AnimationEventListener.";

  bool is_mouseeventlistener = std::is_base_of<graphics::MouseEventListener, Game>::value;
  ASSERT_TRUE(is_mouseeventlistener) << "The game class should derive graphics::is_mouseeventlistener.";
  
}

TEST(Game, ParametrizedConstructor) {
  Game my_game(300, 400);
  ASSERT_EQ(my_game.GetGameScreen().GetWidth(), 300) << "The game width should be equal to the first constructor parameter.";
  ASSERT_EQ(my_game.GetGameScreen().GetHeight(), 400) << "The game width should be equal to the first constructor parameter.";
}

void CreateOpponents() {
  Game my_game;
  my_game.CreateOpponents();
  int num_opponents = my_game.GetOpponents().size();
  ASSERT_GT(num_opponents, 0) << "The function should create at least one opponent and store it in the corresponding vector.";
}

TEST(Game, CreateOpponents) {
  ASSERT_EXIT((CreateOpponents(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  CreateOpponents();
}

void MoveActiveGameElements() {
  Game my_game;
  std::vector<std::unique_ptr<Opponent>>& opponents = my_game.GetOpponents();
  std::vector<std::unique_ptr<OpponentProjectile>>& opponent_projectiles = my_game.GetOpponentProjectiles();
  std::vector<std::unique_ptr<PlayerProjectile>>& player_projectiles = my_game.GetPlayerProjectiles();

  Opponent my_opponent(20, 20);
  OpponentProjectile my_opponent_projectile(120, 120);
  PlayerProjectile my_player_projectile(400, 400);

  opponents.push_back(std::make_unique<Opponent>(20, 20));
  opponent_projectiles.push_back(std::make_unique<OpponentProjectile>(120, 120));
  player_projectiles.push_back(std::make_unique<PlayerProjectile>(400, 400));

  my_game.MoveGameElements();

  ASSERT_TRUE(opponents[0]->GetX() != 20 ||
              opponents[0]->GetY() != 20) << "The function should have moved all opponents on the screen.";

  ASSERT_TRUE(opponent_projectiles[0]->GetX() != 120 ||
              opponent_projectiles[0]->GetY() != 120) << "The function should have moved all opponent projectiles on the screen.";

  ASSERT_TRUE(player_projectiles[0]->GetX() != 400 ||
              player_projectiles[0]->GetY() != 400) << "The function should have moved all player projectiles on the screen.";
}

TEST(Game, MoveActiveGameElements) {
  ASSERT_EXIT((MoveActiveGameElements(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  MoveActiveGameElements();
}

void MoveInActiveGameElements() {
  Game my_game;
  std::vector<std::unique_ptr<Opponent>>& opponents = my_game.GetOpponents();
  std::vector<std::unique_ptr<OpponentProjectile>>& opponent_projectiles = my_game.GetOpponentProjectiles();
  std::vector<std::unique_ptr<PlayerProjectile>>& player_projectiles = my_game.GetPlayerProjectiles();

  Opponent my_opponent(-20, -20);
  OpponentProjectile my_opponent_projectile(-120, -120);
  PlayerProjectile my_player_projectile(-400, -400);

  opponents.push_back(std::make_unique<Opponent>(-20, -20));
  opponent_projectiles.push_back(std::make_unique<OpponentProjectile>(-120, -120));
  player_projectiles.push_back(std::make_unique<PlayerProjectile>(-400, -400));

  my_game.MoveGameElements();

  ASSERT_FALSE(opponents[0]->GetIsActive()) << "Opponents that move outside the bounds of the screen should be inactive.";
  ASSERT_FALSE(opponent_projectiles[0]->GetIsActive()) << "Opponent projectiless that move outside the bounds of the screen should be inactive.";
  ASSERT_FALSE(player_projectiles[0]->GetIsActive()) << "Player projectiles that move outside the bounds of the screen should be inactive.";
}

TEST(Game, MoveInActiveGameElements) {
  ASSERT_EXIT((MoveInActiveGameElements(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  MoveInActiveGameElements();
}

void LaunchProjectiles() {
  Game my_game;
  std::vector<std::unique_ptr<Opponent>>& opponents = my_game.GetOpponents();

  Opponent my_opponent(10, 10);
  opponents.push_back(std::make_unique<Opponent>(10, 10));
  
  bool has_projectile = false;
  for (int i = 0; i < 100 && !has_projectile; i++) {
    my_game.LaunchProjectiles();
    if (my_game.GetOpponentProjectiles().size() > 0) {
      has_projectile = true;
    }
  }
  ASSERT_TRUE(has_projectile) << "Opponents should launch a projectile.";
}

TEST(Game, LaunchProjectiles) {
  ASSERT_EXIT((LaunchProjectiles(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  LaunchProjectiles();
}

void FilterIntersectionsWithOpponentProjectile() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();
  std::vector<std::unique_ptr<OpponentProjectile>>& opponent_projectiles = my_game.GetOpponentProjectiles();

  OpponentProjectile my_first_opponentprojectile(20, 20);
  OpponentProjectile my_second_opponentprojectile(200, 200);
  opponent_projectiles.push_back(std::make_unique<OpponentProjectile>(20, 20));
  opponent_projectiles.push_back(std::make_unique<OpponentProjectile>(200, 200));

  my_player.SetX(21);
  my_player.SetY(21);

  my_game.FilterIntersections();

  ASSERT_FALSE(my_player.GetIsActive()) << "A player object should be inactive when it intersects with an opponent projectile object.";
  ASSERT_FALSE(opponent_projectiles[0]->GetIsActive()) << "An opponent projectile object should be inactive when it intersects with a player object.";
  ASSERT_TRUE(opponent_projectiles[1]->GetIsActive()) << "An opponent projectile that does not intersect with the player should remain active.";
  ASSERT_TRUE(my_game.HasLost()) << "The player should lose when their character intersects with an opponent projectile.";
}

TEST(Game, FilterIntersectionsWithOpponentProjectile) {
  ASSERT_EXIT((FilterIntersectionsWithOpponentProjectile(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  FilterIntersectionsWithOpponentProjectile();
}

void FilterIntersectionsWithOpponentProjectileNoIntersect() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();
  std::vector<std::unique_ptr<OpponentProjectile>>& opponent_projectiles = my_game.GetOpponentProjectiles();

  OpponentProjectile my_first_opponentprojectile(20, 20);
  opponent_projectiles.push_back(std::make_unique<OpponentProjectile>(20, 20));

  my_player.SetX(200);
  my_player.SetY(200);

  my_game.FilterIntersections();

  ASSERT_TRUE(my_player.GetIsActive()) << "A player object should remain active when it doesn't intersect with an opponent projectile object.";
  ASSERT_TRUE(opponent_projectiles[0]->GetIsActive()) << "An opponent projectile object should remain active when it doesn't intersect with a player object.";
  ASSERT_FALSE(my_game.HasLost()) << "The player should not lose when their character does not intersect with an opponent projectile.";
}

TEST(Game, FilterIntersectionsWithOpponentProjectileNoIntersect) {
  ASSERT_EXIT((FilterIntersectionsWithOpponentProjectileNoIntersect(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  FilterIntersectionsWithOpponentProjectileNoIntersect();
}

void FilterIntersectionsWithOpponent() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();
  std::vector<std::unique_ptr<Opponent>>& opponents = my_game.GetOpponents();
  std::vector<std::unique_ptr<PlayerProjectile>>& player_projectiles = my_game.GetPlayerProjectiles();

  Opponent my_first_opponent(20, 20);
  Opponent my_second_opponent(200, 200);
  Opponent my_third_opponent(400, 400);
  opponents.push_back(std::make_unique<Opponent>(20, 20));
  opponents.push_back(std::make_unique<Opponent>(200, 200));
  opponents.push_back(std::make_unique<Opponent>(400, 400));

  my_player.SetX(21);
  my_player.SetY(21);

  PlayerProjectile my_first_projectile(201, 201);
  PlayerProjectile my_second_projectile(500, 500);
  player_projectiles.push_back(std::make_unique<PlayerProjectile>(201, 201));
  player_projectiles.push_back(std::make_unique<PlayerProjectile>(500, 500));  

  my_game.FilterIntersections();

  ASSERT_FALSE(my_player.GetIsActive()) << "A player object should be inactive when it intersects with an opponent object.";
  ASSERT_FALSE(opponents[0]->GetIsActive()) << "An opponent object should be inactive when it intersects with a player object.";
  ASSERT_FALSE(player_projectiles[0]->GetIsActive()) << "A player projectile should be inactive when it intersects with an opponent object.";
  ASSERT_FALSE(opponents[1]->GetIsActive()) << "An opponent object should be inactive when it intersects with a player projcetile object.";
  ASSERT_TRUE(opponents[2]->GetIsActive()) << "An opponent that does not intersect with the player or player projectile should remain active.";
  ASSERT_TRUE(player_projectiles[1]->GetIsActive()) << "A player projectile that does not intersect with the opponent should remain active.";
  ASSERT_TRUE(my_game.HasLost()) << "A player should lose when their character intersects with the opponent";
}

TEST(Game, FilterIntersectionsWithOpponent) {
  ASSERT_EXIT((FilterIntersectionsWithOpponent(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  FilterIntersectionsWithOpponent();
}

void FilterIntersectionsWithOpponentNoIntersect() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();
  std::vector<std::unique_ptr<Opponent>>& opponents = my_game.GetOpponents();

  opponents.push_back(std::make_unique<Opponent>(20, 20));

  my_player.SetX(600);
  my_player.SetY(21);

  my_game.FilterIntersections();

  ASSERT_TRUE(my_player.GetIsActive()) << "A player object should remain active when it doesn't intersect with an opponent object.";
  ASSERT_TRUE(opponents[0]->GetIsActive()) << "An opponent object should remain active when it doesn't intersects with a player object.";
  ASSERT_FALSE(my_game.HasLost()) << "A player should not lose when their character dooesn't intersect with the opponent";
}

TEST(Game, FilterIntersectionsWithOpponentNoIntersect) {
  ASSERT_EXIT((FilterIntersectionsWithOpponentNoIntersect(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  FilterIntersectionsWithOpponentNoIntersect();
}

void Score() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();
  std::vector<std::unique_ptr<Opponent>>& opponents = my_game.GetOpponents();
  std::vector<std::unique_ptr<PlayerProjectile>>& player_projectiles = my_game.GetPlayerProjectiles();

  opponents.push_back(std::make_unique<Opponent>(20, 20));
  opponents.push_back(std::make_unique<Opponent>(200, 200));
  opponents.push_back(std::make_unique<Opponent>(400, 400));

  my_player.SetX(600);
  my_player.SetY(21);

  player_projectiles.push_back(std::make_unique<PlayerProjectile>(201, 201));
  player_projectiles.push_back(std::make_unique<PlayerProjectile>(401, 401));  

  my_game.FilterIntersections();

  ASSERT_TRUE(my_player.GetIsActive()) << "A player object should remain inactive when it doesn't intersect with an opponent object.";
  ASSERT_TRUE(opponents[0]->GetIsActive()) << "An opponent object should remain active when it doesn't intersect with a player or player projectile object.";
  ASSERT_FALSE(player_projectiles[0]->GetIsActive()) << "A player projectile should be inactive when it intersects with an opponent object.";
  ASSERT_FALSE(opponents[1]->GetIsActive()) << "An opponent object should be inactive when it intersects with a player projcetile object.";
  ASSERT_FALSE(opponents[2]->GetIsActive()) << "An opponent object should be inactive when it intersects with a player projcetile object.";
  ASSERT_FALSE(player_projectiles[1]->GetIsActive()) << "A player projectile should be inactive when it intersects with an opponent object.";
  ASSERT_FALSE(my_game.HasLost()) << "A player should not lose when their character doesn't intersect with an opponent or opponent projectile object";
  ASSERT_EQ(my_game.GetScore(), 2) << "A player should get one point for every opponent it's projectiles hit.";
}

TEST(Game, Score) {
  ASSERT_EXIT((Score(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  Score();
}

void ScoreInactivePlayer() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();
  std::vector<std::unique_ptr<Opponent>>& opponents = my_game.GetOpponents();
  std::vector<std::unique_ptr<PlayerProjectile>>& player_projectiles = my_game.GetPlayerProjectiles();

  opponents.push_back(std::make_unique<Opponent>(20, 20));
  opponents.push_back(std::make_unique<Opponent>(200, 200));
  opponents.push_back(std::make_unique<Opponent>(400, 400));

  my_player.SetX(600);
  my_player.SetY(21);
  my_player.SetIsActive(false);

  player_projectiles.push_back(std::make_unique<PlayerProjectile>(201, 201));
  player_projectiles.push_back(std::make_unique<PlayerProjectile>(401, 401));  

  my_game.FilterIntersections();

  ASSERT_TRUE(opponents[0]->GetIsActive()) << "An opponent object should remain active when it doesn't intersect with a player or player projectile object.";
  ASSERT_FALSE(player_projectiles[0]->GetIsActive()) << "A player projectile should be inactive when it intersects with an opponent object.";
  ASSERT_FALSE(opponents[1]->GetIsActive()) << "An opponent object should be inactive when it intersects with a player projcetile object.";
  ASSERT_FALSE(opponents[2]->GetIsActive()) << "An opponent object should be inactive when it intersects with a player projcetile object.";
  ASSERT_FALSE(player_projectiles[1]->GetIsActive()) << "A player projectile should be inactive when it intersects with an opponent object.";
  ASSERT_EQ(my_game.GetScore(), 0) << "A player should not get points when they are inactive";
}

TEST(Game, ScoreInactivePlayer) {
  ASSERT_EXIT((ScoreInactivePlayer(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  ScoreInactivePlayer();
}

void RemoveInactive() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();
  std::vector<std::unique_ptr<Opponent>>& opponents = my_game.GetOpponents();
  std::vector<std::unique_ptr<PlayerProjectile>>& player_projectiles = my_game.GetPlayerProjectiles();
  std::vector<std::unique_ptr<OpponentProjectile>>& opponent_projectiles = my_game.GetOpponentProjectiles();
  
  opponents.push_back(std::make_unique<Opponent>(20, 20));
  opponents.push_back(std::make_unique<Opponent>(200, 200));
  opponents.push_back(std::make_unique<Opponent>(400, 400));

  my_player.SetX(600);
  my_player.SetY(21);

  player_projectiles.push_back(std::make_unique<PlayerProjectile>(201, 201));
  player_projectiles.push_back(std::make_unique<PlayerProjectile>(401, 401));

  my_game.FilterIntersections();
  
  ASSERT_TRUE(opponents[0]->GetIsActive()) << "An opponent object should remain active when it doesn't intersect with a player or player projectile object.";
  ASSERT_FALSE(player_projectiles[0]->GetIsActive()) << "A player projectile should be inactive when it intersects with an opponent object.";
  ASSERT_FALSE(opponents[1]->GetIsActive()) << "An opponent object should be inactive when it intersects with a player projcetile object.";
  ASSERT_FALSE(opponents[2]->GetIsActive()) << "An opponent object should be inactive when it intersects with a player projcetile object.";
  ASSERT_FALSE(player_projectiles[1]->GetIsActive()) << "A player projectile should be inactive when it intersects with an opponent object.";

  opponent_projectiles.push_back(std::make_unique<OpponentProjectile>(200, 210));
  opponent_projectiles.push_back(std::make_unique<OpponentProjectile>(200, 700));
  opponent_projectiles.push_back(std::make_unique<OpponentProjectile>(200, 230));

  opponent_projectiles[1]->SetIsActive(false);

  my_game.RemoveInactive();
  ASSERT_EQ(my_game.GetOpponents().size(), 1) << "Inactive opponents need to be removed from the opponents vector.";
  ASSERT_EQ(my_game.GetPlayerProjectiles().size(), 0) << "Inactive player projectile need to be removed from the player projectile vector.";
  ASSERT_EQ(my_game.GetOpponentProjectiles().size(), 2) << "Inactive opponent projectiles need to be removed from the opponent projectiles vector.";
}

TEST(Game, RemoveInactive) {
  ASSERT_EXIT((RemoveInactive(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  RemoveInactive();
}

void Init() {
  Game my_game;
  my_game.Init();

  Player my_player = my_game.GetPlayer();
  ASSERT_NE(my_player.GetX(), 0) << "The Player's x location should not be 0."; 
  ASSERT_NE(my_player.GetY(), 0) << "The Player's y location should not be 0."; 
}

TEST(Game, Init) {
  ASSERT_EXIT((Init(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  Init();
}

void UpdateScreen() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();
  my_player.SetX(400);
  my_player.SetY(20);

  my_game.UpdateScreen();

  std::vector<std::unique_ptr<Opponent>>& opponents = my_game.GetOpponents();
  std::vector<std::unique_ptr<OpponentProjectile>>& opponent_projectiles = my_game.GetOpponentProjectiles();
  std::vector<std::unique_ptr<PlayerProjectile>>& player_projectiles = my_game.GetPlayerProjectiles();
  
  graphics::Image& my_game_screen = my_game.GetGameScreen();

  bool has_player = false;
  bool has_extraneous_drawing = false;

  graphics::Color white(255, 255, 255);
  for (int y = 0; y < my_game_screen.GetHeight(); y++) {
    for (int x = 0; x < my_game_screen.GetWidth(); x++) {
      if (x >= my_player.GetX() && x <= my_player.GetX() + my_player.GetWidth() && y >= my_player.GetY() && y <= my_player.GetY() + my_player.GetHeight()) {
        if (my_game_screen.GetColor(x, y) != white) {
          has_player = true;
        }
      } else {
        if (my_game_screen.GetColor(x, y) != white) {
          has_extraneous_drawing = true;
        }
      }
    }
  }
  ASSERT_TRUE(has_player) << "The Player object should be drawn on the game screen.";
  // ASSERT_FALSE(has_extraneous_drawing) << "There shouldn't be any other game elements drawn on the screen.";

  Opponent my_active_opponent(20, 20);
  Opponent my_inactive_opponent(80, 20);
  my_inactive_opponent.SetIsActive(false);
  opponents.push_back(std::make_unique<Opponent>(100, 20));
  opponents.push_back(std::make_unique<Opponent>(180, 20));

  opponents[1]->SetIsActive(false);

  my_game.UpdateScreen();
    
  int num_opponents = opponents.size();
  int found_opponents = 0;
  for (int i = 0; i < opponents.size(); i++) {
    bool has_element = false;
    for (int y = 0; y < my_game_screen.GetHeight(); y++) {
      for (int x = 0; x < my_game_screen.GetWidth(); x++) {
        if (x >= opponents[i]->GetX() && x <= opponents[i]->GetX() + opponents[i]->GetWidth() && y >= opponents[i]->GetY() && y <= opponents[i]->GetY() + opponents[i]->GetHeight()) {
          if (my_game_screen.GetColor(x, y) != white) {
            has_element = true;
          }
        }
      }
    }
    if (has_element) {
      found_opponents++;
    }
  }
  ASSERT_EQ(found_opponents, 1) << "Only active opponent objects should be drawn on the screen.";

  OpponentProjectile my_active_opponent_projectile(20, 100);
  OpponentProjectile my_inactive_opponent_projectile(80, 100);
  my_inactive_opponent_projectile.SetIsActive(false);
  opponent_projectiles.push_back(std::make_unique<OpponentProjectile>(20, 100));
  opponent_projectiles.push_back(std::make_unique<OpponentProjectile>(80, 100));
  opponent_projectiles[1]->SetIsActive(false);

  my_game.UpdateScreen();

  int num_opponent_projectiles = opponent_projectiles.size();
  int found_opponent_projectiless = 0;
  for (int i = 0; i < opponent_projectiles.size(); i++) {
    bool has_element = false;
    for (int y = 0; y < my_game_screen.GetHeight(); y++) {
      for (int x = 0; x < my_game_screen.GetWidth(); x++) {
        if (x >= opponent_projectiles[i]->GetX() && x <= opponent_projectiles[i]->GetX() + opponent_projectiles[i]->GetWidth() && y >= opponent_projectiles[i]->GetY() && y <= opponent_projectiles[i]->GetY() + opponent_projectiles[i]->GetHeight()) {
          if (my_game_screen.GetColor(x, y) != white) {
            has_element = true;
          }
        }
      }
    }
    if (has_element) {
      found_opponent_projectiless++;
    }
  }
  ASSERT_EQ(found_opponent_projectiless, 1) << "Only active opponent projectiles should be drawn on the screen.";

  PlayerProjectile my_active_player_projectile(20, 200);
  PlayerProjectile my_inactive_player_projectile(80, 200);
  my_inactive_player_projectile.SetIsActive(false);
  player_projectiles.push_back(std::make_unique<PlayerProjectile>(20, 200));
  player_projectiles.push_back(std::make_unique<PlayerProjectile>(80, 200));
  player_projectiles[1]->SetIsActive(false);

  my_game.UpdateScreen();

  int num_player_projectiles = player_projectiles.size();
  int found_player_projectiless = 0;
  for (int i = 0; i < player_projectiles.size(); i++) {
    bool has_element = false;
    for (int y = 0; y < my_game_screen.GetHeight(); y++) {
      for (int x = 0; x < my_game_screen.GetWidth(); x++) {
        if (x >= player_projectiles[i]->GetX() && x <= player_projectiles[i]->GetX() + player_projectiles[i]->GetWidth() && y >= player_projectiles[i]->GetY() && y <= player_projectiles[i]->GetY() + player_projectiles[i]->GetHeight()) {
          if (my_game_screen.GetColor(x, y) != white) {
            has_element = true;
          }
        }
      }
    }
    if (has_element) {
      found_player_projectiless++;
    }
  }
  ASSERT_EQ(found_player_projectiless, 1) << "Only active player projectiles should be drawn on the screen.";
}

TEST(Game, UpdateScreen){
  ASSERT_EXIT((UpdateScreen(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  UpdateScreen();
}

void UpdateScreenNoElements() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();
  my_player.SetIsActive(false);
  my_player.SetX(200);
  my_player.SetY(20);
  my_game.UpdateScreen();

  graphics::Image& my_game_screen = my_game.GetGameScreen();
  bool has_player = false;
  bool has_extraneous_drawing = false;

  graphics::Color white(255, 255, 255);
  for (int y = 0; y < my_game_screen.GetHeight(); y++) {
    for (int x = 0; x < my_game_screen.GetWidth(); x++) {
      if (x >= my_player.GetX() && x <= my_player.GetX() + my_player.GetWidth() && y >= my_player.GetY() && y <= my_player.GetY() + my_player.GetHeight()) {
        if (my_game_screen.GetColor(x, y) != white) {
          has_player = true;
        }
      } else {
        if (my_game_screen.GetColor(x, y) != white) {
          has_extraneous_drawing = true;
        }
      }
    }
  }
  ASSERT_FALSE(has_player) << "An inactive Player object should not be drawn on the game screen.";
  //ASSERT_FALSE(has_extraneous_drawing) << "There shouldn't be any other game elements drawn on the screen.";
}

TEST(Game, UpdateScreenNoElements){
  ASSERT_EXIT((UpdateScreenNoElements(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  UpdateScreenNoElements();
}

void OnAnimationStep() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();
  std::vector<std::unique_ptr<Opponent>>& opponents = my_game.GetOpponents();
  
  graphics::Image& my_game_screen = my_game.GetGameScreen();

  Opponent my_first_opponent(20, 20);
  Opponent my_second_opponent(80, 20);
  opponents.push_back(std::make_unique<Opponent>(20, 20));
  opponents.push_back(std::make_unique<Opponent>(180, 20));

  my_player.SetX(181);
  my_player.SetY(21);
  
  my_game.OnAnimationStep();

  ASSERT_NE(opponents[0]->GetX(), 20) << "OnAnimationStep should call the MoveGameElements function to move all elements.";
  ASSERT_NE(opponents[0]->GetY(), 20) << "OnAnimationStep should call the MoveGameElements function to move all elements.";

  ASSERT_FALSE(my_player.GetIsActive()) << "OnAnimationStep should call the FilterIntersections function to deactivate game elements that intersect.";
  //ASSERT_FALSE(opponents[1]->GetIsActive()) << "OnAnimationStep should call the FilterIntersections function to deactivate game elements that intersect.";

  ASSERT_EQ(opponents.size(), 1) << "OnAnimationStep should call the RemoveInactive function to remove inactive elements from the opponent, opponent projectile, and player projectile vectors.";

  bool has_player = false;

  graphics::Color white(255, 255, 255);
  for (int y = 0; y < my_game_screen.GetHeight(); y++) {
    for (int x = 0; x < my_game_screen.GetWidth(); x++) {
      if (x >= my_player.GetX() && x <= my_player.GetX() + my_player.GetWidth() && y >= my_player.GetY() && y <= my_player.GetY() + my_player.GetHeight()) {
        if (my_game_screen.GetColor(x, y) != white) {
          has_player = true;
        }
      } 
    }
  }
  ASSERT_FALSE(has_player) << "OnAnimationStep should call UpdateScreen, which doesn't draw the Player object when it is inactive."; 

  int num_opponents = opponents.size();
  int found_opponents = 0;
  for (int i = 0; i < opponents.size(); i++) {
    bool has_element = false;
    for (int y = 0; y < my_game_screen.GetHeight(); y++) {
      for (int x = 0; x < my_game_screen.GetWidth(); x++) {
        if (x >= opponents[i]->GetX() && x <= opponents[i]->GetX() + opponents[i]->GetWidth() && y >= opponents[i]->GetY() && y <= opponents[i]->GetY() + opponents[i]->GetHeight()) {
          if (my_game_screen.GetColor(x, y) != white) {
            has_element = true;
          }
        }
      }
    }
    if (has_element) {
      found_opponents++;
    }
  }
  ASSERT_EQ(found_opponents, 1) << "OnAnimationStep should call UpdateScreen, which only draws active game elements.";
}

TEST(Game, OnAnimationStep){
  ASSERT_EXIT((OnAnimationStep(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  OnAnimationStep();
}

void OnAnimationStepProjectiles() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();
  std::vector<std::unique_ptr<Opponent>>& opponents = my_game.GetOpponents();
  
  graphics::Image& my_game_screen = my_game.GetGameScreen();

  Opponent my_first_opponent(20, 20);
  Opponent my_second_opponent(80, 20);
  opponents.push_back(std::make_unique<Opponent>(20, 20));
  opponents.push_back(std::make_unique<Opponent>(80, 20));

  my_player.SetX(450);
  my_player.SetY(10);

  bool launched_opponent_projectile = false;
  for (int i = 0; i < 100  && !launched_opponent_projectile; i++) {
    my_game.OnAnimationStep();
    if (my_game.GetOpponentProjectiles().size() > 0) {
      launched_opponent_projectile = true;
    }    
  }

  ASSERT_TRUE(launched_opponent_projectile) << "OnAnimationStep should call the LaunchProjectiles function that results in an opponent launching a projectile.";
}

TEST(Game, OnAnimationStepProjectiles){
  ASSERT_EXIT((OnAnimationStepProjectiles(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  OnAnimationStepProjectiles();
}

void OnMouseEvent() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();

  graphics::MouseEvent my_event(100, 80, graphics::MouseAction::kMoved);
  my_game.OnMouseEvent(my_event);

  int new_player_x = 100 - my_player.GetWidth() / 2;
  int new_player_y = 80 - my_player.GetHeight() / 2;
  ASSERT_EQ(my_player.GetX(), new_player_x) << "The player's x location should be to the left of the mouse location to center it when moved.";
  ASSERT_EQ(my_player.GetY(), new_player_y) << "The player's y location should be above the mouse location to center it when moved.";
}

TEST(Game, OnMouseEvent){
  ASSERT_EXIT((OnMouseEvent(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  OnMouseEvent();
}

void OnMouseEventDragged() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();

  graphics::MouseEvent my_event(100, 80, graphics::MouseAction::kDragged);
  my_game.OnMouseEvent(my_event);

  int new_player_x = 100 - my_player.GetWidth() / 2;
  int new_player_y = 80 - my_player.GetHeight() / 2;
  ASSERT_EQ(my_player.GetX(), new_player_x) << "The player's x location should be to the left of the mouse location to center it when dragged.";
  ASSERT_EQ(my_player.GetY(), new_player_y) << "The player's y location should be above the mouse location to center it when dragged.";
  ASSERT_GE(my_game.GetPlayerProjectiles().size(), 1) << "The player should launch a projectile whenever the mouse is dragged.";
}

TEST(Game, OnMouseEventDragged){
  ASSERT_EXIT((OnMouseEventDragged(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  OnMouseEventDragged();
}

void OnMouseEventPressed() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();

  graphics::MouseEvent my_event(100, 80, graphics::MouseAction::kPressed);
  my_game.OnMouseEvent(my_event);
  my_game.OnMouseEvent(my_event);

  ASSERT_GE(my_game.GetPlayerProjectiles().size(), 1) << "The player should launch a projectile whenever the mouse is pressed.";
}

TEST(Game, OnMouseEventPressed){
  ASSERT_EXIT((OnMouseEventPressed(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  OnMouseEventPressed();
}

void OnMouseEventOutOfBounds() {
  Game my_game;
  Player& my_player = my_game.GetPlayer();
  my_player.SetX(400);
  my_player.SetY(300);

  graphics::MouseEvent my_event(1000, 1000, graphics::MouseAction::kMoved);
  my_game.OnMouseEvent(my_event);

  ASSERT_EQ(my_player.GetX(), 400) << "The player's x location should not change if the mouse moves outside the game screen.";
  ASSERT_EQ(my_player.GetY(), 300) << "The player's y location should not change if the mouse moves outside the game screen.";
}

TEST(Game, OnMouseEventOutOfBounds){
  ASSERT_EXIT((OnMouseEventOutOfBounds(),exit(0)),::testing::ExitedWithCode(0),".*")
        << "    Should not crash or infinitely loop.";
  OnMouseEventOutOfBounds();
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  bool skip = true;
  for (int i = 0; i < argc; i++) {
    if (std::string(argv[i]) == "--noskip") {
      skip = false;
    }
  }
  if (skip) {
    ::testing::UnitTest::GetInstance()->listeners().Append(new SkipListener());
  }
  return RUN_ALL_TESTS();
}
