/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#ifndef ROOK_H
#define ROOK_H

#include "chesspiece.h"
class Rook:public ChessPiece
{
public:
    Rook(QString team, QGraphicsItem *parent = nullptr);

    void setImage();
    int moves(int oldX, int oldY, int newX, int newY, int gameIdx);
};

#endif // ROOK_H
