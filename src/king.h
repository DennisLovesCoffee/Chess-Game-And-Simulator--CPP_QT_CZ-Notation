/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#ifndef KING_H
#define KING_H

#include "chesspiece.h"
class King:public ChessPiece
{
private:
    int recursiveKingCheck = 0;
public:
    King(QString team,QGraphicsItem *parent = nullptr);
    void setImage();
    int moves(int oldX, int oldY, int newX, int newY, int gameIdx);
};

#endif // KING_H
