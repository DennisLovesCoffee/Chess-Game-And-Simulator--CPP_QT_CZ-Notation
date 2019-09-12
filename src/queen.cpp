/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#include "queen.h"
#include "game.h"
#include <QDebug>
extern QList <Game *> games;

Queen::Queen(QString team,QGraphicsItem *parent):ChessPiece(team,parent)
{
    setImage();
}

void Queen::setImage()
{
    if(side == "WHITE")
        setPixmap(QPixmap(":/png/pictures/queen1.png"));
    else
        setPixmap(QPixmap(":/png/pictures/queen.png"));
}

int Queen::moves(int oldX, int oldY, int newX, int newY, int gameIdx)
{
    Game *game = games[gameIdx];

    //get this teams side
    QString team = this->getSide();
    QString enemyTeam = getEnemyTeam(team);

    int retVal = 0;
    //down right || up, right ||  down, left || up, left
    if(((oldX < newX && oldY < newY) || (oldX < newX && oldY > newY)
            || (oldX > newX && oldY < newY) || (oldX > newX && oldY > newY))  && abs(oldX - newX) == abs(oldY-newY))
    {
        if(checkIfFreePathAngular(game, oldX, oldY, newX, newY)){
            //check if the destination point is a field with an enepy piece
            return checkDestination(game, newX, newY);
        }
        else{
            return -1;
        }

    }
    //horizontal ---   only x changes -> it HAS TO CHANGE, we can not stay at the same position
    else if(oldY == newY && (oldX > newX || oldX < newX) ){
        if(checkIfFreePathHorizontal(game, oldX, oldY, newX)){
            return checkDestination(game, newX, newY);
        }
        else{
             return -1;
        }
    }
    //vertical  |||    only Y changes
    else if(oldX == newX && (oldY > newY || oldY < newY)){
        if(checkIfFreePathVertical(game, oldX, oldY, newY)){
            //check if the destination point is a field with an enepy piece
            return checkDestination(game, newX, newY);
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
