/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#ifndef PAWN_H
#define PAWN_H
#include "game.h"
#include "chesspiece.h"
#include "chessbox.h"
class Pawn:public ChessPiece
{
public:
    Pawn(QString team,QGraphicsItem *parent = nullptr);
    void setImage();

    /**
     * @brief moves simulates a move, returns accordingly
     * @param oldX the starting X position
     * @param oldY the starting Y position
     * @param newX the final X position
     * @param newY the final Y position
     * @param gameIdx the index of the game instance in which we moved
     * @return -1 when we can not perform the given move, 1 when we are throwing out en enemy piece,
     * 2 when we can exchange for an enemy piece
     *
     */
    int moves(int oldX, int oldY, int newX, int newY, int gameIdx);
};

#endif // PAWN_H
