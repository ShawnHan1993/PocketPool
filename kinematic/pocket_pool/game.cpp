#include "game.h"


Game::Game()
{
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,480,272);

    setBackgroundBrush(QBrush(Qt::green));
    setScene(scene);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(480,272);

    whiteball = new whiteBall(0);
    whiteball->setPos(30, 160);
    whiteball->setState(6.2,0.1);

    scene->addItem(whiteball);

    whiteball2 = new whiteBall(1);
    whiteball2->setPos(250, 130);
    whiteball2->setState(0,0);
    scene->addItem(whiteball2);

    whiteball3 = new whiteBall(2);
    whiteball3->setPos(270, 140);
    whiteball3->setState(0,0);
    scene->addItem(whiteball3);

    whiteball4 = new whiteBall(3);
    whiteball4->setPos(270, 120);
    whiteball4->setState(0,0);
    scene->addItem(whiteball4);

    whiteball5 = new whiteBall(4);
    whiteball5->setPos(290, 130);
    whiteball5->setState(0,0);
    scene->addItem(whiteball5);

    whiteball6 = new whiteBall(5);
    whiteball6->setPos(290, 150);
    whiteball6->setState(0,0);
    scene->addItem(whiteball6);

    whiteball7 = new whiteBall(6);
    whiteball7->setPos(290, 110);
    whiteball7->setState(0,0);
    scene->addItem(whiteball7);

    whiteball8 = new whiteBall(7);
    whiteball8->setPos(310, 140);
    whiteball8->setState(0,0);
    scene->addItem(whiteball8);

}
