#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>

class Score: public QGraphicsTextItem{
public:
    Score(QGraphicsItem * parent=0);
    void increase1();
    void increase2();
    int getScore1();
    int getScore2();
private:
    int score1;
    int score2;
};

#endif // SCORE_H
