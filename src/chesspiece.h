/**
 * \class ChessPiece
 * \ingroup classGroup
 * \brief This class is the "root class" for all specific Chess Piece
 *
 * This class holds the information and functions of all the Chess Piece
 *
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */
#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#include <QGraphicsPixmapItem>
#include "chessbox.h"
#include <QGraphicsSceneMouseEvent>

class Game;
class ChessBox;
class ChessPiece:public QGraphicsPixmapItem
{
public:
    /**
     * @brief ChessPiece A constructor of a Chess Piece
     * @param team The param which holds the team/side/color of the created Chess Piece ("BLACK" or "WHITE")
     */
    ChessPiece(QString team = "",QGraphicsItem *parent = nullptr);

    /**
     * @brief moves Simulate a move of the Chess Piece from its old to its new coordinates
     * @param oldX the Starting X coordinate
     * @param oldY the Starting Y coordinate
     * @param newX the End X coordinate
     * @param newY the End X coordinate
     * @param gameIdx the idx of the Game Instance in which we want the move being simulated
     *
     * virtual void, so that we can overrde it with the specific movement of each Chess Piece
     * @return values below 0, if the move is false
     * @return 0 if the move is valid and we would not throw out an enemy Chess Piece
     * @return 1 if the move is valid and we would throw out an enemy Chess Piece
     * @return 2 if the Chess Piece is a Pawn and it would reache the end of the Chess Board and throw out an enemy Chess Piece
     * @return 3 if the Chess Piece is a Pawn and it would reache the end of the Chess Board and not throw out an enemy Chess Piece
     */
    virtual int moves(int oldX, int oldY, int newX, int newY, int gameIdx) = 0;

    ///Sets up the  box reference it holds
    void setCurrentBox(ChessBox *box);

    ///returns a reference to the box the chessPiece is on
    ChessBox *getCurrentBox() ;

    ///returns the color of the chessPiece
    QString getSide() ;

    ///Sets u the side of the chessPiece
    void setSide( QString value);

    ///Gets overwritten in every piece class
    virtual void setImage() = 0;

    ///Checks weather the chessPiece is placed, alive
    bool getIsPlaced();

    void setIsPlaced(bool value);

    void decolor();

    ///this is used by paws only, so they can move 2 fields at a time at the start only
    bool firstMove;

    ///returns the string with the enemy team color
    QString getEnemyTeam(QString team);
    ///checks weather we can move towards that destination
    int checkDestination(Game *game, int X,int Y);

    ///checks weather there is a free path from the start to the end verticall
    bool checkIfFreePathVertical(Game *game, int oldX, int oldY, int newY);

    ///checks weather there is a free path from the start to the end horizontal
    bool checkIfFreePathHorizontal(Game *game, int oldX, int oldY,int newX);

    ///checks weather there is a free path from the start to the end angular(Queen0
    bool checkIfFreePathAngular(Game *game, int oldX, int oldY,int newX,int newY);

    ///Holds either BLACK or WHITE, depending on the chessPiece side
    QString side;

protected:
    ChessBox *currentBox; ///Holds a reference to the current chessBox
    bool isPlaced;///Keeps track if it is alive or dead
    QList <ChessBox *> location;

};

#endif // CHESSPIECE_H
