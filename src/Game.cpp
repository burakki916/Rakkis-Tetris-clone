#include "Game.h"
Game::Game() : m_window("Rakki's Perfect Tetris Ripoff", sf::Vector2u(1280, 720))
{
    // m_window.ToggleFullscreen();

    // m_window.GetEventManager()->AddCallback("Move", &Game::MoveSprite, this);
    tetris.initialize();
    tetris.initializeInput(&m_window);
}
Game::~Game()
{
}

void Game::handleInput()
{
    // TODO: HANDLE INPUT!!
    //! or else something bad will happen
    //*important
    //** suport important
    //? hello?
    //@param myParam this
}

void Game::Update()
{
    m_window.Update();
    // m_elapsed = m_clock.getElapsedTime();
    // float timestep = 1.0f / 0.011;
    // if (m_elapsed > sf::seconds(5)) {

    // }
    tetris.update();
}

void Game::Render()
{
    m_window.BeginDraw(); // clears screen
    tetris.render(&m_window);
    m_window.EndDraw(); // display
}

Window *Game::GetWindow()
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
