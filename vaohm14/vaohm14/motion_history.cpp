#include "opencv/cv.h"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

int main(int argc, char** argv)
{
	VideoCapture capture(0);
    Mat* motion_history = NULL;

	if (capture.isOpened())
	{
		namedWindow("MotionHistoryImages", 1);

		for (;;)
		{
			Mat frame;
			capture >> frame;
			
			if(frame.empty())
				break;

			if (motion_history == NULL)
				motion_history = new cv::Mat(cv::Mat::zeros(frame.rows, frame.cols, CV_8UC3));

			// Process the frame somehow and save it to motion
			
			imshow("MotionHistoryImages", *motion_history);

			if (waitKey(10) >= 0) return 0;
		}
	}

	return 9;
}
