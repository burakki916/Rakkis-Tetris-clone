#define SFML_STATIC
#include "Game.h"
#include <conio.h>
#include <fstream>
#include <string>

void keyconfigy();
sf::Keyboard::Key getDown();

int main()
{
    std::string input;
    std::cout << "would you like to change the controls of the game? [Y/n]";
    std::cin >> input;
    if (input == "Y" || input == "y")
    {
        keyconfigy();
    }

    Game newGame;
    while (!newGame.GetWindow()->IsDone())
    {
        newGame.handleInput();
        newGame.Update();
        newGame.Render();
    }
}
void keyconfigy()
{
    std::fstream bindings;
    bindings.open("keys.cfg");
    std::string newBindings;
    if (!bindings.is_open())
    {
        std::cout << "! Failed loading keys.cfg ! " << std::endl;
        return;
    }
    std::string line;
    int number;
    bool ignoreMode = false;
    while (std::getline(bindings, line))
    {
        if (line[0] == '#')
            ignoreMode = true;
        if (ignoreMode)
        {
            newBindings += line + "\n";
            continue;
        }
        if (line.size() == 0)
            continue;
        std::string name = line.substr(0, line.find(" "));
        if (name[name.size() - 1] != 'U')
        {
            std::cout << "what would you like " << name << " to be set to? " << std::endl;
            getch();
            number = getDown();
            if (number == sf::Keyboard::Enter)
            {
                newBindings += line + "\n";
                continue;
            }
            std::cout << "keycode : " << number << "\n\n" << std::endl;
        }
        line.erase(line.begin() + line.find(":") + 1, line.end());
        line += std::to_string(number);
        std::cout << line << std::endl;
        newBindings += line + "\n";
    }
    bindings.close();

    std::ofstream saveFile;
    saveFile.open("keys.cfg");
    saveFile << newBindings;
    bindings.close();
}
sf::Keyboard::Key getDown()
{

    while (true)
    {
        for (int i = 0; i < sf::Keyboard::KeyCount; i++)
        {
            if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i))
            {

                return (sf::Keyboard::Key)i;
            }
        }
    }
    return (sf::Keyboard::Key)-1;
}
