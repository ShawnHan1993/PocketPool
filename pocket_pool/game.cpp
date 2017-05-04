#include "game.h"
#include "button.h"
#include <QPair>
#include <QBrush>
#include <QDebug>
#include <QDialog>
#include <QGraphicsRectItem>
#include <QDialogButtonBox>
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
    reset = 0;
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

    score = new Score();
    scene->addItem(score);

    jindong = 0;
    winner = 0;
    useritem = new QGraphicsRectItem;
    useritem->setRect(0,0,10,10);
    useritem->setPos(450,5);
    useritem->setBrush(* new QBrush(Qt::yellow));
    scene->addItem(useritem);
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()),
            this, SLOT(gameLogic()));
    timer->start(50);


}

void Game::gameLogic()
{
    if (state == 0){
        reset = 0;
        timer->stop();
        qDebug()<<"balls rest";
        //scene->clear();
        resetBalls();
        delete score;
        score = new Score();
        scene->addItem(score);

        jindong = 0;
        winner = 0;
        //balls[0]->setState(5,0);
        state = 1;
        timer->start();
        return;
    }
    else if (state == 1){
        scene->addItem(cursor);
        timer->stop();
        QPair<double, double> tmp = tracker.startTracking();
        //scene->removeItem(cursor);
        for (int i = 0; i < balls.size(); i++)
            balls[i]->stop = 0;
        balls[0]->setState(tmp.first, tmp.second);
        state = 2;
        timer->start(50);
        scene->removeItem(cursor);
        return;
    }
    else if (state == 2){
        ballMoveHandler();

    return;
    }
}

void Game::resetgames(){
    delete timer1;
    for(size_t i = 0,n = balls.size();i<n;i++){
        if(balls[i]->inPocket == 0){
            qDebug() << " ball" << i;
            scene->removeItem(balls[i]);
        }
    }
    //scene->clear();
    delete panel;
    delete quit;
    delete playAgain;
    delete gameText;
    user = 0;
    useritem->setBrush(* new QBrush(Qt::yellow));
    state = 0;
    timer->start();

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
        if (jindong == 0){
            user = user == 0? 1:0;
            qDebug() << "user is ::::" << user;
            switchuser();
        }
        else
            jindong = 0;
        state = 1;
    }
}

void Game::resetBalls()
{

    position.append(qMakePair(200, 130));


    position.append(qMakePair(250, 130));// 1
    position.append(qMakePair(310, 160));// 2
    position.append(qMakePair(330, 170));// 2
    position.append(qMakePair(290, 110));// 1
    position.append(qMakePair(270, 120));// 2
    position.append(qMakePair(290, 150));// 1
    position.append(qMakePair(330, 90));// 2
    position.append(qMakePair(290, 130)); // 8
    position.append(qMakePair(310, 100));// 1

    position.append(qMakePair(330, 110));// 2
    position.append(qMakePair(330, 130));// 2
    position.append(qMakePair(310, 120));// 1

    position.append(qMakePair(310, 140));// 2
    position.append(qMakePair(270, 140));// 1
    position.append(qMakePair(330, 150));// 2


    for (int i = 0; i < balls.size(); i++){
        balls[i] = new Ball(i);
        balls[i]->setPos(position[i].first, position[i].second);
        balls[i]->setState(0, 0);
        scene->addItem(balls[i]);
    }

}
void Game::gameover(){
    timer->stop();

    timer1 = new QTimer(this);
    for(int i = 0;i<balls.size();i++){
        if(balls[i]->inPocket == 0)
            balls[i]->setState(0, 0);
    }

    QString message;
    if(winner == 1){
        message = "Player 1 win!";
    }
    else if(winner == 2){
        message = "Player 2 win!";
    }
    else{
       if(user == 0)
            message = "Player 1 lose!";
       else
            message = "Player 2 lose!";
    }

    for (size_t i = 0, n = scene->items().size(); i < n; i++){
            scene->items()[i]->setEnabled(false);
        }


    /*QDialog *yourDialog= new QDialog(this, Qt::Popup | Qt:: Dialog);

    yourDialog->exec();
    delete yourDialog;*/


    // pop up semi transparent panel
    //drawPanel(0,0,482,272,Qt::black,0.65);

        // draw panel
    drawPanel(0,0,482,272,Qt::black,0.5);
        // create playAgain button

    playAgain = new Button(QString("Replay"));
    playAgain->setPos(250,150);
    scene->addItem(playAgain);
    /*if(playAgain->clicked()){
        reset = 1;
    }*/

    connect(playAgain,SIGNAL(clicked()),this,SLOT(resetgames()));

        // create quit button
    quit = new Button(QString("Quit"));
    quit->setPos(250,200);
    scene->addItem(quit);
    connect(quit,SIGNAL(clicked()),this,SLOT(close()));
    timer1->start(50);
        // create text annoucning winner
    gameText = new QGraphicsTextItem(message);
    gameText->setPos(200,100);
    setFont(QFont("times",20));
    scene->addItem(gameText);


}

void Game::drawPanel(int x, int y, int width, int height, QColor color, double opacity){
    // draws a panel at the specified location with the specified properties
    panel = new QGraphicsRectItem(x,y,width,height);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    panel->setBrush(brush);
    panel->setOpacity(opacity);
    scene->addItem(panel);
}

bool Game::judgement(int user1_2){

    bool win = true;
    if(user1_2 == 1){
        for(int i =1 ;i<8;i++){
            if(balls[i]->inPocket == 0){
                win = false;
                break;
            }
        }
    }
    else{
        for(int i = 9 ;i<16;i++){
            if(balls[i]->inPocket == 0){
                win = false;
                break;
            }
        }
    }
    return win;
}

void Game::switchuser(){
    if(user == 0){
        useritem->setBrush(* new QBrush(Qt::yellow));
        qDebug() << "user = " << user;}
    else{
        useritem->setBrush(* new QBrush(Qt::blue));
        qDebug() << "user = " << user;
    }
}
