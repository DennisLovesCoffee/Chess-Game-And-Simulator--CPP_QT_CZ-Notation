/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <QGraphicsRectItem>
#include "chesspiece.h"

class ChessBoard
{
public:
    ChessBoard();

    /**
     * @brief drawBoxes draws out the chess fields and colors them(one box is one ChessBox instance)
     * @param x the size of the box
     * @param y the y size of the box
     * @param gameIdx the Game instance in which we are drawing boxes
     */
    void drawBoxes(int x, int y, int gameIdx);

    ///sets up the white piece
    void setUpWhite();

    ///sets up the black piece
    void setUpBlack();

    /**
    * @brief addChessPiece adds the chess pieces to the game, the starting setup
    * @param gameIdx the index of the Game instance
    */
    void addChessPiece(int gameIdx);

    ///Holds all white chess pieces
    QList <ChessPiece *> white;

    ///Holds all black chess pieces
    QList <ChessPiece *> black;

    /**
     * @brief reset resets the game, reviwes all pieces and sets them up from a fresh start
     * @param gameIdx the index of the Game instance
     */
    void reset(int gameIdx);

private:

};

#endif // CHESSBOARD_H
