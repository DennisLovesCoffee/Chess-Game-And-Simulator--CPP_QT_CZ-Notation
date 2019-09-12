/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#include "chesspiece.h"
#include "game.h"
#include <QDebug>
#include "king.h"

ChessPiece::ChessPiece(QString team, QGraphicsItem *parent):QGraphicsPixmapItem(parent)
{
    side = team;
    isPlaced = true;
    firstMove = true;
}

void ChessPiece::setCurrentBox(ChessBox *box)
{

    currentBox = box;
}

ChessBox *ChessPiece::getCurrentBox()
{
    return currentBox;
}

QString ChessPiece::getSide()
{
    return side;
}

void ChessPiece::setSide( QString value)
{
    side = value;
}

bool ChessPiece::getIsPlaced()
{
    return isPlaced;
}

void ChessPiece::setIsPlaced(bool value)
{
    isPlaced = value;

}

void ChessPiece::decolor()
{
    for(size_t i = 0, n = location.size(); i < n;i++) {
         location[i]->resetOriginalColor();
    }
}

QString ChessPiece::getEnemyTeam(QString team)
{
    QString enemyTeam = "NONE";
    if(team == "WHITE")
        enemyTeam = "BLACK";
    else if(team == "BLACK")
        enemyTeam = "WHITE";

    return enemyTeam;
}

int ChessPiece::checkDestination(Game *game, int X, int Y)
{
    QString team = this->getSide();
    QString enemyTeam = getEnemyTeam(team);
    QString piece = game->collection[X][Y]->getChessPieceColor();

    if(!game->collection[X][Y]->getHasChessPiece()){
        return 0;   // if there is no piece
    }
    if(piece == enemyTeam){
        return 1;   // if there is enemy piece
    }
    else if(piece == team){
        return -2;  // if there is teammate
    }
    else{
        return -1;
    }
}

bool ChessPiece::checkIfFreePathVertical(Game *game, int oldX, int oldY, int newY)
{
    if(newY > oldY){
        newY = newY -1;
    }
    else if(newY < oldY){
        newY = newY +1;
    }

    //get the start and end
    int start, end;
    //moving UP
    if(oldY > newY){
        start = newY;
        end = oldY-1;
    }
    //moving down
    else{
        start = oldY+1;
        end = newY;
    }

    for(int y = start; y <= end; ++y){
        if(game->collection[oldX][y]->getHasChessPiece()){
            return false;
        }

    }
    return true;
}

bool ChessPiece::checkIfFreePathHorizontal(Game *game, int oldX, int oldY, int newX)
{
    if(newX > oldX){
        newX = newX-1;
    }
    else if(newX < oldX){
        newX = newX+1;
    }

    int start, end;
    if(oldX > newX){
        start = newX;
        end = oldX-1;
    }
    else{
        start = oldX+1;
        end = newX;
    }

    for(int x = start; x <= end; ++x){
        if(game->collection[x][oldY]->getHasChessPiece())
            return false;
    }
    return true;
}

bool ChessPiece::checkIfFreePathAngular(Game *game, int oldX, int oldY, int newX, int newY)
{
    int startX, startY;

     //moving DOWN, RIGHT
    if(newX > oldX){
        newX = newX -1;
    }
    else if(newX < oldX){
        newX = newX+1;
    }
    if(newY > oldY){
        newY = newY -1;
    }
    else if(newY < oldY){
        newY = newY+1;
    }
    //moving DOWN, RIGHT
    if(oldX < newX && oldY < newY){
        startX = oldX+1;
        startY = oldY+1;
        for(int x = startX, y = startY; x <= newX && y <= newY; ++x, ++y){
            if(game->collection[x][y]->getHasChessPiece())
                return false;
        }

        return true;
    }
    //UP, RIGHT
    else if(oldX < newX && oldY > newY){
        startX = oldX+1;
        startY = oldY-1;
        for(int x = startX, y = startY; x <= newX && y >= newY; ++x, --y){
            if(game->collection[x][y]->getHasChessPiece())
                return false;
        }

        return true;
    }
    //DOWN, LEFT
    else if(oldX > newX && oldY < newY){
        startX = oldX-1;
        startY = oldY+1;
        for(int x = startX, y = startY; x >= newX && y <= newY; --x, ++y){
            if(game->collection[x][y]->getHasChessPiece())
                return false;
        }

        return true;
    }
    //UP, LEFT
    else if(oldX > newX && oldY > newY){
        startX = oldX-1;
        startY = oldY-1;
        for(int x = startX, y = startY; x >= newX && y >= newY; --x, --y){
            if(game->collection[x][y]->getHasChessPiece())
                return false;
        }

        return true;
    }


    return true;
}
