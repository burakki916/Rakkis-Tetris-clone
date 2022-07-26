#pragma once
#include <iostream>
#include <string>
#include <stack>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <queue>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>   

#include "Window.h"
//maybe i should put this stuff in a namespace?? 

struct directions{
    static sf::Vector2i up;
    static sf::Vector2i down;
    static sf::Vector2i left;
    static sf::Vector2i right; 
};


using tileContainer = std::vector<std::vector<int>>;
using bagContainer = std::queue<int>;
class Tetris
{
public:
    Tetris();
    void initialize();
    void initializeInput(Window* l_window);
    void restart(); 
    void update(); 
    void render(Window* l_window);
    void renderSavedPiece(Window* l_window);
    
    
private:



    enum pieceTypes // the enum value is to allign with the corresponding piece in the piece array
    {
        non =-1,Bar,O,J,L,T,Z,S,
    };
    enum tileTypes{  
        ghost =-1,clear, alive, dead , // more of a self note of what each number means in the grid
    };
    int pieces[7][4][4] = {
        {
            {0, 0, 0, 0},
            {1, 1, 1, 1}, //bar
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0}, //O
            {0, 1, 1, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 1, 0, 0}, //J
            {0, 1, 1, 1},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0}, //L
            {0, 0, 0, 1},
            {0, 1, 1, 1},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},//T
            {0, 0, 1, 0},
            {0, 1, 1, 1},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},//Z
            {0, 1, 1, 0},
            {0, 0, 1, 1},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 0, 1, 1}, //S
            {0, 1, 1, 0},
            {0, 0, 0, 0},
        },
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
    sf::Color pallet[8]={
        sf::Color(114, 159, 250), //blue - bar 
        sf::Color(203, 204, 53), //yellow - O 
        sf::Color(121, 198, 207), //darkblue - J
        sf::Color(218, 113, 84), //orange - L 
        sf::Color(187, 142, 201), //purple - T
        sf::Color(234, 89, 83), //red - Z
        sf::Color(162, 216, 116), //green - S
        sf::Color(41, 45, 62), //background
    };
    enum colors{
        blue, 
        yellow, 
        darkblue, 
        orange,
        purple,
        red,
        green,
        background,
    };
    int pointsForRowsCleared[4] = {40,100,300,1200};
    int score = 0; 
    void updatePiece();
    void solidify();

    void spawner(pieceTypes type = pieceTypes::non);
    void bagGenerator(bagContainer& bag); 
    bool collisionCheck(sf::Vector2i& direction); //ngl not even sure how this is going to work lol 
    void clearCheck();
    void rowDrop();  
    void ghostPiece(bool replace= false); // if set to true, the calculated ghost piece will become real

    void rotateC(EventDetails* l_details);//theres defintely better ways of doing this than what 
    void rotateCC(EventDetails* l_details);// i have set up in these functions. but i think its going to work for now
    bool rotationCheck(int (&testMatrix)[4][4]);
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
    const int bonusHeight = 1; 
    const int verticalBlocks = 20 + bonusHeight; 
    
    tileContainer board;
    std::vector<std::vector<colors>> boardColors;
    sf::Vector2f boardSizeRel = sf::Vector2f(1,1);
    sf::Vector2f boardSizeExact = sf::Vector2f(0,0);
    sf::Vector2f boardPos= sf::Vector2f(0,0);
    sf::Vector2f margin =  sf::Vector2f(0,0);
    float tileBoarderSize = 1;
    float gravity = 400; // in how many miliseconds till it increments 
    //need to figure out a way to make the gravity match the real games 
    int queueSize = 4; 

    pieceEntity fallingPiece;
    pieceTypes savedPiece = pieceTypes::non; 
    bool usedSave = false;   
    bagContainer bag1;
    bagContainer bag2;

    sf::Font textFont; 
    sf::Text scoreText; 
    void handleText(Window* window); 
      
};