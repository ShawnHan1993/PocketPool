#ifndef CUETRACK_H
#define CUETRACK_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QPair>
#include <vector>
#include <QObject>
#include <QTimer>

using namespace cv;
using namespace std;
class Cuetrack: public QObject
{
    Q_OBJECT
public:
    Cuetrack();
    QPair<double, double> startTracking();
    vector<Point> spiral_marker_detect(Mat img, size_t n, int number, vector<Point> pre_vec);
    vector<Point> detect_marker(Mat img, Scalar target, size_t n, int &number);
    int sim_detect_marker(unsigned char *data, int l, int &number);
private:
    double vX;
    double vY;
    QTimer * timer;
    bool filter(Point *start, Point *end, int m, int n, int Type);
    vector<Point> predict_marker(vector<Point> pre, vector<Point> cur);
private slots:
    void timerHandler();

};

#endif // CUETRACK_H
