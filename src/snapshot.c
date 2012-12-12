#include "cv.h"
#include "highgui.h"
#include <stdio.h>
 
IplImage* GetThresholdedImage(IplImage* imgHSV);

int main(int argc,char** argv)
{
   int do_thresh = 1;
   char path[255];
 
   // For Linux, path to save the captured image
   strcpy(path,"/home/root/screencap.jpg");
 
   // For Windows, path to save the captured image
   // strcpy(path,"c:\\home.jpg");
 
   // Pointer to current frame
   IplImage *frame;
 
  // Create capture device ready
  // here 0 indicates that we want to use camera at 0th index
   CvCapture *capture = cvCaptureFromCAM(-1);
   cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 320);
   cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 240);
 
   frame = cvQueryFrame(capture);
 
while(1)
{
   // Query for Frame From Camera
   frame = cvQueryFrame(capture);
   IplImage* imgThresh;
   if (do_thresh == 1) {
   IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
     cvCvtColor(frame, imgHSV, CV_BGR2HSV);
     imgThresh = GetThresholdedImage(imgHSV);
   } 
  char ch =  cvWaitKey(1000);  // Wait for 25 ms for user to hit any key
  if(ch==27) break;  // If Escape Key was hit just exit the loop
 
  // Save image if s was keyboard
//  if(ch=='s')
//  {
     if (do_thresh == 1)
          cvSaveImage(path,imgThresh,0);
     else 
          cvSaveImage(path,frame,0);
     fprintf(stderr, "Saved screenshot!");
 // }
  }
 
   // Release All Images and Windows
   cvReleaseImage(&frame);
   cvDestroyWindow("capture");
    return 0;
}

IplImage* GetThresholdedImage(IplImage* imgHSV){
       IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
       cvInRangeS(imgHSV, cvScalar(5,100,50,0), cvScalar(15,255,255,0), imgThresh);
       return imgThresh;
}
