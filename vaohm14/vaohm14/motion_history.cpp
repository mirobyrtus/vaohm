// Yeah we need some includes
#include "opencv/cv.h"
#include "opencv2/highgui/highgui.hpp"
#include <string>

// I prefer to use namespace more than use cv::everything cv::everywhere
using namespace cv;

// This is just for me, you can ignore it. I used it to turn ON/OFF logs when something went wrong
const bool debug = false;

// How long should the "effect" of motion history last? in seconds...
const double delay_seconds = 0.7;
// We can set transparency from 0 - 255
const double alpha_range = 255;
// If we want to represent the duration of motion by alpha channel...
const double delay_seconds_normalized = alpha_range / delay_seconds;

// Another constant, this time for thresholding, for details scroll down to "threshold(" call
const double threshold_value = 33; // This is just "random" value i found OK while testing

// We need a buffer to store the moves of X last frames
Mat* buffer;
// This is the X from last comment :P 
const int buffer_size = 4;
// We need to store somehow on which index of buffer we are 
int last_index = 0;

// Some temp matrices we use for load the video, process it and send it back to show in window
Mat motion_history_image;
Mat output_mhimage;

/// 
/// Takes input frame, process it, and the result writes directly into output frame
///
/// @param input_frame  (from webcam/videofile)
/// @param output_frame (reference to the Matrix that will be displayed into window)
///
void showMotion(Mat input_frame, Mat* output_frame)
{
	// Yes, we need to count time... I prefer to work with ticks rather than seconds
	double timestamp = (double)clock() / CLOCKS_PER_SEC;

	// Current and next index (for buffer), so that we know where we are
	int current_index, next_index;
	current_index = last_index;

	// TODO 
	Mat silhouette;

	// First time the mhi will be empty. We will initialize our Buffer and Matrices
	if (motion_history_image.empty()) { 

		// Initialize Buffer ...
		buffer = new Mat[buffer_size];
		// ... with zero-matrices
		for (int i = 0; i < buffer_size; i++) {
			buffer[i] = Mat::zeros(input_frame.size(), CV_8UC1); // In buffer we just need 1 channel
		}
		
		// And also our temporary Matrices
		motion_history_image = Mat::zeros(input_frame.size(), CV_32FC1);
		output_mhimage       = Mat::zeros(input_frame.size(), CV_8UC1);
	}
	
	// Copy Frame to buffer at the latest position
	cvtColor(input_frame, buffer[last_index], CV_BGR2GRAY); // Buffer has only one channel - copy it to grayscale
	
	// Calculate next index
	next_index = last_index + 1; // Move one index further
	next_index %= buffer_size;   // Don't reach out of the buffer, make it cyclic
	last_index = next_index;     // Now this is the last index
	
	// Difference from last two frames
	silhouette = buffer[next_index];
	absdiff(buffer[current_index], buffer[next_index], silhouette);
	
	// Get rid of the noise 
	threshold(silhouette, silhouette, threshold_value, 1, CV_THRESH_BINARY);

	// Now update motion image
	// For every pixel...
	for (int row = 0; row < motion_history_image.rows; ++row)
    {
		// Well, if you run it from web-cam, slower version is enough since webcam hasn't usualy high resolution
		// Whereas if you want to run it with HD video, you will probably get bad results. In this case try to run 
		// The second piece of code.. 
		/*
		// 1) Slower version - nicer code
		for (int col = 0; col < motion_history_image.cols; ++col)
        {				
			// If there was some change (Motion) in the image, ...
			if (silhouette.at<unsigned char>(row, col)) {  
				// ... mark it when it happened!
				motion_history_image.at<float>(row, col) = (float)timestamp; 
			} 
			// Otherwise, if there was some change (Motion) on this pixel, check if it was before "delay" seconds.
			else if (motion_history_image.at<float>(row, col) < (timestamp - delay_seconds)) {
				// If yes, set remove the "motion flag" = set it to zero!
				motion_history_image.at<float>(row, col) = 0;
			}			
        }
		*/
		// I don't think there is need to commend second block. It does exactly the same, but since 
		// "at" call cost pretty much time (i guess because of casting), we first get whole columns once
		// and then just index those arrays.

		
		// 2) Faster version!
		const unsigned char* silhouette_col = silhouette.ptr<uchar>(row);
		float* motion_history_image_col = motion_history_image.ptr<float>(row);
		
		for(int col = 0; col < motion_history_image.cols; ++col)
        {				
			if (silhouette_col[col]) {
				motion_history_image_col[col] = (float)timestamp;
			} 
			else if (motion_history_image_col[col] < (timestamp - delay_seconds)) {
				motion_history_image_col[col] = 0;
			}			
        }
		
    }

	// Copy MHImage to output_mhimage with transparency
	double alpha = delay_seconds_normalized; // This variable is explained at the declaration point
	double beta = (delay_seconds - timestamp)* delay_seconds_normalized; // The longer it takes (timestamp) the lower the beta
	if (debug) printf("ALPHA = %f, BETA = %f \n", alpha, beta);
	
	// Copy the processed motion_history_image into temp output_image with alpha and beta set
	convertScaleAbs(
		motion_history_image, output_mhimage,
		alpha, beta
	);

	// Copy mask to output frame - to be displayed in the output window
	output_mhimage.copyTo(*output_frame);
}

int main(int argc, char** argv)
{
	// You can either take the frames from webcam (0), or from a videofile (path_to_file) :
	// VideoCapture capture("C:\\Development\\VAoHM\\video_1.mp4");
	VideoCapture capture(0);
	
	// Placeholder for the result Motion History Image
	Mat* motion_history = NULL;

	// While you can read from File / capture from webcam do.. 
	if (capture.isOpened())
	{
		// We need some window to display our results
		namedWindow("MotionHistoryImages", 1);

		// Just work! Unless somebody/something break/stop you
		for (;;)
		{
			// Capture the frame
			Mat frame;
			capture >> frame;
			
			// If the frame is empty -> End
			if(frame.empty())
				break;

			// If it's NULL (first loop), initliazize it to Zero-Matrix
			if (motion_history == NULL)
				motion_history = new cv::Mat(cv::Mat::zeros(frame.rows, frame.cols, CV_8UC3));

			// Do our magic (Comments inside of the function)
			showMotion(frame, motion_history);

			// Show our results
			imshow("MotionHistoryImages", *motion_history);

			// Wait for keyboard input 
			waitKey(33); // 33 is random number since i dont use it. Better to use the key for ESC / Space..
		}
	}

	// Can't do anything more here, let's leave
	return 0;
}
	