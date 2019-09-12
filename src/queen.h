/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#ifndef QUEEN_H
#define QUEEN_H
#include "chesspiece.h"

class Queen:public ChessPiece
{
public:
    Queen(QString team,QGraphicsItem *parent = nullptr);
    void setImage();
    int moves(int oldX, int oldY, int newX, int newY, int gameIdx);

};

#endif // QUEEN_H
