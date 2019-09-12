/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#ifndef TRANSCRIPTSCROLLAREA_H
#define TRANSCRIPTSCROLLAREA_H
#include <QScrollArea>
#include <QPushButton>
#include <QGroupBox>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>

class TranscriptScrollArea: public QWidget
{
    Q_OBJECT
public:
    ///Creates a scroll area with a given size
    TranscriptScrollArea(QWidget *parent = nullptr);

    ///holds a reference to the main layout
    QGridLayout *mainLayout;
    ///holds a reference to the button container
    QWidget *buttonsContainer;
    ///Holds a reference to the layout
    QVBoxLayout *buttonsContainerLayout;
    ///holds a reference to the scrollArea
    QScrollArea *scrollArea;

    ///QList for all the pushButtons in the scrollArea
    QList< QPushButton*> buttonList;

    /**
     * @brief setUpTranscriptButtons creates all buttons, sets up style sheets for buttons
     * @param lines is a string-list which contains the transcript for the whole game
     * @param thisGame the index of the game instance
     */
    void setUpTranscriptButtons(QStringList lines, int thisGame);

    /**
     * @brief updateTranscript updates the transcript scroll area buttons when a change occures
     * @param idx the clicked button index
     */
    void updateTranscript(int idx);

public slots:
    ///gets called once a button has been clicked
    void onClickAction();

private:
    ///holds the index of the game instance
    int gameIdx;
};

#endif // TRANSCRIPTSCROLLAREA_H
