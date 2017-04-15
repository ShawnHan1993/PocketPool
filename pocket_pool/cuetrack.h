#ifndef CUETRACK_H
#define CUETRACK_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QPair>
#include <vector>
using namespace cv;
using namespace std;
class Cuetrack
{
public:
    Cuetrack();
    QPair<double, double> startTracking();
    vector<Point> spiral_marker_detect(Mat img, Scalar target, size_t n, int number, vector<Point> pre_vec);
    vector<Point> detect_marker(Mat img, Scalar target, size_t n, int &number);
private:
    double vX;
    double vY;
    
    bool filter(Point *start, Point *end, int m, int n, int Type);
    vector<Point> predict_marker(vector<Point> pre, vector<Point> cur);
};

#endif // CUETRACK_H
