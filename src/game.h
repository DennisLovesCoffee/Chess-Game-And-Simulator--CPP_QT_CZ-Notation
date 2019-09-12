/**
 * \class Game
 * \defgroup classGroup
 * \ingroup classGroup
 * \brief This class is the "root class"
 * This class gets created when a new Game Tab(Instance) gets created
 * and sets up the game interface.
 *
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

/*! \mainpage Chess Simulator, CPP project 2018/2019
 *
 * 
 * This project is a chess simulator and emulator.\n
 * You can play/simulate several chess games at once in different tabs.\n
 * It also takes in standard chess notation (the czech version).\n\n\n
 * 
 * More helpfull information:\n
 *
 * - \subpage installGuid "How to install this APP?"
 * - \subpage userInterface "How does it work? (UI basics)"
 * 
 * Authors: Dennis Vymer (xvymer00), Pave Gos (xgospa00)
 */

/*! \page userInterface How does it work?
 *
 * The UI buttons change inbetween the enabled and disabled state depending on if they can or can not be used.\n
 * Fore example the Pause Button can NOT be clicked (when u hover over the button it does not change color)\n
 * if there is no automated game simulation.\n
 * 
 * The user interface was created to be verry intuitive and simple:\n
 * - 'New tab' button creates a new Tab, to which the user will be redirected
 * - 'Close tab' button will close the current tab, you can switch inbetween tabs just like in your browser
 * - 'Open file' button will open a new file, this will reset the game in the current tab, so be carefull
 * - 'Reset game' button  will reset the game, does not matter if you have a open file or are just playing without a transcript
 * - 'Prev move' button will jump to the previous move (if possible)
 * - 'Next move' button will jump one move ahead (if possible)
 * - 'Start game' button will start the automated game simulation with the entered timestamp (has to be a correct number in ms)
 * - 'Pause game' button will pause the automated game simulation
 * 
 * 
 */

/*! \page installGuid How to install this APP?
 *
 * Simply write 'make' or 'make run' in your command line.\n
 * - You need Qt 5.6.2 or higher ([Qt](https://www.qt.io/) required) \n 
 * - After this there will be a program called 'chess' ready for you.\n
 */

#ifndef GAME_H
#define GAME_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <chessboard.h>
#include "chessbox.h"
#include "button.h"
#include <QVBoxLayout>
#include "filework.h"
#include <QLineEdit>
#include "transcriptscrollarea.h"

class Game:public QGraphicsView
{
    Q_OBJECT
public:
    ///Create an Game instance, sets up the basic interface for a chessGame
    Game(QWidget *parent = nullptr);

    ///Draws a chessBoard onto the game scene
    void drawChessBoard();

    /**
     * @brief Adds an item to the game scene if it is not a nullptr
     * @param item is a QGraphicsItem we want to add to the game scene
     */
    void addToScene(QGraphicsItem *item);
    /**
     * @brief Removes an item from the game scene if it is not a nullptr
     * @param item is a QGraphicsItem we want to remove from the game scene
     */
    void removeFromScene(QGraphicsItem *item);

    ///displays the current white dead piece in the left top corner
    void displayDeadWhite();
    ///displays the current black dead piece in the left bottom corner
    void displayDeadBlack();

    /**
     * @brief placeInDeadPlace places the passed in ChessPiece in a QList for dead piece
     * @param piece the piece which we want to place in the Qlist for dead piece
     * using the Qstring  ChessPiece->side, finds out what collor the chessPiece is
     * and removes it from the active/alive piece and from the gameBoard and adds it
     * to the dead piece Qlist (according to its side)
     */
    void placeInDeadPlace(ChessPiece *piece);

    ///Holds all the references to the created chessBox instances
    ChessBox *collection[8][8];

    ///Variable for displaying the check text, after it occures
    QGraphicsTextItem *check;

    ///holds all the alive Chess piece in a list, black and white
    QList <ChessPiece *> alivePiece;

    ///Sets up the MainMenu, which are all the userButtons in the top right corner
    void displayMainMenu();

    ///Displays the chess board Numbers and Letters (A-..., 1-...)
    void drawNumAndLet();

    ///Displays the text elements, like the header
    void drawTextElements();

    ///Gets called at the start, to set up a game class instance
    void start();

    ///holds the idx of the current instance, used for accesing a game instance from a differend class
    int gameIdx;

    ///Holds a reference to the created Chess Board instance
    ChessBoard *chess;

    ///Holds a reference to all the white Dead Piece
    QList <ChessPiece *> whiteDead;

    ///Holds a reference to all the black Dead Piece
    QList <ChessPiece *> blackDead;

    /**
     * @brief jumpToTranscriptPosition jumps to the passed in idx in the transcript
     * @param idx the idx we want to jump to in the transcript, one line is one idx
     * Gets called by the prev button
     * First it resets the whole game, than simulates the chess game up untill we reach
     * the wanted idx in the game transcript, one idx is one turn(white or black)
     */
    void jumpToTranscriptPosition(int idx);

    ///Gets set to TRUE if the automated game simulation is running, else FALSE
    bool simulationRunning;

    ///Gets set to TRUE if the user clicked on a Chess Piece to move it, else FALSE
    bool choosingChessPieceDestBox;

    ///Holds a reference to the Chess Piece, which has been selected by the user, else nullptr
    ChessPiece *selectedChessPiece;

    ///Holds a string with the current turn either WHITE or BLACK
    QString currentTurn;

    ///If the turn is WHITE, changes it to BLACK and updates the turn text being displayed
    void changeCurrentTurn();

    /**
     * @brief generateEntry generates and entry to the selected file, if there is no file selected, no entry is being generated
     * @param moved reference to the Chess Piece which has been moved
     * @param retVal int which holds the return value of the move
     * @param oldX int which holds the oldX value of the passed in Chess Piece
     * @param oldY int which holds the oldY value of the passed in Chess Piece
     * @param newX int which holds the newX value of the passed in Chess Piece
     * @param newY int which holds the newY value of the passed in Chess Piece
     */
    void generateEntry(ChessPiece *moved, int retVal, int oldX, int oldY, int newX, int newY);

    /**
     * @brief exchangeDeadPiece gets called when a PAWN reaches the opposite end of the Chess Board
     * @param newX is an int with the X position of the PAWN
     * @param newY is an int with the X position of the PAWN
     * @param forWhat s string which holds the shortCut of the Character we want the Pawn to be exchanged for
     * @return true if the exchange is valid, else false
     * This function finds the pawn using the passed in X and Y coordinates
     * than gets its side(Black or White) and tries to find
     * a deadPiece which coresponds to the passed in QString forWhat.
     * If there is a dead Chess Piece which has the same side and same Shortcut as the passed-in QString
     * Than it will exchange the 2 Piece and return true
     */
    bool exchangeDeadPiece(int newX, int newY, QString forWhat);

    /**
     * @brief displayCheckText displays the "CHECK" text for 2.5 seconds(if we do not press next or prev move)
     * @param blackInDanger true if the black king is in danger, else false
     * uses a QTimer to call the removeCheck function after 2.5 seconds
     */
    void displayCheckText(bool blackInDanger);

    ///TRUE if the "CHECK" text is being displayed
    bool beingDisplayed;

    ///Holds a reference to the "CHECK" QGraphicsTextItem
    QGraphicsTextItem *checkText;

    ///Removes the "CHECK" text from the game scene
    void removeCheck();

    /**
     * @brief isCheck function controls if a king is in check
     * @param isBlack is true if we want to control black king, false otherwise
     * @param KingX is -1 if we want to check current position of a king, else it should be possible coordinate X of a king we want to check
     * @param KingY is -1 if we want to check current position of a king, else it should be possible coordinate Y of a king we want to check
     * @return 0 if king's position is not in check, 1 otherwise
     */
    int isCheck(bool isBlack, int KingX, int KingY);

public slots:
    ///Connected to the Close Tab button
    void quit();
    ///Connected to the New Tab button
    void newTab();

    ///Connected to the Open File button
    void openFile();
    ///Connected to the Reset button
    void resetGame();

    ///Connected to the Prev. Move button
    void previous();
    ///Connected to the Next Move button
    int next();

    ///Connected to the Start button
    void startGame();
    ///Connected to the Pause Move button
    void pausGame();

private:
    ///A reference to the QTextItem which displays the current turn (Turn: White/Black)
    QGraphicsTextItem *currentTurnText;

    ///QList of UI button, used for restricting certain buttons at certain states
    QList <Button *> uiButtons;

    ///Used to check weather we are opening out first file or if we have already opened multiple files
    bool firstFile = true;

    ///A reference to the time input window for the automated simulation
    QLineEdit *textInput;

    ///a reference to the clickavle and scrollable Transcript area
    TranscriptScrollArea *TrscrollArea;

    ///holds an int representing the automated time difference between steps in ms
    int timeForStep;

    ///Simulates the automated chess game
    void simulateChessGame();

    ///Holds a reference to the "GAME OVER: ... won" text
    QGraphicsTextItem* GameOverText;

    ///Holds a reference to the "Game Transcript" text over the Scrollable Transcript Area
    QGraphicsTextItem *gameTranscript;

    ///This holds a boolean which says if we want to display the "Game Transcript" header or not
    ///Will be set to true after we open a file
    bool dontDisplayNoTranscriptGeneration = false;

    bool color = false;  // color indicator

    /**
     * @brief movePawn function moves with Pawn
     * @param oldX is current coordinate X of the piece(if the coordinate is -1 and old Y is -1 - function process short notation -> without current position).
     * @param oldY is current coordinate Y of the piece.
     * @param newX is coordinate X where the piece should be moved to.
     * @param newY is coordinate Y where the piece should be moved to.
     * @param isBlack is true if it is black turn, false otherwise.
     * @return -1 if there is an error, 1 if piece capture is performed or 2 if piece capture is performed and the current piece is exchanged, 0 if piece is moved and nothing else happened, 3 if is is end of the game
     */
    int movePawn(int oldX, int oldY, int newX, int newY, bool isBlack);
    /**
     * @brief movePawn function moves with Pawn
     * @param oldX is current coordinate X of the piece(if the coordinate is -1 and old Y is -1 - function calls processShortNotation).
     * @param oldY is current coordinate Y of the piece.
     * @param newX is coordinate X where the piece should be moved to.
     * @param newY is coordinate Y where the piece should be moved to.
     * @param isBlack is true if it is black turn, false otherwise.
     * @return -1 if there is an error, 1 if piece capture is performed or 2 if piece capture is performed and the current piece is exchanged, 0 if piece is moved and nothing else happened, 3 if is is end of the game
     */
    int moveKnight(int oldX, int oldY, int newX, int newY, bool isBlack);
    /**
     * @brief movePawn function moves with Pawn
     * @param oldX is current coordinate X of the piece(if the coordinate is -1 and old Y is -1 - function process short notation -> without current position).
     * @param oldY is current coordinate Y of the piece.
     * @param newX is coordinate X where the piece should be moved to.
     * @param newY is coordinate Y where the piece should be moved to.
     * @param isBlack is true if it is black turn, false otherwise.
     * @return -1 if there is an error, 1 if piece capture is performed or 2 if piece capture is performed and the current piece is exchanged, 0 if piece is moved and nothing else happened, 3 if is is end of the game
     */
    int moveQueen(int oldX, int oldY, int newX, int newY, bool isBlack);
    /**
     * @brief movePawn function moves with Pawn
     * @param oldX is current coordinate X of the piece(if the coordinate is -1 and old Y is -1 - function process short notation -> without current position).
     * @param oldY is current coordinate Y of the piece.
     * @param newX is coordinate X where the piece should be moved to.
     * @param newY is coordinate Y where the piece should be moved to.
     * @param isBlack is true if it is black turn, false otherwise.
     * @return -1 if there is an error, 1 if piece capture is performed or 2 if piece capture is performed and the current piece is exchanged, 0 if piece is moved and nothing else happened, 3 if is is end of the game
     */
    int moveKing(int oldX, int oldY, int newX, int newY, bool isBlack);
    /**
     * @brief movePawn function moves with Pawn
     * @param oldX is current coordinate X of the piece(if the coordinate is -1 and old Y is -1 - function calls processShortNotation).
     * @param oldY is current coordinate Y of the piece.
     * @param newX is coordinate X where the piece should be moved to.
     * @param newY is coordinate Y where the piece should be moved to.
     * @param isBlack is true if it is black turn, false otherwise.
     * @return -1 if there is an error, 1 if piece capture is performed or 2 if piece capture is performed and the current piece is exchanged, 0 if piece is moved and nothing else happened, 3 if is is end of the game
     */
    int moveRook(int oldX, int oldY, int newX, int newY, bool isBlack);
    /**
     * @brief movePawn function moves with Pawn
     * @param oldX is current coordinate X of the piece(if the coordinate is -1 and old Y is -1 - function calls processShortNotation).
     * @param oldY is current coordinate Y of the piece.
     * @param newX is coordinate X where the piece should be moved to.
     * @param newY is coordinate Y where the piece should be moved to.
     * @param isBlack is true if it is black turn, false otherwise.
     * @return -1 if there is an error, 1 if piece capture is performed or 2 if piece capture is performed and the current piece is exchanged, 0 if piece is moved and nothing else happened, 3 if is is end of the game
     */
    int moveBishop(int oldX, int oldY, int newX, int newY, bool isBlack);

    /**
     * @brief processShortNotation function checks notation and calls function moveWithPiece. If short notation is given, function gets the right piece which should be moved.
     * @param piece1 is pointer to first piece which might be moved
     * @param piece2 is pointer to second piece which might be moved
     * @param oldX is current coordinate X of the piece(if the coordinate is -1 and old Y is -1 - function process short notation -> without current position).
     * @param oldY is current coordinate Y of the piece.
     * @param newX is coordinate X where the piece should be moved to.
     * @param newY is coordinate Y where the piece should be moved to.
     * @return -1 if there is an error, 1 if piece capture is performed, 0 if piece is moved and nothing else happened
     */
    int processShortNotation(ChessPiece *piece1, ChessPiece *piece2, int oldX, int oldY, int newX, int newY);
    /**
     * @brief moveWithPiece if parameter retval is 0 or 1 -> function moves given piece to the given coordinates
     * @param piece is pointer to the piece which should be moved
     * @param newX is coordinate X where the piece should be moved to.
     * @param newY is coordinate Y where the piece should be moved to.
     * @param retVal is an integer which indicates what should happen - retval < 0 is error, 1 or 2 is capturing a piece at the destination, 0 is simple move to the destionation.
     * @return value of parameter retVal
     */
    int moveWithPiece(ChessPiece *piece, int newX, int newY, int retVal);
    /**
     * @brief checkIfAlive checks if the given piece is not captured
     * @param piece is pointer to the piece which is checked
     * @param isBlack is boolean variable -> true if the piece is black, false if the piece is white
     * @return true if the piece is not captured, false otherwise
     */
    bool checkIfAlive(ChessPiece *piece, bool isBlack);
    void gameOver(bool blackWon);
    void updateUiButtonColor();

    ///Holds a reference to the FileWOrk class instance, which holds and prepares the user Input File
    FileWork *file;

    ///Holds a reference to the Scene being displayed to the user
    QGraphicsScene *gameScene;

    /**
     * @brief performeMove function controls sings like "x","#","+" -> if any of it was found, a flag is set to 1 and the sign is removed from the notation we work with(the file is not changed). After that processInstruction is called.
     * @return value returned from processInstruction function
     */
    int performeMove();
    /**
     * @brief processInstruction gets coordinates from the notation, checks if it is correct and calls makeAMove function
     * @param Instruction is modificated notation read form a file
     * @param color is true if black color instruction is processed, false otherwise
     * @param piece is string describing the piece which should be moved.
     * @param xFlag is an integer which indicates piece capture
     * @param checkFlag is an integer which indicates check
     * @return 0 if no error accrued, -1 if there was an error
     */
    int processInstruction(QString Instruction, bool color, QString piece, int xFlag, int checkFlag);
    /**
     * @brief getCoordinate gets coordinate from the given string/notation
     * @param coordinate is string containing coordinate
     * @return value of coordinate or -1 if an error accrued
     */
    int getCoordinate(QString coordinate);
    /**
     * @brief makeAMove calls function for moving given kind of piece
     * @param piece is string describing piece which should be moved(K/V/D/S/J for King/Rook/Queen/Bishop/Horse -> if there is a string not matching any of the previous then it moves with Pawn)
     * @return value returned from function moveKing/moveRook/moveQueen/moveBishop/moveKnight or movePawn
     */
    int makeAMove(QString piece, int xOld, int yOld, int xNew, int yNew, bool color);
    /**
     * @brief Castling function implemented for Castling - not used though because it does not have to be supported
     * @param isBlack is true if it is black turn, false otherwise
     * @param queenSideCastle is true if it is queen side castle, false otherwise
     * @return 0 if everything is OK, -1 if an error accrued
     */
    int Castling(bool isBlack, bool queenSideCastle);
};

#endif // GAME_H
