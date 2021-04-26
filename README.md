# SILI Game: Milestone 05
In this final milestone, we will complete our game. We will detect mouse clicks to produce player projectiles, and will have the opponents launch projectiles. The game will keep track of your score and end the game when the player gets hit by an opponent projectile or collides with an opponent.

## GameElement(game_element.h/game_element.cc)
Modify the `IntersectsWith` member function to receive a `GameElement` pointer instead of a constant reference. As a result you will need to modify the code to use pointers.

*Hint: Make sure to dereference pointers to objects before calling their methods.*

## Opponent(opponent.h/opponent.cc)
Create a function called `LaunchProjectile` that returns a `std::unique_ptr` to an `OpponentProjectile` object. The function should return either a `nullptr` or a `std::unique_ptr` to an `OpponentProjectile` object. You are free to design when to return an opponent projectile and the location of the opponent projectile. For example, you can set up a counter data member so that it only creates and returns a unique pointer to an opponent projectile that is at the middle bottom position of the opponent on the 10th time the function is called (it returns `nullptr` for the first nine times). The counter can control how quickly the opponent launches its projectiles.

## Game (game.h/game.cc)
Replace the opponent, opponent projectile, and player projectile vectors to contain `std::unique_ptr`s to the objects. For example, the opponent vector should now contain `std::unique_ptr<Opponent>`. This change requires changes to code that use vectors. Use the list below as a guide to update your code:

1. Accessors for the opponent, opponent projectile, and player projectile vectors should return references to a vector of unique pointers.
1. The `CreateOpponent` member function should now create `std::unique_ptr`s to `Opponent` objects and push them into the corresponding vector.
1. Modify the `MoveGameElements` member function to dereference the `std::unique_ptr` in the vector before calling their `Move` and `GetIsActive` functioin. Accessing an element of a vector returns a referene to the `std::unique_ptr` it contains.
1. Modify the `FilterIntersections` member function to dereference the `std::unique_ptr` in the vector before calling their `GetIsActive` and `IntersectsWith` function.
1. Modify the `UpdateScreen` member function to dereference the `std::unique_ptr` in the vector before calling their `GetIsActive` and `Draw` function.

Create an `int` to track the player's score and a `bool` to track whether the user is still playing the game or has lost the game. Create a `GetScore` function to return the player's score and a `HasLost` function to return whether the user has lost (`true` or `false` value).

Remove `CreateOpponentProjectiles` and `CreatePlayerProjectiles` as we no longer need them.

### Init function
Remove the calls to `CreateOpponent`, `CreateOpponentProjectiles`, and `CreatePlayerProjectiles` as we no longer need them.

### LaunchProjectiles function
Call the `LaunchProjectile` function of each opponent. If the function does not return `nullptr` push the returned `std::unique_ptr` to an `OpponentProjectile` object into the opponents vector.

*Hint: Take note that you need to transfer ownership when passing `std::unique_ptr`s.*

### FilterIntersections function
We need to modify the call to the `IntersectsWith` function because we are now dealing with `std::unique_ptr`s. Take note that the `IntersectsWith` function of `GameElement` now accepts a pointer. Retrieve the 'raw' pointer from the `std::unique_ptr` in the corresponding vector index using it's `get` function and pass it as the parameter for the function call.

*Hint: given a `std::unique_ptr<Dog>` called `ptr_pet`, we can retrieve it's raw pointer with `ptr_pet.get()`.*

Add 1 point to the score data member if a player is active and a player projectile intersects with an opponent. Set the playing state to indicate that the user lost whenever the player intersects with an opponent or opponent projectile.

### UpdateScreen function
Draw text on the screen to show the user's score. For example, *Score: 20* on the top left corner of the screen.

*Hint: You can use `graphics::Image`'s `DrawText` function.*

If the playing state indicates that the player lost, display a *Game Over* text in the middle of the screen.

### RemoveInactive function
Create a `RemoveInactive` member function that removes all game elements that are not active. When dealing with game elements inside vectors, you can use it's `erase` function to remove an element given a specific index. The sample code below shows an example of removing the middle element of the vector.

```cpp
std::vector<int> numbers = {1, 2, 3};
numbers.erase(numbers.begin() + 1); // numbers.begin() indicates the locatin of the first element (index 0) and the number that follows is the index of the element to erase.
```

*Hint: When erasing elements of a vector with a loop, it is a good idea to go over the elements in reverse. For example, start from the last element (index 2 which is 3) up to the first element (index 0 which is 1). If we start looping from 0, erasing an element changes the index and could cause some access issues.*

### OnMouseEvent function
When the user clicks the mouse or drags the mouse, you should now create a `std::unique_ptr` to a `PlayerProjectile`. You can use the `kPressed` or `kDragged` `graphics::MouseAction` to detect such events. Make sure to add the `std::unique_ptr` into the player projectiles vector.

*Hint: Take note that you need to transfer ownership when passing `std::unique_ptr`s.*

### OnAnimationStep function
The first thing that the `OnAnimationStep` function should perform is check for the number of opponents on the screen. It should call the `CreateOpponents` member function if there are no opponents on the screen.

Call the `LaunchProjectiles` function after calling `MoveGameElements`.

Call the `RemoveInactive` member function after calling `FilterInterscetions`.

## Compiling and running the program
To manually test your code you can compile and run this program with:

```
clang++ -std=c++17 main.cc game.cc game_element.cc player.cc opponent.cc cpputils/graphics/image.cc -o main -lm -lX11 -lpthread
./main
```

*Note: you need a few additional flags on Mac which are not shown here.*

However, that's a lot to type, so I've included a shortcut to compile and create ``main``:

```
make build
./main
```

## Run the unit tests

I've provided unit tests, which you can try with ``make test``. Use the output to help you debug any issues in your program.

## Run the style and format checkers

You can use two checkers to avoid common errors and make sure your code is easy to read. Use their output to follow programming best practices and make your code readable.

```
make stylecheck
```

```
make formatcheck
```

# Submission checklist
1. Compiled and ran the driver (`main`).
1. Manually checked for compilation and logical errors.
1. Ensured no errors on the unit test (`make test`).
1. Followed advice from the stylechecker (`make stylecheck`).
1. Followed advice from the formatchecker to improve code readability (`make formatcheck`).
1. Answered the questions in `reflection.md`

# Code evaluation
Open the terminal and navigate to the folder that contains this exercise. Assuming you have pulled the code inside of `/home/student/labex02-tuffy` and you are currently in `/home/student` you can issue the following commands

```
cd labex02-tuffy
```

You also need to navigate into the problem you want to answer. To access the files needed to answer problem 1, for example, you need to issue the following command.

```
cd prob01
```

When you want to answer another problem, you need to go back up to the parent folder and navigate into the next problem. Assuming you are currently in `prob01`, you can issue the following commands to go to the parent folder then go into another problem you want to answer; `prob02` for example.

```
cd ..
cd prob02
```

Use the `clang++` command (or ``make build``) to compile your code and the `./` command to run it. The sample code below shows how you would compile code save in `main.cc` and into the executable file `main`. Make sure you use the correct filenames required in this problem.  Take note that if you make any changes to your code, you will need to compile it first before you see changes when running it.

```
clang++ -std=c++17 main.cc game.cc game_element.cc player.cc opponent.cc cpputils/graphics/image.cc -o main -lm -lX11 -lpthread
./main
```

You can run one, two, or all the commands below to `test` your code, `stylecheck` your code's design, or `formatcheck` your work. Kindly make sure that you have compiled and executed your code before issuing any of the commands below to avoid errors.

```
make test
make stylecheck
make formatcheck
```

A faster way of running all these tests uses the `all` parameter.

```
make all
```

# Submission

I recommend pushing to Github frequently to back up your work.
