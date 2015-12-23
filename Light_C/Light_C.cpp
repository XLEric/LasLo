// Light_C.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include "GL_Process.h"
#include <glut.h>
using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
#if 1
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (320*2,240*2); 
	glutInitWindowPosition (0, 0);

	CreateThread(NULL,0,ThreadProc,NULL,0,NULL);//创建一个线程，去执行ThreadProc函数

	//SuspendThread(&ThreadProc);
	SetThreadPriority(&ThreadProc,THREAD_PRIORITY_ABOVE_NORMAL);
	//ResumeThread(&ThreadProc);  
#endif

	
	CvCapture* capture1=cvCreateCameraCapture(1 );


	cvSetCaptureProperty ( capture1, CV_CAP_PROP_FRAME_WIDTH,  320 );  
	cvSetCaptureProperty ( capture1, CV_CAP_PROP_FRAME_HEIGHT, 240 );
	IplImage *image = cvQueryFrame( capture1 );
	printf("\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ GL_Start!\n");
	printf("\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ GL_Start!\n");
	printf("\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ GL_Start!\n");
	cvWaitKey(300);
	while(1)
	{
		
		image = cvQueryFrame( capture1 );
		if( !image ) break;
		//cvShowImage("Image",image);

		for(int i=0;i<5000;i++)
		{
			;
		}
		int c = cvWaitKey(300);
		if(c == 'p'){  
			c = 0;
			while(c != 'p' && c != 27){
				c = cvWaitKey(250);
			}
		}

		if(c == 27)
			break;
		if(flag_stop)break;
		glutPostRedisplay();
	}
	cvReleaseCapture( &capture1 );
	return 0;
}

