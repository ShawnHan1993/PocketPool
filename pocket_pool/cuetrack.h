#ifndef CUETRACK_H
#define CUETRACK_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include<QPair>
using namespace cv;

class Cuetrack
{
public:
    Cuetrack();
    QPair<double, double> startTracking(QPair<double, double> ballPos);
    vector<Point> spiral_marker_detect(Mat img, Scalar target, size_t n, int number, vector<Point> pre_vec);
    vector<Point> detect_marker(Mat img, Scalar target, size_t n, int &number);
private:
    double vX;
    double vY;
    
    bool filter(Point *start, Point *end, int m, int n, int Type);
    vector<Point> detect_marker(Mat img, Scalar target, size_t n, int &number);
    vector<Point> predict_marker(vector<Point> pre, vector<Point> cur)
};

#endif // CUETRACK_H
