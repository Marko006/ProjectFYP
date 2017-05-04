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

using namespace cv;



void HSV_Tbl(int, void*)
{

}

void HSV_Table() {
	
	namedWindow("HSVTable", 0);
	
	char HSVValueTable[50];
	sprintf(HSVValueTable, "H_MIN", H_MIN);
	sprintf(HSVValueTable, "H_MAX", H_MAX);
	sprintf(HSVValueTable, "S_MIN", S_MIN);
	sprintf(HSVValueTable, "S_MAX", S_MAX);
	sprintf(HSVValueTable, "V_MIN", V_MIN);
	sprintf(HSVValueTable, "V_MAX", V_MAX);
	    
	createTrackbar("H_MIN", "HSVTable", &H_MIN, H_MAX, HSV_Tbl);
	createTrackbar("H_MAX", "HSVTable", &H_MAX, H_MAX, HSV_Tbl);
	createTrackbar("S_MIN", "HSVTable", &S_MIN, S_MAX, HSV_Tbl);
	createTrackbar("S_MAX", "HSVTable", &S_MAX, S_MAX, HSV_Tbl);
	createTrackbar("V_MIN", "HSVTable", &V_MIN, V_MAX, HSV_Tbl);
	createTrackbar("V_MAX", "HSVTable", &V_MAX, V_MAX, HSV_Tbl);


}

void drawTable()
{
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

	
	CvSize ImageSize = cvSize(640, 480);			
	
	CvCapture* WebcamFeed;					
	
	IplImage* LiveVideo;			
	IplImage* ProcessedView;			
	IplImage* ImageHSV_val;             

	CvMemStorage* p_strStorage;			

	CvSeq* DetectedCircle;				
										

	float* Circle_XYPos;				
										
										
										

	int i;								
	char ExitCheck;			

	WebcamFeed = cvCaptureFromCAM(0);	

	if (WebcamFeed == NULL) {			
		printf("error: capture is NULL \n");	
		getchar();								
		return(-1);									}

	
	cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);		
	cvNamedWindow("Processed", CV_WINDOW_AUTOSIZE);		

	HSV_Table();
	ProcessedView = cvCreateImage(ImageSize,			
		IPL_DEPTH_8U,				1);					

	ImageHSV_val = cvCreateImage(ImageSize, IPL_DEPTH_8U, 3);

	while (1) {								
		LiveVideo = cvQueryFrame(WebcamFeed);		// get frame from webcam
		drawTable();

		if (LiveVideo == NULL) {					// if frame was not captured successfully . . .
			printf("error: frame is NULL \n");		// error message to std out
			getchar();
			break;
		}

		cvCvtColor(LiveVideo, ImageHSV_val, CV_BGR2HSV);

		cvInRangeS(ImageHSV_val,				// function input
			cvScalar(H_MIN, S_MIN, V_MIN),			
			ProcessedView);				// function output

		p_strStorage = cvCreateMemStorage(0);	// allocate necessary memory storage variable to pass into cvHoughCircles()

												
		cvSmooth(ProcessedView,		
			ProcessedView,		
			CV_GAUSSIAN,			
			9,						
			9);					

								
		DetectedCircle = cvHoughCircles(ProcessedView,		// input image, nothe that this has to be grayscale (no color)
			p_strStorage,			
			CV_HOUGH_GRADIENT,	
			2,					
			ProcessedView->height / 4,	
			100,					
			50,						
			1,	 
			400);						// max circle radius, in pixels

		if (DetectedCircle->total == 0)
		{
			// Initialize orientation
			std::cout << "Nothing Detected" << std::endl;
		
		}


		
		if (DetectedCircle->total == 1)
		{
			//printf("FPS:%d\n", fps);
			Circle_XYPos = (float*)cvGetSeqElem(DetectedCircle, 1);
			
			printf("ball position x = %f, y = %f, r = %f \n", Circle_XYPos[0],		// x position of center point of circle
				Circle_XYPos[1],		// y position of center point of circle
				Circle_XYPos[2]);	

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

			
			
			cvCircle(LiveVideo,										
				cvPoint(cvRound(Circle_XYPos[0]), cvRound(Circle_XYPos[1])),		
				3,													
				CV_RGB(255, 255, 0),									
				CV_FILLED);										

																
			cvCircle(LiveVideo,										
				cvPoint(cvRound(Circle_XYPos[0]), cvRound(Circle_XYPos[1])),		
				cvRound(Circle_XYPos[2]),					
				CV_RGB(255, 0, 0),									
				3);											
		}	// end for

		cvShowImage("Original", LiveVideo);			
		cvShowImage("Processed", ProcessedView);		

		cvReleaseMemStorage(&p_strStorage);				

		ExitCheck = cvWaitKey(10);				
		if (ExitCheck == 27) break;				
	}	// end while

	cvReleaseCapture(&WebcamFeed);					

	cvDestroyWindow("Original");
	cvDestroyWindow("Processed");

	// This closes the Serial Port
	CloseHandle(SerialPort);

	return(0);
}