#pragma once
#include <ctime>
#include <stdio.h>      
#include <stdlib.h>    
#include <time.h>   
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Window.h"
#include <cmath>
#include "Tetris.h"
class Game
{
public:
	Game();
	~Game();
	void handleInput();
	void Update();
	void Render();
	Window* GetWindow();
	sf::Time GetElapsed();
	void RestartClock(); 
	void MoveSprite(EventDetails* l_details); 
private:
	Window m_window;
	sf::Vector2f m_increment;
	sf::Vector2u windowSize =sf::Vector2u(1020,720);
	sf::Clock m_clock; 
	sf::Time m_elapsed;
	sf::RectangleShape myBox;
	Tetris tetris; 
	
};

// make it so where setting the intiial velocity changes initVelocity