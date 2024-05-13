/**
 * @file controller.cpp
 * @brief Implementation of the Controller class.
 */

#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

/**
 * @brief Changes the direction of the snake based on user input.
 *
 * This function changes the direction of the snake based on the user's input.
 * It ensures that the snake cannot immediately reverse its direction, unless it consists of only one segment.
 *
 * @param snake The snake object.
 * @param input The new direction input by the user.
 * @param opposite The opposite direction of the current snake direction.
 */
void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const
{
  if (snake.direction != opposite || snake.size == 1)
    snake.direction = input;
  return;
}

/**
 * @brief Handles user input events.
 *
 * This function handles user input events such as key presses.
 * It updates the snake's direction based on the user's input.
 *
 * @param running A boolean flag indicating whether the game is running.
 * @param snake The snake object.
 */
void Controller::HandleInput(bool &running, Snake &snake) const
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
    {
      running = false;
    }
    else if (e.type == SDL_KEYDOWN)
    {
      switch (e.key.keysym.sym)
      {
      case SDLK_UP:
        ChangeDirection(snake, Snake::Direction::kUp,
                        Snake::Direction::kDown);
        break;

      case SDLK_DOWN:
        ChangeDirection(snake, Snake::Direction::kDown,
                        Snake::Direction::kUp);
        break;

      case SDLK_LEFT:
        ChangeDirection(snake, Snake::Direction::kLeft,
                        Snake::Direction::kRight);
        break;

      case SDLK_RIGHT:
        ChangeDirection(snake, Snake::Direction::kRight,
                        Snake::Direction::kLeft);
        break;
      }
    }
  }
}