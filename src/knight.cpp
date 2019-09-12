/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#include "knight.h"
#include "game.h"
#include <QDebug>
extern QList <Game *> games;

Knight::Knight(QString team,QGraphicsItem *parent):ChessPiece(team,parent)
{
    setImage();
}

void Knight::setImage()
{
    if(side == "WHITE")
        setPixmap(QPixmap(":/png/pictures/horse1.png"));
    else
        setPixmap(QPixmap(":/png/pictures/horse.png"));
}

int Knight::moves(int oldX, int oldY, int newX, int newY, int gameIdx)
{
    //get the chessBox and ask it if there is anything
    Game *game = games[gameIdx];

    //get this teams side
    QString team = this->getSide();

    QString enemyTeam = getEnemyTeam(team);

    //Calculate if we can actually make that move 8 moves possible
    ChessBox *newBox;
    int retVal = 0;
    //1st move: down, down, left
    if(oldX-1 == newX && oldY+2 == newY){
        newBox = game->collection[newX][newY];
    }
    //2nd move: down, down, right
    else if(oldX+1 == newX && oldY+2 == newY){
        newBox = game->collection[newX][newY];
    }
    //3rd move: down left left
    else if(oldX-2 == newX && oldY+1 == newY){
        newBox = game->collection[newX][newY];
    }
    //4th move: down right right
    else if(oldX+2 == newX && oldY+1 == newY){
        newBox = game->collection[newX][newY];
    }
    //5th move: up up left
    else if(oldX-1 == newX && oldY-2 == newY){
        newBox = game->collection[newX][newY];
    }
    //6th move: up up right
    else if(oldX+1 == newX && oldY-2 == newY){
        newBox = game->collection[newX][newY];
    }
    //7th move: up left left
    else if(oldX-2 == newX && oldY-1 == newY){
        newBox = game->collection[newX][newY];
    }
    //8th move: up right right
    else if(oldX+2 == newX && oldY-1 == newY){
        newBox = game->collection[newX][newY];
    }
    else{
        return -1;
    }

    //we can not jump onto one of our own, we are no CANIBALS
    if(newBox->getChessPieceColor() == team){
        return -1;
    }
    else if(newBox->getChessPieceColor() == enemyTeam){
        retVal = 1;
    }

    return retVal;
}
