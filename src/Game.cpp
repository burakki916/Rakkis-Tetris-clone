#include "Game.h"
Game::Game()
	:m_window("Rakki's Perfect Tetris Ripoff", sf::Vector2u(1020, 720))
{
	//m_window.ToggleFullscreen();

	m_window.GetEventManager()->AddCallback("Move", &Game::MoveSprite, this);
	tetris.initialize(); 
}
Game::~Game() {}

void Game::handleInput()
{

}

void Game::Update()
{
	m_window.Update(); 
	m_elapsed = m_clock.getElapsedTime(); 
	float timestep = 1.0f / 0.011;
	if (m_elapsed > sf::seconds(5)) {
		//myFirework.SetTrailColors({ sf::Color::Blue, sf::Color::Red });
	}

}

void Game::Render()
{
	m_window.BeginDraw(); //clears screen
	tetris.render(&m_window);
	m_window.EndDraw(); // display
}

Window* Game::GetWindow()
{
	return &m_window;
}

sf::Time Game::GetElapsed()
{
	return m_elapsed; 
}

void Game::RestartClock()
{
	m_elapsed = m_clock.restart(); 
}

void Game::MoveSprite(EventDetails* l_details)
{
	sf::Vector2i mousepos = m_window.GetEventManager()->GetMousePos(m_window.GetRenderWindow());
	myBox.setPosition(mousepos.x, mousepos.y); 
	std::cout << "moving sprite to : (" << mousepos.x << "," << mousepos.y << ")" << std::endl;
}
