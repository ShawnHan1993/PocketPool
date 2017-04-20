#include "game.h"
#include <QPair>
#include <QBrush>
#include <QDebug>

Game::Game()
{
    //tracker = new Cuetrack();
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,480,272);
    scene->setBackgroundBrush(QBrush(Qt::green));
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setFixedSize(480,272);
    cursor = new QGraphicsRectItem();
    cursor->setRect(0,0,3,3);
    cursor->setBrush(QBrush(Qt::blue));
    cursor->setPos(200,200);
    //scene->removeItem(cursor);
    state = 0;
    // Set the table
    table = new Table();
    table->setPos(0, 25);
    scene->addItem(table);

    // set Holes
    holes.resize(6);
    for (int i = 0; i < holes.size(); i++){
        holes[i] = new Holes(i);
        scene->addItem(holes[i]);
    }

    // Set Balls
    balls.resize(16);
    // set Pockets
    pockets.resize(12);
    for (int i = 0; i < pockets.size(); i++){
        pockets[i] = new Pocket(i);
        scene->addItem(pockets[i]);
    }


    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()),
            this, SLOT(gameLogic()));
    timer->start(50);


}

void Game::gameLogic()
{
    if (state == 0){
        timer->stop();
        qDebug()<<"balls rest";
        resetBalls();
        //balls[0]->setState(5,0);
        state = 1;
        timer->start(50);
        return;
    }
    else if (state == 1){
        if (tracker.detecting == 0){
            scene->addItem(cursor);
            tracker.detecting = 1;
            qDebug()<<"start tracking";
            tracker.startTracking();
            return;
        }
        else if (tracker.detecting == 1){
            return;
        }
        else if (tracker.detecting == 2){
            timer->stop();
            state = 2;
            scene->removeItem(cursor);
            for (int i = 0; i < balls.size(); i++)
                if (balls[i]->inPocket == 0)
                    balls[i]->stop = 0;
            tracker.detecting = 0;
            balls[0]->setState(tracker.hitValue.first, tracker.hitValue.second);
            timer->start(5);
            qDebug()<<"start moving balls";
        }
    }
    else if (state == 2){
        ballMoveHandler();
    return;
    }
}

void Game::scoreChangeHandler(int ballNumber)
{
    // do something
}

void Game::ballMoveHandler()
{
    int tmp = 0;
    for (int i = 0; i < balls.size(); i++){
        if (balls[i]->inPocket == 0)
            balls[i]->move();
    }
    for (int i = 0; i < balls.size(); i++){
        tmp += balls[i]->stop;
    }
    if (tmp == balls.size()){
        qDebug()<<"All balls stopped!!!";
        state = 1;
    }
}

void Game::resetBalls()
{
    position.append(qMakePair(200, 130));
    position.append(qMakePair(250, 130));
    position.append(qMakePair(270, 120));
    position.append(qMakePair(270, 140));
    position.append(qMakePair(290, 110));
    position.append(qMakePair(290, 130));
    position.append(qMakePair(290, 150));
    position.append(qMakePair(310, 100));
    position.append(qMakePair(310, 120));
    position.append(qMakePair(310, 140));
    position.append(qMakePair(310, 160));
    position.append(qMakePair(330, 90));
    position.append(qMakePair(330, 110));
    position.append(qMakePair(330, 130));
    position.append(qMakePair(330, 150));
    position.append(qMakePair(330, 170));

    for (int i = 0; i < balls.size(); i++){
        balls[i] = new Ball(i);
        balls[i]->setPos(position[i].first, position[i].second);
        balls[i]->setState(0, 0);
        scene->addItem(balls[i]);
    }

}
