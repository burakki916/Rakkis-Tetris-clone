#pragma once
#include "Tetris.h"
#include "Window.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
class Game
{
  public:
    Game();
    ~Game();
    void handleInput();
    void Update();
    void Render();
    Window *GetWindow();
    sf::Time GetElapsed();
    void RestartClock();

  private:
    Window m_window;
    sf::Vector2f m_increment;
    sf::Vector2u windowSize = sf::Vector2u(1020, 720);
    sf::Clock m_clock;
    sf::Time m_elapsed;
    Tetris tetris;
};

// make it so where setting the intiial velocity changes initVelocity