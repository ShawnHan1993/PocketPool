#include "cuetrack.h"
#include "game.h"
#include <vector>
#include <QBrush>
#include <QDebug>
#include "stdlib.h"
#define TOP 1
#define LEFT 2
#define BOTTOM 3
#define RIGHT 4

extern Game * game;

Cuetrack::Cuetrack()
{
    detecting = 0;
}

void Cuetrack::startTracking()
{
    int max = 20;
    int min = 10;
    double v = rand() % (max-min+1)+min;
    double y = rand() % (21) -10 ;
    hitValue = qMakePair(v, y);
    detecting = 2;
    return;
}
