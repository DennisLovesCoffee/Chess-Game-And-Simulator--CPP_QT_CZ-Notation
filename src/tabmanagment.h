/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#ifndef TABMANAGMENT_H
#define TABMANAGMENT_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTabWidget>
class TabManagment:public QTabWidget
{
    Q_OBJECT
public:
    ///Creates a new tab, sets up the size
    TabManagment(QWidget *parent = nullptr);

    ///makes a new Game instance and adds it to the global games list created in main.cpp
    void addNewTab();

};

#endif // TABMANAGMENT_H
