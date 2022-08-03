#include "Tetris.h"
// direction defs
sf::Vector2i directions::up = sf::Vector2i(0, -1);
sf::Vector2i directions::down = sf::Vector2i(0, 1);
sf::Vector2i directions::left = sf::Vector2i(-1, 0);
sf::Vector2i directions::right = sf::Vector2i(1, 0);

Tetris::Tetris()
{ // ugg i gottta fix this default constructor shit later;

    initialize();
    // idk do some initializtion shit here or smth
    if (!textFont.loadFromFile("./wendy.ttf"))
    {
        std::cout << "failed to load font! " << std::endl;
    }

    if (!music.openFromFile("tetris.wav"))
    {
        std::cout << "failed to load music!" << std::endl;
    }
    music.play();
    music.setVolume(25);
    music.setLoop(true);
}
void Tetris::initialize()
{
    // insert code for initializing the event manager to make bindings for the
    // various things that require input
    totalRowsCleared = 0;
    board.clear();
    boardColors.clear();
    score = 0;
    level = 1;
    // make the board
    for (int y = 0; y < verticalBlocks; y++)
    {
        std::vector<int> curRow;
        std::vector<colors> curColorRow;
        for (int x = 0; x < horizontalBlocks; x++)
        {
            curRow.push_back(0);
            curColorRow.push_back(colors::background);
        }
        board.push_back(curRow);
        boardColors.push_back(curColorRow);
    }
    // load pieces into the queue
    bagGenerator(bag1);
    bagGenerator(bag2);
    savedPiece = pieceTypes::non;

    spawner();
}
void Tetris::restart()
{
    std::cout << "restarting!!" << std::endl; //!
    initialize();
}
void Tetris::initializeInput(Window *l_window)
{
    std::cout << "trying to handle input" << std::endl;
    l_window->GetEventManager()->AddCallback("left", &Tetris::left, this);
    l_window->GetEventManager()->AddCallback("leftU", &Tetris::leftU, this);
    l_window->GetEventManager()->AddCallback("right", &Tetris::right, this);
    l_window->GetEventManager()->AddCallback("rightU", &Tetris::rightU, this);
    l_window->GetEventManager()->AddCallback("up", &Tetris::up, this);
    l_window->GetEventManager()->AddCallback("down", &Tetris::down, this);
    l_window->GetEventManager()->AddCallback("downU", &Tetris::downU, this);
    l_window->GetEventManager()->AddCallback("rotateC", &Tetris::rotateC, this);
    l_window->GetEventManager()->AddCallback("rotateCC", &Tetris::rotateCC, this);
    l_window->GetEventManager()->AddCallback("save", &Tetris::savePiece, this);
    l_window->GetEventManager()->AddCallback("hardDrop", &Tetris::hardDrop, this);
    l_window->GetEventManager()->Update();
}
void Tetris::update()
{
    handleMovement();
    // code for updating the position of the falling piece
    for (int y = 0; y < verticalBlocks; y++)
    { // make this into a function called clear board sometime
        for (int x = 0; x < horizontalBlocks; x++)
        {
            if (board[y][x] != tileTypes::dead)
            {
                board[y][x] = tileTypes::clear;
            }
        }
    }
    updateGhostPiece();
    updatePiece();
    // if bag2.size = 0 bagGenerator(bag2)

    // gravity formula, made by me. prob bad but idc
}
void Tetris::render(Window *l_window)
{
    float tileWidth = l_window->GetWindowSize().x / (horizontalBlocks); // adjust the way the board size and position is
                                                                        // determined to be more dynamic.
    float tileHeight = l_window->GetWindowSize().y / (verticalBlocks - bonusHeight);
    if (tileWidth > tileHeight)
        tileWidth = tileHeight;
    else
        tileHeight = tileWidth;

    margin = sf::Vector2f((l_window->GetWindowSize().x - (horizontalBlocks * tileWidth)) / 2, 0);
    boardPos = margin;
    boardSizeExact = sf::Vector2f(tileWidth * horizontalBlocks, l_window->GetWindowSize().y * boardSizeRel.y);

    sf::RectangleShape background;
    background.setSize(boardSizeExact);
    background.setPosition(boardPos);
    background.setFillColor(sf::Color::Black);
    l_window->Draw(background); // makes the background white

    for (int y = bonusHeight; y < verticalBlocks; y++)
    {
        for (int x = 0; x < horizontalBlocks; x++)
        {
            sf::Vector2f position;
            position.x = boardPos.x + x * tileWidth;
            position.y = (boardPos.y + y - bonusHeight) * tileHeight;

            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(tileWidth - (2 * tileBoarderSize), tileHeight - (2 * tileBoarderSize)));
            rectangle.setPosition(position.x + tileBoarderSize, position.y + tileBoarderSize);
            if (board[y][x] != tileTypes::clear)
            {
                sf::Color drawColor = pallet[boardColors[y][x]];
                if (board[y][x] == tileTypes::ghost)
                {
                    drawColor.r *= .20;
                    drawColor.g *= .20;
                    drawColor.b *= .20;
                }

                rectangle.setFillColor(drawColor);
            }
            else
            {
                rectangle.setFillColor(pallet[colors::background]);
            }

            l_window->Draw(rectangle);
        }
    }
    // place here code for rendering the currently falling piece
    // nvm i think i can take care of it in the above code

    // place here code for rendering the pieces in the queue

    // place rendering code for the saved piece

    float savedPadding = 0.1;
    sf::Vector2f savedTilePosition = sf::Vector2f((margin.x * savedPadding), margin.x * savedPadding);
    float savedTileSize = (margin.x * (1 - savedPadding)) / 8;

    if (savedPiece != pieceTypes::non)
    {
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (pieces[savedPiece][y][x] == 1)
                {
                    sf::RectangleShape tile;
                    tile.setSize(sf::Vector2f(savedTileSize - 1, savedTileSize - 1));
                    tile.setPosition(savedTilePosition.x + x * savedTileSize, savedTilePosition.y + y * savedTileSize);
                    tile.setFillColor(pallet[savedPiece]);
                    l_window->Draw(tile);
                }
            }
        }
    }
    savedTileSize = (margin.x * (1 - savedPadding)) / 20;
    for (int i = 0; i < 4; i++)
    {
        int pieceToRender = bag1[i];
        sf::Vector2f queueNumPosition = sf::Vector2f(savedTilePosition.x, i * savedTileSize * 4 * 1.20 + 300);
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (pieces[pieceToRender][y][x] == 1)
                {
                    sf::RectangleShape tile;
                    tile.setSize(sf::Vector2f(savedTileSize - 1, savedTileSize - 1));
                    tile.setPosition(queueNumPosition.x + x * savedTileSize, queueNumPosition.y + y * savedTileSize);
                    tile.setFillColor(pallet[pieceToRender]);
                    l_window->Draw(tile);
                }
            }
        }
    }

    handleText(l_window); // handles rendering the score
                          // place rendering code for various stats
}
void Tetris::updatePiece()
{
    elapsed = clock.getElapsedTime();
    float gravityToUse = gravity[gravityLevel];
    if (softDropping && softDroppingSpeed < gravity[gravityLevel])
    {
        // std::cout << "using softDropping speed" << std::endl;
        gravityToUse = softDroppingSpeed;
    }
    if (elapsed.asMilliseconds() > gravityToUse)
    { // incrementing the piece down
        clock.restart();
        if (!collisionCheck(directions::down, fallingPiece))
        {
            fallingPiece.position += directions::down;
        }
        else
        {
            std::cout << "collision down detected!" << std::endl;
            updateScores(score, level);
            showScores();
            waitForKeyPress();
            solidify(); // kills the active piece
            spawner();
            clearCheck(); // check to see if theres any rows filled spawner(); //
                          // spawns a new piece from the bag
        }
    }
    updatePieceOnBoard(fallingPiece);
}

void Tetris::updatePieceOnBoard(pieceEntity &curPiece)
{
    for (int y = 0; y < 4; y++)
    { // updates tiles on the board to match the
      // piece
        for (int x = 0; x < 4; x++)
        {
            int xBlock = curPiece.position.x + x;
            int yBlock = curPiece.position.y + y;
            // std::cout << "potential Block : (" << xBlock << "," << yBlock << ")"
            // <<std::endl;
            if (curPiece.pieceArray[y][x] == tileTypes::alive)
            {                                                // check to see if in the piece array, if the
                                                             // piece is alive
                board[yBlock][xBlock] = curPiece.pieceState; // updates the board to make that part alive
                boardColors[yBlock][xBlock] = (colors)curPiece.name;
            }
        }
    }
    //! hello there!
    // std::cout << "------------------"<<std::endl;
    // for(int y = 0; y<verticalBlocks;y++){
    //     for(int x = 0; x<horizontalBlocks; x++){
    //         if(board[y][x] != tileTypes::clear){
    //             std::cout << board[y][x];
    //         }else{
    //             std::cout << ".";
    //         }
    //     }
    //     std::cout << std::endl;
    // }
}
void Tetris::updateGhostPiece()
{
    // make code to render and potentially place the harddrop where the ghost is
    //  medium priority
    ghostPiece = fallingPiece;
    ghostPiece.pieceState = tileTypes::ghost;

    while (!collisionCheck(directions::down, ghostPiece))
    {
        ghostPiece.position.y++;
    }
    updatePieceOnBoard(ghostPiece);
}
void Tetris::solidify()
{
    usedSave = false;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (fallingPiece.pieceArray[y][x] == tileTypes::alive)
            {
                int xBlock = fallingPiece.position.x + x;
                int yBlock = fallingPiece.position.y + y;
                board[yBlock][xBlock] = tileTypes::dead;
            }
        }
    }

    clearCheck();
}

void Tetris::spawner(pieceTypes type)
{
    handleLeveling();
    // make code that spawns each piece in the proper location
    if (bag2.size() == 0)
    {
        // std::cout << "bag2 has been emptied!" << std::endl;
        bagGenerator(bag2);
    }

    fallingPiece.position = sf::Vector2i((horizontalBlocks / 2) - 2, 0);
    if (type == pieceTypes::non)
    {
        fallingPiece.name = (pieceTypes)bag1.front();
        bag1.erase(bag1.begin());
        bag1.push_back(bag2.front());
        bag2.erase(bag2.begin());
    }
    else
    {
        fallingPiece.name = type;
    }
    for (int y = 0; y < 4; y++)
    { // changing the falling piece to be whatever is next in the queue
        for (int x = 0; x < 4; x++)
        {
            fallingPiece.pieceArray[y][x] = pieces[fallingPiece.name][y][x];
        }
    }
    // std::cout << "bag1 : ";
    // for(int i =0; i<bag1.size(); i++){
    //     std::cout << bag1.at(i) << ",";
    // }
    // std::cout << "\n bag2 : ";
    // for(int i =0; i<bag2.size(); i++){
    //     std::cout << bag2.at(i) << ",";
    // }
    // std::cout << "\n";

    sf::Vector2i nullDirection = sf::Vector2i(0, 0);
    if (collisionCheck(nullDirection, fallingPiece))
    {
        restart();
    }
}
void Tetris::bagGenerator(bagContainer &bag)
{
    /*
    ---Bag Generation
        -In the beginning the game generates the 2 bags
        -Everytime a piece is used, the latest piece from bag 2 inserts at the end
    of bag 1 -the bags are each 7 pieces big (every bag contains one of each
    piece) -bag 2 is only regenerated once bag 2 is empty
    */

    int order[7] = {0, 1, 2, 3, 4, 5, 6};
    for (int i = 0; i < 20; i++)
    { // goes through and randomly swaps pairs - shuffling the bag
        int rand1 = rand() % 7;
        int rand2 = rand() % 7; // more test required to see if this is a valid way
                                // of shuffling the bag
        while (rand1 == rand2)
        { // if the numbers are the same, it'll swap numbers
          // of the second one.
            rand2 = rand() % 7;
        }
        std::swap(order[rand1], order[rand2]);
    }
    for (int i = 0; i < 7; i++)
    {
        bag.push_back((pieceTypes)order[i]);
    }
}
bool Tetris::collisionCheck(sf::Vector2i &direction, pieceEntity &curPiece)
{
    // return false;
    // make code to set the boundaries floor - wall
    for (int y = 0; y < 4; y++)
    { // def going to need to go back over this and see if it works
        for (int x = 0; x < 4; x++)
        {
            if (curPiece.pieceArray[y][x] == 1)
            {
                int xCheck = curPiece.position.x + x + direction.x;
                int yCheck = curPiece.position.y + y + direction.y;

                // boundary checks
                if (xCheck >= horizontalBlocks || xCheck < 0)
                {
                    // std::cout << "horizontal boundary" << std::endl;
                    return true;
                }
                if (yCheck >= verticalBlocks || yCheck < 0)
                {
                    // std::cout << "Verticle Boundary" << std::endl;
                    return true;
                }
                // dead pieces check
                // std::cout << "Checking : (" << xCheck << "," << yCheck << ")" <<
                // std::endl;

                if (board[yCheck][xCheck] == tileTypes::dead)
                { // chance for this to go out of bounds and crash
                    // hopefully it doesnt because of the before checks though
                    // std::cout << "dead found" << std::endl;
                    // std::cout << "(" << xCheck << "," << yCheck <<")"<< std::endl;

                    return true;
                }
            }
        }
    }
    return false;
}
void Tetris::clearCheck()
{
    int cleared[4] = {0, 0, 0, 0};
    int i = -1;
    for (int y = (verticalBlocks - 1); y >= 0; y--)
    {
        int RowCount = 0;
        for (int x = 0; x < horizontalBlocks; x++)
        {
            if (board[y][x] > 1)
                RowCount++;
        }
        // std::cout << "Y : " << RowCount << std::endl;
        if (RowCount == horizontalBlocks)
        {
            i++;
            totalRowsCleared++;
            cleared[i] = y;
            // store which row it happened on
            // develop an alogirthm that efficiently activates gravity on the
            // remaining tiles
        }
    }
    // std::cout << "we have " << i << "rows to clear " << std::endl;
    if (i != -1)
        score += (pointsForRowsCleared[i] * (level + 1)); // adds points depending on how many rows were filled

    for (auto itr : cleared)
    {
        // std::cout << itr << ",";
    }
    // std::cout << std::endl;
    while (i >= 0)
    {
        for (int y = cleared[i]; y >= 1; y--)
        {
            for (int x = 0; x < horizontalBlocks; x++)
            {
                board[y][x] = board[y - 1][x];
                boardColors[y][x] = boardColors[y - 1][x];
            }
        }
        i--;
    }
}
void Tetris::rowDrop()
{ // addparemeters to know which rows need fixing
  // develop an alogirthm that efficiently activates gravity on the remaining
  // tiles also be sure to reward the player with score, depending on various
  // factors
}

void Tetris::rotateC(EventDetails *l_details)
{
    if (fallingPiece.name == pieceTypes::O)
        return;
    int mod = 0; // controls which rotation scheme to use. 0 for the bar, 1 for
                 // everything else.
    if (fallingPiece.name > pieceTypes::O)
    {
        mod = 1; // kinda messed around with rotating arrays untill it worked
    }            // so dont expect an explanation lol
    int newMatrix[4][4] = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    };

    for (int y = mod; y < 4; y++)
    {
        for (int x = mod; x < 4; x++)
        {
            newMatrix[y][x] = fallingPiece.pieceArray[(3 + mod) - x][y];
        }
    }
    newMatrix[0][0] = 0; // trying to see if this will fix a bug, though this
                         // method is rather crude
    if (rotationCheck(newMatrix))
    {
        for (int y = 0; y < 4; y++)
        { // i feel like theres a more efficient way of
          // doing this, but idfc rn
            for (int x = 0; x < 4; x++)
            {
                fallingPiece.pieceArray[y][x] = newMatrix[y][x];
            }
        }
    }
}
void Tetris::rotateCC(EventDetails *l_details)
{
    // yes im copying code, kill me. easiest way to do it working with the event
    // manager i got set up
    if (fallingPiece.name == pieceTypes::O)
        return;
    int mod = 0;
    if (fallingPiece.name > pieceTypes::O)
    {
        mod = 1;
    }
    int newMatrix[4][4] = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    };

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            newMatrix[y][x] = fallingPiece.pieceArray[x][(3 + mod) - y];
        }
    }
    if (rotationCheck(newMatrix))
    {
        for (int y = 0; y < 4; y++)
        { // i feel like theres a more efficient way of
          // doing this, but idfc rn
            for (int x = 0; x < 4; x++)
            {
                fallingPiece.pieceArray[y][x] = newMatrix[y][x];
            }
        }
    }
}

bool Tetris::rotationCheck(int (&testMatrix)[4][4])
{ // returns true if the test matrix is valid
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (testMatrix[y][x] != 1)
                continue;
            int xCheck = fallingPiece.position.x + x;
            int yCheck = fallingPiece.position.y + y;

            if (xCheck >= horizontalBlocks || xCheck < 0)
            { // write some code to auto move the piece so the
              // rotation can happen
                // adjust the move commands to take in increments for numbers.
                // potentially call the collision check in the move commands instead of
                // it being everywhere else
                std::cout << "horizontal boundary" << std::endl;
                return false;
            }
            if (yCheck >= (verticalBlocks - bonusHeight) || yCheck < 0)
            {
                std::cout << "Verticle Boundary" << std::endl;
                return false;
            }
            if (board[yCheck][xCheck] == dead)
            {
                return false;
            }
        }
    }
    return true;
}

void Tetris::savePiece(EventDetails *l_details)
{
    std::cout << "trying to save a piece!" << std::endl;
    if (usedSave)
        return;
    std::cout << "you can use save!" << std::endl;
    usedSave = true;

    if (savedPiece == pieceTypes::non)
    {
        std::cout << "there was no piece in your saved already" << std::endl;
        savedPiece = fallingPiece.name;
        spawner();
    }
    else
    {
        std::cout << "there was a piece in your saved already" << std::endl;
        auto tempPiece = fallingPiece.name;
        spawner(savedPiece);
        savedPiece = tempPiece;
    }
}
void Tetris::hardDrop(EventDetails *l_details)
{
    fallingPiece.position = ghostPiece.position;
    solidify(); // kills the active piece
    spawner();
    clearCheck();
}

// movement
void Tetris::up(EventDetails *l_details)
{
    // std::cout << "pop!" << std::endl;
    if (fallingPiece.position.y == 0)
        return;
    if (!collisionCheck(directions::up, fallingPiece))
    {
        // fallingPiece.position += directions::up; //is only used for debugging
        // purposes. commented out for playing
    }
}
void Tetris::down(EventDetails *l_details)
{
    softDropping = true;
}
void Tetris::downU(EventDetails *l_details)
{
    softDropping = false;
}
void Tetris::left(EventDetails *l_details)
{
    if (!collisionCheck(directions::left, fallingPiece))
    {
        fallingPiece.position += directions::left;
        movementClock.restart();
        movementTickTime = 200;
        // updateGhostPiece();
    }
    directionHeld[possibleDirections::lefty] = true;
}
void Tetris::leftU(EventDetails *l_details)
{
    directionHeld[possibleDirections::lefty] = false;
}
void Tetris::right(EventDetails *l_details)
{
    if (!collisionCheck(directions::right, fallingPiece))
    {
        fallingPiece.position += directions::right;
        movementClock.restart();
        movementTickTime = 200;
        // updateGhostPiece();
    }
    directionHeld[possibleDirections::righty] = true;
}
void Tetris::rightU(EventDetails *l_details)
{
    directionHeld[possibleDirections::righty] = false;
}

void Tetris::handleMovement()
{
    movementClockElapsed = movementClock.getElapsedTime();
    if (movementClockElapsed.asMilliseconds() > movementTickTime)
    {
        movementTickTime = 50;
        movementClock.restart();
        if (directionHeld[lefty])
        {
            if (!collisionCheck(directions::left, fallingPiece))
            {
                fallingPiece.position += directions::left;
            }
        }
        if (directionHeld[righty])
        {
            if (!collisionCheck(directions::right, fallingPiece))
            {
                fallingPiece.position += directions::right;
            }
        }
    }
}

void Tetris::handleText(Window *window)
{
    scoreText.setFont(textFont);
    scoreText.setString("Score : \n " + std::to_string(score) + "\n\n Gravity : \n " +
                        std::to_string(gravity[gravityLevel]) + "\n Level: " + std::to_string(level));
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(pallet[colors::blue]); // add your white then make it this
    scoreText.setPosition(window->GetWindowSize().x * .80,
                          100); // make this better
    window->Draw(scoreText);
}

void Tetris::handleLeveling()
{
    if (totalRowsCleared > level * 10)
    {
        std::cout << "totalRowsCleared : " << totalRowsCleared << std::endl;
        level++;
    }

    if (level <= 9)
    {
        gravityLevel = level;
    }
    if (level >= 10)
    {
        gravityLevel = 10;
    }
    if (level >= 13)
    {
        gravityLevel = 11;
    }
    if (level >= 16)
    {
        gravityLevel = 12;
    }
    if (level >= 19)
    {
        gravityLevel = 13;
    }
    if (level >= 29)
    {
        gravityLevel = 14;
    }
}
void Tetris::showScores(Window *l_window)
{
    sf::Vector2f scoreSize = l_window->GetWindowSize().x * .20, l_window->GetWindowSize().y * .80;
    sf::Vector2f scorePosition = sf::Vector2f(l_window->GetWindowSize().x / 2 - scoreSize / 2, 100);
    sf::RectangleShape scoreBackDrop;
    scoreBackDrop.setSize(scoreSize);
    scoreBackDrop.setPosition(scorePosition);
    scoreBackDrop.setFilColor(sf::Color::Black); // to change later
    l_window->Draw(scoreBackDrop);
    for (int i = 0; i < scores.size() || i < 10; i++)
    {
        sf::Text curScore;
        curScore.setFont(textFont);
        curScore.setString(i + ". Score : " + scores[i].first + " Level  : " + scores[i].second);
        curScore.setPosition(scorePosition.x + 10, scorePosition.y + (i * scoreSize - 20) / 10);
        curScore.setFillColor(pallet[colors::blue]);
        curScore.setCharacterSize((scoreSize.y - 50) / 10);
        window->Draw(curScore);
    }
}
void Tetris::updateScores(int latestLevel, int latestScore)
{
    std::string scoreOutput;
    for (auto itr; scores)
    {
        if (latestScore > itr.first)
        {
            scores.insert(itr - 1, std::pair<int, int>(latestScore, latestLevel));
            scoreOutput += latestScore + "," + latestLevel + "\n";
        }
        scoreOutput += itr.first + "," + itr.second + "\n";
    }
    std::ofstream scoreFile;
    scoreFile.open("scores.txt");
    scoreFile << scoreOutput;
}
void Tetris::loadScoreFile()
{
    std::fstream scoreFile;
    scoreFile.open("scores.txt");
    std::string line;
    while (std::getline(scoreFile, line))
    {
        int curScore = std::stoi(line.substr(0, line.find(",")));
        int curLevel = std::stoi(line.substr(line.find(","), line.size()));
        scores.insert(scores.front(), std::pair<int, int>(curScore, curLevel));
    }
}
void Tetris::waitForKeyPress(Window *l_window)
{
    sf::Event event;
    while (l_window->getWindow().pollEvent(event))
    {
        if (event.type == sf::Event::KeyPressed)
        {
            continue;
        }
    }
}