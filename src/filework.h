/**
 * \class FileWork
 * \defgroup classGroup
 * \ingroup classGroup
 * \brief This class is used for processing files andsetting up the transcript scroll area
 * This class is created when we open a file
 *
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#ifndef FILEWORK_H
#define FILEWORK_H
#include <QFileDialog>
#include <QList>

class FileWork
{
public:
    FileWork(QString fileName);

    /**
     * @brief start tries to open the file chosen by the user and processes it
     * Tries to open the file inputed by the user, runs it through a Regular Expression
     * which cheks if the transcript is written correctly.
     * That it processes the file, splits it up into turns and sets populates QStringList lines
     * with the turn strings.
     */
    void start(int idx);

    ///Holds the name of the opened file
    QString filename;

    ///Holds the game Transcript, one string for one turn
    QStringList lines;

    ///holds the current idx of the gameLineInstruction
    int currentTranscriptIdx;

    ///holds the index of the game instance
    int gameIdx;


    /**
     * @brief increaseCounter increases the currentTranscriptIdx if possible
     * @return TRUE if we can and have increased the counter for the current turn, else FALSE
     */
    bool increaseCounter();

    /**
     * @brief decreaseCounter decreases the currentTranscriptIdx if possible
     * @return TRUE if we can and have decreased the counter for the current turn, else FALSE
     */
    bool decreaseCounter();

    /**
     * @brief createTranscriptString creates a new entry in the gamtranscript and updates the transcript scroll area
     * @param pieceName the name of the moving piece or if we are exchanging the Pawn, it contains the name of the new piece chosen by the player
     * @param turn the current turn, so that we know whos turn it is
     * @param retVal in the interval from 0 to 3, where:
     *          0 means that we are just moving with any piece, no throwing out
     *          1 means that we are throwing an enemy figure out
     *          2 means that we moved with a PAWN and are exchanging it + we also threw en enemy piece out
     *          3 means that we moved with a PAWN and exchanged it and DID NOT throw anybody out
     * @param oldX is the starting X position, gets transformed to a letter
     * @param oldY is the starting Y position
     * @param newX is the new X location, gets transformed to a letter
     * @param newY is the new Y position of the piece
     */
    void createTranscriptString(QString pieceName, QString turn, int retVal, int oldX, int oldY, int newX, int newY);

private:
    /**
     * @brief writeTranscriptToFile Writes the current transcript to the open file
     * Takes no parameters, since we have the file, which has been opened saved to
     * a variable called filename
     */
    void writeTranscriptToFile();
};

#endif // FILEWORK_H
