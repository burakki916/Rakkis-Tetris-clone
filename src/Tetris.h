#pragma once
#include <iostream>
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <queue>
#include "Window.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>   

using tileContainer = std::vector<std::vector<int>>;
using bagContainer = std::queue<pieceTypes>;
class Tetris
{
public:
    Tetris(); 
    void initialize();
    void update(); 
    void render(Window* l_window);
    
private:
    void spawner(pieceTypes type = pieceTypes::non);
    void bagGenerator(bagContainer& bag); 
    bool collisionCheck(sf::Vector2i& direction); //ngl not even sure how this is going to work lol 
    void clearCheck();
    void rowDrop();  
    void ghostPiece(bool replace= false); // if set to true, the calculated ghost piece will become real

    void rotateC(EventDetails* l_details);//theres defintely better ways of doing this than what 
    void rotateCC(EventDetails* l_details);// i have set up in these functions. but i think its going to work for now
    void savePiece(EventDetails* l_details);
    void hardDrop(EventDetails* l_details);

    //movement 
    void up(EventDetails* l_details);
    void down(EventDetails* l_details);
    void left(EventDetails* l_details);
    void right(EventDetails* l_details); 

    //timing 
    sf::Clock clock; 
	sf::Time elapsed; 

    const int horizontalBlocks = 10;  
    const int verticalBlocks = 20;
    tileContainer board;
    sf::Vector2f boardSizeRel = sf::Vector2f(1,1);
    sf::Vector2f boardPos= sf::Vector2f(0,0);
    sf::Vector2f margin =  sf::Vector2f(0,0);
    float tileBoarderSize = 1;
    float gravity = 2000; //in tiles per milisecond
    int queueSize = 4; 

    pieceEntity fallingPiece;
    pieceTypes savedPiece;  
    bagContainer bag1;
    bagContainer bag2; 
    
      
};
//maybe i should put this stuff in a namespace?? 


enum pieceTypes // the enum value is to allign with the corresponding piece in the piece array
{
    non =-1,l,o,t,l,j,z,s,
};
enum tileTypes{  
    ghost =-1,clear, alive, dead , // more of a self note of what each number means in the grid
};
int pieces[1][4][4] = {
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    }
};
struct pieceEntity{
    sf::Vector2i position = sf::Vector2i(0,0);
    pieceTypes name; 
    int pieceArray[4][4]  =
        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        };
};
namespace directions{
    sf::Vector2i up = sf::Vector2i(0,-1);
    sf::Vector2i down = sf::Vector2i(0,1);
    sf::Vector2i left = sf::Vector2i(-1,0);
    sf::Vector2i right = sf::Vector2i(1,0); 
};