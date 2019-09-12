/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#include <QApplication>
#include "game.h"
#include <QVBoxLayout>
#include "tabmanagment.h"

//A list of all game instances
QList <Game *> games;

//a reference to our only tab manager
TabManagment *tab;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //create the tab manager
    tab = new TabManagment();
    //Add the first game to the tab
    tab->addNewTab();

    tab->show();

    return a.exec();
}
