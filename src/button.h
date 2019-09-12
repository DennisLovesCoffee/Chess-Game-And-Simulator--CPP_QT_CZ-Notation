/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#ifndef BUTTON_H
#define BUTTON_H
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>


class Button:public QObject, public QGraphicsRectItem
{
     Q_OBJECT
public:
    ///creates a button with the passed in name
    Button(QString name, QGraphicsItem * parent = nullptr);


    ///this event gets triggered when we press a button
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    ///this event gets triggered when we hover a button
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    ///thisevent gets triggered when we leave a button
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
signals:
    ///this signal gets called whenw e click a button
    void clicked();
private:
    ///holds the reference to the text item on the button
    QGraphicsTextItem *text;
};
#endif // BUTTON_H
