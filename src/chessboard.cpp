/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#include "chessboard.h"
#include "chessbox.h"
#include "game.h"
#include "queen.h"
#include "rook.h"
#include "pawn.h"
#include "king.h"
#include "knight.h"
#include "bishop.h"
#include "tabmanagment.h"

//extern Game *game;
extern QList <Game *> games;
extern TabManagment *tab;
ChessBoard::ChessBoard()
{
    setUpBlack();
    setUpWhite();
}
void ChessBoard::drawBoxes(int x,int y, int gameIdx)
{
    Game *game = games[gameIdx];
    //x, y = 0 is at the top LEFT and bouth increase as get further from the main point
    int SHIFT = 100;
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++)
            {
                ChessBox *box = new ChessBox(gameIdx);
                game->collection[i][j] = box;
                box->X = i;
                box->Y = j;
                box->setPos(x+SHIFT*i,y+SHIFT*j);
                if((i+j)%2==0)
                    box->setOriginalColor(Qt::white);
                else
                    box->setOriginalColor(Qt::darkGray);
                game->addToScene(box);
                box->currentPiece = nullptr;
            }
        }
}

void ChessBoard::addChessPiece(int gameIdx) {
    //Temporary, would be better in a loop or something once we have more pieces
    ChessBox *box;
    Game *game = games[gameIdx];
    //get black pieces on theTo the View
    int q = 0;
    for(int y = 0; y <= 1; ++y){
        for(int x = 0; x < 8 ; ++x){
            box = game->collection[x][y];
            box->placePiece(black[q]);
            game->alivePiece.append(black[q]);
            game->addToScene(black[q++]);
        }
    }

    //set up the white pieces
    q = 0;
    for(int y = 6; y <= 7; ++y){
        for(int x = 0; x < 8 ; ++x){
            box = game->collection[x][y];
            box->placePiece(white[q]);
            game->alivePiece.append(white[q]);
            game->addToScene(white[q++]);
        }
    }
}


void ChessBoard::setUpWhite()
{
    ChessPiece *piece;
    //first append 8 pawns
    for(int i = 0; i < 8; ++i){
        piece = new Pawn("WHITE");
        white.append(piece);
    }

    piece = new Rook("WHITE");
    white.append(piece);
    piece = new Knight("WHITE");
    white.append(piece);
    piece = new Bishop("WHITE");
    white.append(piece);
    piece = new Queen("WHITE");
    white.append(piece);
    piece = new King("WHITE");
    white.append(piece);
    piece = new Bishop("WHITE");
    white.append(piece);
    piece = new Knight("WHITE");
    white.append(piece);
    piece = new Rook("WHITE");
    white.append(piece);
}

void ChessBoard::setUpBlack()
{
    ChessPiece *piece = new Rook("BLACK");
    black.append(piece);
    piece = new Knight("BLACK");
    black.append(piece);
    piece = new Bishop("BLACK");
    black.append(piece);
    piece = new Queen("BLACK");
    black.append(piece);
    piece = new King("BLACK");
    black.append(piece);
    piece = new Bishop("BLACK");
    black.append(piece);
    piece = new Knight("BLACK");
    black.append(piece);
    piece = new Rook("BLACK");
    black.append(piece);

    for(int i = 0; i < 8; ++i){
        piece = new Pawn("BLACK");
        black.append(piece);
    }

}

void ChessBoard::reset(int gameIdx) {
    Game *game = games[gameIdx];
    ChessBox *box;
    //remove them from all the slots
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++)
        {

            box =game->collection[i][j];
            box->setHasChessPiece(false);
            box->setChessPieceColor("NONE");
            box->currentPiece = nullptr;
        }
    }

    game->whiteDead.clear();
    game->blackDead.clear();
    game->alivePiece.clear();

    //add the pieces back to where they were
    int q = 0;
    for(int y = 0; y <= 1; ++y){
        for(int x = 0; x < 8 ; ++x){
            box = game->collection[x][y];
            box->placePiece(black[q]);
            black[q]->firstMove = true;
            game->alivePiece.append(black[q++]);

        }
    }

    //set up the white pieces
    q = 0;
    for(int y = 6; y <= 7; ++y){
        for(int x = 0; x < 8 ; ++x){
            box = game->collection[x][y];
            box->placePiece(white[q]);
            white[q]->firstMove = true;
            game->alivePiece.append(white[q++]);
        }
    }
}
