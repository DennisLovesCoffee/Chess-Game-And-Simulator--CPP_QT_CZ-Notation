/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#include "tabmanagment.h"
#include <QTabWidget>
#include "game.h"
#include <QDebug>
#include <QMessageBox>

extern QList <Game *> games;
extern TabManagment *tab;
TabManagment::TabManagment(QWidget *parent)
{
    Q_UNUSED(parent);
    setFixedSize(1405,930);
    setUsesScrollButtons(false);
}

void TabManagment::addNewTab()
{
    if(tab->count() == 15){
        QMessageBox::information(nullptr, "Fail", "Can't create more than 15 tabs at once");
        return;
    }

    static int gameIdx = 0;
    //create new game instance
    Game *game = new Game();

    //Give each instance a unique GAME IDX
    game->gameIdx = gameIdx++;

    games.append(game);

    //get the tab name right
    QString newTabName = "Game" + QString::number(gameIdx);
    tab->addTab(game, newTabName);
    tab->setCurrentIndex(tab->count()-1);

    //Wont work for some reason
    game->show();
    game->start();
}
