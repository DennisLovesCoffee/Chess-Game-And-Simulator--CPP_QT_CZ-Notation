/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#include "chessbox.h"
#include "game.h"
#include <QDebug>
#include "king.h"
extern QList <Game *> games;
ChessBox::ChessBox(int thisGameIdx, QGraphicsItem *parent):QGraphicsRectItem(parent)
{
    //making the Square CHess Box
    setRect(0,0,100,100);
    brush.setStyle(Qt::SolidPattern);
    setZValue(-1);
    setHasChessPiece(false);
    setChessPieceColor("NONE");
    currentPiece = nullptr;
    gameIdx = thisGameIdx;
    movable = false;
}

ChessBox::~ChessBox()
{
    delete this;
}

void ChessBox::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Game *game = games[gameIdx];
    //dont do anything if the game is running a simulation
    if(game->simulationRunning)
        return;

    ChessPiece *clickedPiece = this->currentPiece;

    //we already haveselected a chessPiece and we are moving right now
    //and than move the game->selectedCHessPiece to the wanted destination
    //reset all chessBoxes
    if (game->choosingChessPieceDestBox && game->selectedChessPiece != nullptr && this->movable == true){
        game->choosingChessPieceDestBox = false;
        //move the piece to the selected area, check the retVal again
        ChessBox *oldBox = game->selectedChessPiece->getCurrentBox();
        int retVal = game->selectedChessPiece->moves(oldBox->X, oldBox->Y, X, Y, gameIdx);

        if(retVal == 1 or retVal == 2){
            //remove the PIECE
            game->placeInDeadPlace(currentPiece);
        }

        //move the piece
        this->placePiece(game->selectedChessPiece);
        currentPiece->firstMove = false;

        oldBox->currentPiece = nullptr;
        oldBox->setChessPieceColor("NONE");
        oldBox->setHasChessPiece(false);

        game->changeCurrentTurn();

        //Will generate an entry
        game->generateEntry(currentPiece, retVal, oldBox->X, oldBox->Y, X, Y);

        resetALlBoxes();
    }
    //we clicked on a chessPiece which we want to be "selected"
    else if(clickedPiece != nullptr){
        //If we have something selected, but are clicking on another piece:
        //reset stuff than select a new piece
        if(game->selectedChessPiece != nullptr){
            resetALlBoxes();
        }

        //CHECK THE COLOR OHE PIECE and ev. change the bool
        if(game->currentTurn != clickedPiece->side){
            return;
        }

        game->choosingChessPieceDestBox = true;
        game->selectedChessPiece = clickedPiece;
        ChessPiece *blackKing = game->chess->black[4];
        ChessPiece *whiteKing = game->chess->white[12];
        //letstry to move to ALLFIELDS
        for(int x = 0; x < 8; ++x){
            for(int y = 0; y < 8; ++y){
                if(clickedPiece->moves(X, Y, game->collection[x][y]->X, game->collection[x][y]->Y, gameIdx) >= 0){

                    if(clickedPiece == blackKing){
                        if(game->isCheck(true, x, y) != 0){ //if the move causes check of this king, then it is permitted move
                            continue;
                        }
                    }
                    else if(clickedPiece == whiteKing){
                        if(game->isCheck(false, x, y) != 0){//if the move causes check of this king, then it is permitted move
                            continue;
                        }
                    }
                    game->collection[x][y]->movable = true;
                    game->collection[x][y]->setColor(Qt::green);
                }
            }
        }
    }
    else{
        //reset all chessBoxes
        resetALlBoxes();
        game->selectedChessPiece = nullptr;
        game->choosingChessPieceDestBox = false;
    }
}

void ChessBox::setColor(QColor color)
{
    brush.setColor(color);
    setBrush(color);
}

void ChessBox::placePiece(ChessPiece *piece)
{

    piece->setPos(x()+50- piece->pixmap().width()/2  ,y()+50-piece->pixmap().width()/2);
    piece->setCurrentBox(this);
    setHasChessPiece(true,piece);
    currentPiece = piece;
}

void ChessBox::resetOriginalColor()
{
    setColor(originalColor);
}

void ChessBox::setOriginalColor(QColor value)
{
    originalColor = value;
    setColor(originalColor);
}

bool ChessBox::getHasChessPiece()
{
    return hasChessPiece;
}

void ChessBox::setHasChessPiece(bool value, ChessPiece *piece)
{
    hasChessPiece = value;
    if(value)
        setChessPieceColor(piece->getSide());
    else
        setChessPieceColor("NONE");
}


QString ChessBox::getChessPieceColor()
{
    return chessPieceColor;
}

void ChessBox::setChessPieceColor(QString value)
{
    chessPieceColor = value;
}

void ChessBox::resetALlBoxes()
{
    Game *game = games[gameIdx];
    for(int x = 0; x < 8; ++x){
        for(int y = 0; y < 8; ++y){
            //TODO collor the up + change the movable to true
            game->collection[x][y]->movable = false;
            game->collection[x][y]->resetOriginalColor();
        }
    }
}
