#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include "ball.h"
#include "mytcpsocket.h"
#include "table.h"
#include "holes.h"
#include "cuetrack.h"
#include "button.h"
#include "pocket.h"
#include "score.h"
#include <QVector>
#include <QTimer>
#include <QObject>
#include <QList>
#include <QGraphicsRectItem>
#include <QPair>
#include <QDialogButtonBox>

class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game();
    QGraphicsScene * scene;
    QGraphicsRectItem * cursor;
    QGraphicsRectItem * useritem;
    QVector<Ball*> balls;
    //QVector<pockets*> pocket;
    Cuetrack tracker;
    QList<QPair<int, int> > position;
    Table *table;
    Score *score;
    //Button * button;
    Button* playAgain;
    Button* quit;
    QVector<Pocket*> pockets;
    QVector<Holes*> holes;
    int state;
    int user;
    void gameover();
    void switchuser();
    int jindong;
    int winner;
    bool judgement(int user1_2);
    int reset;
    MyTcpSocket * s;
private:
    QTimer * timer;
    QTimer * timer1;
    QGraphicsTextItem* gameText;
    QGraphicsRectItem* panel;
    void resetBalls();
    void ballMoveHandler();
    void scoreChangeHandler(int ballNumber);
    void design(int x, int y, int width, int height, QColor color, double opacity);
    //void startgame();
    //void resetgames();
private slots:
    void gameLogic();
    void resetgames();
};

#endif // GAME_H
