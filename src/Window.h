#pragma once
#include "EventManager.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>

// # define M_PI           3.14159265358979323846  /* pi */
// # define M_G			9.8
class Window
{
  public:
    Window();
    Window(const std::string &l_title, const sf::Vector2u &l_size);
    ~Window();
    void Close(EventDetails *l_details = nullptr);

    void BeginDraw(); // clears the window
    void EndDraw();   // display the changes

    void Update();

    bool IsDone();
    bool IsFullScreen();
    bool IsFocused();
    sf::Vector2u GetWindowSize();

    void ToggleFullscreen(EventDetails *l_details);
    void Draw(sf::Drawable &l_drawable);
    sf::RenderWindow *GetRenderWindow();
    EventManager *GetEventManager();

  private:
    void Setup(const std::string &l_title, const sf::Vector2u &l_size);
    void Destroy();
    void Create();
    sf::RenderWindow m_window;
    sf::Vector2u m_windowSize;
    std::string m_windowTitle;
    bool m_isDone;
    bool m_isFullScreen;
    EventManager m_eventManager;
    bool m_isFocused;
    void cunt(EventDetails *l_details)
    {
        std::cout << "cunt funcy" << std::endl;
    }
};
