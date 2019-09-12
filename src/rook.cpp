/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#include "rook.h"
#include <QDebug>
#include "game.h"
extern QList <Game *> games;
Rook::Rook(QString team,QGraphicsItem *parent):ChessPiece(team,parent)
{
    setImage();
}


void Rook::setImage()
{
    if(side == "WHITE")
        setPixmap(QPixmap(":/png/pictures/rook1.png"));
    else
        setPixmap(QPixmap(":/png/pictures/rook.png"));
}

int Rook::moves(int oldX, int oldY, int newX, int newY, int gameIdx)
{
    Game *game = games[gameIdx];

    //get this teams side
    QString team = this->getSide();
    QString enemyTeam = getEnemyTeam(team);

    int retVal = 0;
    //does not matter if we are black or white, the moves are the same
    //1st move: horizontal ---   only x changes -> it HAS TO CHANGE, we can not stay at the same position
    if(oldY == newY && (oldX > newX || oldX < newX)){
        if(checkIfFreePathHorizontal(game, oldX, oldY, newX)){
            return checkDestination(game, newX, newY);
        }
        else{
                return -2;
        }
    }
    //2nd move: vertical  |||    only Y changes
    else if(oldX == newX && (oldY > newY || oldY < newY)){
        if(checkIfFreePathVertical(game, oldX, oldY, newY)){
            //check if the destination point is a field with an enemy piece
            return checkDestination(game, newX, newY);
        }
        else{
                return -2;
        }
    }
    else{
        return -1;
    }
    return retVal;
}
