#include "opencv/cv.h"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

const double duration_seconds = 1;
const int N = 4;

int last_index = 0;
Mat* buffer;

Mat motion_history_image;
Mat mask;

void motionUpdate( Mat silhouette, Mat motion_history_imagemg, double timestamp, double duration )
{
	Size size = motion_history_imagemg.size();

    if( CV_IS_MAT_CONT( motion_history_imagemg.type() & silhouette.type() ))
    {
        size.width *= size.height;
        size.height = 1;
    }

    float ts = (float)timestamp;
    float delbound = (float)(timestamp - duration);
    int x, y;

    for( y = 0; y < size.height; y++ )
    {
        const uchar* silhData = silhouette.data + silhouette.step*y;
        float* motion_history_imageData = (float*)(motion_history_imagemg.data + motion_history_imagemg.step*y);
        x = 0;

        for( ; x < size.width; x++ )
        {
            float val = motion_history_imageData[x];
            val = silhData[x] ? ts : val < delbound ? 0 : val;
            motion_history_imageData[x] = val;
        }
    }
}

static void  update(Mat input_frame, Mat* dst2, int diff_threshold)
{
	double timestamp = (double)clock() / CLOCKS_PER_SEC; // get current time in seconds
	
	Size size = Size(input_frame.rows, input_frame.cols);
	
	int i, idx1 = last_index, idx2;
	Mat silh;
	Rect comp_rect;

	if (motion_history_image.empty() || motion_history_image.rows != size.width || motion_history_image.cols != size.height) {
		
		if (buffer == NULL) {
			buffer = new Mat[4];
		}

		for (i = 0; i < N; i++) {
			buffer[i] = Mat::zeros(Size(size.height, size.width), CV_8UC1); // Should have just 1 channel
		}
		
		motion_history_image = Mat::zeros(Size(size.height, size.width), CV_32FC1);
		mask = Mat(Size(size.height, size.width), CV_8UC1);
	} 
	
	cvtColor(input_frame, buffer[last_index], CV_BGR2GRAY); 
	
	idx2 = (last_index + 1) % N; // index of (last_index - (N-1))th frame
	last_index = idx2;

	silh = buffer[idx2];
	absdiff(buffer[idx1], buffer[idx2], silh);
	
	threshold(silh, silh, diff_threshold, 1, CV_THRESH_BINARY);
	motionUpdate(silh, motion_history_image, timestamp, duration_seconds);
	
	convertScaleAbs(
		motion_history_image,
		mask,
		255. / duration_seconds,
		(duration_seconds - timestamp)*255. / duration_seconds
	);
	
	mask.copyTo(*dst2);
}

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

			update(frame, motion_history, 30);
			imshow("MotionHistoryImages", *motion_history);

			if (waitKey(10) >= 0) return 0;
		}
	}

	return 9;
}
