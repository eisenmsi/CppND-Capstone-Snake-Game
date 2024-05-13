#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <fstream>
#include <string>

/**
 * @brief The main function is the entry point of the program.
 *
 * @return int The exit status of the program.
 */
int main()
{
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);

  std::string name;
  std::string filename = "highscores.txt";
  std::cout << "Enter your name: ";
  std::getline(std::cin, name); // Read full line to include names with spaces

  float initialSpeed;
  std::cout << "Enter initial speed of the game (e.g., 1.0): ";
  std::cin >> initialSpeed;

  constexpr std::size_t kFramesPerSecond{60};
  std::size_t kMsPerFrame{static_cast<std::size_t>(1000 / (kFramesPerSecond * initialSpeed))};

  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";

  // Open the file in append mode to add new entries rather than overwrite
  std::ofstream outfile(filename, std::ios::app);

  if (!outfile.is_open())
  {
    std::cerr << "Failed to open the file." << std::endl;
    return 1;
  }

  // Write the name and score to the file
  outfile << name << " " << game.GetScore() << std::endl;

  outfile.close();

  std::cout << "High score saved successfully!" << std::endl;

  return 0;
}