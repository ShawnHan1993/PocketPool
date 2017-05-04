#include "ball.h"
#include "game.h"
#include "pocket.h"
#include "holes.h"
#include "score.h"
#include <QTimer>
#include <QList>
#include <QDebug>
#include <math.h>
#include <typeinfo>
#include <QString>
#include <QEvent>
#include <QPair>

extern Game * game;

Ball::Ball(int num)
{
    if (num == 0){
        setPixmap(QPixmap(":/new/balls/whiteBall.png")
                  .scaled(20,20));
        setData(0, num);
    }
    else if (num < 8){
        setPixmap(QPixmap(":/new/balls/b1.png")
                  .scaled(20,20));
        setData(0, num);
    }
    else if (num > 8){
        setPixmap(QPixmap(":/new/balls/b2.png")
                  .scaled(20,20));
        setData(0, num);
    }
    else{
        setData(0, num);
        setPixmap(QPixmap(":/new/balls/b8.png")
                  .scaled(20,20));
        }
    colliding.resize(game->balls.size());
    for (int i = 0; i < game->balls.size(); i++){
        colliding[i] = 0;
    }
    stop = 0;
    pocketCounter = 0;
    inPocket = 0;
    fangui = 0;
    switchuser = 1;
}


void Ball::setState(double x, double y)
{
    vX = x;
    vY = y;
}


void Ball::move()
{
    double theta1, theta2, alpha, beta1, beta2, d_x, d_y,
            delta_vX, delta_vY, delta_v, cos_alpha, sin_alpha;
    double enemyX, enemyY, enemyVx, enemyVy;

    VX = vX;
    VY = vY;

    for (int i = 0; i < game->balls.size(); i++){
        if (colliding[i] > 0){
            colliding[i] --;
        }
    }

    if (pocketCounter > 0)
        pocketCounter --;


    for (int i = 0; i < game->balls.size(); i++){
        if (i != data(0).toInt() && game->balls[i]->inPocket == 0 ){

            Ball *b = game->balls[i];
            if (b->colliding[data(0).toInt()] == 0 &&
                    colliding[b->data(0).toInt()] == 0){
            if (sqrt(pow(b->x() - x(),2) + pow(b->y()-y(), 2)) < 20){
                //qDebug()<<"hit!!";
                //qDebug()<<"ID:::::::::"<<this->data(0);
                //qDebug()<<"colliding!!";
                colliding[b->data(0).toInt()] = 4;
                b->colliding[data(0).toInt()] = 4;
                enemyX = b->x();
                enemyY = b->y();
                enemyVx = b->VX;
                enemyVy = b->VY;
                //qDebug()<<"enemyVx:"<<enemyVx<<"enemyVy:"<<enemyVy;
                d_x = x() - enemyX;
                d_y = y() - enemyY;
                //qDebug()<<"d_x:"<<d_x<<"d_y:"<<d_y;
                alpha = getRad(d_x, d_y);
                beta1 = getRad(vX, vY);
                theta1 = beta1 - alpha;
                beta2 = getRad(enemyVx, enemyVy);
                theta2 = beta2 - alpha;
                //qDebug()<<"alpha:"<<alpha;
                delta_v = sqrt(enemyVx*enemyVx + enemyVy*enemyVy)*
                        cos(theta2) - sqrt(vX*vX+vY*vY)*cos(theta1);

                //qDebug()<<"cos(theta2):"<<cos(theta1)<<"  cos(theta2):"<<cos(theta2);

                //qDebug()<<"theta1:"<<theta1<<"  theta2:"<<theta2;

                cos_alpha = (d_x) / (sqrt(pow(d_x, 2)+pow(d_y, 2)));
                sin_alpha = (d_y) / (sqrt(pow(d_x, 2)+pow(d_y, 2)));
                //qDebug()<<"delta_v:"<<delta_v;
                delta_vX = getAbs(delta_v) * cos_alpha;
                delta_vY = getAbs(delta_v) * sin_alpha;
                //qDebug()<<"delta_vX:"<<delta_vX<<"delta_vy:"<<delta_vY;
                vX = vX + delta_vX * 0.9;
                vY = vY + delta_vY * 0.9;

                //qDebug()<<"vx:"<<vX<<"vy:"<<vY;

                b->setState(b->vX - delta_vX * 0.9,
                            b->vY - delta_vY * 0.9);



        }
            }
    }
    }





    for (int i = 0; i < game->holes.size();i++){
        Holes *h = game->holes[i];
        int bqjd = 0;
        if(sqrt(pow((h->x()) - (x()+10),2) + pow((h->y())-(y()+10), 2)) < 20){
            game->jindong = 1;
            if (data(0).toInt() == 0){
                bqjd = 1;
                game->scene->removeItem(this);

                //resetWhiteBall();
                QPair<int,int> a;
                a.first = 240;
                a.second = 150;
                for(int i =1 ;i< game->balls.size();i++){
                    //Ball *b = game->balls[i];
                    if(game->balls[i]->inPocket == 0){
                        while(sqrt(pow(game->position[i].first - a.first, 2) + pow(game->position[i].second-a.second, 2)) < 20){
                            a.first += 5;
                            while(a.first > game->scene->height() - 28)
                                a.first -= 1;

                        }
                    }
                }
                //game->position[0] = a;
                game->balls[0]->inPocket = 0;
                game->balls[0]->stop = 0;
                setPos(a.first,a.second);
                game->scene->addItem(this);


            }
            else if (data(0).toInt() < 8 && game->user == 0){
                game->scene->removeItem(this);
                game->score->increase1();
                game->user == 0? 1: 0;
                game->switchuser();
            }
            else if (data(0).toInt() > 8 && game->user == 1){
                game->scene->removeItem(this);
                game->score->increase2();
                game->user == 0? 1: 0;
                game->switchuser();
            }

            else if ((data(0).toInt() > 8 && game->user == 0 )|| (data(0).toInt() < 8 && game->user == 1)){
                game->scene->removeItem(this);
                game->user == 0? 1: 0;
                game->switchuser();

            }

            else{
                if((game->user == 0 && game->score->getScore1() == 7 )|| (game->user == 1 && game->score->getScore2() == 7)){
                    game->scene->removeItem(this);
                    game->winner = game->user + 1;
                    inPocket = 1;
                    stop = 1;
                    game->gameover();
                }
                else if( (game->user == 0 && game->judgement(1) )|| (game->user == 1 && game->judgement(2))){
                    game->scene->removeItem(this);
                    game->winner = game->user + 1;
                    inPocket = 1;
                    stop = 1;
                    game->gameover();
                }
                else{
                    game->scene->removeItem(this);
                    inPocket = 1;
                    stop = 1;
                    game->gameover();
                }
                //game->user == 0? 1: 0;
                //game->switchuser();
                return;
            }
            //game->scene->removeItem(this);
            if (bqjd != 1){
                inPocket = 1;
                stop = 1;
            }
            else{
                bqjd = 0;
                game->user == 0? 1: 0;
                game->switchuser();
            }

            qDebug()<<"ball "<<data(0).toInt()<<" falls in the pocket "<<h->ID;
            return;
        }
    }
    int edgeX = 8;
    int edgeY1 = 33;
    int edgeY2 = 8;

    if (pocketZone() == 0)
    {
        if (pos().y() > scene()->height() - 20 - edgeY2
                || pos().y() < edgeY1)
        {
            //qDebug()<<"xxxx!!";
            if(pos().x() > scene()->width() - 20 - edgeX
                    || pos().x() < edgeX)
                vX = - vX * 1.1;
            vY = - vY * 1.1;
        }
        else if(pos().x()> scene()->width() - 20 - edgeX
                || pos().x() < edgeX)
        {
            //qDebug()<<"yyyyy!!";
            if (pos().y() > scene()->height() - 20 - edgeY2
                    || pos().y() < edgeY1)
                vY = - vY * 1.1;
            vX = - vX * 1.1;
        }
    }

    setPos(x()+vX, y()+vY);
    vX = vX * 0.96;
    vY = vY * 0.96;

    if (pow((vX * vX + vY * vY),2) < 0.01)
    {
        vX = 0;
        vY = 0;
        stop = 1;
    }
    else{
        stop = 0;
    }
}

double Ball::getRad(double x, double y)
{
    if (x > 0 && y > 0){
        return atan(getAbs(y/x));}
    else if (x < 0 && y > 0){
        return PI-atan(getAbs(y/x));}
    else if (x < 0 && y < 0)
        return atan(getAbs(y/x)) + PI;
    else if (x > 0 && y < 0)
        return (2 * PI)-atan(getAbs(y/x));
    else if (x > 0 && y == 0)
        return 0;
    else if (x < 0 && y == 0)
        return PI;
    else if (x == 0 && y > 0)
        return 3*PI/2;
    else if (x == 0 && y < 0)
        return PI/2;
    else if (x == 0 && y == 0)
        return 0;
    else{
        qDebug()<<"getRad Error!";
        return 0;
    }
}

double Ball::getAbs(double x)
{
    if (x<0)
        return -x;
    return x;
}

int Ball::pocketZone()
{
    if (x() + 10 < 26 && y() + 10 < 52)
        return 1;
    else if(x() + 10 < 26 && y() + 10 > 244)
        return 1;
    else if(x() + 10 > 453 && y() + 10 > 244)
        return 1;
    else if(x() + 10 > 453 && y() + 10 < 52)
        return 1;
    else if(x() + 10 > 223 && x() + 10 < 257)
        return 1;
    else
        return 0;
}

/*void resetWhiteBall(){
    QEvent *event;

    switch(event){
        case QEvent::TouchBegin:
                QList<QTouchEvent::TouchPoint> touchBeginPoints = static_cast<QTouchEvent *>(event)->touchPoints(); }
    QPair<int,int> a;
    a.first = 240;
    a.second = 150;
    for(int i =1 ;i< game->balls.size();i++){
        //Ball *b = game->balls[i];
        if(game->balls[i]->inPocket == 0){
            while(sqrt(pow(game->position[i].first - a.first, 2) + pow(game->position[i].second-a.second, 2)) < 20){
                a.first += 5;
                while(a.first > game->scene->height() - 28)
                    a.first -= 1;

            }
        }
    }
    //game->position[0] = a;
    setPos(240,150);
    game->scene->addItem(this);


}*/
