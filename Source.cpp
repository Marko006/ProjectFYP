#include <iostream>

#include<opencv/cvaux.h>
#include<opencv/highgui.h>
#include<opencv/cxcore.h>

#include <sstream>
#include <string>
#include <opencv\cv.h>
#include <opencv2\highgui\highgui.hpp>

#include<stdio.h>
#include<stdlib.h>

// Need to include this for serial port communication
#include <Windows.h>

#define CAM_PIX_WIDTH 640
#define CAM_PIX_HEIGHT 480
///////////////////////////////////////////////////////////////////////////////////////////////////
IplImage* frameGrabbed;
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int fps = 60;
int V_MAX = 256;
int table_pix_minx;
int table_pix_miny;
int table_pix_maxx;
int table_pix_maxy;

using namespace cv;// variables to draw the table


//const string trackbarWindowName = "Trackbars";
void HSV_Tbl(int, void*)
{

}

void HSV_Table() {
	//create window for trackbars

	
	namedWindow("HSVTable", 0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf(TrackbarName, "H_MIN", H_MIN);
	sprintf(TrackbarName, "H_MAX", H_MAX);
	sprintf(TrackbarName, "S_MIN", S_MIN);
	sprintf(TrackbarName, "S_MAX", S_MAX);
	sprintf(TrackbarName, "V_MIN", V_MIN);
	sprintf(TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
	createTrackbar("H_MIN", "HSVTable", &H_MIN, H_MAX, HSV_Tbl);
	createTrackbar("H_MAX", "HSVTable", &H_MAX, H_MAX, HSV_Tbl);
	createTrackbar("S_MIN", "HSVTable", &S_MIN, S_MAX, HSV_Tbl);
	createTrackbar("S_MAX", "HSVTable", &S_MAX, S_MAX, HSV_Tbl);
	createTrackbar("V_MIN", "HSVTable", &V_MIN, V_MAX, HSV_Tbl);
	createTrackbar("V_MAX", "HSVTable", &V_MAX, V_MAX, HSV_Tbl);


}
///////////////////////////////////////////////////////////////////////////////////////////////////
// Draw the table marks
void drawTable()
{
	// Draw marks on image
	// Draw marker on center
	cvLine(frameGrabbed, cvPoint(CAM_PIX_WIDTH / 4 - 5, CAM_PIX_HEIGHT / 4), cvPoint(CAM_PIX_WIDTH / 4 + 5, CAM_PIX_HEIGHT / 4), cvScalar(100, 255, 200), 1);
	cvLine(frameGrabbed, cvPoint(CAM_PIX_WIDTH / 4, CAM_PIX_HEIGHT / 4 - 5), cvPoint(CAM_PIX_WIDTH / 4, CAM_PIX_HEIGHT / 4 + 5), cvScalar(100, 255, 200), 1);
	//Draw table limits
	cvLine(frameGrabbed, cvPoint(table_pix_minx, table_pix_miny), cvPoint(table_pix_minx + 20, table_pix_miny), cvScalar(100, 255, 200), 1);
	cvLine(frameGrabbed, cvPoint(table_pix_minx, table_pix_maxy), cvPoint(table_pix_minx + 20, table_pix_maxy), cvScalar(100, 255, 200), 1);
	cvLine(frameGrabbed, cvPoint(table_pix_maxx - 20, table_pix_miny), cvPoint(table_pix_maxx, table_pix_miny), cvScalar(100, 255, 200), 1);
	cvLine(frameGrabbed, cvPoint(table_pix_maxx - 20, table_pix_maxy), cvPoint(table_pix_maxx, table_pix_maxy), cvScalar(100, 255, 200), 1);
	cvLine(frameGrabbed, cvPoint(table_pix_minx, table_pix_miny), cvPoint(table_pix_minx, table_pix_miny + 20), cvScalar(100, 255, 200), 1);
	cvLine(frameGrabbed, cvPoint(table_pix_maxx, table_pix_miny), cvPoint(table_pix_maxx, table_pix_miny + 20), cvScalar(100, 255, 200), 1);
	cvLine(frameGrabbed, cvPoint(table_pix_minx, table_pix_maxy - 20), cvPoint(table_pix_minx, table_pix_maxy), cvScalar(100, 255, 200), 1);
	cvLine(frameGrabbed, cvPoint(table_pix_maxx, table_pix_maxy - 20), cvPoint(table_pix_maxx, table_pix_maxy), cvScalar(100, 255, 200), 1);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	// Setup serial port connection and needed variables.
	HANDLE SerialPort = CreateFile(L"COM5", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (SerialPort != INVALID_HANDLE_VALUE)
	{
		printf("Port opened! \n");

		DCB SerialParameters;
		GetCommState(SerialPort, &SerialParameters);

		SerialParameters.BaudRate = CBR_9600;
		SerialParameters.ByteSize = 8;
		SerialParameters.Parity = NOPARITY;
		SerialParameters.StopBits = ONESTOPBIT;

		SetCommState(SerialPort, &SerialParameters);
	}
	else
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("Serial port doesn't exist! \n");
		}

		printf("Error while setting up serial port! \n");
	}
	
	
	char OutputCharacters[] = "c";
	DWORD btsIO;

	// Setup OpenCV variables and structures
	CvSize ImageSize = cvSize(640, 480);			// use a 640 x 480 size for all windows, also make sure your webcam is set to 640x480 !!
	
	CvCapture* WebcamFeed;						// we will assign our web cam video stream to this later . . .
	
	IplImage* LiveVideo;			// pointer to an image structure, this will be the input image from webcam
	IplImage* ProcessedView;			// pointer to an image structure, this will be the processed image
	IplImage* ImageHSV_val;                 // pointer to an image structure, this will hold the image after the color has been changed from RGB to HSV
										// IPL is short for Intel Image Processing Library, this is the structure used in OpenCV 1.x to work with images

	CvMemStorage* p_strStorage;			// necessary storage variable to pass into cvHoughCircles()

	CvSeq* DetectedCircle;				// pointer to an OpenCV sequence, will be returned by cvHough Circles() and will contain all circles
										// call cvGetSeqElem(p_seqCircles, i) will return a 3 element array of the ith circle (see next variable)

	float* Circle_XYPos;				// pointer to a 3 element array of floats
										// [0] => x position of detected object
										// [1] => y position of detected object
										// [2] => radius of detected object

	int i;								// loop counter
	char ExitCheck;			// char for checking key press (Esc exits program)

	WebcamFeed = cvCaptureFromCAM(0);	// 0 => use 1st webcam, may have to change to a different number if you have multiple cameras

	if (WebcamFeed == NULL) {			// if capture was not successful . . .
		printf("error: capture is NULL \n");	// error message to standard out . . .
		getchar();								// getchar() to pause for user see message . . .
		return(-1);								// exit program
	}

	// declare 2 windows
	cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);		// original image from webcam
	cvNamedWindow("Processed", CV_WINDOW_AUTOSIZE);		// the processed image we will use for detecting circles

	HSV_Table();
	ProcessedView = cvCreateImage(ImageSize,			// 640 x 480 pixels (CvSize struct from earlier)
		IPL_DEPTH_8U,		// 8-bit color depth
		1);					// 1 channel (grayscale), if this was a color image, use 3

	ImageHSV_val = cvCreateImage(ImageSize, IPL_DEPTH_8U, 3);
	//cvSetCaptureProperty(WebcamFeed, CV_CAP_PROP_FPS, fps);//ps

	// Main program loop
	while (1) {								// for each frame . . .
		LiveVideo = cvQueryFrame(WebcamFeed);		// get frame from webcam
		drawTable();

		if (LiveVideo == NULL) {					// if frame was not captured successfully . . .
			printf("error: frame is NULL \n");		// error message to std out
			getchar();
			break;
		}

		// Change the color model from RGB (BGR) to HSV. This makes it easier to choose a color based on Hue
		cvCvtColor(LiveVideo, ImageHSV_val, CV_BGR2HSV);

		cvInRangeS(ImageHSV_val,				// function input
			cvScalar(H_MIN, S_MIN, V_MIN),			// min filtering value (if color is greater than or equal to this)
			cvScalar(H_MAX, S_MAX, V_MAX),			// max filtering value (if color is less than this)
			ProcessedView);				// function output

		p_strStorage = cvCreateMemStorage(0);	// allocate necessary memory storage variable to pass into cvHoughCircles()

												// smooth the processed image, this will make it easier for the next function to pick out the circles
		cvSmooth(ProcessedView,		// function input
			ProcessedView,		// function output
			CV_GAUSSIAN,			// use Gaussian filter (average nearby pixels, with closest pixels weighted more)
			9,						// smoothing filter window width
			9);					// smoothing filter window height

								// fill sequential structure with all circles in processed image
		DetectedCircle = cvHoughCircles(ProcessedView,		// input image, nothe that this has to be grayscale (no color)
			p_strStorage,			// provide function with memory storage, makes function return a pointer to a CvSeq
			CV_HOUGH_GRADIENT,	// two-pass algorithm for detecting circles, this is the only choice available
			2,					// size of image / 2 = "accumulator resolution", i.e. accum = res = size of image / 2
			ProcessedView->height / 4,	// min distance in pixels between the centers of the detected circles
			100,						// high threshold of Canny edge detector, called by cvHoughCircles
			50,						// low threshold of Canny edge detector, called by cvHoughCircles
			1,	 //10					// min circle radius, in pixels
			400);						// max circle radius, in pixels

										// Run this if the camera doesn't detect any circles
		if (DetectedCircle->total == 0)
		{
			// Initialize orientation
			std::cout << "Nothing Detected" << std::endl;
		
		}


		// Run this if the camera can see at least one circle
		if (DetectedCircle->total == 1)
		{
			//printf("FPS:%d\n", fps);
			Circle_XYPos = (float*)cvGetSeqElem(DetectedCircle, 1);	// from the sequential structure, read the ith value into a pointer to a float
			
			printf("ball position x = %f, y = %f, r = %f \n", Circle_XYPos[0],		// x position of center point of circle
				Circle_XYPos[1],		// y position of center point of circle
				Circle_XYPos[2]);	// radius of circle

//**********************************************************************************************************************
			if (Circle_XYPos[0] < 190)
			{
				OutputCharacters[0] = 'a';
				WriteFile(SerialPort, OutputCharacters, strlen(OutputCharacters), &btsIO, NULL);

				std::cout << "outputchars = " << OutputCharacters[0] << std::endl;
				std::cout << "Move Forward" << std::endl;
			}

			if (Circle_XYPos[0] > 190)
			{
				OutputCharacters[0] = 'd';
				WriteFile(SerialPort, OutputCharacters, strlen(OutputCharacters), &btsIO, NULL);

				std::cout << "outputchars = " << OutputCharacters[0] << std::endl;
				std::cout << "Move Back" << std::endl;
			}
			
			
			if (Circle_XYPos[1] < 125)
			{
				OutputCharacters[0] = '2';
				WriteFile(SerialPort, OutputCharacters, strlen(OutputCharacters), &btsIO, NULL);
				std::cout << "outputchars = " << OutputCharacters[0] << std::endl;
				std::cout << "Move to P2" << std::endl;
			}

			if (Circle_XYPos[1] >150 && Circle_XYPos[1] <170)
			{
				OutputCharacters[0] = '3';
				WriteFile(SerialPort, OutputCharacters, strlen(OutputCharacters), &btsIO, NULL);
				std::cout << "outputchars = " << OutputCharacters[0] << std::endl;
				std::cout << "Move to P3" << std::endl;
			}

			if (Circle_XYPos[1] > 190 && Circle_XYPos[1] < 210)
			{
				OutputCharacters[0] = '4';
				WriteFile(SerialPort, OutputCharacters, strlen(OutputCharacters), &btsIO, NULL);
				std::cout << "outputchars = " << OutputCharacters[0] << std::endl;
				std::cout << "Move to P4" << std::endl;
			}

			if (Circle_XYPos[1] >230 && Circle_XYPos[1] < 247)
			{
				OutputCharacters[0] = '5';
				WriteFile(SerialPort, OutputCharacters, strlen(OutputCharacters), &btsIO, NULL);
				std::cout << "outputchars = " << OutputCharacters[0] << std::endl;
				std::cout << "Move to P5" << std::endl;
			}
			if (Circle_XYPos[1] > 250 && Circle_XYPos[1] < 290)
			{
				OutputCharacters[0] = '6';
				WriteFile(SerialPort, OutputCharacters, strlen(OutputCharacters), &btsIO, NULL);
				std::cout << "outputchars = " << OutputCharacters[0] << std::endl;
				std::cout << "Move to P6" << std::endl;
			}
			if (Circle_XYPos[1] > 295 && Circle_XYPos[1] < 328)
			{
				OutputCharacters[0] = '7';
				WriteFile(SerialPort, OutputCharacters, strlen(OutputCharacters), &btsIO, NULL);
				std::cout << "outputchars = " << OutputCharacters[0] << std::endl;
				std::cout << "Move to P7" << std::endl;
			}

			if (Circle_XYPos[1] > 330 && Circle_XYPos[1] <368)
			{
				OutputCharacters[0] = '8';
				WriteFile(SerialPort, OutputCharacters, strlen(OutputCharacters), &btsIO, NULL);
				std::cout << "outputchars = " << OutputCharacters[0] << std::endl;
				std::cout << "Move to P8" << std::endl;
			}

			
			///////////////////////////////////////////////////////////////////
			// draw a small green circle at center of detected object
			cvCircle(LiveVideo,										// draw on the original image
				cvPoint(cvRound(Circle_XYPos[0]), cvRound(Circle_XYPos[1])),		// center point of circle
				3,													// 3 pixel radius of circle
				CV_RGB(255, 255, 0),									// draw pure green
				CV_FILLED);										// thickness, fill in the circle

																// draw a red circle around the detected object
			cvCircle(LiveVideo,										// draw on the original image
				cvPoint(cvRound(Circle_XYPos[0]), cvRound(Circle_XYPos[1])),		// center point of circle
				cvRound(Circle_XYPos[2]),							// radius of circle in pixels
				CV_RGB(255, 0, 0),									// draw pure red
				3);												// thickness of circle in pixels
		}	// end for

		cvShowImage("Original", LiveVideo);			// original image with detectec ball overlay
		cvShowImage("Processed", ProcessedView);		// image after processing

		cvReleaseMemStorage(&p_strStorage);				// deallocate necessary storage variable to pass into cvHoughCircles

		ExitCheck = cvWaitKey(10);				// delay (in ms), and get key press, if any
		if (ExitCheck == 27) break;				// if Esc key (ASCII 27) was pressed, jump out of while loop
	}	// end while

	cvReleaseCapture(&WebcamFeed);					// release memory as applicable

	cvDestroyWindow("Original");
	cvDestroyWindow("Processed");

	// This closes the Serial Port
	CloseHandle(SerialPort);

	return(0);
}