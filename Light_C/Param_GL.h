/**************************��ز���************************/
#pragma once
#ifndef __PARAM_GL__
#define __PARAM_GL__
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <math.h>

typedef struct _GL_Vector_
{
	float x;
	float y;
	float z;
}GL_Vector;

GL_Vector Plan_XNorml,Plan_YNorml;// X/Yɨ�� ƽ�� ������
GL_Vector LineRays[10];//ͷ�Ե�� ����
typedef struct _GL_Plan_
{
	float A;
	float B;
	float C;
	float D;
}GL_Plan;

GL_Plan GL_PlanX,GL_PlanY; // X/Yɨ�� ƽ��

GL_Plan GL_PlanXS[10],GL_PlanYS[10];//ͷ�Ե�� �������� Xƽ�桢Yƽ��

typedef struct _GL_Point_
{
	float x;
	float y;
	float z;

}GL_Point;

GL_Point HeadPlay_Pt[10];//ͷ������
bool EnableX[10];//Xɨ�� ʹ��
bool EnableY[10];//Yɨ�� ʹ��

GL_Point CV_PointS[10];//ƽ��ӳ������

typedef struct _GL_Quater_ //��Ԫ������ �ṹ��
{
	float q0;
	float q1;
	float q2;
	float q3;
}GL_Quater;

//-------------------------------------------------------
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;        // ��Ԫ��
float Ww=0;

// ƫ���ǡ���Yaw,�����ǡ���Pitch,�����ǡ���Rool
float Pitch;//����
float Rool ;
float Yaw  ;

short Pitch_angle;//�Ƕ�
short Rool_angle ;
short Yaw_angle  ;

float G0=9.8;//������ʼ��

//-------------------------------------------------------- ����������
float xp0_Old=0,yp0_Old=0,zp0_Old=0;// ǰһ֡ ��ʵ����
float x1_Old=0,y1_Old=0,z1_Old=0;   // ǰһ֡ ��������

GL_Point GL_SamplePt[500];//500 ֡ ���������
GL_Point GL_Sample_AvgPt; //��������� ��ֵ
GL_Point GL_Sample_VarianzPt;//������

int Erron_CntBest=0;//�������

#endif