#include "ball.h"
#include "game.h"
#include "pocket.h"
#include "holes.h"
#include <QTimer>
#include <QList>
#include <QDebug>
#include <math.h>
#include <typeinfo>
#include <QString>



extern Game * game;


Ball::Ball(int num)
{
    if (num == 0){
        setPixmap(QPixmap(":/new/balls/whiteBall.png")
                  .scaled(20,20));
        setData(0, num);
    }
    else{
        setData(0, num);
        setPixmap(QPixmap(":/new/balls/1.png")
                  .scaled(20,20));
        }
    colliding.resize(game->balls.size());
    for (int i = 0; i < game->balls.size(); i++){
        colliding[i] = 0;
    }
    stop = 0;
    pocketCounter = 0;
    inPocket = 0;
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

    QList<QGraphicsItem *>colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size();i<n;++i){
        if (typeid(*(colliding_items[i])) == typeid(Ball)){

            Ball *t = (Ball*)colliding_items[i];

            if (t->colliding[data(0).toInt()] == 0 &&
                    colliding[t->data(0).toInt()] == 0)
{

            //qDebug()<<"ID:::::::::"<<this->data(0);
            //qDebug()<<"colliding!!";
            colliding[colliding_items[i]->data(0).toInt()] = 4;
            t->colliding[data(0).toInt()] = 4;
            enemyX = colliding_items[i]->pos().x();
            enemyY = colliding_items[i]->pos().y();
            enemyVx = t->VX;
            enemyVy = t->VY;
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

            t->setState(t->vX - delta_vX * 0.9,
                        t->vY - delta_vY * 0.9);
}

        }
        else if(typeid(*(colliding_items[i])) == typeid(Pocket) && pocketCounter == 0){
            //qDebug()<<"hit the pocket";
            pocketCounter = 2;
            double tmpX = vX;
            double tmpY = vY;
            Pocket *p = (Pocket*)colliding_items[i];
            if (p->ID == 0){
                vX = tmpY * 1.01;
                vY = tmpX * 1.01;
            }
            else if(p->ID == 1){
                vX = tmpY * 1.01;
                vY = tmpX * 1.01;
            }
            else if(p->ID == 2){
                vX = tmpY * -1.01;
                vY = tmpX * -1.01;
            }
            else if(p->ID == 3){
                vX = tmpY * -1.01;
                vY = tmpX * -1.01;
            }
            else if(p->ID == 4){
                vX = tmpY * 1.01;
                vY = tmpX * 1.01;
            }
            else if(p->ID == 5){
                vX = tmpY * 1.01;
                vY = tmpX * 1.01;
            }
            else if(p->ID == 6){
                vX = tmpY * -1.01;
                vY = tmpX * -1.01;
            }
            else if(p->ID == 7){
                vX = tmpY * -1.01;
                vY = tmpX * -1.01;
            }
            else if(p->ID == 8){
                vX = tmpY * -1.01;
                vY = tmpX * -1.01;
            }
            else if(p->ID == 9){
                vX = tmpY * 1.01;
                vY = tmpX * 1.01;
            }
            else if(p->ID == 10){
                vX = tmpY * -1.01;
                vY = tmpX * -1.01;
            }
            else if(p->ID == 11){
                vX = tmpY * 1.01;
                vY = tmpX * 1.01;
            }
        }
        else if(typeid(*(colliding_items[i])) == typeid(Holes)){
            Holes *h = (Holes*)colliding_items[i];
            game->scene->removeItem(this);
            inPocket = 1;
            stop = 1;
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
                vX = - vX * 1.01;
            vY = - vY * 1.01;
        }
        else if(pos().x()> scene()->width() - 20 - edgeX
                || pos().x() < edgeX)
        {
            //qDebug()<<"yyyyy!!";
            if (pos().y() > scene()->height() - 20 - edgeY2
                    || pos().y() < edgeY1)
                vY = - vY * 1.01;
            vX = - vX * 1.01;
        }
    }

    setPos(x()+vX, y()+vY);
    vX = vX * 0.996;
    vY = vY * 0.996;

    if (pow((vX * vX + vY * vY),2) < 0.000001)
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
