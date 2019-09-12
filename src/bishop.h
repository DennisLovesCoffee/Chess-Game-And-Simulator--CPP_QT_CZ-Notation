/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#ifndef BISHOP_H
#define BISHOP_H

#include "chesspiece.h"
class Bishop:public ChessPiece
{
public:
    Bishop(QString team,QGraphicsItem *parent = nullptr);
    void setImage();
    int moves(int oldX, int oldY, int newX, int newY, int gameIdx);
};


#endif // BISHOP_H
