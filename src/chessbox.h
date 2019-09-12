/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#ifndef CHESSBOX_H
#define CHESSBOX_H

#include <QGraphicsRectItem>
#include <QBrush>
#include "chesspiece.h"
#include <QGraphicsSceneMouseEvent>
class ChessPiece;
class ChessBox:public QGraphicsRectItem
{
public:

    ///Constructor
    ChessBox(int thisGameIdx, QGraphicsItem *parent=nullptr);
    ~ChessBox();
    ///gets called when we click onto a chessBox, shows the possible moves of the user can take
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    ///sets up the collor of the chessBox
    void setColor(QColor color);

    ///places a piece on this chessBox
    void placePiece(ChessPiece *piece);

    ///resets the original chess box color(from green to white/black)
    void resetOriginalColor();
    ///when we set it up, we keep track of the original color
    void setOriginalColor(QColor value);

    ///returns true if there is a chessPiece on the ChessBox
    bool getHasChessPiece();

    ///Lets the chessBox know, that we have placed a chessPiece on it, and which
    void setHasChessPiece(bool value,ChessPiece *piece = 0);

    ///Returns BLACK or WHITE depending on the chessPiece placed on it
    QString getChessPieceColor() ;

    ///Sets up the chessPiece color of the piece
    void setChessPieceColor(QString value);

    int X; ///Holds the X position of the chessBox
    int Y; ///Holds the Y position on the chessBox
    bool movable;///Gets set to TRUE if the user can move towards this box with the selected piece
    ChessPiece * currentPiece;///holds a reference to the ChessPiece on this box or nullptr
private:
    int gameIdx;///Holds the index of the GameInstance this is in
    QBrush brush;///Color
    QColor originalColor;///Original color
    bool hasChessPiece;///if there is a piece on this chessbox
    QString chessPieceColor;///the chessPieceColor
    void resetALlBoxes();///Resets all boxes to its original state

};

#endif // CHESSBOX_H
