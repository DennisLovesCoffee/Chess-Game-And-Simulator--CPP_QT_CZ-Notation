/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#include "pawn.h"
#include <QDebug>
#include "game.h"
extern QList <Game *> games;
Pawn::Pawn(QString team,QGraphicsItem *parent):ChessPiece(team,parent)
{
    setImage();
    firstMove = true;
}

void Pawn::setImage()
{
    if(side == "WHITE")
        setPixmap(QPixmap(":/png/pictures/pawn1.png"));
    else
        setPixmap(QPixmap(":/png/pictures/pawn.png"));
}

/*
 * returns 3 if we are at the end and can exchange
 * returns 2 if we threw somebody out and can Exchange
 */
int Pawn::moves(int oldX, int oldY, int newX, int newY, int gameIdx)
{
    Game *game = games[gameIdx];

    //get this teams side
    QString team = this->getSide();
    QString enemyTeam = getEnemyTeam(team);

    int retValue = 0;
    //BLACK can only move down
    if(team == "BLACK"){
        //1st move: down down
        if(firstMove && oldY+2 == newY && oldX == newX){
            if(!checkIfFreePathVertical(game, oldX, oldY, newY) || checkDestination(game, newX, newY) != 0)
                return -1;  // the path is not free
        }
        //2nd move: down
        else if(oldY+1 == newY && oldX == newX){
            if(!checkIfFreePathVertical(game, oldX, oldY, newY) || checkDestination(game, newX, newY) != 0)
                return -1;  // the path is not free
        }
        //3rd move: down right || left   || IF THERE IS A FIGURE TO THROW OUT
        else if( (oldX+1 == newX || oldX-1 == newX ) && oldY+1 == newY ){
            ChessBox *newB = game->collection[newX][newY];
            if(newB->getChessPieceColor() == enemyTeam){
                //if we are throwing out AND changing the pawn//if there are any pieces in the thrownOutArye
                if(side == "BLACK" && newY == 7 && game->blackDead.count() > 0)
                    retValue = 2;
                else
                    retValue = 1;
            }
            else{
                return -1;
            }
        }
        else{
            return -1;
        }
    }
    //White can only move up
    else{
        if(firstMove && oldY-2 == newY && oldX == newX){
            if(!checkIfFreePathVertical(game, oldX, oldY, newY) || checkDestination(game, newX, newY) != 0)   // if the path is free
                return -1;  // the path is not free
        }
        else if(oldY-1 == newY && oldX == newX){
            if(!checkIfFreePathVertical(game, oldX, oldY, newY) || checkDestination(game, newX, newY) != 0)   // if the path is free
                return -1;  // the path is not free
        }
        //throw Out move up left||right   ||| if there is somebody, else error
        else if((oldX+1 == newX || oldX-1 == newX ) && oldY-1 == newY){
            ChessBox *newB = game->collection[newX][newY];
            if(newB->getChessPieceColor() == enemyTeam){
                if(side == "WHITE" && newY == 0 && game->whiteDead.count() > 0)
                    retValue = 2;
                else
                    retValue = 1;
            }
            else{
                return -1;
            }
        }
        else{
            return -1;
        }
    }
    //if we are just moving, check weather we are at the end
    if(retValue == 0 && ( (side == "WHITE" && newY == 0 && game->whiteDead.count() > 0) or (side == "BLACK" && newY == 7 && game->blackDead.count() > 0) )){
        retValue = 3;
    }

    return retValue;
}



