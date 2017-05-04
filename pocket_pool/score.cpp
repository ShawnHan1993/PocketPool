#include "Score.h"
//#include "game.h"
#include <QFont>
#include <QDialog>

//extern Game * game;

Score::Score(QGraphicsItem *parent): QGraphicsTextItem(parent){
    // initialize the score to 0
    score1 = 0;
    score2 = 0;
    // draw the text
    setPlainText(QString("Score for player1:  ") + QString::number(score1) + QString("   Score for player2:  ")+ QString::number(score2)) ;
    setDefaultTextColor(Qt::blue);
    setFont(QFont("times",16));
}

void Score::increase1(){
    score1++;
    setPlainText(QString("Score for player1:  ") + QString::number(score1) + QString("   Score for player2:  ")+ QString::number(score2)) ; // Score: 1
}

void Score::increase2(){
    score2++;
    setPlainText(QString("Score for player1:  ") + QString::number(score1) + QString("   Score for player2:  ")+ QString::number(score2)); // Score: 1
}

int Score::getScore1(){
    return score1;
}

int Score::getScore2(){
    return score2;
}


