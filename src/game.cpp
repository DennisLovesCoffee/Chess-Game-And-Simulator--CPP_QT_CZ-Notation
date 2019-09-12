/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#include "game.h"
#include <QPixmap>
#include <QDebug>
#include "chessboard.h"
#include "tabmanagment.h"
#include "filework.h"
#include "pawn.h"
#include <QRegularExpression>
#include <QBrush>
#include <QFont>
#include <QTimer>
#include <QMessageBox>

extern TabManagment *tab;
extern QList <Game *> games;
Game::Game(QWidget *parent ):QGraphicsView(parent)
{
    //Making the Scene
    gameScene = new QGraphicsScene();
    gameScene->setSceneRect(0,0,1400,900);

    //Making the view
    setFixedSize(1400,900);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(gameScene);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::lightGray);
    setBackgroundBrush(brush);

    simulationRunning = choosingChessPieceDestBox = beingDisplayed =  false;
    //ADD THIS NULLPTR INSTEAD OF ALL NULL THINGS!!
    GameOverText = nullptr;
    file = nullptr;
    selectedChessPiece = nullptr;
    currentTurn = "WHITE";
    currentTurnText = nullptr;
    gameTranscript = nullptr;
}

void Game::drawChessBoard()
{
    chess = new ChessBoard();
    chess->drawBoxes(width()/2-400,50, gameIdx);
}

void Game::addToScene(QGraphicsItem *item)
{
    if(item)
        gameScene->addItem(item);
   else{
        QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "Trying to add null to gameScene");
    }
}

void Game::removeFromScene(QGraphicsItem *item)
{
    if(item){
        gameScene->removeItem(item);
    }
}

void Game::displayDeadWhite()
{
    int SHIFT = 50;
    int j = 0;
    int k = 0;
    for(size_t i = 0,n = whiteDead.size(); i<n; i++) {
            if(j == 4){
                k++;
                j = 0;
            }
            whiteDead[i]->setPos(30+SHIFT*j++,50+SHIFT*1.5*k);
    }
}

void Game::displayDeadBlack()
{
    int SHIFT = 50;
    int j = 0;
    int k = 0;
    for(size_t i = 0,n = blackDead.size(); i<n; i++) {
        if(j == 4){
            k++;
            j = 0;
        }
        blackDead[i]->setPos(30+SHIFT*j++,470+SHIFT*1.5*k);
    }
}

void Game::placeInDeadPlace(ChessPiece *piece)
{
    if(piece->getSide() == "WHITE") {
        whiteDead.append(piece);
        displayDeadWhite();
    }
    else{
        blackDead.append(piece);
        displayDeadBlack();
    }

    alivePiece.removeAll(piece);

    //check if it is one of the kings, just for good practice
    if(piece == chess->black[4])
        gameOver(false);
    else if(piece == chess->white[12])
        gameOver(true);
}

void Game::start()
{
    //other UI stuff
    drawTextElements();
    drawNumAndLet();

    //get the chessBoard
    drawChessBoard();
    chess->addChessPiece(gameIdx);

    //get the buttons
    displayMainMenu();
}

void Game::jumpToTranscriptPosition(int idx)
{
    if(simulationRunning)
        return;

    chess->reset(gameIdx);
    color = false;
    removeFromScene(GameOverText);
    if(currentTurn != "WHITE")
        changeCurrentTurn();
    selectedChessPiece = nullptr;
    choosingChessPieceDestBox = false;

    file->currentTranscriptIdx = -1;
    while(file->currentTranscriptIdx < idx){
        if(next() == -1){   // if an errorr appeared
            break;
        }
    }

}

void Game::changeCurrentTurn()
{
    if(currentTurn == "BLACK")
        currentTurn = "WHITE";
    else
        currentTurn = "BLACK";

    //if we are just simulating a chessGame
    if(currentTurnText)
        currentTurnText->setPlainText("Turn: " + currentTurn.toLower());
}

void Game::generateEntry(ChessPiece *moved, int retVal, int oldX, int oldY, int newX, int newY)
{
    //a normal move
    //1st : Figure out what piece we are moving with

    QString pieceName = "";

    if(moved->side == "BLACK"){
        for(int idx = 0; idx < chess->black.count(); ++idx){
            if(chess->black[idx] == moved){
                //rook
                if(idx == 0 or idx == 7)
                    pieceName = "V";
                //Horseeee
                else if(idx == 1 or idx == 6)
                    pieceName = "J";
                //bishop
                else if(idx == 2 or idx == 5)
                    pieceName = "S";
                else if(idx == 3)
                    pieceName = "D";
                else if(idx == 4)
                    pieceName = "K";
                break;
            }

        }
    }
    else{
        for(int idx = 0; idx < chess->white.count(); ++idx){
            if(chess->white[idx] == moved){
                //rook
                if(idx == 8 or idx == 15)
                    pieceName = "V";
                //Horseeee
                else if(idx == 9 or idx == 14)
                    pieceName = "J";
                //bishop
                else if(idx == 10 or idx == 13)
                    pieceName = "S";
                else if(idx == 11)
                    pieceName = "D";
                else if(idx == 12)
                    pieceName = "K";
                break;
            }
        }
    }

    //we are at the end and want to exchange
    if(pieceName == "" && retVal >= 2){
        QMessageBox msgBox;
        msgBox.setText("Game:"+QString::number(gameIdx+1) + "Choose what you want the "+moved->side.toLower() +" pawn to be replaced with");
        QAbstractButton *pButtonQueen = nullptr;
        QAbstractButton *pButtonBishop = nullptr;
        QAbstractButton *pButtonRook = nullptr;
        QAbstractButton *pButtonKnight = nullptr;
        QAbstractButton *pButtonPawn = nullptr;

        pButtonPawn = msgBox.addButton(tr("Pawn"), QMessageBox::YesRole);//always display
        Q_UNUSED(pButtonPawn);

        //check weather the piece are in the deadPlace
        if(moved->side == "WHITE"){
            //  Rook
            if(whiteDead.contains(chess->white[8]) or whiteDead.contains(chess->white[15])){
                pButtonRook = msgBox.addButton(tr("Rook"), QMessageBox::YesRole);
            }

            //  Queen
            if(whiteDead.contains(chess->white[11])){
                pButtonQueen =  msgBox.addButton(tr("Queen"), QMessageBox::YesRole);
            }

            //  Bishop
            if(whiteDead.contains(chess->white[10]) or whiteDead.contains(chess->white[13])){
                pButtonBishop = msgBox.addButton(tr("Bishop"), QMessageBox::YesRole);
            }

            // try if Horse
            if(whiteDead.contains(chess->white[9]) or whiteDead.contains(chess->white[14])){
                pButtonKnight = msgBox.addButton(tr("Knight"), QMessageBox::YesRole);
            }

        }
        else{
            //  Rook
            if(blackDead.contains(chess->black[0]) or blackDead.contains(chess->black[7]))
                pButtonRook = msgBox.addButton(tr("Rook"), QMessageBox::YesRole);
            //  Queen
            if(blackDead.contains(chess->black[3]))
                pButtonQueen =  msgBox.addButton(tr("Queen"), QMessageBox::YesRole);

            //  Bishop
            if(blackDead.contains(chess->black[2]) or blackDead.contains(chess->black[5]))
                pButtonBishop = msgBox.addButton(tr("Bishop"), QMessageBox::YesRole);

            // try if Horse
           if(blackDead.contains(chess->black[1]) or blackDead.contains(chess->black[6]))
                pButtonKnight = msgBox.addButton(tr("Knight"), QMessageBox::YesRole);
        }

        msgBox.exec();


        if (msgBox.clickedButton()==pButtonQueen) {
            exchangeDeadPiece(moved->getCurrentBox()->X, moved->getCurrentBox()->Y, "D");
            pieceName = "D";
        }
        else if (msgBox.clickedButton()==pButtonBishop) {
            exchangeDeadPiece(moved->getCurrentBox()->X, moved->getCurrentBox()->Y, "S");
            pieceName = "S";
        }
        else if (msgBox.clickedButton()==pButtonRook) {
            exchangeDeadPiece(moved->getCurrentBox()->X, moved->getCurrentBox()->Y, "V");
            pieceName = "V";
        }
        else if (msgBox.clickedButton()==pButtonKnight) {
            exchangeDeadPiece(moved->getCurrentBox()->X, moved->getCurrentBox()->Y, "J");
            pieceName = "J";
        }
    }


    //wont create transcript if no file is open
    if(!file){
        if(!dontDisplayNoTranscriptGeneration){
            dontDisplayNoTranscriptGeneration = true;
            QMessageBox::information(nullptr, "INFO: in Game"+QString::number(gameIdx+1), "No file has been selected.\nThat means there will be no game transcript generated and previous move wont work.");
        }
        return;
    }

    //3rd: Add the string and remove further movesin lines
    file->createTranscriptString(pieceName, moved->side, retVal, oldX, oldY, newX, newY);

    //4th create a new scrollArea, destroy the old one
    if(TrscrollArea)
        TrscrollArea->~TranscriptScrollArea();

    TrscrollArea = new TranscriptScrollArea();
    TrscrollArea->move(1145, 485);
    gameScene->addWidget(TrscrollArea);//show the scrollArea
    TrscrollArea->setUpTranscriptButtons(file->lines, gameIdx);
    TrscrollArea->updateTranscript(file->currentTranscriptIdx);
}

bool Game::exchangeDeadPiece(int newX, int newY, QString piece)
{
    ChessBox *box = collection[newX][newY];
    ChessPiece *pawn = box->currentPiece;
    ChessPiece *newPiece = nullptr;

    //check weather the piece are in the deadPlace
    if(pawn->side == "WHITE"){
        //  Rook
        if(piece == "V")
        {
            if(whiteDead.contains(chess->white[8]))
                newPiece = chess->white[8];
            else if(whiteDead.contains(chess->white[15]))
                newPiece = chess->white[8];
        }
        //  Queen
        else if(piece == "D")
        {
            if( !whiteDead.contains(chess->white[11]))
                newPiece = chess->white[11];
        }
        //  Bishop
        else if(piece == "S")
        {
            if(whiteDead.contains(chess->white[10]))
                newPiece = chess->white[10];
            else if(whiteDead.contains(chess->white[13]))
                newPiece = chess->white[13];

        }
        // try if Horse
        else if(piece == "J")
        {
            if(whiteDead.contains(chess->white[9]))
                newPiece = chess->white[9];
            else if(whiteDead.contains(chess->white[14]))
                newPiece = chess->white[14];
        }
        // pawn
        else
        {
            return true;//Just dont even bother i guess?
        }
    }
    else{
        //  Rook
        if(piece == "V")
        {
            if(blackDead.contains(chess->black[0]))
                newPiece = chess->black[0];
            else if(blackDead.contains(chess->black[7]))
                newPiece = chess->black[7];
        }
        //  Queen
        else if(piece == "D")
        {
            if(blackDead.contains(chess->black[3]))
                newPiece = chess->black[3];
        }
        //  Bishop
        else if(piece == "S")
        {
            if(blackDead.contains(chess->black[2]))
                newPiece = chess->black[2];
            else if(blackDead.contains(chess->black[5]))
                newPiece = chess->black[5];
        }
        // try if Horse
        else if(piece == "J")
        {
            if(blackDead.contains(chess->black[1]))
                 newPiece = chess->black[1];
            else if(blackDead.contains(chess->black[6]))
                newPiece = chess->black[6];
        }
        // pawn
        else
        {
            return true;//Just dont even bother i guess?
        }
    }


    if(newPiece == nullptr)
        return false;

    //Now it is a valid exchange, so just exchange thous bad boys
    if(pawn->side == "WHITE")
        whiteDead.removeAll(newPiece);
    else
        blackDead.removeAll(newPiece);

    alivePiece.append(newPiece);

    placeInDeadPlace(pawn);
    box->placePiece(newPiece);
    return true;
}

void Game::displayCheckText(bool blackInDanger)
{
    if(!beingDisplayed){
        beingDisplayed = true;
        auto timer2 = new QTimer();
        timer2->setSingleShot(true);

        connect(timer2, &QTimer::timeout, this, &Game::removeCheck);

        timer2->start(2500);//2secs

        if(blackInDanger)
            checkText->setPlainText("CHECK: Black King in Danger");
        else
            checkText->setPlainText("CHECK: White King in Danger");

        addToScene(checkText); // add to scene for 2.5 secs
    }
}

void Game::removeCheck()
{
    if(beingDisplayed){
        removeFromScene(checkText); //remove from scene after it gets called again
        beingDisplayed = false;
    }

}

////////////////////////////////////////////////////////////////////////////////////////
///////////////////// SLOTS ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void Game::quit()
{
    if(simulationRunning){
        //displays a message box
        QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "can not close game when running");
        pausGame();
        return;
    }
    if(tab->count() <= 1){
        QMessageBox::critical(nullptr, "Error", "Can not close the only tab");
        return;
    }

    tab->removeTab(tab->currentIndex());
    //destruct the current instance
    this->~Game();
    games[tab->currentIndex()] = nullptr;
}

void Game::newTab()
{
    tab->addNewTab();
    updateUiButtonColor();
}

void Game::openFile()
{
    if(simulationRunning){
        QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "Can not open a file when game is running");
        pausGame();
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if(!fileName.isEmpty()){
        file = new FileWork(fileName);
        file->start(gameIdx);


        //destroy the object if there is something to destroy->so that we can create a new one
        if(!firstFile){
            removeFromScene(GameOverText);
            color = false;
            if(TrscrollArea)
                TrscrollArea->~TranscriptScrollArea();

        }
        chess->reset(gameIdx);
        selectedChessPiece = nullptr;
        currentTurn = "WHITE";
        simulationRunning = choosingChessPieceDestBox = false;

        //create a completely new scrollArea
        TrscrollArea = new TranscriptScrollArea();
        TrscrollArea->move(1145, 485);
        gameScene->addWidget(TrscrollArea);//show the scrollArea
        TrscrollArea->setUpTranscriptButtons(file->lines, gameIdx);
        TrscrollArea->updateTranscript(file->currentTranscriptIdx);
        firstFile = false;
        updateUiButtonColor();

        currentTurn = "WHITE";

        if(currentTurnText)
            currentTurnText->setPlainText("Turn: " + currentTurn.toLower());

        //Game transcript text
        if(!gameTranscript){
            gameTranscript = new QGraphicsTextItem();
            gameTranscript ->setPos(1190,440);
            gameTranscript ->setZValue(1);
            gameTranscript ->setDefaultTextColor(Qt::black);
            gameTranscript ->setFont(QFont("",16));
            gameTranscript ->setPlainText("Game Transcript");
            addToScene(gameTranscript);
        }

    }
    else{
        QMessageBox::information(nullptr, "Fail in Game"+QString::number(gameIdx+1), "No file has been selected");
    }

}

void Game::resetGame()
{
    if(simulationRunning){
        QMessageBox::information(nullptr, "Fail", "Can not reset a game that is running");
        return;
    }
    else{
        chess->reset(gameIdx);
        color = false;
        removeFromScene(GameOverText);
        selectedChessPiece = nullptr;
        choosingChessPieceDestBox = false;

        if(currentTurn == "BLACK")
            changeCurrentTurn();

        if(file){
            file->currentTranscriptIdx = -1;
        }
    }
}

void Game::startGame()
{
    if(simulationRunning or !file){
        QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "Can not start a running simulation or no file");
        pausGame();
        return;
    }

    //qDebug() << "StartGame, INDEX: "<< file->currentTranscriptIdx<< "\n";
    QString userInput = textInput->text();


    //check weather userInput is a valid number
    QRegExp re("\\d*");
    if (not re.exactMatch(userInput)){
        QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), " Please enter a valid number");
        updateUiButtonColor();
    }
    else{
        previous();
        simulationRunning = true;
        timeForStep = userInput.toInt();
        simulateChessGame();
        updateUiButtonColor();
    }


}

void Game::pausGame()
{
    if(!simulationRunning){
        return;
    }
    else{
        simulationRunning = false;
        updateUiButtonColor();
    }
}

void Game::simulateChessGame()
{
    if(simulationRunning){
        //try to make a move
        if (file->increaseCounter()){
            if(timeForStep <= 30){
                //just jump to the end if the time entered is 50ms or lower
                simulationRunning = false;
                jumpToTranscriptPosition(file->lines.count()-1);
                return;
            }
            //make a move + increase counter
            if(performeMove() == -1){   // if an error appeared
                return;
            }

            //call itself with delay
            auto timer = new QTimer();
            timer->setSingleShot(true);
            connect(timer, &QTimer::timeout, this, &Game::simulateChessGame);
            timer->start(timeForStep);
        }
        else{
            //this way it will turn itself OFF after it reaches the last step
            simulationRunning = false;
        }
    }

    updateUiButtonColor();
}

void Game::previous()
{
    if(file && !simulationRunning)
        jumpToTranscriptPosition(file->currentTranscriptIdx-1);
}

int Game::next()
{
    if(file && !simulationRunning){
        if(file->increaseCounter()){
            int retval = performeMove();
            changeCurrentTurn();
            //remove the check thing if there is any
            removeCheck();
            return retval;
        }
        return 0;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// HELPER FUNCTIONS FOR MOVE  ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
int Game::movePawn(int oldX, int oldY, int newX, int newY, bool isBlack)
{
    int retVal = -1;
    ChessPiece *pawn;
    ChessPiece *thisPiece;
    if(oldX == -1 or oldY == -1){
        int found = 0; //has to be equal to 1 if valid move
        int pawnX, pawnY, tmp;
        //for black piece
        if(isBlack){
            if(oldX == -1 and oldY == -1){
                for(int i = 8; i < 16; ++i){
                    pawn = chess->black[i];
                    pawnX = pawn->getCurrentBox()->X;
                    pawnY = pawn->getCurrentBox()->Y;
                    tmp = pawn->moves(pawnX, pawnY, newX, newY, gameIdx);
                    if(tmp >= 0){
                        thisPiece = pawn;
                        found++;
                        retVal = tmp;
                    }
                }
            }
            else if(oldX == -1){
                for(int i = 8; i < 16; ++i){
                    pawn = chess->black[i];
                    pawnX = pawn->getCurrentBox()->X;
                    pawnY = pawn->getCurrentBox()->Y;
                    if(pawnY == oldY){
                        tmp = pawn->moves(pawnX, pawnY, newX, newY, gameIdx);
                        if(tmp >= 0){
                            thisPiece = pawn;
                            found++;
                            retVal = tmp;
                        }
                    }

                }
            }
            else if(oldY == -1){
                for(int i = 8; i < 16; ++i){
                    pawn = chess->black[i];
                    pawnX = pawn->getCurrentBox()->X;
                    pawnY = pawn->getCurrentBox()->Y;
                    if(pawnX == oldX){
                        tmp = pawn->moves(pawnX, pawnY, newX, newY, gameIdx);
                        if(tmp >= 0){
                            thisPiece = pawn;
                            found++;
                            retVal = tmp;
                        }
                    }
                }
            }
        }
        //white piece
        else{
            if(oldX == -1 and oldY == -1){
                for(int i = 0; i < 8; ++i){
                    pawn = chess->white[i];
                    pawnX = pawn->getCurrentBox()->X;
                    pawnY = pawn->getCurrentBox()->Y;
                    tmp = pawn->moves(pawnX, pawnY, newX, newY, gameIdx);
                    if(tmp >= 0){
                        thisPiece = pawn;
                        found++;
                        retVal = tmp;
                    }
                }
            }
            else if(oldX == -1){
                for(int i = 0; i < 8; ++i){
                    pawn = chess->white[i];
                    pawnX = pawn->getCurrentBox()->X;
                    pawnY = pawn->getCurrentBox()->Y;
                    if(pawnY == oldY){
                        tmp = pawn->moves(pawnX, pawnY, newX, newY, gameIdx);
                        if(tmp >= 0){
                            thisPiece = pawn;
                            found++;
                            retVal = tmp;
                        }
                    }

                }
            }
            else if(oldY == -1){
                for(int i = 0; i < 8; ++i){
                    pawn = chess->white[i];
                    pawnX = pawn->getCurrentBox()->X;
                    pawnY = pawn->getCurrentBox()->Y;
                    if(pawnX == oldX){
                        tmp = pawn->moves(pawnX, pawnY, newX, newY, gameIdx);
                        if(tmp >= 0){
                            thisPiece = pawn;
                            found++;
                            retVal = tmp;
                        }
                    }
                }
            }
        }
        if(found != 1){
            return -1;
        }
        else{
            if(retVal == 1 || retVal == 0){
                thisPiece->firstMove = false;
                retVal = moveWithPiece(thisPiece, newX, newY, retVal);
            }
            return retVal;
        }
    }

    // full notation
    ChessBox *thisBox= collection[oldX][oldY];
    thisPiece = thisBox->currentPiece;
    ChessPiece *PawnCheck;
    bool found = false;
    if(isBlack){
        //Check if the given piece is actually a pawn, than continue
        for(int i = 8; i < 16; ++i){
            PawnCheck = chess->black[i];
            if(PawnCheck == thisPiece)
                found = true;
        }
    }
    else{
        for(int i =0; i < 8; ++i){
            PawnCheck = chess->white[i];
            if(PawnCheck == thisPiece)
                found = true;
        }
    }

    if(found){
        if(checkIfAlive(thisPiece, isBlack)){
            retVal = thisPiece->moves(oldX, oldY, newX, newY, gameIdx);
            thisPiece->firstMove = false;
            return moveWithPiece(thisPiece, newX, newY, retVal);
        }
        else
            return -1;
    }
    else{
        QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "No Pawn stands on that position:");
        pausGame();
        return -1;
    }
}

int Game::moveKnight(int oldX, int oldY, int newX, int newY, bool isBlack)
{
    ChessPiece *Knight1;
    ChessPiece *Knight2;
    //get the available knights to check weather one of them matches
    if(isBlack){
        Knight1 = chess->black[1];
        Knight2 = chess->black[6];
    }
    else{
        Knight1 = chess->white[9];
        Knight2 = chess->white[14];
    }
    //short notation
    if(oldX == -1 or oldY == -1)
        return processShortNotation(Knight1, Knight2, oldX, oldY, newX, newY);

    ChessBox *thisBox = collection[oldX][oldY];
    ChessPiece *thisPiece = thisBox->currentPiece;

    if(Knight1 != thisPiece && Knight2 != thisPiece){
        QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "No Knight stands on that position");
        pausGame();
        return -1;
    }

    if(checkIfAlive(thisPiece, isBlack)){
        int retVal = thisPiece->moves(oldX, oldY, newX, newY, gameIdx);
        return moveWithPiece(thisPiece, newX, newY, retVal);
    }
    else
        return -1;
}

int Game::moveQueen(int oldX, int oldY, int newX, int newY, bool isBlack)
{
    ChessPiece *Queen;
    //get the available knights to check weather one of them matches
    if(isBlack){
        Queen = chess->black[3];
    }
    else{
        Queen = chess->white[11];
    }

    if(oldX == -1 or oldY == -1){
        int queenX = Queen->getCurrentBox()->X;
        int queenY = Queen->getCurrentBox()->Y;
        if((oldX == -1 and oldY == -1) or (oldX == -1 and oldY == newY) or (oldX == newX and oldY == -1)){
            int retVal = Queen->moves(queenX, queenY, newX, newY, gameIdx);
            return moveWithPiece(Queen, newX, newY, retVal);
        }
        else{
            QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "queen short notaton is wrong");
            pausGame();
            return -1;
        }
    }

    ChessBox *thisBox = collection[oldX][oldY];
    ChessPiece *thisPiece = thisBox->currentPiece;

    if(Queen != thisPiece) {
        QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "No Queen stands on that position");
        pausGame();
        return -1;
    }

    if(checkIfAlive(thisPiece, isBlack)){
        int retVal = thisPiece->moves(oldX, oldY, newX, newY, gameIdx);
        return moveWithPiece(thisPiece, newX, newY, retVal);
    }
    else
        return -1;
}
int Game::moveKing(int oldX, int oldY, int newX, int newY, bool isBlack)
{
    ChessPiece *King;
    //get the available knights to check weather one of them matches
    if(isBlack){
        King= chess->black[4];
    }
    else{
        King = chess->white[12];
    }

    if(oldX == -1 or oldY == -1){
        int queenX = King->getCurrentBox()->X;
        int queenY = King->getCurrentBox()->Y;
        if((oldX == -1 and oldY == -1) or (oldX == -1 and oldY == newY) or (oldX == newX and oldY == -1)){
            int retVal = King->moves(queenX, queenY, newX, newY, gameIdx);
            King->firstMove = false;
            if(retVal >= 0){
                if(isCheck(isBlack, newX, newY) == 1){  // check if king does not move into check
                    return -1;
                }
            }
            return  moveWithPiece(King, newX, newY, retVal);
        }
        else{
            QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "King short notaton is wrong");
            pausGame();
            return -1;
        }
    }

    ChessBox *thisBox = collection[oldX][oldY];
    ChessPiece *thisPiece = thisBox->currentPiece;

    if(King != thisPiece) {
        QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "No King stands on that position");
        pausGame();
        return -1;
    }

    if(checkIfAlive(thisPiece, isBlack)){
        int retVal = thisPiece->moves(oldX, oldY, newX, newY, gameIdx);
        thisPiece->firstMove = false;
        if(retVal >= 0){
            if(isCheck(isBlack, newX, newY) == 1){  // check if king does not move into check
                return -1;
            }
        }
        return moveWithPiece(thisPiece, newX, newY, retVal);
    }
    else
        return -1;
}

int Game::moveRook(int oldX, int oldY, int newX, int newY, bool isBlack)
{
       ChessPiece *Rook1;
       ChessPiece *Rook2;
       //get the available knights to check weather one of them matches
       if(isBlack){
           Rook1 = chess->black[0];
           Rook2 = chess->black[7];
       }
       else{
           Rook1 = chess->white[8];
           Rook2 = chess->white[15];
       }

       //short notation
       if(oldX == -1 or oldY == -1)
           return processShortNotation(Rook1, Rook2, oldX, oldY, newX, newY);


       ChessBox *thisBox = collection[oldX][oldY];
       ChessPiece *thisPiece = thisBox->currentPiece;

       if(Rook1 != thisPiece && Rook2 != thisPiece){
           QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "No Rook stands on that position");
           pausGame();
           return -1;
       }

       if(checkIfAlive(thisPiece, isBlack)){
           int retVal = thisPiece->moves(oldX, oldY, newX, newY, gameIdx);
           thisPiece->firstMove = false;
           return moveWithPiece(thisPiece, newX, newY, retVal);
       }
       else
           return -1;
}

int Game::moveBishop(int oldX, int oldY, int newX, int newY, bool isBlack)
{
       ChessPiece *Bishop1;
       ChessPiece *Bishop2;
       //get the available knights to check weather one of them matches
       if(isBlack){
           Bishop1 = chess->black[2];
           Bishop2 = chess->black[5];
       }
       else{
           Bishop1 = chess->white[10];
           Bishop2 = chess->white[13];
       }
       //short notation
       if(oldX == -1 or oldY == -1)
           return processShortNotation(Bishop1, Bishop2, oldX, oldY, newX, newY);

       ChessBox *thisBox = collection[oldX][oldY];
       ChessPiece *thisPiece = thisBox->currentPiece;

       if(Bishop1 != thisPiece && Bishop2 != thisPiece){
           QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "No Bishop stands on that position");
           pausGame();
           return -1;
       }

        if(checkIfAlive(thisPiece, isBlack)){
            int retVal = thisPiece->moves(oldX, oldY, newX, newY, gameIdx);
            thisPiece->firstMove = false;
            return moveWithPiece(thisPiece, newX, newY, retVal);
        }
        else
            return -1;

}

int Game::processShortNotation(ChessPiece *piece1, ChessPiece *piece2, int oldX, int oldY, int newX, int newY)
{
    int piece1X = piece1->getCurrentBox()->X;
    int piece1Y = piece1->getCurrentBox()->Y;
    int piece2X = piece2->getCurrentBox()->X;
    int piece2Y = piece2->getCurrentBox()->Y;

    int retVal1 = -1;
    int retVal2 = -1;

    if(oldX == -1 and oldY == -1){
        retVal1 = piece1->moves(piece1X, piece1Y, newX, newY, gameIdx);
        retVal2 = piece2->moves(piece2X, piece2Y, newX, newY, gameIdx);
        //if we move into one of our own returns -2
    }
    else if(oldX == -1){
        if(oldY == piece1Y){
             retVal1 = piece1->moves(piece1X, piece1Y, newX, newY, gameIdx);
        }
        else if(oldY == piece2Y){
            retVal2 = piece2->moves(piece2X, piece2Y, newX, newY, gameIdx);
        }
        else{
            QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "Wrong short Format");
            pausGame();
            return -1;
        }
    }
    else if(oldY == -1){
        if(oldX == piece1X){
             retVal1 = piece1->moves(piece1X, piece1Y, newX, newY, gameIdx);
        }
        else if(oldX == piece2X){
             retVal2 = piece2->moves(piece2X, piece2Y, newX, newY, gameIdx);
        }
        else{
            QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "Wrong short Format2");
            pausGame();
            return -1;
        }
    }


    if(retVal1 == 1 || retVal1 == 0){
        piece1->firstMove = false;
        return moveWithPiece(piece1, newX, newY, retVal1);
    }
    else if(retVal2 == 1 || retVal2 == 0){
        piece2->firstMove = false;
        return moveWithPiece(piece2, newX, newY, retVal2);
    }

    return -1;
}
// if retval is 0 or 1 then moves given piece to the given coordinates
int Game::moveWithPiece(ChessPiece *piece, int newX, int newY, int retVal)
{
    Game *game = games[gameIdx];
    ChessBox *newBox = game->collection[newX][newY];
    ChessBox *oldBox = game->collection[piece->getCurrentBox()->X][piece->getCurrentBox()->Y];
    ChessPiece *thisPiece = oldBox->currentPiece;


    if(retVal < 0){ // if retval is not 1 or 0, then this function has no effect and return the same retval
        return retVal;
    }

    if(retVal == 1 or retVal == 2){   // removes piece from destination
        game->placeInDeadPlace(game->collection[newX][newY]->currentPiece);
    }
    newBox->placePiece(thisPiece);
    oldBox->currentPiece = nullptr;
    oldBox->setChessPieceColor("NONE");
    oldBox->setHasChessPiece(false);
    return retVal;

}


bool Game::checkIfAlive(ChessPiece *piece, bool isBlack)
{
    if(isBlack){
        for(size_t i = 0,n = blackDead.size(); i<n; i++) {
            if(blackDead[i] == piece){
                return false;
            }
        }
    }
    else{
        for(size_t i = 0,n = whiteDead.size(); i<n; i++) {
            if(whiteDead[i] == piece){
                return false;
            }
        }
    }
    return true;
}

//will write the game won text stuffs
void Game::gameOver(bool blackWon)
{
    if(blackWon){
        GameOverText = new QGraphicsTextItem("Game Over: Black Won");

    }
    else{
        GameOverText = new QGraphicsTextItem("Game Over: White Won");
    }

    GameOverText->setFont(QFont("arial" , 35));
    GameOverText->setDefaultTextColor(Qt::red);
    int xPos = width()/2 - GameOverText->boundingRect().width()/2;
    int yPos = height()/2;
    GameOverText->setPos(xPos,yPos);
    addToScene(GameOverText);
}

void Game::updateUiButtonColor()
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    //no file-> enable only newTab CloseTab, OpenFile
    if(!file){
        for(int i = 0; i < uiButtons.count(); ++i){
            if(i <= 2 or i == 3){
                uiButtons[i]->setEnabled(true);
                uiButtons[i]->setBrush(brush);
                uiButtons[i]->setAcceptHoverEvents(true);
            }else{
                uiButtons[i]->setAcceptHoverEvents(false);
                uiButtons[i]->setBrush(brush);
                uiButtons[i]->setEnabled(false);
            }
        }
    }
    //only pause and newTab are clickable
    else if(simulationRunning){
        for(int i = 0; i < uiButtons.count(); ++i){
            if(i == 0 or i == 7){
                uiButtons[i]->setBrush(brush);
                uiButtons[i]->setAcceptHoverEvents(true);
                uiButtons[i]->setEnabled(true);

            }else{
                uiButtons[i]->setBrush(brush);
                uiButtons[i]->setEnabled(false);
                uiButtons[i]->setAcceptHoverEvents(false);

            }
        }
    }
    else{
        for(int i = 0; i < uiButtons.count(); ++i){
            if(i == 7){
                uiButtons[i]->setBrush(brush);
                uiButtons[i]->setEnabled(false);
                uiButtons[i]->setAcceptHoverEvents(false);

            }else{ 
                uiButtons[i]->setBrush(brush);
                uiButtons[i]->setAcceptHoverEvents(true);
                uiButtons[i]->setEnabled(true);
            }
        }
    }
}



////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// PERFORMING MOVE  /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//This will decrypt the given line of code and call the according function with
//according params
//the bool forwards just means if we are going forward or backwards in "time"
int Game::performeMove()
{
    QString Instruction = file->lines[file->currentTranscriptIdx];
    TrscrollArea->updateTranscript(file->currentTranscriptIdx);

    QRegularExpression re("^[(KDVSJp]");
    QRegularExpressionMatch match = re.match(Instruction);
    Instruction = Instruction.remove(match.captured(0));
    QString matched = match.captured(0);
    QRegularExpression reg("x");
    match = reg.match(Instruction);
    int checkFlag = 0;
    int xFlag = 0;
    if(match.captured(0) == "x")
    {
        Instruction = Instruction.remove(QString("x")); // removes x sign from instruction
        xFlag = 1;   // capture flag
    }

    QRegularExpression regex("#");
    QRegularExpressionMatch matchMate = regex.match(Instruction);

    QRegularExpression rege("\\+");
    match = rege.match(Instruction);
    if(match.captured(0) == "+")
    {
        checkFlag = 1;
    }
    else if(matchMate.captured(0) == "#")
    {
        checkFlag = 2;
    }

    Instruction = Instruction.remove("+"); // removes check sign from instruction
    Instruction = Instruction.remove("#"); // removes mate sign from instruction
    int retval = processInstruction(Instruction, color, matched, xFlag, checkFlag); // checks length of the instruction and calls makeAMove function with given coordinates
    color = !color;
    return retval;
}

int Game::isCheck(bool isBlack, int kingX, int kingY)
{
    ChessPiece *blackKing = chess->black[4];
    ChessPiece *whiteKing = chess->white[12];
    ChessPiece *piece;
    int whiteKingX = whiteKing->getCurrentBox()->X;
    int whiteKingY = whiteKing->getCurrentBox()->Y;
    int blackKingX = blackKing->getCurrentBox()->X;
    int blackKingY = blackKing->getCurrentBox()->Y;
    if(kingX != -1 && kingY != -1)
    {
        if(isBlack)
        {
            blackKingX = kingX;
            blackKingY = kingY;
        }
        else
        {
            whiteKingX = kingX;
            whiteKingY = kingY;

        }
    }

    int Check = 0;
    int pieceX, pieceY;

    // checks if black king is in check
    if(isBlack)
    {
        int skip = 0;
        for(int i =0; i < chess->white.count(); i++)
        {
            piece = chess->white[i];
            for(int x = 0; x < whiteDead.count(); x++)  // checks if the piece is not dead
            {
                if(whiteDead[x] == piece){
                    skip =1;
                    break;
                }
            }
            if(skip == 1){
                skip = 0;
                continue;
            }
            pieceX = piece->getCurrentBox()->X;
            pieceY = piece->getCurrentBox()->Y;
            if(i >= 0 && i < 8) // pawns
            {
                if(pieceY-1 == blackKingY && (pieceX+1 == blackKingX || pieceX-1 == blackKingX)) // if an enemy pawn can move on the box where king stands -> check
                {
                    Check = 1;
                }
            }
            else if(piece->moves(pieceX, pieceY, blackKingX, blackKingY, gameIdx) >= 0) // if an enemy piece can move on the box where king stands -> check
            {
                Check = 1;
            }
        }

    }
    // checks if white king is in check
    else
    {
        int skip = 0;
        for(int i =0; i < chess->black.count(); i++)
        {
            piece = chess->black[i];
            for(int x = 0; x < blackDead.count(); x++) // checks if the piece is not dead
            {
                if(blackDead[x] == piece){
                    skip =1;
                    break;
                }
            }
            if(skip == 1){
                skip = 0;
                continue;
            }
            pieceX = piece->getCurrentBox()->X;
            pieceY = piece->getCurrentBox()->Y;
            if(i > 7 && i < 16) // pawns
            {
                if(pieceY+1 == whiteKingY && (pieceX+1 == whiteKingX || pieceX-1 == whiteKingX)) // if an enemy pawn can move on the box where king stands -> check
                {
                    Check = 1;
                }
            }
            else if(piece->moves(pieceX, pieceY, whiteKingX, whiteKingY, gameIdx) >= 0)   // if an enemy piece can move on the box where king stands -> check
            {
                Check = 1;
            }
        }
    }
    return Check;
}

int Game::Castling(bool isBlack, bool queenSideCastle)
{
    ChessPiece *king;
    ChessPiece *rook;
    Game *game = games[gameIdx];
    int x1 = 5;
    int x2 = 6;
    int x3;
    if(queenSideCastle){
        x1 = 3;
        x2 = 2;
        x3 = 1;
    }
    int y;
    if(isBlack)
    {
        king = chess->black[4];
        rook = chess->black[7];
        if(queenSideCastle)
            rook = chess->black[0];
        y = 0;
    }
    else
    {
        king = chess->white[12];
        rook = chess->white[15];
        if(queenSideCastle)
             rook = chess->white[8];
        y = 7;
    }
    if(queenSideCastle)
    {
        if(game->collection[x3][y]->currentPiece != nullptr)
        {
            return -1;  // if its queen side castling, then 1 7 or 1 0(depending on color) must be free
        }
    }
    // (5 0 and 6 0) or (5 7 and 6 7) must be free(depending on color) and nothing cannt be in check + king and rook must not move the whole game until this move
    if(game->collection[x1][y]->currentPiece == nullptr && game->collection[x2][y]->currentPiece == nullptr)
    {
        if(isCheck(isBlack, -1, -1) != 1 && isCheck(isBlack, x1, y) != 1 && isCheck(isBlack, x2, y) != 1)
        {
            if(king->firstMove && rook->firstMove)
            {
                moveWithPiece(king, x2, y, 0);
                moveWithPiece(rook, x1, y, 0);
                return 0;
            }
        }
    }
    return -1; // if the move is note possible
}

int Game::processInstruction(QString Instruction, bool color, QString piece, int xFlag, int checkFlag)
{
    int xOld = -1;
    int xNew;
    int yOld = -1;
    int yNew;
    bool exchangeFlag = false;
    QRegularExpression re("[(KDVSJp]$");
    QRegularExpressionMatch match = re.match(Instruction);
    if(match.hasMatch())
    {
        Instruction = Instruction.remove(match.captured(0));
        exchangeFlag = true;
    }

    if(Instruction.length() == 4)   // full notation
    {
        xOld = getCoordinate(Instruction.at(0));
        yOld = getCoordinate(Instruction.at(1));
        xNew = getCoordinate(Instruction.at(2));
        yNew = getCoordinate(Instruction.at(3));
    }
    else if(Instruction.length() == 3)  // notation with one old coordinate
    {
        QRegularExpression re("^[0-7]");
        QRegularExpressionMatch yAxe = re.match(Instruction.at(0));
        QRegularExpression reg("^[a-h]");
        QRegularExpressionMatch xAxe = reg.match(Instruction.at(0));
        if(xAxe.hasMatch())
        {
            xOld = getCoordinate(Instruction.at(0));
        }
        else if(yAxe.hasMatch())
        {
            yOld = getCoordinate(Instruction.at(0));
        }
        xNew = getCoordinate(Instruction.at(1));
        yNew = getCoordinate(Instruction.at(2));
    }
    else if(Instruction.length() == 2)  // short notation without old coordinates
    {
        xNew = getCoordinate(Instruction.at(0));
        yNew = getCoordinate(Instruction.at(1));
    }
    else
    {
        QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "Wrong format of instruction: "+Instruction);
        resetGame();
        return -1;
    }
    int retval = makeAMove(piece, xOld, yOld, xNew, yNew, color);   // makes a move from given coordinates xOld,yOld to xNew,yNew - if it is possible
    if(isCheck(!color, -1, -1))
    {
        // checkFlag must be set to 1 or 2 -> 1 is check 2 is checkmate
        if(checkFlag == 1)
        {
            displayCheckText(!color);
        }
        else if(checkFlag == 2)
        {
            gameOver(color); // if there is an checkmate -> this color won
        }
        // if checkFlag is not 1 or 2 even tho check occured -> error
        else{
            QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "Wrong Check");
            resetGame();
            return -1;
        }
    }
    // if check should occur and it didnt -> error
    else if(checkFlag == 1 || checkFlag == 2)
    {
        QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "Wrong Check in notation");
        resetGame();
        return -1;
    }

    if(retval  == 0 || retval == 1){
        if(retval != xFlag)
        {
            QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "Wrong piece capture");
            resetGame();
            return -1;
        }
    }
    // pawn captures sb and changes itself for a better piece
    else if(retval == 2 || retval == 3)
    {
        if(xFlag != 1 && retval == 2)
        {
            QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "Wrong piece capture");
            resetGame();
            return -1;
        }
        else if(exchangeFlag != true)
        {
            QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "Unexpected piece exchange");
            resetGame();
            return -1;
        }
        else
        {
            if(exchangeDeadPiece(xNew, yNew, match.captured(0)) != true)
            {
                QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "Wrong piece exchange");
                resetGame();
                return -1;
            }
        }
    }
    else
    {
        if(simulationRunning){
            pausGame();
        }
        resetGame();
        QMessageBox::critical(nullptr, "Error in Game"+QString::number(gameIdx+1), "Wrong move: "+Instruction);
        //qDebug() << "INDEX: "<< file->currentTranscriptIdx<< "\n";
        return -1;
    }
    return 0;
}



// gets ascii code of char and returns value which coresponds to coordinate on our chessboard
int Game::getCoordinate(QString coordinate)
{
    QChar ch = coordinate.at(0);
    int x = ch.toLatin1();

    // ascii of 1-8 numbers
    if(x > 48 && x < 57)
    {
        if((x-56) < 0)
        {
            return -(x-56);
        }
        return (x-56);
    }
    else if(x > 96 && x < 105)
    {
        return (x-97);
    }
    else
    {
        return -1;
    }
}

int Game::makeAMove(QString piece, int xOld, int yOld, int xNew, int yNew, bool color)
{
    // move King
    if(piece == "K")
    {
        return moveKing(xOld, yOld, xNew, yNew, color);
    }
    // move Rook
    else if(piece == "V")
    {
        return moveRook(xOld, yOld, xNew, yNew, color);
    }
    // move Queen
    else if(piece == "D")
    {
        return moveQueen(xOld, yOld, xNew, yNew, color);
    }
    // move Bishop
    else if(piece == "S")
    {
        return moveBishop(xOld, yOld, xNew, yNew, color);
    }
    // move Horse
    else if(piece == "J")
    {
        return moveKnight(xOld, yOld, xNew, yNew, color);
    }
    // pawn move
    else
    {
        return movePawn(xOld, yOld, xNew, yNew, color);

    }
}

////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// UI RELATED STUFF /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void Game::displayMainMenu()
{
    int pxPos1 = width() - (width()/6 + 20);
    int pxPos2 = width() - (width()/6 - 100);

    //NewTab button
    Button * newTabButton = new Button("New Tab");
    int pyPos = 50;
    newTabButton->setPos(pxPos1,pyPos);
    connect(newTabButton,SIGNAL(clicked()) , this , SLOT(newTab()));
    addToScene(newTabButton);


    //CloseTab Button
    Button * QuitTab = new Button("Close Tab");
    QuitTab->setPos(pxPos2,pyPos);
    connect(QuitTab,SIGNAL(clicked()) , this , SLOT(quit()));
    addToScene(QuitTab);


    pyPos += 70;
    //OpenTextFile Button
    Button * OpenFileButton = new Button("Open File");
    OpenFileButton->setPos(pxPos1,pyPos);
    connect(OpenFileButton, SIGNAL(clicked()) , this , SLOT(openFile()));
    addToScene(OpenFileButton);


    Button * ResetButton = new Button("Reset Game");
    ResetButton->setPos(pxPos2,pyPos);
    connect(ResetButton, SIGNAL(clicked()) , this , SLOT(resetGame()));
    addToScene(ResetButton);

    pyPos += 70;
    Button * PreviousButton = new Button("Prev Move");
    PreviousButton->setPos(pxPos1,pyPos);
    connect(PreviousButton, SIGNAL(clicked()) , this , SLOT(previous()));
    addToScene(PreviousButton);

    Button * NextButton = new Button("Next Move");
    NextButton->setPos(pxPos2,pyPos);
    connect(NextButton, SIGNAL(clicked()) , this , SLOT(next()));
    addToScene(NextButton);

    pyPos += 70;
    Button * startGame = new Button("Start Game");
    startGame->setPos(pxPos1,pyPos);
    connect(startGame, SIGNAL(clicked()) , this , SLOT(startGame()));
    addToScene(startGame);

    Button * pauseButton = new Button("Pause Game");
    pauseButton->setPos(pxPos2,pyPos);
    connect(pauseButton, SIGNAL(clicked()) , this , SLOT(pausGame()));
    addToScene(pauseButton);

    uiButtons.append(newTabButton); //0
    uiButtons.append(QuitTab);//1
    uiButtons.append(OpenFileButton);//2
    uiButtons.append(ResetButton);//3
    uiButtons.append(PreviousButton);//4
    uiButtons.append(NextButton);//5
    uiButtons.append(startGame);//6
    uiButtons.append(pauseButton);//7
    updateUiButtonColor();

    pyPos += 70;
    //textInput
    textInput = new QLineEdit("1000");
    textInput->move(pxPos1, pyPos);
    textInput->resize(100, 50);
    textInput->setMaxLength(5);
    textInput->setFrame(true);
    textInput->setTextMargins(21, 1, 1,1);
    textInput->setFont(QFont("", 20));
    gameScene->addWidget(textInput);

    //Adding the MS text
    QGraphicsTextItem *miliSec = new QGraphicsTextItem();
    miliSec ->setPos(pxPos2-10 ,pyPos+15);
    miliSec ->setZValue(1);
    miliSec ->setDefaultTextColor(Qt::black);
    miliSec ->setFont(QFont("",20));
    miliSec ->setPlainText("ms");
    addToScene(miliSec);
}

void Game::drawNumAndLet()
{
    //numbers from 8 to 1
    int y = 85;
    //8
    QGraphicsTextItem *numbers = new QGraphicsTextItem();
    numbers ->setPos(270,y);
    numbers ->setZValue(1);
    numbers ->setDefaultTextColor(Qt::black);
    numbers ->setFont(QFont("",30));
    numbers ->setPlainText("8");
    addToScene(numbers);
    //7
    QGraphicsTextItem *nr7= new QGraphicsTextItem();
    nr7->setPos(270,y+100);
    nr7->setZValue(1);
    nr7->setDefaultTextColor(Qt::black);
    nr7->setFont(QFont("",30));
    nr7->setPlainText("7");
    addToScene(nr7);
    //6
    QGraphicsTextItem *nr6= new QGraphicsTextItem();
    nr6->setPos(270,y+200);
    nr6->setZValue(1);
    nr6->setDefaultTextColor(Qt::black);
    nr6->setFont(QFont("",30));
    nr6->setPlainText("6");
    addToScene(nr6);
    //5
    QGraphicsTextItem *nr5= new QGraphicsTextItem();
    nr5->setPos(270,y+300);
    nr5->setZValue(1);
    nr5->setDefaultTextColor(Qt::black);
    nr5->setFont(QFont("",30));
    nr5->setPlainText("5");
    addToScene(nr5);
    //4
    QGraphicsTextItem *nr4= new QGraphicsTextItem();
    nr4->setPos(270,y+400);
    nr7->setZValue(1);
    nr4->setDefaultTextColor(Qt::black);
    nr4->setFont(QFont("",30));
    nr4->setPlainText("4");
    addToScene(nr4);
    //3
    QGraphicsTextItem *nr3= new QGraphicsTextItem();
    nr3->setPos(270,y+500);
    nr3->setZValue(1);
    nr3->setDefaultTextColor(Qt::black);
    nr3->setFont(QFont("",30));
    nr3->setPlainText("3");
    addToScene(nr3);
    //2
    QGraphicsTextItem *nr2= new QGraphicsTextItem();
    nr2->setPos(270,y+600);
    nr2->setZValue(1);
    nr2->setDefaultTextColor(Qt::black);
    nr2->setFont(QFont("",30));
    nr2->setPlainText("2");
    addToScene(nr2);
    //1
    QGraphicsTextItem *nr1= new QGraphicsTextItem();
    nr1->setPos(270,y+700);
    nr1->setZValue(1);
    nr1->setDefaultTextColor(Qt::black);
    nr1->setFont(QFont("",30));
    nr1->setPlainText("1");
    addToScene(nr1);
    //A
    int x = 335;
    QGraphicsTextItem *letterA= new QGraphicsTextItem();
    letterA->setPos(x,850);
    letterA->setZValue(1);
    letterA->setDefaultTextColor(Qt::black);
    letterA->setFont(QFont("",30));
    letterA->setPlainText("A");
    addToScene(letterA);
    //B
    QGraphicsTextItem *letterB= new QGraphicsTextItem();
    letterB->setPos(x+100,850);
    letterB->setZValue(1);
    letterB->setDefaultTextColor(Qt::black);
    letterB->setFont(QFont("",30));
    letterB->setPlainText("B");
    addToScene(letterB);
    //C
    QGraphicsTextItem *letterC= new QGraphicsTextItem();
    letterC->setPos(x+200,850);
    letterC->setZValue(1);
    letterC->setDefaultTextColor(Qt::black);
    letterC->setFont(QFont("",30));
    letterC->setPlainText("C");
    addToScene(letterC);
    //D
    QGraphicsTextItem *letterD= new QGraphicsTextItem();
    letterD->setPos(x+300,850);
    letterD->setZValue(1);
    letterD->setDefaultTextColor(Qt::black);
    letterD->setFont(QFont("",30));
    letterD->setPlainText("D");
    addToScene(letterD);
    //E
    QGraphicsTextItem *letterE= new QGraphicsTextItem();
    letterE->setPos(x+400,850);
    letterE->setZValue(1);
    letterE->setDefaultTextColor(Qt::black);
    letterE->setFont(QFont("",30));
    letterE->setPlainText("E");
    addToScene(letterE);
    //F
    QGraphicsTextItem *letterF= new QGraphicsTextItem();
    letterF->setPos(x+500,850);
    letterF->setZValue(1);
    letterF->setDefaultTextColor(Qt::black);
    letterF->setFont(QFont("",30));
    letterF->setPlainText("F");
    addToScene(letterF);
    //G
    QGraphicsTextItem *letterG= new QGraphicsTextItem();
    letterG->setPos(x+600,850);
    letterG->setZValue(1);
    letterG->setDefaultTextColor(Qt::black);
    letterG->setFont(QFont("",30));
    letterG->setPlainText("G");
    addToScene(letterG);
    //H
    QGraphicsTextItem *letterH= new QGraphicsTextItem();
    letterH->setPos(x+700,850);
    letterH->setZValue(1);
    letterH->setDefaultTextColor(Qt::black);
    letterH->setFont(QFont("",30));
    letterH->setPlainText("H");
    addToScene(letterH);
}

void Game::drawTextElements()
{
    checkText = new QGraphicsTextItem();
    checkText->setFont(QFont("arial" , 35));
    checkText->setDefaultTextColor(Qt::red);
    int xPos = width()/2 - checkText->boundingRect().width()/2;
    int yPos = height()/2;
    checkText->setPos(xPos,yPos);

    //TURN TEXT
    currentTurnText = new QGraphicsTextItem();
    currentTurnText->setPos(300,10);
    currentTurnText->setZValue(1);
    currentTurnText->setDefaultTextColor(Qt::black);
    currentTurnText->setFont(QFont("",22));
    addToScene(currentTurnText);
    currentTurnText->setPlainText("Turn: " + currentTurn.toLower());

    /////////////TEXT STUFF, HEATHER ETC//////////////////////////////////////////////////
    //Create the title2
    QGraphicsTextItem *titleText = new QGraphicsTextItem("Chess Game");
    QFont titleFont("arial" , 35);
    titleText->setFont( titleFont);
    xPos = width()/2 - titleText->boundingRect().width()/2;
    yPos = -10;
    titleText->setPos(xPos,yPos);
    addToScene(titleText);

    //white puieces stuff
    QGraphicsTextItem* whitePiece = new QGraphicsTextItem();
    whitePiece->setPos(70,10);
    whitePiece->setZValue(1);
    whitePiece->setDefaultTextColor(Qt::black);
    whitePiece->setFont(QFont("",16));
    whitePiece->setPlainText("White Pieces");
    addToScene(whitePiece);

    //Area for black pieces, which
    QGraphicsTextItem *blackPiece = new QGraphicsTextItem();
    blackPiece->setPos(70,440);
    blackPiece->setZValue(1);
    blackPiece->setDefaultTextColor(Qt::black);
    blackPiece->setFont(QFont("",16));
    blackPiece->setPlainText("Black Pieces");
    addToScene(blackPiece);

    //userOptions, like Faster/slower Load File etc
    QGraphicsTextItem *userOpts = new QGraphicsTextItem();
    userOpts ->setPos(1170, 10);
    userOpts ->setZValue(1);
    userOpts ->setDefaultTextColor(Qt::black);
    userOpts ->setFont(QFont("",16));
    userOpts ->setPlainText("User Options");
    addToScene(userOpts);
}
