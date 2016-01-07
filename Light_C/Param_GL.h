/**************************相关参数************************/
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

GL_Vector Plan_XNorml,Plan_YNorml;// X/Y扫描 平面 法向量
GL_Vector LineRays[10];//头显点的 射线
typedef struct _GL_Plan_
{
	float A;
	float B;
	float C;
	float D;
}GL_Plan;

GL_Plan GL_PlanX,GL_PlanY; // X/Y扫描 平面

GL_Plan GL_PlanXS[10],GL_PlanYS[10];//头显点的 所在最优 X平面、Y平面

typedef struct _GL_Point_
{
	float x;
	float y;
	float z;

}GL_Point;

GL_Point HeadPlay_Pt[10];//头显坐标
bool EnableX[10];//X扫描 使能
bool EnableY[10];//Y扫描 使能

GL_Point CV_PointS[10];//平面映射坐标

typedef struct _GL_Quater_ //四元数简易 结构体
{
	float q0;
	float q1;
	float q2;
	float q3;
}GL_Quater;

//-------------------------------------------------------
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;        // 四元数
float Ww=0;

// 偏航角——Yaw,俯仰角——Pitch,翻滚角——Rool
float Pitch;//弧度
float Rool ;
float Yaw  ;

short Pitch_angle;//角度
short Rool_angle ;
short Yaw_angle  ;

float G0=9.8;//重力初始化

//-------------------------------------------------------- 误差分析数据
float xp0_Old=0,yp0_Old=0,zp0_Old=0;// 前一帧 真实坐标
float x1_Old=0,y1_Old=0,z1_Old=0;   // 前一帧 估计坐标

GL_Point GL_SamplePt[500];//500 帧 采样缓存点
GL_Point GL_Sample_AvgPt; //采样缓存点 均值
GL_Point GL_Sample_VarianzPt;//均方差

int Erron_CntBest=0;//错误计数

#endif