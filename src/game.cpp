#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : grid_width(grid_width), grid_height(grid_height),
      snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      random_food_type(0, 2)
{ // Assume three types of food
  PlaceFood();
  PlaceFood();
  PlaceFood();
  PlaceObstacles();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running)
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, foods, obstacles);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000)
    {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration)
    {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood()
{
  int x, y;
  while (true)
  {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y))
    {
      AddFood(x, y);
      return;
    }
  }
}

void Game::PlaceObstacles()
{
  // Place fixed and moving obstacles
  for (int i = 0; i < 3; ++i)
  { // Fixed obstacles
    int x = random_w(engine);
    int y = random_h(engine);
    if (!snake.SnakeCell(x, y))
    {
      obstacles.push_back(Obstacle(x, y, false, 0, 0));
    }
  }
  for (int i = 0; i < 2; ++i)
  { // Moving obstacles
    int x = random_w(engine);
    int y = random_h(engine);
    int dx = 0, dy = 0;
    if (i % 2 == 0)
      dx = 1;
    else
      dy = 1;
    if (!snake.SnakeCell(x, y))
    {
      obstacles.push_back(Obstacle(x, y, true, dx, dy));
    }
  }
}

void Game::MoveObstacles()
{
  for (auto &obs : obstacles)
  {
    obs.Move(grid_width, grid_height);
  }
}

void Game::AddFood(int x, int y)
{
  Food food;
  food.position.x = x;
  food.position.y = y;

  int type = random_food_type(engine);
  switch (type)
  {
  case 0:
    food.scoreValue = 1;
    food.speedIncrement = 0.02;
    food.color = {255, 0, 0, 255}; // Red
    break;
  case 1:
    food.scoreValue = 2;
    food.speedIncrement = 0.05;
    food.color = {0, 255, 0, 255}; // Green
    break;
  case 2:
    food.scoreValue = 3;
    food.speedIncrement = -0.01;   // Example of a negative effect
    food.color = {0, 0, 255, 255}; // Blue
    break;
  }
  foods.push_back(food);
}

void Game::Update()
{
  if (!snake.alive)
    return;

  MoveObstacles(); // Update moving obstacles
  snake.Update();

  // Check collision with obstacles
  for (const auto &obs : obstacles)
  {
    if (static_cast<int>(snake.head_x) == obs.x && static_cast<int>(snake.head_y) == obs.y)
    {
      snake.alive = false; // Snake dies on hitting an obstacle
      return;
    }
  }

  // Check collision with food
  for (auto it = foods.begin(); it != foods.end();)
  {
    if (it->position.x == static_cast<int>(snake.head_x) && it->position.y == static_cast<int>(snake.head_y))
    {
      score += it->scoreValue;
      snake.GrowBody();
      snake.speed += it->speedIncrement;
      it = foods.erase(it);
      PlaceFood();
    }
    else
    {
      ++it;
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }