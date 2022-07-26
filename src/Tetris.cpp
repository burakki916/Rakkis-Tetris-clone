#include "Tetris.h"
//direction defs 
sf::Vector2i directions::up = sf::Vector2i(0,-1);
sf::Vector2i directions::down = sf::Vector2i(0,1);
sf::Vector2i directions::left = sf::Vector2i(-1,0);
sf::Vector2i directions::right = sf::Vector2i(1,0); 


Tetris::Tetris(){//ugg i gottta fix this default constructor shit later; 
    
    initialize(); 
    //idk do some initializtion shit here or smth 
    if(!textFont.loadFromFile("./wendy.ttf")){
        std::cout << "failed to load font! " << std::endl; 
    }
}
void Tetris::initialize(){
    //insert code for initializing the event manager to make bindings for the various things that require input 

    board.clear(); 
    boardColors.clear(); 
    //make the board 
    for(int y = 0; y < verticalBlocks; y++){
        std::vector<int> curRow;
        std::vector<colors> curColorRow;  
        for(int x =0; x<horizontalBlocks; x++){
            curRow.push_back(0);
            curColorRow.push_back(colors::background); 
        }
        board.push_back(curRow);
        boardColors.push_back(curColorRow); 
    }
    //load pieces into the queue 
    bagGenerator(bag1);
    bagGenerator(bag2);
    savedPiece = pieceTypes::non;

    spawner();

}
void Tetris::restart(){
    initialize(); 
}
void Tetris::initializeInput(Window* l_window){
    std::cout << "trying to handle input" << std::endl; 
    l_window->GetEventManager()->AddCallback("left", &Tetris::left, this);
    l_window->GetEventManager()->AddCallback("right", &Tetris::right, this);
    l_window->GetEventManager()->AddCallback("up", &Tetris::up, this);
    l_window->GetEventManager()->AddCallback("down", &Tetris::down, this);
    l_window->GetEventManager()->AddCallback("rotateC", &Tetris::rotateC, this);
    l_window->GetEventManager()->AddCallback("rotateCC", &Tetris::rotateCC, this);
    l_window->GetEventManager()->AddCallback("save", &Tetris::savePiece, this);
    l_window->GetEventManager()->AddCallback("hardDrop", &Tetris::hardDrop, this);
    l_window->GetEventManager()->Update(); 
} 
void Tetris::update(){
    //code for updating the position of the falling piece 
    updatePiece(); 
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
    float tileWidth = l_window->GetWindowSize().x/(horizontalBlocks); //adjust the way the board size and position is determined to be more dynamic. 
    float tileHeight = l_window->GetWindowSize().y/(verticalBlocks - bonusHeight);
    if(tileWidth>tileHeight) tileWidth = tileHeight; else tileHeight = tileWidth;

    margin = sf::Vector2f((l_window->GetWindowSize().x-(horizontalBlocks*tileWidth))/2,0);
    boardPos = margin; 
    boardSizeExact = sf::Vector2f(tileWidth*horizontalBlocks, l_window->GetWindowSize().y * boardSizeRel.y);
    
    sf::RectangleShape background; 
    background.setSize(boardSizeExact);
    background.setPosition(boardPos);
    background.setFillColor(sf::Color::Black);
    l_window->Draw(background);//makes the background white
    
    for(int y = bonusHeight; y < verticalBlocks; y++){ 
        for(int x =0; x<horizontalBlocks; x++){
           sf::Vector2f position; 
           position.x = boardPos.x+x*tileWidth;
           position.y = (boardPos.y+y-bonusHeight)*tileHeight;

           sf::RectangleShape rectangle;
           rectangle.setSize(sf::Vector2f(tileWidth-(2*tileBoarderSize),tileHeight-(2*tileBoarderSize)));
           rectangle.setPosition(position.x+ tileBoarderSize,position.y+tileBoarderSize);
            if(board[y][x]!=tileTypes::clear){
                rectangle.setFillColor(pallet[boardColors[y][x]]);
            } else {
                rectangle.setFillColor(pallet[colors::background]);
            }
            
           l_window->Draw(rectangle);     
        }
    }
    //place here code for rendering the currently falling piece 
    //nvm i think i can take care of it in the above code

    //place here code for rendering the pieces in the queue 

    //place rendering code for the saved piece 

    float savedPadding = 0.1;
    sf::Vector2f savedTilePosition = sf::Vector2f((margin.x *savedPadding),margin.x *savedPadding);
    float savedTileSize = (margin.x * (1-savedPadding)) / 8; 

    if(savedPiece!=pieceTypes::non){
        for(int y = 0; y<4; y++){
            for(int x =0; x<4; x++){
                if(pieces[savedPiece][y][x]==1){
                    sf::RectangleShape tile; 
                    tile.setSize(sf::Vector2f(savedTileSize-1,savedTileSize-1));
                    tile.setPosition(savedTilePosition.x+ x*savedTileSize,savedTilePosition.y+ y*savedTileSize);
                    tile.setFillColor(pallet[savedPiece]);
                    l_window->Draw(tile);
                }
            }
        }
    }

    handleText(l_window); //handles rendering the score 
    //place rendering code for various stats 
}
void Tetris::updatePiece(){
    for(int y = 0; y<verticalBlocks; y++){
        for(int x = 0; x<horizontalBlocks; x++){
            if(board[y][x]!=tileTypes::dead){
                board[y][x] = tileTypes::clear;
            }
        }
    }
    elapsed = clock.getElapsedTime();
    if(elapsed.asMilliseconds() >gravity){ //incrementing the piece down
        clock.restart(); 
        if(!collisionCheck(directions::down)){
             fallingPiece.position +=directions::down;
        }else{
            solidify(); //kills the active piece 
            clearCheck(); //check to see if theres any rows filled 
            spawner(); // spawns a new piece from the bag 
        }
    }

    for(int y =0; y<4; y++){ //updates tiles on the board to match the piece 
        for(int x =0; x<4; x++){
            int xBlock = fallingPiece.position.x + x;
            int yBlock =  fallingPiece.position.y + y;
            //std::cout << "potential Block : (" << xBlock << "," << yBlock << ")" <<std::endl;
            if(fallingPiece.pieceArray[y][x] ==  tileTypes::alive){//check to see if in the piece array, if the piece is alive
                board[yBlock][xBlock] =1; //updates the board to make that part alive
                boardColors[yBlock][xBlock] = (colors)fallingPiece.name;  
                //the next else statement is no longer needed. 
            }else{ //if the array piece isnt alive, make sure its not alive on the board // 
                // if(board[yBlock][xBlock] != tileTypes::dead){ //making sure it doesnt clear an already dead piece 
                //     board[yBlock][xBlock] = tileTypes::clear; //clears the piece now that its not there anymore
                // }
            }
            
        }
    }
}
void Tetris::solidify(){
    usedSave = false; 
    for(int y =0; y<4; y++){
        for(int x =0; x<4; x++){
            if(fallingPiece.pieceArray[y][x]==tileTypes::alive){
                int xBlock = fallingPiece.position.x + x;
                int yBlock =  fallingPiece.position.y + y;
                board[yBlock][xBlock] = tileTypes::dead;   
            }
        }
    }
    clearCheck(); 
}

void Tetris::spawner(pieceTypes type){
    
    //make code that spawns each piece in the proper location 
    if(bag2.size()==0){
        bagGenerator(bag2);
    }

    fallingPiece.position = sf::Vector2i((horizontalBlocks/2)-2,0);
    if(type == pieceTypes::non ){
        fallingPiece.name = (pieceTypes)bag1.front();
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
    sf::Vector2i nullDirection = sf::Vector2i(0,0);
    if(collisionCheck(nullDirection)){
        restart();  
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
    //return false; 
    //make code to set the boundaries floor - wall 
    for(int y =0;  y<4; y++){ //def going to need to go back over this and see if it works 
        for(int x = 0; x<4; x++ ){
            if(fallingPiece.pieceArray[y][x] == 1){
                int xCheck = fallingPiece.position.x + x + direction.x;
                int yCheck =  fallingPiece.position.y + y + direction.y;

                //boundary checks 
                if(xCheck>=horizontalBlocks || xCheck<0){
                    std::cout << "horizontal boundary" << std::endl; 
                    return true; 
                }
                if(yCheck>=verticalBlocks|| yCheck<0){
                    std::cout << "Verticle Boundary" << std::endl; 
                    return true; 
                }
                //dead pieces check 
               // std::cout << "Checking : (" << xCheck << "," << yCheck << ")" << std::endl;

                if(board[yCheck][xCheck] == tileTypes::dead){//chance for this to go out of bounds and crash 
                                    //hopefully it doesnt because of the before checks though 
                    std::cout << "dead found" << std::endl;
                    //std::cout << "(" << xCheck << "," << yCheck <<")"<< std::endl;

                    

                    return true; 
                }
            }
        }
    }
    return false; 
}
void Tetris::clearCheck(){
    int cleared[4] = {0,0,0,0};
    int i =-1;   
    for(int y=(verticalBlocks-1); y>=0; y--){
        int RowCount = 0;
        for(int x = 0; x<horizontalBlocks; x++){
            if(board[y][x] >1) RowCount++; 
        }
        //std::cout << "Y : " << RowCount << std::endl; 
        if(RowCount==horizontalBlocks){
            i++; 
            cleared[i] = y; 
            //store which row it happened on 
            //develop an alogirthm that efficiently activates gravity on the remaining tiles 
        }
    }
    std::cout << "we have " << i << "rows to clear " << std::endl;
    if(i!=-1) score+=pointsForRowsCleared[i]; //adds points depending on how many rows were filled

    for(auto itr : cleared){
        std::cout << itr << ","; 
    }
    std::cout << std::endl; 
    while(i>=0){
        for(int y = cleared[i]; y>=1;y--){
            for(int x = 0; x<horizontalBlocks; x++){
                board[y][x] = board[y-1][x];
            }
        }
        i--;
    }
}
void Tetris::rowDrop(){//addparemeters to know which rows need fixing
    //develop an alogirthm that efficiently activates gravity on the remaining tiles 
    //also be sure to reward the player with score, depending on various factors 
}
void Tetris::ghostPiece(bool replace){
    //make code to render and potentially place the harddrop where the ghost is
    // medium priority 
}

void Tetris::rotateC(EventDetails* l_details){
    if(fallingPiece.name == pieceTypes::O) return; 
    int mod =0; //controls which rotation scheme to use. 0 for the bar, 1 for everything else. 
    if(fallingPiece.name > pieceTypes::O){
        mod =1; //kinda messed around with rotating arrays untill it worked
    }            //so dont expect an explanation lol 
    int newMatrix[4][4] ={
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        };

    for(int y = mod; y<4; y++){
        for(int x = mod; x<4; x++){
            newMatrix[y][x] = fallingPiece.pieceArray[(3+mod)-x][y];
        }
    }
    newMatrix[0][0] = 0; // trying to see if this will fix a bug, though this method is rather crude 
    if(rotationCheck(newMatrix)){
        for(int y = 0; y<4; y++){//i feel like theres a more efficient way of doing this, but idfc rn 
            for(int x = 0; x<4; x++){
                fallingPiece.pieceArray[y][x] = newMatrix[y][x];
            }
        }
    }
     
}
void Tetris::rotateCC(EventDetails* l_details){
    //yes im copying code, kill me. easiest way to do it working with the event manager i got set up 
    if(fallingPiece.name == pieceTypes::O) return; 
    int mod =0; 
    if(fallingPiece.name > pieceTypes::O){
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
    if(rotationCheck(newMatrix)){
        for(int y = 0; y<4; y++){//i feel like theres a more efficient way of doing this, but idfc rn 
            for(int x = 0; x<4; x++){
                fallingPiece.pieceArray[y][x] = newMatrix[y][x];
            }
        }
    }

}

bool Tetris::rotationCheck(int (&testMatrix)[4][4]){ //returns true if the test matrix is valid
    for(int y =0; y<4;y++){

        for(int x=0; x<4; x++){
            if(testMatrix[y][x]!=1) continue; 
            int xCheck = fallingPiece.position.x + x; 
            int yCheck = fallingPiece.position.y + y;

            if(xCheck>=horizontalBlocks || xCheck<0){//write some code to auto move the piece so the rotation can happen 
            //adjust the move commands to take in increments for numbers.
            //potentially call the collision check in the move commands instead of it being everywhere else
                std::cout << "horizontal boundary" << std::endl; 
                return false; 
            }
            if(yCheck>=(verticalBlocks-bonusHeight)|| yCheck<0){
                std::cout << "Verticle Boundary" << std::endl; 
                return false; 
            }
            if(board[yCheck][xCheck]==dead){
                return false; 
            }
        }
    }
    return true; 
}

void Tetris::savePiece(EventDetails* l_details){
    std::cout << "trying to save a piece!" << std::endl; 
    if(usedSave) return;
    std::cout << "you can use save!" << std::endl; 
    usedSave=true; 

    if(savedPiece==pieceTypes::non){
        std::cout << "there was no piece in your saved already" << std::endl; 
        savedPiece = fallingPiece.name;
        spawner();
    }else{
        std::cout << "there was a piece in your saved already" << std::endl; 
        auto tempPiece = fallingPiece.name; 
        spawner(savedPiece);
        savedPiece = tempPiece; 
    }
}
void Tetris::hardDrop(EventDetails* l_details){
    return;
    ghostPiece(true); //probably unfitting leaving this in ghostDrop but the code in ghostdrop is so simular might as well
}

//movement 
void Tetris::up(EventDetails* l_details){
    //std::cout << "pop!" << std::endl;
    if(fallingPiece.position.y == 0) return; 
    if(!collisionCheck(directions::up)){
        //fallingPiece.position += directions::up; //is only used for debugging purposes. commented out for playing
    }
}
void Tetris::down(EventDetails* l_details){
    //std::cout << "pop!" << std::endl; 
    if(!collisionCheck(directions::down)){
        fallingPiece.position += directions::down;
    }
}
void Tetris::left(EventDetails* l_details){
    //std::cout << "pop!" << std::endl; 
    if(!collisionCheck(directions::left)){
        fallingPiece.position += directions::left;
    }
}
void Tetris::right(EventDetails* l_details){
    //std::cout << "pop!" << std::endl; 
    if(!collisionCheck(directions::right)){
        fallingPiece.position += directions::right;
    }
}

void Tetris::handleText(Window* window){
    scoreText.setFont(textFont);
    scoreText.setString ("Score : \n " + std::to_string(score)); 
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(pallet[colors::blue]); //add your white then make it this
    scoreText.setPosition(window->GetWindowSize().x*.80, 100); //make this better 
    window->Draw(scoreText); 
}