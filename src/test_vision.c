//gcc -o test_vision test_vision.c -std=c99 `pkg-config --cflags --libs opencv`

#include <stdio.h>
#include "cv.h"
#include "highgui.h"

#define DETECTION_THRESH 10

void saturate_sv( IplImage* img );
IplImage* GetThresholdedImage(IplImage* imgHSV);      
int is_orange_seen(CvCapture* capture);


main( int argc, char* argv[] ) {
	CvCapture* capture = 0;
	//capture = cvCreateCameraCapture(CV_CAP_ANY);
	capture = cvCaptureFromCAM(-1);
	assert( capture != NULL );
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 640);
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480);
	// Rest of program proceeds totally ignorant
  //cvNamedWindow( "mywindow", 0); //CV_WINDOW_AUTOSIZE );
   // Show the image captured from the camera in the window and repeat
	while(1) {
		fprintf(stderr, "%d ", is_orange_seen(capture));
     // Do not release the frame!
     //If ESC key pressed, Key=0x10001B under OpenCV 0.9.7(linux version),
     //remove higher bits using AND operator
     if ( (cvWaitKey(10) & 255) == 27 ) 
		break;
	}
   // Release the capture device housekeeping
   cvReleaseCapture( &capture );
//   cvDestroyWindow( "mywindow" );
}   

int is_orange_seen(CvCapture* capture) {
     int detected = 0;
		// Get one frame
     IplImage* frame = cvQueryFrame( capture );
     if ( !frame ) {
       fprintf( stderr, "ERROR: frame is null...\n" );
       getchar();
       return -1;
     }
//	  int thresh = 100;
//	  int colour=255;
//	  threshold(frame,frame, thresh,colour,CV_THRESH_OTSU);
	  cvSmooth(frame, frame, CV_GAUSSIAN,3,3,3.0,3.0); 
	  IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
	  cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
     IplImage* imgThresh = GetThresholdedImage(imgHSV);
	  CvScalar avg = cvAvg(imgThresh, NULL);
	  //printf("%f\n", avg.val[0]);
//     cvShowImage( "mywindow", imgThresh );
//     cvShowImage( "mywindow", frame);
		if (avg.val[0] > DETECTION_THRESH) {
			detected = 1;
		}
	return detected;
}


void saturate_sv( IplImage* img ) {

  for( int y=0; y<img->height; y++ ) {
    uchar* ptr = (uchar*) (
      img->imageData + y * img->widthStep
    );
    for( int x=0; x<img->width; x++ ) {
      ptr[3*x+1] = 255;
  //    ptr[3*x+2] = 255;
    }
  }
}

IplImage* GetThresholdedImage(IplImage* imgHSV){ 
       IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
       cvInRangeS(imgHSV, cvScalar(5,100,50,0), cvScalar(15,255,255,0), imgThresh); 
       return imgThresh;
} 



