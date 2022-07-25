#include "Tetris.h"
Tetris::Tetris(){//ugg i gottta fix this default constructor shit later; 
    initialize(); 
    //idk do some initializtion shit here or smth 
}
void Tetris::initialize(){
    //insert code for initializing the event manager to make bindings for the various things that require input 

    //make the board 
    for(int y = 0; y < verticalBlocks; y++){
        std::vector<int> curRow; 
        for(int x =0; x<horizontalBlocks; x++){
            curRow.push_back(0);
        }
        board.push_back(curRow);
    }
    //load pieces into the queue 
    bagGenerator(bag1);
    bagGenerator(bag2);

}
void Tetris::update(){
    //code for updating the position of the falling piece 
    elapsed = clock.getElapsedTime();
    if(elapsed.asMilliseconds() >(1/gravity)){
        if(!collisionCheck(directions::down)){
            fallingPiece.position +=directions::down; 
        }else{
            spawner();
            clearCheck(); 
        }
    } 
    //if bag2.size = 0 bagGenerator(bag2)
    /*
    ---Bag Generation 
        -In the beginning the game generates the 2 bags 
        -Everytime a piece is used, the latest piece from bag 2 inserts at the end of bag 1
        -the bags are each 7 pieces big (every bag contains one of each piece)
        -bag 2 is only regenerated once bag 2 is empty
    */
}
void Tetris::render(Window* l_window){
    
    float tileWidth = l_window->GetWindowSize().x/(horizontalBlocks);
    float tileHeight = l_window->GetWindowSize().y/(verticalBlocks);
    if(tileWidth>tileHeight) tileWidth = tileHeight; else tileHeight = tileWidth;

    margin = sf::Vector2f((l_window->GetWindowSize().x-(horizontalBlocks*tileWidth))/2,0);
    boardPos = margin; 

    sf::Vector2f boardSizeExact = sf::Vector2f(l_window->GetWindowSize().x * boardSizeRel.x, l_window->GetWindowSize().y * boardSizeRel.y);
    for(int y = 0; y < verticalBlocks; y++){ 
        for(int x =0; x<horizontalBlocks; x++){
           sf::Vector2f position; 
           position.x = boardPos.x+x*tileWidth;
           position.y = boardPos.y+y*tileHeight;

           sf::RectangleShape rectangle;
           rectangle.setSize(sf::Vector2f(tileWidth-(2*tileBoarderSize),tileHeight-(2*tileBoarderSize)));
           rectangle.setPosition(position.x+ tileBoarderSize,position.y+tileBoarderSize);
            switch(board[y][x]){ //i forgot to worry about colors, so now everything is going to look weird for now 
                case clear :
                    rectangle.setFillColor(sf::Color::Black);
                    break;  
                case alive :
                    rectangle.setFillColor(sf::Color::Cyan);
                    break;

                case dead :
                    rectangle.setFillColor(sf::Color::Red);
                    break;   
                case wall : 
                    rectangle.setFillColor(sf::Color::Yellow);
                    break;  
                case ghost :
                    rectangle.setFillColor(sf::Color::Blue);
                    break;  
                default : 
                    rectangle.setFillColor(sf::Color::Black);
                    break;  
            } 
            if(board[y][x] == 0){
                rectangle.setFillColor(sf::Color::Cyan);//make this a more detailed switch statement 
            }
             

           l_window->Draw(rectangle);     
        }
    }
    //place here code for rendering the currently falling piece 
    //nvm i think i can take care of it in the above code

    //place here code for rendering the pieces in the queue 

    //place rendering code for the saved piece 

    //place rendering code for various stats 

}
void Tetris::spawner(pieceTypes type = pieceTypes::non){
    if(bag2.size()==0){
        bagGenerator(bag2);
    }

    fallingPiece.position = sf::Vector2i(horizontalBlocks/2,0);
    if(type == pieceTypes::non ){
        fallingPiece.name = bag1.front();
        bag1.pop();
        bag1.emplace(bag2.front());
        bag2.pop(); 

    }else{
        fallingPiece.name = type; 
    }  
    for(int y = 0; y<4; y++){//changing the falling piece to be whatever is next in the queue
        for(int x=0; x<4; x++){
            fallingPiece.pieceArray[y][x] = pieces[fallingPiece.name][y][x];
        }
    }

}
void Tetris::bagGenerator(bagContainer& bag){
    int order[7] = {0,1,2,3,4,5,6};  
    for(int i =0; i <20; i++){ //goes through and randomly swaps pairs - shuffling the bag 
        int rand1 = rand() % 7;
        int rand2 = rand() % 7; //more test required to see if this is a valid way of shuffling the bag 
        while(rand1==rand2){//if the numbers are the same, it'll swap numbers of the second one. 
            rand2 = rand() % 7;
        }
        std::swap(order[rand1],order[rand2]);
    }

    for(int i = 0; i<7; i++){
        bag.emplace((pieceTypes)order[i]);
    }
}
bool Tetris::collisionCheck(sf::Vector2i& direction){
    for(int y =0;  y<4; y++){ //def going to need to go back over this and see if it works 
        for(int x = 0; x<4; x++ ){
            if(fallingPiece.pieceArray[y][x] == 1){
                int xCheck = fallingPiece.position.x + x + direction.x;
                int yCheck =  fallingPiece.position.y + y + direction.y;
                if(board[yCheck][xCheck]!=0){
                    return true; 
                }
            }
        }
    }
    return false; 
}
void Tetris::clearCheck(){
    for(int y=(verticalBlocks-1); y<=0; y++){
        int RowCount = 0; 
        for(int x = 0; x<horizontalBlocks; x++){
            if(board[y][x] >1) RowCount++; 
        }
        if(RowCount==horizontalBlocks){
            //store which row it happened on 
            //develop an alogirthm that efficiently activates gravity on the remaining tiles 
        }
    }
}
void Tetris::rowDrop(){//addparemeters to know which rows need fixing
    //develop an alogirthm that efficiently activates gravity on the remaining tiles 
    //also be sure to reward the player with score, depending on various factors 
}
void Tetris::ghostPiece(bool replace=false){
    //make code to render and potentially place the harddrop where the ghost is
    // medium priority 
}

void Tetris::rotateC(EventDetails* l_details){
    if(fallingPiece.name == pieceTypes::o) return; 
    int mod =0; //controls which rotation scheme to use. 0 for the bar, 1 for everything else. 
    if(fallingPiece.name > pieceTypes::o){
        mod =1; //kinda messed around with rotating arrays untill it worked
    }            //so dont expect an explanation lol 
    int newMatrix[4][4] ={
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        };

    for(int y = 0; y<4; y++){
        for(int x = 0; x<4; x++){
            newMatrix[y][x] = fallingPiece.pieceArray[(3+mod)-x][y];
        }
    }
    for(int y = 0; y<4; y++){//i feel like theres a more efficient way of doing this, but idfc rn 
        for(int x = 0; x<4; x++){
            fallingPiece.pieceArray[y][x] = newMatrix[y][x];
        }
    }
     
}
void Tetris::rotateCC(EventDetails* l_details){
    //yes im copying code, kill me. easiest way to do it working with the event manager i got set up 
    if(fallingPiece.name == pieceTypes::o) return; 
    int mod =0; 
    if(fallingPiece.name > pieceTypes::o){
        mod =1; 
    } 
    int newMatrix[4][4] ={
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        };

    for(int y = 0; y<4; y++){
        for(int x = 0; x<4; x++){
            newMatrix[y][x] = fallingPiece.pieceArray[x][(3+mod)-y];
        }
    }

    for(int y = 0; y<4; y++){//i feel like theres a more efficient way of doing this, but idfc rn 
        for(int x = 0; x<4; x++){
            fallingPiece.pieceArray[y][x] = newMatrix[y][x];
        }
    }
}
void Tetris::savePiece(EventDetails* l_details){
    if(savedPiece==pieceTypes::non){
        savedPiece = fallingPiece.name;
        spawner();
    }else{
        auto tempPiece = fallingPiece.name; 
        spawner(savedPiece);
        savedPiece = tempPiece; 
    }
}
void Tetris::hardDrop(EventDetails* l_details){
    ghostPiece(true); //probably unfitting leaving this in ghostDrop but the code in ghostdrop is so simular might as well
}

//movement 
void Tetris::up(EventDetails* l_details){
    if(!collisionCheck(directions::up)){
        fallingPiece.position += directions::up;
    }
}
void Tetris::down(EventDetails* l_details){
    if(!collisionCheck(directions::down)){
        fallingPiece.position += directions::down;
    }
}
void Tetris::down(EventDetails* l_details){
    if(!collisionCheck(directions::left)){
        fallingPiece.position += directions::left;
    }
}
void Tetris::down(EventDetails* l_details){
    if(!collisionCheck(directions::right)){
        fallingPiece.position += directions::right;
    }
}

