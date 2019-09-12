/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#include "transcriptscrollarea.h"
#include <QScrollArea>
#include <QPushButton>
#include <QGroupBox>
#include <QApplication>
#include "game.h"
#include <QDebug>

extern QList <Game *> games;
TranscriptScrollArea::TranscriptScrollArea(QWidget *parent)
{
    Q_UNUSED(parent);
    resize(225, 365);
    mainLayout = new QGridLayout;
    buttonsContainer = new QWidget;
    buttonsContainerLayout = new QVBoxLayout;
    scrollArea = new QScrollArea();

}

void TranscriptScrollArea::setUpTranscriptButtons(QStringList lines, int thisGame)
{
    //for onClickEffects
    gameIdx = thisGame;

    //create one button for each line
    QPushButton *button;
    for(int i = 0; i < lines.count(); ++i){
        if(i % 2 == 0 or i == 0){
            //just the IDX of the given button, not clickable at all though xD
            QPushButton *idxButton = new QPushButton(QString::number( (i/2)+1 ));
            idxButton->setFixedSize(160, 25);
            idxButton->setStyleSheet("QPushButton { background-color: darkGray; color: white;}");
            idxButton->setEnabled(false);
            buttonsContainerLayout->addWidget(idxButton);
        }
        button = new QPushButton(lines[i]);
        button->setFixedSize(160, 25);
        button->setStyleSheet("Text-align:left"); //dont think this is working though xD
        //connect the button
        connect(button, SIGNAL(clicked()) , this , SLOT(onClickAction()));
        buttonList.append(button);
        buttonsContainerLayout->addWidget(button);
    }

    //NO CLUE why this has to be here AFTER we set up the buttons, really NO CLUE LOL
    buttonsContainer->setLayout(buttonsContainerLayout);
    scrollArea->setWidget(buttonsContainer);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
    setWindowTitle(QApplication::translate("testscrollable", "Test Scrollable"));
}

void TranscriptScrollArea::updateTranscript(int idx)
{
    //we do start at -1 at the verry start with the idx
    QPushButton *button;
    //set up all grey, no yellow bad boyz
    if (idx<0){
        for(int i = 0; i < buttonList.count(); ++i){
            button = buttonList[i];
            button->setStyleSheet("QPushButton { background-color: lightGrey}");
        }
    }
    else{
        for(int i = 0; i < buttonList.count(); ++i){
            button = buttonList[i];
            if(idx == i){
                button->setStyleSheet("QPushButton { background-color: lightBlue}");
                scrollArea->ensureWidgetVisible(button, 50, 50);
            }
            else
                button->setStyleSheet("QPushButton { background-color: lightGrey}");
        }
    }
}

void TranscriptScrollArea::onClickAction()
{
    QPushButton *pushedButton = qobject_cast<QPushButton*>(sender());
    if(!pushedButton)
        return;

    Game *game = games[gameIdx];
    int idx;
    for(idx = 0; idx < buttonList.count(); ++idx){
        if(buttonList[idx] == pushedButton)
            break;
    }

    game->jumpToTranscriptPosition(idx);
}
