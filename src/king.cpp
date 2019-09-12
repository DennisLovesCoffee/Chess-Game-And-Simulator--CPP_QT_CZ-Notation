/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#include "king.h"
#include <QDebug>
#include "game.h"
extern QList <Game *> games;

King::King(QString team,QGraphicsItem *parent):ChessPiece(team,parent)
{
    setImage();
}

void King::setImage()
{
    if(side == "WHITE")
        setPixmap(QPixmap(":/png/pictures/king1.png"));
    else
        setPixmap(QPixmap(":/png/pictures/king.png"));
}

int King::moves(int oldX, int oldY, int newX, int newY, int gameIdx)
{
    Game *game = games[gameIdx];

    //get this teams side
    QString team = this->getSide();
    QString enemyTeam = getEnemyTeam(team);

    int retVal = 0;

    //move1,2 : UP or DOWN -> vertical
    if((oldX == newX && oldY-1 == newY) || (oldX == newX && oldY+1 == newY)){
        if(checkIfFreePathVertical(game, oldX, oldY, newY)){
            retVal = checkDestination(game, newX, newY);
            return retVal;
        }
        else{
            return -1;
        }
    }
    //move 3,4 : LEFT or RIGHT -> horizontal
    else if((oldX+1 == newX && oldY == newY) || (oldX-1 == newX && oldY == newY)) {
        if(checkIfFreePathHorizontal(game, oldX, oldY, newX)){
            retVal = checkDestination(game, newX, newY);
            return retVal;
        }
        else{
            return -1;
        }
    }
    //move 4-8 angular
    else if((oldX+1 == newX && oldY+1 == newY) || (oldX+1 == newX && oldY-1 == newY)
            || (oldX-1 == newX && oldY+1 == newY) || (oldX-1 ==  newX && oldY-1 == newY))
    {
        if(checkIfFreePathAngular(game, oldX, oldY, newX, newY)){
            //check if the destination point is a field with an enemy piece
            retVal = checkDestination(game, newX, newY);
            return retVal;
        }
        else{
              return -1;
        }
    }
    else{
        return -1;
    }
    return retVal;
}

