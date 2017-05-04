#ifndef CUETRACK_H
#define CUETRACK_H

#include <QPair>
#include <vector>



class Cuetrack
{
public:
    Cuetrack();
    void startTracking();
    int detecting;
    QPair<double, double> hitValue;
};

#endif // CUETRACK_H
