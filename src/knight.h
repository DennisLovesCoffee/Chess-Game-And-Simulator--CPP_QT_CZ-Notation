/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#ifndef KNIGHT_H
#define KNIGHT_H

#include "chesspiece.h"
class Knight:public ChessPiece
{
public:
    Knight(QString team,QGraphicsItem *parent = nullptr);
    void setImage();
    int moves(int oldX, int oldY, int newX, int newY, int gameIdx);
};

#endif // KNIGHT_H
