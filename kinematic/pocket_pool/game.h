#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include "whiteball.h"
#include <QVector>
#include <QTimer>


class Game : public QGraphicsView
{
public:
    Game();
    QGraphicsScene * scene;

    QVector<whiteBall*> Ball;

    whiteBall * whiteball;
    whiteBall * whiteball2;
    whiteBall * whiteball3;
    whiteBall * whiteball4;
    whiteBall * whiteball5;
    whiteBall * whiteball6;
    whiteBall * whiteball7;
    whiteBall * whiteball8;
    whiteBall * whiteball9;
    whiteBall * whiteball10;
private:
    QTimer * timer;
};

#endif // GAME_H
