#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"

struct Food
{
  SDL_Point position;
  int scoreValue;
  double speedIncrement;
  SDL_Color color; // Color to distinguish different food types visually
};

class Obstacle
{
public:
  int x;
  int y;
  bool isMoving;
  int dx; // Delta x - movement direction on the x-axis
  int dy; // Delta y - movement direction on the y-axis

  Obstacle(int x, int y, bool isMoving, int dx, int dy)
      : x(x), y(y), isMoving(isMoving), dx(dx), dy(dy) {}

  void Move(std::size_t grid_width, std::size_t grid_height)
  {
    if (!isMoving)
      return;

    x += dx;
    y += dy;

    // Wrap around logic to keep obstacles within the game grid
    if (x >= static_cast<int>(grid_width))
      x = 0;
    if (y >= static_cast<int>(grid_height))
      y = 0;
    if (x < 0)
      x = static_cast<int>(grid_width - 1);
    if (y < 0)
      y = static_cast<int>(grid_height - 1);
  }
};

class Renderer
{
public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const &snake, std::vector<Food> const &foods, std::vector<Obstacle> const &obstacles);
  void UpdateWindowTitle(int score, int fps);

private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif