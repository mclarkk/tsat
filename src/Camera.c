/****************************************************************
 *
 * Author: Meghan Clark
 * Title: Camera.c
 * Date: Dec 10, 2012
 * Description: Camera loop for Tsat
 *
 ****************************************************************/

#include <stdio.h>
#include <math.h>
#include "../include/Config.h"
#include "../include/Utilities.h"
#include <stdio.h>
#include "cv.h"
#include "highgui.h"

#define DETECTION_THRESH 10


/******************************************************
 * Local variables
 ******************************************************/
long int total_white_pixels = 0;
long int total_pixels = 0;
int pixels_per_frame = 0;
int num_frames_captured = 0;
CvCapture* capture = 0;

/******************************************************
 * Local functions
 ******************************************************/
IplImage* GetThresholdedImage(IplImage* imgHSV);
int white_pixel_count();


int Camera_init()
{
        capture = cvCaptureFromCAM(-1);
        assert( capture != NULL );
        int width = (int) cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH );
        int height = (int) cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT);
        pixels_per_frame = width*height;
       	
        return 0;
}
  
void Camera_loop()
{
        total_white_pixels += white_pixel_count();
        num_frames_captured += 1;
}


int Camera_deinit()
{
        total_pixels = pixels_per_frame * num_frames_captured;
        fprintf(stderr, "White pixels seen: %ld\nTotal pixels seen (assuming error-free captures): %ld\n", total_white_pixels, total_pixels);
        return 0;
}

int white_pixel_count() {
     int white_pix_count = 0;
     IplImage* frame = cvQueryFrame( capture );
     if ( !frame ) {
       fprintf( stderr, "ERROR: frame is null...\n" );
       return -1;
     }
     IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
     cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
     IplImage* imgThresh = GetThresholdedImage(imgHSV);
     CvScalar sum = cvSum(imgThresh);
     white_pix_count = sum.val[0]/255;
          //printf("%f\n", avg.val[0]);
     return white_pix_count;
}

IplImage* GetThresholdedImage(IplImage* imgHSV){
       IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
       cvInRangeS(imgHSV, cvScalar(5,100,50,0), cvScalar(15,255,255,0), imgThresh);
       return imgThresh;
}

