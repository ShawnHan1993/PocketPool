#include "cuetrack.h"
#include "game.h"
#include <vector>
#include <QBrush>
#include <QDebug>
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
    hitValue = qMakePair(20.0, 2.0);
    detecting = 2;
    return;
}
