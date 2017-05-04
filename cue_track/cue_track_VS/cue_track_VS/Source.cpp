#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#define TOP 1
#define LEFT 2
#define BOTTOM 3
#define RIGHT 4
using namespace std;
using namespace cv;

vector<Point> detect_marker(Mat img, size_t n, int &number);
vector<Point> predict_marker(vector<Point>, vector<Point>);
bool filter(Point *start, Point *end, int m, int n, int Type);
vector<Point> spiral_marker_detect(Mat img, size_t n, int number, vector<Point> pre_vec);
int sim_detect_marker(unsigned char *data, int l, int &number);

int main() {
	VideoCapture capture;
	if (!capture.open(1))
		return 0;
	Mat cam_img;
	Mat cam;
	//int i = (int)(uchar)img.data + img.step[0] * 10 + img.step[1] * 10;
	//int j = (int)i;
	/*Mat convas = imread("convas.jpg");*/
	namedWindow("Camera");
	vector<Point> cur;
	vector<Point> pre;
	vector<Point> pre_corner;
	vector<Point> cur_corner;
	Mat red_hue_range;
	Mat blue_hue_range;
	while (1) {
		capture >> cam;
		//GaussianBlur(cam, cam, cv::Size(9, 9), 2, 2);
		cvtColor(cam, cam_img, CV_BGR2HSV);
		//GaussianBlur(cam, cam_img, Size(5, 5), 0, 0);
		//cam_img = imread("convas.jpg");
		if (cam_img.empty()) break;
		inRange(cam_img, cv::Scalar(110, 50, 50), cv::Scalar(130, 255, 255), blue_hue_range);
		cur_corner = spiral_marker_detect(blue_hue_range, size_t(5), 1, pre_corner);
		inRange(cam_img, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), red_hue_range);
		//inRange(cam_img, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);
		//addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
		//cur = detect_marker(cam_img, tar, size_t(20), 1);
		cur = spiral_marker_detect(red_hue_range, size_t(5), 1, pre);
		if (cur.size() != 0) {
			if (cur_corner.size() != 0) {
				cur[0].x -= cur_corner[0].x;
				cur[0].y -= cur_corner[0].y;
			}
		}
		//cur = predict_marker(pre, cur);
		//addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
		
		//HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 1, red_hue_image.rows / 8, 100, 20, 0, 0);

		//
		
		//if (cur_corner.size() != 0) {
			//rectangle(cam, cur_corner[0], Point(cur_corner[0].x + 20, cur_corner[0].y + 20), Scalar(0, 0, 0), 2);
		//}
 		imshow("Camera", cam);
		pre = cur;
		pre_corner = cur_corner;
		char c = cvWaitKey(50);
	}
	//namedWindow("目标");
	//imshow("目标", img);
	//namedWindow("convas");
	//imshow("convas", convas);
	//vector<Point> res = detect_marker(convas, target, size_t(10), 1);
	// 等待6000 ms后窗口自动关闭    
	//waitKey(6000);
	return 0;
}
vector<Point> spiral_marker_detect(Mat img, size_t n, int number, vector<Point> pre_vec) {
	int counter = 1;
	uchar tmp_R;
	uchar tmp_G;
	uchar tmp_B;
	vector<Point> res;
	int accum = 2;
	if (pre_vec.size() == 0) {
		return detect_marker(img, n, number);
	}
	Point pre = pre_vec[0];
	if ((pre.x > img.cols - 10) || (pre.x < 10) || (pre.y > img.rows - 10) || (pre.y < 10) )
		return detect_marker(img, n, number);
	Point top_start(pre.x, pre.y - 1);
	Point top_end(top_start.x + 1, top_start.y);
	Point right_start(top_end.x, top_end.y + 1);
	Point right_end(right_start.x, right_start.y + 1);
	Point bottom_start(right_end.x - 1, right_end.y);
	Point bottom_end(bottom_start.x - 1, bottom_start.y);
	Point left_start(bottom_end.x, bottom_end.y - 1);
	Point left_end(left_start.x, left_start.y - 1);
	bool flag = true;
	while (flag) {
		flag = false;
		if (filter(&top_start, &top_end, img.rows, img.cols, TOP)) {
			//Mat dist(img, Range(top_start.y, top_end.y + 1), Range(top_start.x, top_end.x + 1));
			int l = sim_detect_marker(img.data + (top_start.x + top_start.y * img.cols), top_end.x - top_start.x, number);
			if (l != -1) {
				res.push_back(Point(top_start.x + l, top_start.y));
				//return res;
			}
			flag = true;
			top_start.x--;
			top_start.y--;
			top_end.x++;
			top_end.y--;
		}
		/*if (filter(&right_start, &right_end, img.rows, img.cols, RIGHT)) {
			Mat dist(img, Range(right_start.y, right_end.y + 1), Range(right_start.x, right_end.x + 1));
			res = detect_marker(dist, target, n, number);
			if (number == 0) {
				res[0].y += right_start.y;
				res[0].x = right_start.x;
				return res;
			}
			flag = true;
			right_start.x++;
			right_start.y--;
			right_end.x++;
			right_end.y++;
		}*/
		if (filter(&bottom_start, &bottom_end, img.rows, img.cols, BOTTOM)) {
			//Mat dist(img, Range(bottom_end.y, bottom_start.y + 1), Range(bottom_end.x, bottom_start.x + 1));
			int l = sim_detect_marker(img.data + (bottom_end.x + bottom_end.y * img.cols), bottom_start.x - bottom_start.x, number);
			if (res.size() > 0) {
				if (l != -1) {
					Point tmp = res[0];
					res.pop_back();
					res.push_back(Point((bottom_end.x + l + tmp.x )/2, (bottom_end.y + tmp.y)/2));
					return res;
				}
				res.pop_back();
			}
			flag = true;
			bottom_start.x++;
			bottom_start.y++;
			bottom_end.x--;
			bottom_end.y++;
		}
		/*if (filter(&left_start, &left_end, img.rows, img.cols, LEFT)) {
			Mat dist(img, Range(left_end.y, left_start.y + 1), Range(left_end.x, left_start.x + 1));
			res = detect_marker(dist, target, n, number);
			if (number == 0) {
				res[0].y += left_end.y;
				res[0].x = left_end.x;
				return res;
			}
			flag = true;
			left_start.x--;
			left_start.y++;
			left_end.x--;
			left_end.y--;
		}*/
	}
	return res;
}
bool filter(Point *start, Point *end, int m, int n, int Type){
	switch (Type)
	{
	case TOP:
		if (start->y < 0)
			return false;
		start->x = (start->x < 0) ? 0 : start->x;
		end->x = (end->x < n) ? end->x : n - 1;
		break;
	case BOTTOM:
		if (start->y > m - 1)
			return false;
		start->x = (start->x > n - 1) ? n - 1 : start->x;
		end->x = (end->x > -1) ? end->x : 0;
		break;
	case LEFT:
		if (start->x < 0)
			return false;
		start->y = (start->y > m - 1) ? m - 1 : start->y;
		end->y = (end->y < 0) ? 0 : end->y;
		break;
	case RIGHT:
		if (start->x > n - 1)
			return false;
		start->y = (start->y > -1) ? start->y : 0;
		end->y = (end->y < m - 1) ? end->y : m - 1;
		break;
	default:
		return false;
	}
	return true;
}
int sim_detect_marker(unsigned char *data, int l, int &number) {
	int accum = 0;
	for (int i = 0; i < l; i++) {
		if (data[i] != 0)
			accum++;
		if (accum > 3) {
			number--;
			return i;
		}
	}
	return -1;
}
vector<Point> detect_marker(Mat img, size_t n, int &number) {
	uchar tmp_R;
	uchar tmp_G;
	uchar tmp_B;
	vector<Point> res;
	int accum = 0;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			/*Vec3b intensity = img.at<Vec3b>(i, j);
			tmp_B = (uchar)intensity.val[0];
			tmp_G = (uchar)intensity.val[1];
			tmp_R = (uchar)intensity.val[2];
			tmp_R = target.val[2] > tmp_R ? target.val[2] - tmp_R : tmp_R - target.val[2];
			tmp_G = target.val[1] > tmp_G ? target.val[1] - tmp_G : tmp_G - target.val[1];
			tmp_B = target.val[0] > tmp_B ? target.val[0] - tmp_B : tmp_B - target.val[0];
			if ((tmp_R > target.val[2] * 0.7) || (tmp_G > target.val[1] * 0.2) || (tmp_B > target.val[0] * 0.2)) {
				if (accum > 0)
					accum--;
				continue;
			}*/
			if (img.at<char>(i, j) != 0)
				accum++;
			if (accum > n) {
				number--;
				res.push_back(Point(j + n / 2, i + n / 2));
			}
			if (number < 1)
				return res;
		}
	}
	return res;
}

vector<Point> predict_marker(vector<Point> pre, vector<Point> cur) {
	if (cur.size() == 0) {
		return pre;
	}
	vector<Point> res;
	if (pre.size() == 0)
		return cur;
	res.push_back(Point((cur[0].x + pre[0].x)/2, (cur[0].y + pre[0].y) / 2));
	return res;
}

