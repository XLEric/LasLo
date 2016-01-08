#pragma once
#ifndef __GL_PROCESS__
#define __GL_PROCESS__


#include <cv.h>
#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <stdio.h>
#include <GL/glut.h>
#include <vector>
#include <glut.h>
#include <GLAUX.h>

#include <iostream>
#include <cxcore.h>
#include <windows.h>
#include <wchar.h>
#include "GLB_Math.h"
#include "Param_GL.h"

using namespace cv;
using namespace std;

#define ABS(x)((x)>0?(x):-(x))

const int ESC = 27;
//
void reshape (int w, int h);

CvFont font;  
char buf_T[256];

int angle_GL=0;
bool flag_stop=0;

GLfloat eyex=0,eyey=130,eyez=320;
float Step_X1=0,Step_X2=0;
bool flag_StepX=0;
float Angle_Y=0,Angle_X=0;

bool Flag_X=0,Flag_Y=1;

float pos_x=0,pos_y=65,pos_z=150;
int gxr_Global=0;
int gyr_Global=0;
int gzr_Global=0;

bool GL_Flag_Stop=0;
bool Best_Flag=0;//一帧显示一次最优拟合标志
float t1_Global=60,t2_Global=60,t3_Global=60,t4_Global=60,t5_Global=60;
IplImage *frame_GL=cvCreateImage(cvSize(800,400),IPL_DEPTH_8U, 3);

int Step_Recursion=0;
int Cnt_Correct=0;
int Cnt_Err=0;
int Cnt_Correct3=0;
int Cnt_Err3=0;
int Cnt_Correct12=0;
int Cnt_Err12=0;

 float DistsX_min[10];
 float DistsY_min[10];

 bool New_FpsX[10],New_FpsY[10];
 int Fps_World=0;
//------------------------------------------------------------------ OpenGL
/**********************************************************/
void LastData_Init()
{
	FILE *pFile;
	pFile = fopen(".//LastData.txt","r+");

	while(!feof(pFile))
	{
	 
	 fscanf(pFile,"(%f,%f,%f),(%f,%f,%f,%f),(%f,%f,%f),(%d,%d),(%d,%d),(%d,%d)\n",
		 &pos_x,&pos_y,&pos_z,&q0,&q1,&q2,&q3,&t1_Global,&t2_Global,&t3_Global,
		 &Cnt_Correct,&Cnt_Err,&Cnt_Correct3,&Cnt_Err3,&Cnt_Correct12,&Cnt_Err12
		 );

	}
	fclose(pFile);

}
/**********************************************************/
void init(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
}
/**********************************************************/
void glutKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ESC:flag_stop=1;exit(0);break;
	case'q':eyex+=6;break;
	case'Q':eyex-=6;break;
	case'w':eyey+=6;break;
	case'W':eyey-=6;break;
	case'e':eyez+=6;break;
	case'E':eyez-=6;break;

	case'x':pos_x-=1;break;
	case'X':pos_x+=1;break;
	case'y':pos_y-=1;break;
	case'Y':pos_y+=1;break;
	case'z':pos_z-=1;break;
	case'Z':pos_z+=1;break;

	case'a':gxr_Global=3;break;
	case'A':gxr_Global=-3;break;
	case's':gyr_Global=3;break;
	case'S':gyr_Global=-3;break;
	case'd':gzr_Global=3;break;
	case'D':gzr_Global=-3;break;

	case'p':
	case'P':GL_Flag_Stop=1;break;
	case'o':
	case'O':GL_Flag_Stop=0;break;

	case'r':
	case'R':
		eyex=0;eyey=130;eyez=320;
		pos_x=0;pos_y=28;pos_z=150;
		q0=1;q1=0;q2=0;q3=0;Ww=0;
		break;
	}

	//printf("Move:(%f,%f,%f)\n",pos_x,pos_y,pos_z);
}
/**********************************************************/
void glutMouse(int button, int state, int x, int y)
{
	/*if(state == GLUT_DOWN)
	{
		mOldX = x;
		mOldY = y;
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
			{
				mButton = BUTTON_LEFT_TRANSLATE;
				break;
			} else
			{
				mButton = BUTTON_LEFT;
				break;
			}
		case GLUT_RIGHT_BUTTON:
			mButton = BUTTON_RIGHT;
			break;
		}
	} else if (state == GLUT_UP)
		mButton = -1;*/
}
/**********************************************************/
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(55.0, (GLfloat) w/(GLfloat) h, 1.0, 1550.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (eyex,eyey,eyez, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glRotatef(5,0,1,0);
}
/**********************************************************/
void renderCube(float size)
{
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f( 0.0f, 0.0f, 1.0f);
	glVertex3f( 0.0f,  0.0f,  0.0f);
	glVertex3f( size,  0.0f,  0.0f);
	glVertex3f( size,  size,  0.0f);
	glVertex3f( 0.0f,  size,  0.0f);
	// Back Face
	glNormal3f( 0.0f, 0.0f,-1.0f);
	glVertex3f( 0.0f,  0.0f, size);
	glVertex3f( 0.0f,  size, size);
	glVertex3f( size,  size, size);
	glVertex3f( size,  0.0f, size);		
	// Top Face
	glNormal3f( 0.0f, 1.0f, 0.0f);
	glVertex3f( 0.0f,  size,  0.0f);
	glVertex3f( size,  size,  0.0f);
	glVertex3f( size,  size, size);
	glVertex3f( 0.0f,  size, size);
	// Bottom Face
	glNormal3f( 0.0f,-1.0f, 0.0f);
	glVertex3f( 0.0f,  0.0f,  0.0f);
	glVertex3f( 0.0f,  0.0f, size);
	glVertex3f( size,  0.0f, size);
	glVertex3f( size,  0.0f,  0.0f);
	// Right face
	glNormal3f( 1.0f, 0.0f, 0.0f);
	glVertex3f( size,  0.0f, 0.0f);
	glVertex3f( size,  0.0f, size);
	glVertex3f( size,  size, size);
	glVertex3f( size,  size, 0.0f);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f( 0.0f,  0.0f, 0.0f);
	glVertex3f( 0.0f,  size, 0.0f);
	glVertex3f( 0.0f,  size, size);
	glVertex3f( 0.0f,  0.0f, size);
	glEnd();
}
/**********************************************************/
//场地边长： 45*4 单位
void GL_Draw_Filed()
{
	glPushMatrix();
	glLineWidth(1); 
	glTranslatef(0,0,0);

	glRotatef(angle_GL,0,1,0);

	glColor3f(1.5f, 0.2f, 0.2f); 

	glutWireSphere(5, 15, 15);
	glPopMatrix();
	GLUquadricObj *cylinder_obj;
	cylinder_obj=gluNewQuadric();

	float Edge=45*2;
	float Edge2=2*Edge;
	glPushMatrix();
	glTranslatef(Edge,0,0);
	glRotatef(-90,1,0,0);
	glColor3f(0.5f, 0.2f, 1.0f); 
	gluCylinder(cylinder_obj, 3, 3, 61, 20, 30);   
	glPopMatrix();

	glPushMatrix();//---- 1
	glLineWidth(1); 
	glTranslatef(Edge,61,0);

	glRotatef(angle_GL,0,1,0);

	glColor3f(0.0f, 1.0f, 0.0f); 

	glutWireSphere(6, 15, 15);
	glPopMatrix();

	//-------------------------------------
	glPushMatrix();
	glTranslatef(-Edge,0,0);
	glRotatef(-90,1,0,0);
	glColor3f(0.5f, 0.2f, 1.0f); 
	gluCylinder(cylinder_obj, 3, 3, 61, 20, 30);   
	glPopMatrix();

	glPushMatrix();//---- 2
	glLineWidth(1); 
	glTranslatef(-Edge,61,0);

	glRotatef(angle_GL,0,1,0);

	glColor3f(1.0f, 0.0f, 0.0f); 

	glutWireSphere(6, 15, 15);
	glPopMatrix();
	//-------------------------------
	glPushMatrix();
	glTranslatef(Edge,0,Edge2);
	glRotatef(-90,1,0,0);
	glColor3f(0.5f, 0.2f, 1.0f); 
	gluCylinder(cylinder_obj, 3, 3, 61, 20, 30);   
	glPopMatrix();

	glPushMatrix();
	glLineWidth(1); 
	glTranslatef(Edge,61,Edge2);//---- 3

	glRotatef(angle_GL,0,1,0);

	glColor3f(0.0f, 0.0f, 1.0f); 

	glutWireSphere(6, 15, 15);
	glPopMatrix();

	//-------------------------------------
	glPushMatrix();
	glTranslatef(-Edge,0,Edge2);
	glRotatef(-90,1,0,0);
	glColor3f(0.5f, 0.2f, 1.0f); 
	gluCylinder(cylinder_obj, 3, 3, 61, 20, 30);   
	glPopMatrix();

	glPushMatrix();//---- 4
	glLineWidth(1); 
	glTranslatef(-Edge,61,Edge2);

	glRotatef(angle_GL,0,1,0);

	glColor3f(1.0f, 1.0f, 0.0f); 

	glutWireSphere(6, 15, 15);
	glPopMatrix();


	//-------------------------------- Filed Lines

	glLineWidth(5); 
	glBegin(GL_LINES);//绘制居中轨迹
	glColor3f(0.85f, 0.6f, 0.0f); 
	glVertex3f(Edge,0,0);
	glVertex3f(-Edge,0,0);
	glVertex3f(-Edge,0,0);
	glVertex3f(-Edge,0,Edge2);
	glVertex3f(-Edge,0,Edge2);
	glVertex3f(Edge,0,Edge2);
	glVertex3f(Edge,0,Edge2);
	glVertex3f(Edge,0,0);
	glEnd();

	//------ Light A
	glPushMatrix();
	glLineWidth(1); 
	glTranslatef(-Edge,81,0);
	glRotatef(angle_GL,0,1,0);
	glColor3f(0.0f, 1.0f, 1.0f); 
	glutSolidSphere(6, 15, 15);
	glPopMatrix();

	//------ Light B
	glPushMatrix();
	glLineWidth(1); 
	glTranslatef(Edge,81,Edge2);
	glRotatef(angle_GL,0,1,0);
	glColor3f(1.0f, 0.0f, 1.0f); 
	glutSolidSphere(6, 15, 15);
	glPopMatrix();
}
/******************************XY面扫描****************************/
//扫描步长 ：Step_Scan
void GL_Scan(float Step_Scan)
{
	float Edge=45*2;
	float a1=2*Edge;
	float b1=0;
	float c1=2*Edge;

	float x1,y1,z1;
	//------ X Scan
	//(x-x0)/a1=(z-z0)/c1=t
#if 0
	if(Flag_X==0 )
	{
		if(flag_StepX==0 && !GL_Flag_Stop)
		{
			Step_X1+=0.5f/90.0f;
		}

		if(Step_X1>1.0f)flag_StepX=1;
		x1=Step_X1*a1-Edge;
		y1=0;
		z1=Step_X1*c1-0;

		if(flag_StepX==1 && !GL_Flag_Stop)
		{
			Step_X2+=0.5f/81.0f*81;
			y1=Step_X2;

			if(Step_X2>81)
			{
				flag_StepX=0;
				Step_X1=0;
				Step_X2=0;
				Flag_X=1;
				Flag_Y=0;
			}
		}

		glPushMatrix();
		glTranslatef(x1,y1,z1);
		glRotatef(angle_GL,0,1,0);
		glColor3f(1.0f, 0.0f, 0.0f); 
		glutSolidSphere(3, 15, 15);
		glPopMatrix();

		glLineWidth(2); 
		glBegin(GL_LINES);
		glColor3f(0.15f, 0.9f, 0.0f); 
		glVertex3f(-Edge,81,0);
		glVertex3f(x1,y1,z1);

		glEnd();

		//------

		float Edge2=Edge;
		float xl=x1+Edge2;
		float yl=y1;
		float zl=z1-Edge2;

		glPushMatrix();
		glTranslatef(xl,yl,zl);
		glRotatef(angle_GL,0,1,0);
		glColor3f(0.0f, 1.0f, 0.0f); 
		glutSolidSphere(3, 15, 15);
		glPopMatrix();
		//------
		float xr=x1-Edge2;
		float yr=y1;
		float zr=z1+Edge2;

		glPushMatrix();
		glTranslatef(xr,yr,zr);
		glRotatef(angle_GL,0,1,0);
		glColor3f(0.0f, 1.0f, 0.0f); 
		glutSolidSphere(3, 15, 15);
		glPopMatrix();

		glLineWidth(2); 
		glBegin(GL_LINES);
		glColor3f(0.05f, 0.6f, 0.9f); 
		glVertex3f(-Edge,81,0);
		glVertex3f(xl,yl,zl);
		glVertex3f(-Edge,81,0);
		glVertex3f(xr,yr,zr);
		glVertex3f(xl,yl,zl);
		glVertex3f(xr,yr,zr);
		glEnd();

		//------
		//平面的向量
		float ax=-Edge-x1;
		float ay=81-y1;
		float az=0-z1;

		float bx=xr-xl;
		float by=yr-yl;
		float bz=zr-zl;
		//获得平面叉积 法向量
		Plan_XNorml=GLB_CHAJI(ax, ay, az, bx, by, bz);
		GL_PlanX.A=Plan_XNorml.x;
		GL_PlanX.B=Plan_XNorml.y;
		GL_PlanX.C=Plan_XNorml.z;

		//AX+BY+CZ+D=0 => D=-AX-BY-CZ
		GL_PlanX.D=-(GL_PlanX.A*xr+GL_PlanX.B*yr+GL_PlanX.C*zr);

		glLineWidth(3); //绘制垂线
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.2f, 0.9f); 
		glVertex3f(x1,y1,z1);
		glVertex3f(x1+GL_PlanX.A*20,y1+GL_PlanX.B*20,z1+GL_PlanX.C*20);
		glEnd();
	}
#else
if(Flag_X==0 )
{
	if(!GL_Flag_Stop)
	Angle_X+=Step_Scan;

	float Rr=90*2;
	x1=-Edge;
	y1=(1-cos(float(Angle_X)*CV_PI/180))*81;
	z1=sin(float(Angle_X)*CV_PI/180)*Rr;

	if(Angle_X>90.0f)
	{
		Flag_X=1;
		Flag_Y=0;
	}
	glPushMatrix();
	glTranslatef(x1,y1,z1);
	glRotatef(angle_GL,0,1,0);
	glColor3f(1.0f, 0.0f, 0.0f); 
	glutSolidSphere(3, 15, 15);
	glPopMatrix();

	glLineWidth(2); 
	glBegin(GL_LINES);
	glColor3f(0.15f, 0.9f, 0.0f); 
	glVertex3f(-Edge,81,0);
	glVertex3f(x1,y1,z1);

	glEnd();

	//------

	float Edge2=Edge;
	float xl=x1+Edge2*2;
	float yl=y1;
	float zl=z1;

	glPushMatrix();
	glTranslatef(xl,yl,zl);
	glRotatef(angle_GL,0,1,0);
	glColor3f(1.0f, 1.0f, 0.0f); 
	glutSolidSphere(3, 15, 15);
	glPopMatrix();
	////------
	float xr=Edge2;
	float yr=81;
	float zr=0;



	glPushMatrix();
	glTranslatef(xr,yr,zr);
	glRotatef(angle_GL,0,1,0);
	glColor3f(1.0f, 1.0f, 0.0f); 
	glutSolidSphere(3, 15, 15);
	glPopMatrix();

	glLineWidth(2); 
	glBegin(GL_LINES);
	glColor3f(0.05f, 0.6f, 0.9f); 
	glVertex3f(-Edge,81,0);
	glVertex3f(x1,y1,z1);

	glVertex3f(x1,y1,z1);
	glVertex3f(xl,yl,zl);

	glVertex3f(xl,yl,zl);
	glVertex3f(xr,yr,zr);

	glVertex3f(xr,yr,zr);
	glVertex3f(-Edge,81,0);
	glEnd();

	//------
	//平面的向量
	float ax=-Edge*2;
	float ay=0;
	float az=0;
//
	float bx=x1+Edge;
	float by=y1-81;
	float bz=z1-0;
	//获得平面叉积 法向量
	Plan_XNorml=GLB_CHAJI(ax, ay, az, bx, by, bz);
	GL_PlanX.A=Plan_XNorml.x;
	GL_PlanX.B=Plan_XNorml.y;
	GL_PlanX.C=Plan_XNorml.z;

	//AX+BY+CZ+D=0 => D=-AX-BY-CZ
	GL_PlanX.D=-(GL_PlanX.A*xr+GL_PlanX.B*yr+GL_PlanX.C*zr);

	glLineWidth(3); //绘制垂线
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.2f, 0.9f); 
	glVertex3f(0,y1,z1);
	glVertex3f(GL_PlanX.A*20,y1+GL_PlanX.B*20,z1+GL_PlanX.C*20);
	glEnd();
}
#endif
	//------ Y Scan
	if(Flag_Y==0  )
	{
		float Rr=Edge*2*1.414;
		float x2=cos(float(90.0f-Angle_Y)*CV_PI/180)*Rr-Edge;
		float y2=0;
		float z2=sin(float(90.0f-Angle_Y)*CV_PI/180)*Rr;
		if(!GL_Flag_Stop)
		Angle_Y+=Step_Scan;
		if(Angle_Y>90)
		{
			Angle_Y=0;
			Angle_X=0;
			Flag_X=0;
			Flag_Y=1;

			int num=10;
			//初始化 目标点的X扫描面 与 Y扫描面 标志
			for(int i=0;i<num;i++)
			{
				EnableX[i]=0;
				EnableY[i]=0;
				DistsY_min[i]=60000;
				DistsX_min[i]=60000;

			}
			Best_Flag=0;
			Fps_World++;
			//------
			int xr=rand()%21-10;
			int yr=rand()%13-6;
			int zr=rand()%21-10;

			if(abs(pos_x+xr)<80)
			{
				pos_x=pos_x+xr;
			}

			if(abs(pos_y+yr)>8 && abs(pos_y+yr)<81-8 )
			{
				pos_y=pos_y+yr;
			}

			if((pos_z+zr)<170 && (pos_z+zr)>10 )
			{
				pos_z=pos_z+zr;
			}	

			//-------
			//------
			int gxr=rand()%20-10;
			int gyr=rand()%20-10;
			int gzr=rand()%10-5;

			gxr_Global=gxr;
			gyr_Global=gyr;
			gzr_Global=gzr;

				
		}

		glPushMatrix();
		glTranslatef(x2,y2,z2);
		glRotatef(angle_GL,0,1,0);
		glColor3f(0.0f, 1.0f, 0.0f); 
		glutSolidSphere(3, 15, 15);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(x2,y2+81,z2);
		glRotatef(angle_GL,0,1,0);
		glColor3f(0.0f, 1.0f, 0.0f); 
		glutSolidSphere(3, 15, 15);
		glPopMatrix();

		glBegin(GL_LINES);//绘制居中轨迹
		glColor3f(0.01f, 1.0f, 1.0f); 
		glVertex3f(-Edge,0,0);
		glVertex3f(x2,y2,z2);
		glVertex3f(-Edge,81,0);
		glVertex3f(x2,y2+81,z2);

		glEnd();

		glBegin(GL_LINES);
		glColor3f(1.1f, 0.0f, 0.0f); 

		glVertex3f(x2,y2,z2);
		glVertex3f(x2,y2+81,z2);

		glEnd();

		//平面的向量
		float ax=-Edge-x2;
		float ay=81-y2;
		float az=0-z2;

		float bx=0;
		float by=81;
		float bz=0;
		Plan_YNorml=GLB_CHAJI(ax, ay, az, bx, by, bz);

		GL_PlanY.A=Plan_YNorml.x;
		GL_PlanY.B=Plan_YNorml.y;
		GL_PlanY.C=Plan_YNorml.z;

		//AX+BY+CZ+D=0 => D=-AX-BY-CZ
		GL_PlanY.D=-(GL_PlanY.A*x2+GL_PlanY.B*y2+GL_PlanY.C*z2);

		glLineWidth(3); //绘制垂线
		glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 1.0f); 
		glVertex3f(x2,y2+26,z2);
		glVertex3f(x2+GL_PlanY.A*20,y2+26+GL_PlanY.B*20,z2+GL_PlanY.C*20);
		glEnd();
	}

}
/**********************************************************/
GL_Point GL_Quater_Point(GL_Quater Qt0,float x_offset,float y_offset,float z_offset,float r,float g,float b)
{
	GL_Point Pt;
	
	GL_Quater Qt0_N=Conjugate_Q(Qt0); //------ 根节点四元数 求逆

	GL_Quater Q_Point;
	Q_Point.q0=0;
	Q_Point.q1=x_offset;
	Q_Point.q2=y_offset;
	Q_Point.q3=z_offset;//根节点骨架长度
	//四元数点局部坐标系更新计算 Loc_New_Point=Q*Loc_Old_Point*Q_N
	//------计算1次相乘
	GL_Quater Q011=MUL_Q(Qt0,Q_Point);
	//------计算2次相乘
	GL_Quater Q022=MUL_Q(Q011,Qt0_N);

	glPushMatrix();//储存当前视图矩阵
	glLineWidth(1); 
	glColor3f(r,g,b); 
	glTranslatef(pos_x+Q022.q1,pos_y+Q022.q2,pos_z+Q022.q3);
	glutSolidSphere(0.6, 20, 20);
	glPopMatrix();//弹出上次保存的位置

	Pt.x=pos_x+Q022.q1;
	Pt.y=pos_y+Q022.q2;
	Pt.z=pos_z+Q022.q3;

	return Pt;
}
/**********************************************************/
// Flag_XY :XY平面，0点未在该平面，1点在该平面上。
bool GL_Plan_Dis(GL_Point Pt,GL_Plan &GL_PlanS,GL_Plan Plan,bool &Flag_XY,float &dis_minxy)
{
	bool flag=0;
	float A=Plan.A;
	float B=Plan.B;
	float C=Plan.C;
	float D=Plan.D;

	float x=Pt.x;
	float y=Pt.y;
	float z=Pt.z;
	if((A*A+B*B+C*C)!=0)
	{
		float dist=ABS(A*x+B*y+C*z+D)/sqrt(A*A+B*B+C*C);
		if(dist<4.0f)
		{
			glPushMatrix();//储存当前视图矩阵
			glLineWidth(1); 
			glColor3f(0.8,0.8,0.8); 
			glTranslatef(x,y,z);
			glutSolidSphere(0.8, 20, 20);
			glPopMatrix();//弹出上次保存的位置

			//------如果满足 点在平面要求则认为 点在平面上
			if(dis_minxy>dist)
			{
				GL_PlanS.A=Plan.A;
				GL_PlanS.B=Plan.B;
				GL_PlanS.C=Plan.C;
				GL_PlanS.D=Plan.D;
				dis_minxy=dist;
				flag=1;
				Flag_XY=1;
			}
			
			
		}
	}
	

	return flag;
}
/**********************************************************/
void GL_Head_Display()
{
	//绘制 旋转长方体
	glPushMatrix();
	glTranslatef(pos_x,pos_y,pos_z);         
	glRotatef(Ww,q1,q2,q3);
	glColor3f(0.8f, 0.2f, 0.0f );
	glScaled(10,5,2);
	glutSolidCube(1);
	glPopMatrix(); 

	glPushMatrix();
	glTranslatef(pos_x,pos_y,pos_z);         
	glRotatef(Ww,q1,q2,q3);
	glColor3f(0.0f, 1.0f, 0.0f );
	glScaled(6,2.5,2);
	glutSolidCube(1);
	glPopMatrix();

	//------
	
}
/**********************************************************/
float GL_Distance(float x1,float y1,float z1,float x2,float y2,float z2)
{
	float dis=0;
	dis=(x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2);
	//dis=sqrt(dis);
	return dis;
}
/**********************************************************/
void GL_GotHead_DisPlay_Pt()
{
	GL_Quater Qt0;
	Qt0.q0=q0;
	Qt0.q1=q1;
	Qt0.q2=q2;
	Qt0.q3=q3;
	
	
	HeadPlay_Pt[0]=GL_Quater_Point(Qt0,0,0,0,1.0,0.1,1.0);

	HeadPlay_Pt[1]=GL_Quater_Point(Qt0, 5,0,0, 1,0,0);
	HeadPlay_Pt[2]=GL_Quater_Point(Qt0, -5,0,0, 1,0,0);
	HeadPlay_Pt[3]=GL_Quater_Point(Qt0, 0,-4,0, 0,0,1);
	HeadPlay_Pt[4]=GL_Quater_Point(Qt0, 0,4,0, 0,0,1);

	//
	HeadPlay_Pt[5]=GL_Quater_Point(Qt0, 4,4,0, 0,0.8,0);
	HeadPlay_Pt[6]=GL_Quater_Point(Qt0, -4,4,0, 0,0.8,0);
	HeadPlay_Pt[7]=GL_Quater_Point(Qt0, 4,-4,0, 0.9,0.8,0);
	HeadPlay_Pt[8]=GL_Quater_Point(Qt0, -4,-4,0, 0.9,0.8,0);

	//(0,0,z)点
	HeadPlay_Pt[9]=GL_Quater_Point(Qt0, 0,0,-4, 0,0,0);

	int num=10;
	/*for(int i=0;i<num;i++)
	{
		New_FpsX[i]=0;
		New_FpsY[i]=0;
	}*/
	for(int i=0;i<num;i++)
	{
		bool flagx=GL_Plan_Dis(HeadPlay_Pt[i],GL_PlanXS[i],GL_PlanX,EnableX[i],DistsX_min[i]);
		bool flagy=GL_Plan_Dis(HeadPlay_Pt[i],GL_PlanYS[i],GL_PlanY,EnableY[i],DistsY_min[i]);
		/*if(flagx)
		{
		New_FpsX[i]=1;
		}
		if(flagy)
		{
		New_FpsY[i]=1;
		}*/
	}


	/*FILE *file=fopen(".//log.txt","a+");
	for(int i=0;i<num;i++)
	{
		if(EnableX[i] && EnableY[i])
		{
			fprintf(file,"XY:%d %d:(%f,%f,%f) (%f,%f,%f)\n",EnableX[i],EnableY[i],
				GL_PlanXS[i].A,GL_PlanXS[i].B,GL_PlanXS[i].C,
				GL_PlanYS[i].A,GL_PlanYS[i].B,GL_PlanYS[i].C);
		}
	}
	fclose(file);*/
}
/**********************************************************/
// V ：为扫描射线 向量
// (m1,m2,m3):为扫描球原点 坐标
//  CV_PointN:为二维映射点
void CV_Cross(IplImage *frame,GL_Vector V,float m1,float m2,float m3,GL_Point &CV_PointN,int num)
{
	cvCircle(frame,cvPoint(20,20),8,CV_RGB(0,0,255),3);

	float vp1=0;
	float vp2=0;
	float vp3=1;

	float v1=V.x;
	float v2=V.y;
	float v3=V.z;

	float n1=0;
	float n2=0;
	float n3=180;

	float u1=(n1-m1)*vp1+(n2-m2)*vp2+(n3-m3)*vp3;
	float u2=vp1*v1+vp2*v2+vp3*v3;
	float t=0;

	if(u2!=0)
	{
		t=u1/u2;//（0,0,180）Z=180 平面的 射线与 （Z=180）的相交点 比例尺

		float x=(m1+v1*t);
		float y=(m2+v2*t);
		float z=(m3+v3*t);

		CV_PointN.x=x;
		CV_PointN.y=y;
		CV_PointN.z=z;
		
		if(num==9)
		{
			cvCircle(frame,cvPoint(int(500+5*x),int(300+5*y)),5,CV_RGB(0,255,255),-1);
		}
		else
		{
			cvCircle(frame,cvPoint(int(500+5*x),int(300+5*y)),5,CV_RGB(0,0,255),-1);
		}
		
	}
	else
	{
		CV_PointN.x=65535;
		CV_PointN.y=65535;
	}
}
/**********************************************************/
void GL_Model(IplImage *frame,GL_Point CV_Pt0,GL_Point CV_Pt1,float x_min,float y_min,int num0,int num1 )
{
	CvPoint pt1,pt0;

	//-------------------
	pt0.x=int(200+5.0f*(CV_Pt0.x-x_min));
	pt0.y=int(300+5.0f*(CV_Pt0.y-y_min));
	pt1.x=int(200+5.0f*(CV_Pt1.x-x_min));
	pt1.y=int(300+5.0f*(CV_Pt1.y-y_min));

	if(num0!=0)
	cvLine(frame_GL,pt0,pt1,CV_RGB(0,255,0),2);
	else if(num0==0)
	{
		if(!(num1==5 || num1==6 || num1==7 || num1==8))
		{
			cvLine(frame_GL,pt0,pt1,CV_RGB(25,55,255),2);
		}
		else
		{
			cvLine(frame_GL,pt0,pt1,CV_RGB(205,85,0),2);
		}
	}
	


	cvInitFont( &font, CV_FONT_VECTOR0,0.65, 0.65, 0, 2, 2);
	sprintf_s(buf_T,256,"%d",num0);
	cvPutText(frame_GL,buf_T,pt0,&font,CV_RGB(255,0,255));
	sprintf_s(buf_T,256,"%d",num1);
	cvPutText(frame_GL,buf_T,pt1,&font,CV_RGB(255,0,255));
}
/**********************************************************/
void GL_ModelLine_Connect(IplImage *frame,GL_Point *CV_PointS,float x_min,float y_min)
{
	//HeadPlay_Pt[1]=GL_Quater_Point(Qt0,5,0,0,1,0,0);
	//HeadPlay_Pt[2]=GL_Quater_Point(Qt0,-5,0,0,1,0,0);
	//HeadPlay_Pt[3]=GL_Quater_Point(Qt0,0,-4,0,0,0,1);
	//HeadPlay_Pt[4]=GL_Quater_Point(Qt0,0,4,0,0,0,1);

	////
	//HeadPlay_Pt[5]=GL_Quater_Point(Qt0,4,4,0,0,0.8,0);
	//HeadPlay_Pt[6]=GL_Quater_Point(Qt0,-4,4,0,0,0.8,0);
	//HeadPlay_Pt[7]=GL_Quater_Point(Qt0,4,-4,0,0.9,0.8,0);
	//HeadPlay_Pt[8]=GL_Quater_Point(Qt0,-4,-4,0,0.9,0.8,0);

	if(CV_PointS[0].x!=65535 && CV_PointS[5].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[5], x_min, y_min,0,5 );
	}
	if(CV_PointS[0].x!=65535 && CV_PointS[6].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[6], x_min, y_min,0,6 );
	}
	if(CV_PointS[0].x!=65535 && CV_PointS[7].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[7], x_min, y_min,0,7 );
	}
	if(CV_PointS[0].x!=65535 && CV_PointS[8].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[8], x_min, y_min,0,8 );
	}
	//-------------------------------------------------
	if(CV_PointS[0].x!=65535 && CV_PointS[3].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[3], x_min, y_min,0,3 );
	}

	if(CV_PointS[0].x!=65535 && CV_PointS[4].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[4], x_min, y_min,0,4 );
	}

	if(CV_PointS[0].x!=65535 && CV_PointS[1].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[1], x_min, y_min,0,1 );
	}

	if(CV_PointS[0].x!=65535 && CV_PointS[2].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[2], x_min, y_min,0,2 );
	}

	if(CV_PointS[4].x!=65535 && CV_PointS[5].x!=65535)
	{
		GL_Model(frame,CV_PointS[4],CV_PointS[5], x_min, y_min,4,5 );
	}

	if(CV_PointS[4].x!=65535 && CV_PointS[6].x!=65535)
	{
		GL_Model(frame,CV_PointS[4],CV_PointS[6], x_min, y_min ,4,6);
	}

	if(CV_PointS[3].x!=65535 && CV_PointS[7].x!=65535)
	{
		GL_Model(frame,CV_PointS[3],CV_PointS[7], x_min, y_min ,3,7);
	}
	if(CV_PointS[3].x!=65535 && CV_PointS[8].x!=65535)
	{
		GL_Model(frame,CV_PointS[3],CV_PointS[8], x_min, y_min ,3,8);
	}

	if(CV_PointS[1].x!=65535 && CV_PointS[5].x!=65535)
	{
		GL_Model(frame,CV_PointS[1],CV_PointS[5], x_min, y_min,1,5 );
	}

	if(CV_PointS[2].x!=65535 && CV_PointS[6].x!=65535)
	{
		GL_Model(frame,CV_PointS[2],CV_PointS[6], x_min, y_min ,2,6);
	}

	if(CV_PointS[1].x!=65535 && CV_PointS[7].x!=65535)
	{
		GL_Model(frame,CV_PointS[1],CV_PointS[7], x_min, y_min ,1,7);
	}

	if(CV_PointS[2].x!=65535 && CV_PointS[8].x!=65535)
	{
		GL_Model(frame,CV_PointS[2],CV_PointS[8], x_min, y_min ,2,8);
	}
}
/**********************************************************/
void GL_LineRays()
{
	cvSet(frame_GL,CV_RGB(255,255,255));
	frame_GL->origin=1;
	

	for(int i=0;i<10;i++)
	{
		CV_PointS[i].x=65535;
		CV_PointS[i].y=65535;
	}
	
	int num=10;
	float x_min=65535;
	float y_min=65535;
	for(int i=0;i<num;i++)
	{
		if(EnableX[i] && EnableY[i])
		{
			//获取两平面的交线向量
			float bx=GL_PlanXS[i].A;
			float by=GL_PlanXS[i].B;
			float bz=GL_PlanXS[i].C;

			float ax=GL_PlanYS[i].A;
			float ay=GL_PlanYS[i].B;
			float az=GL_PlanYS[i].C;
			//获得平面叉积 法向量
			LineRays[i]=GLB_CHAJI(ax, ay, az, bx, by, bz);

			

			float x0=-90;//直线上的点
			float y0=81;
			float z0=0;

			CV_Cross(frame_GL,LineRays[i],x0,y0,z0,CV_PointS[i],i);
			if(x_min>CV_PointS[i].x)
			{
				x_min=CV_PointS[i].x;
			}

			if(y_min>CV_PointS[i].y)
			{
				y_min=CV_PointS[i].y;
			}

			float x=HeadPlay_Pt[0].x;
			float y=HeadPlay_Pt[0].y;
			float z=HeadPlay_Pt[0].z;
			//--------------
			float Dis=sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0)+(z-z0)*(z-z0));

			//--------------
			glLineWidth(2); 
			glBegin(GL_LINES);
			glColor3f(0.05f, 0.6f, 0.9f); 
			glVertex3f(x0,y0,z0);
			glVertex3f(x0+LineRays[i].x*Dis,
				y0+LineRays[i].y*Dis,
				z0+LineRays[i].z*Dis);
			glEnd();

			//--------------
			//glPushMatrix();//储存当前视图矩阵
			//glLineWidth(1); 
			//glColor3f(0.8,0.8,0.8); 
			//glTranslatef(HeadPlay_Pt[i].x,HeadPlay_Pt[i].y,HeadPlay_Pt[i].z);
			//glutSolidSphere(0.8, 20, 20);
			//glPopMatrix();//弹出上次保存的位置
		}

	}

	//------------------

	for(int i=0;i<num;i++)
	{
		float x=CV_PointS[i].x;
		float y=CV_PointS[i].y;
		if(x!=65535 && y!=65535)
		{
			cvCircle(frame_GL,cvPoint(int(200+5*(x-x_min)),int(300+5*(y-y_min))),5,CV_RGB(255,0,55),-1);
		}
		
	}
	//------------------
	GL_ModelLine_Connect(frame_GL,CV_PointS, x_min, y_min);

	//------------------
	cvInitFont( &font, CV_FONT_VECTOR0,1.0, 1.0, 0, 2, 2);
	sprintf_s(buf_T,256,"Q(%f,%f,%f,%f)",q0,q1,q2,q3);
	cvPutText(frame_GL,buf_T,cvPoint(10,10),&font,CV_RGB(0,255,255));

	sprintf_s(buf_T,256,"RPY:(%d,%d,%d)",Rool_angle,Pitch_angle,Yaw_angle);
	cvPutText(frame_GL,buf_T,cvPoint(10,40),&font,CV_RGB(0,255,255));
	

	sprintf_s(buf_T,256,"pos x:%f",pos_x);
	cvPutText(frame_GL,buf_T,cvPoint(10,70),&font,CV_RGB(0,255,255));
	sprintf_s(buf_T,256,"pos y:%f",pos_y);
	cvPutText(frame_GL,buf_T,cvPoint(10,100),&font,CV_RGB(0,255,255));
	sprintf_s(buf_T,256,"pos z:%f",pos_z);
	cvPutText(frame_GL,buf_T,cvPoint(10,130),&font,CV_RGB(0,255,255));

	cvNamedWindow("Frame_GL2",1);
	cvShowImage("Frame_GL2",frame_GL);
}
/*****************************构建模型方程系数*****************************/
void GLB_Knn(float Knn[][3],float a1,float b1,float c1,float a2,float b2,float c2,int num)
{
	Knn[num][0]=a1*a1+b1*b1+c1*c1;
	Knn[num][1]=a2*a2+b2*b2+c2*c2;
	Knn[num][2]=-2.0f*(a1*a2+b1*b2+c1*c2);
}
/***************************** 边长距离建模 最速迭代 *****************************/
// DiguNum : 记录递归算法小循环
// DiguCnt : 记录递归算法大循环 N*65535
// Point_Check ：模式选择：算法点数
void GL_Build_Steepest_M(int &Point_Check,int &DiguCnt,int &DiguNum )
{
	Point_Check=4;//模式选择：算法点数
	
	//-------------------
	float t1=0,t2=0,t3=0,t4=0;
	float Ft1,Ft2,Ft3,Ft4;
	t1=t1_Global;t2=t2_Global;t3=t3_Global;t4=t4_Global;
	float t1b=100,t2b=100,t3b=100,t4b=100;;
	float nn=0.00072531;// 递归步长 适中（不能太大也不能太小）。
	if(Point_Check==3)
	{
		nn==0.0021;
	}
	//射线向量
	float a1=LineRays[0].x; float b1=LineRays[0].y; float c1=LineRays[0].z;
	float a2=LineRays[4].x; float b2=LineRays[4].y; float c2=LineRays[4].z;
	float a3=LineRays[5].x; float b3=LineRays[5].y; float c3=LineRays[5].z;
	float a4=LineRays[6].x; float b4=LineRays[6].y; float c4=LineRays[6].z;
	//边长平方
	float E12=16;// 1->2
	float E22=16;// 2->3
	float E32=32;// 1->3
	float E42=32;// 1->4
	float E52=16;// 2->4
	float E62=64;// 3->4
	//------构建模型系数
	float knn[10][3];
	GLB_Knn(knn,a1,b1,c1,a2,b2,c2,0);
	GLB_Knn(knn,a2,b2,c2,a3,b3,c3,1);
	GLB_Knn(knn,a1,b1,c1,a3,b3,c3,2);

	GLB_Knn(knn,a1,b1,c1,a4,b4,c4,3);
	GLB_Knn(knn,a2,b2,c2,a4,b4,c4,4);
	GLB_Knn(knn,a3,b3,c3,a4,b4,c4,5);

	// F(t1,t2,t3)=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)
	//            +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)
	//            +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)

	//            +(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)*(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)
	//            +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)*(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)
	//            +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)*(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)

	//-----------------
	// F(t1,t2,t3)/t1偏导数=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][0]*t1+knn[0][2]*t2)
	//                     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][0]*t1+knn[2][2]*t3)
	//                     +(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)*(2*knn[3][0]*t1+knn[3][2]*t4)

	// F(t1,t2,t3)/t2偏导数=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][1]*t2+knn[0][2]*t1)
	//                     +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][0]*t2+knn[1][2]*t3)
	//                     +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)*(2*knn[4][0]*t2+knn[4][2]*t4)

	// F(t1,t2,t3)/t3偏导数=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][1]*t3+knn[1][2]*t2)
	//                     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][1]*t3+knn[2][2]*t1)
	//                     +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)*(2*knn[5][0]*t3+knn[5][2]*t4)

	// F(t1,t2,t3)/t4偏导数=(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)*(2*knn[3][1]*t4+knn[3][2]*t1)
	//                     +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)*(2*knn[4][1]*t4+knn[4][2]*t2)
	//                     +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)*(2*knn[5][1]*t4+knn[5][2]*t3)

	//-------角度约束
	//GL_Vector Line1_V;// 0->1
	//Line1_V.x=a1*t1-a2*t2;
	//Line1_V.y=b1*t1-b2*t2;
	//Line1_V.z=c1*t1-c2*t2;

	//GL_Vector Line2_V;// 2->3
	//Line1_V.x=a3*t3-a2*t2;
	//Line1_V.y=b3*t3-b2*t2;
	//Line1_V.z=c3*t3-c2*t2;

	// 角度约束函数 0->1 2->3 夹角：F_Angle1=( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
	//                      *( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )


	// F_Angle/t1偏导数=2*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
	//                 *( (a1)*(a3*t3-a2*t2) + (b1)*(b3*t3-b2*t2) + (c1)*(c3*t3-c2*t2) )

	// F_Angle/t2偏导数=2*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
	//                 *( (-a2)*(a3*t3-a2*t2)+ (a1*t1-a2*t2)*(-a2) + (-b2)*(b3*t3-b2*t2)+(b1*t1-b2*t2)*(-b2) + (-c2)*(c3*t3-c2*t2) +(c1*t1-c2*t2)*(-c2) )

	// F_Angle/t3偏导数=2*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
	//                 *( (a1*t1-a2*t2)*(a3) + (b1*t1-b2*t2)*(b3) + (c1*t1-c2*t2)*(c3) )
	//---------------------------------------------------------------------------------------------------------------------
	// 角度约束函数 0->1 2->4 夹角：F_Angle2=( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
	//                      *( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )


	// F_Angle/t1偏导数=2*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
	//                 *( (a1)*(a4*t4-a2*t2) + (b1)*(b4*t4-b2*t2) + (c1)*(c4*t4-c2*t2) )

	// F_Angle/t2偏导数=2*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
	//                 *( (-a2)*(a4*t4-a2*t2)+ (a1*t1-a2*t2)*(-a2) + (-b2)*(b4*t4-b2*t2)+(b1*t1-b2*t2)*(-b2) + (-c2)*(c4*t4-c2*t2) +(c1*t1-c2*t2)*(-c2) )

	// F_Angle/t4偏导数=2*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
	//                 *( (a1*t1-a2*t2)*(a4) + (b1*t1-b2*t2)*(b4) + (c1*t1-c2*t2)*(c4) )


	//if(Step_Recursion<3)//初始前几次递归步长较大之后步长变小。
	//{
	// Step_Recursion++;
	// nn=0.002999;
	//}
	//-------------------

	//	{
	//		//fprintf(file,"(%d,%d,%d) (%f,%f,%f) (%d,%d,%d)\n",G_ax,G_ay,G_az,Rotx,Roty,Rotz,Compass_X,Compass_Y,Compass_Z);
	//		
	//	}

	
	//-------------------------------------------------------------
	 while(DiguCnt==0 ||(DiguCnt!=0 && DiguNum==65535))
		 {
			 /*if(DiguCnt>15)nn=0.000023;*/
			 if(DiguCnt>18)break;
			 DiguNum=0;
			 DiguCnt++;
			 int i=0;
		 for( i=0;i<65535;i++)
		 {
#if 0
			 if(i<25000)
			 {
				 nn=0.0008999;
			 }
			
			 else if(i>=25000 && i<52000)
			 {
				 nn=0.00068531;
			 }
			  else 
			 {
				  nn=0.00002;
			 }
#endif
			 if(Point_Check==10)//四点递归
			 {
				 Ft1=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][0]*t1+knn[0][2]*t2)
					 +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][0]*t1+knn[2][2]*t3)
					 +20*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
					 *( (a1)*(a3*t3-a2*t2) + (b1)*(b3*t3-b2*t2) + (c1)*(c3*t3-c2*t2) );

				 Ft2=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][1]*t2+knn[0][2]*t1)
					 +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][0]*t2+knn[1][2]*t3)
					 +20*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
					 *( (-a2)*(a3*t3-a2*t2)+ (a1*t1-a2*t2)*(-a2) + (-b2)*(b3*t3-b2*t2)+(b1*t1-b2*t2)*(-b2) + (-c2)*(c3*t3-c2*t2) +(c1*t1-c2*t2)*(-c2) );

				 Ft3=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][1]*t3+knn[1][2]*t2)
					 +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][1]*t3+knn[2][2]*t1)
					 +20*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
					 *( (a1*t1-a2*t2)*(a3) + (b1*t1-b2*t2)*(b3) + (c1*t1-c2*t2)*(c3) );
			 }
			 
			 if(Point_Check==4)//四点递归
			 {
			  Ft1=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][0]*t1+knn[0][2]*t2)
			                      +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][0]*t1+knn[2][2]*t3)
			                      +(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)*(2*knn[3][0]*t1+knn[3][2]*t4)
								  +10*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
								  *( (a1)*(a3*t3-a2*t2) + (b1)*(b3*t3-b2*t2) + (c1)*(c3*t3-c2*t2) )
								  /*+10*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
								   *( (a1)*(a4*t4-a2*t2) + (b1)*(b4*t4-b2*t2) + (c1)*(c4*t4-c2*t2) )*/
								  ;

			  Ft2=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][1]*t2+knn[0][2]*t1)
			                      +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][0]*t2+knn[1][2]*t3)
			                      +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)*(2*knn[4][0]*t2+knn[4][2]*t4)
								  +10*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
								  *( (-a2)*(a3*t3-a2*t2)+ (a1*t1-a2*t2)*(-a2) + (-b2)*(b3*t3-b2*t2)+(b1*t1-b2*t2)*(-b2) + (-c2)*(c3*t3-c2*t2) +(c1*t1-c2*t2)*(-c2) )
								  /*+10*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
				                  *( (-a2)*(a4*t4-a2*t2)+ (a1*t1-a2*t2)*(-a2) + (-b2)*(b4*t4-b2*t2)+(b1*t1-b2*t2)*(-b2) + (-c2)*(c4*t4-c2*t2) +(c1*t1-c2*t2)*(-c2) )*/
								  ;

			  Ft3=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][1]*t3+knn[1][2]*t2)
			                      +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][1]*t3+knn[2][2]*t1)
			                      +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)*(2*knn[5][0]*t3+knn[5][2]*t4)
								  +10*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
								  *( (a1*t1-a2*t2)*(a3) + (b1*t1-b2*t2)*(b3) + (c1*t1-c2*t2)*(c3) )
								  ;

			  Ft4=(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)*(2*knn[3][1]*t4+knn[3][2]*t1)
			                      +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)*(2*knn[4][1]*t4+knn[4][2]*t2)
			                      +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)*(2*knn[5][1]*t4+knn[5][2]*t3)
								  /*+10*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
								  *( (a1*t1-a2*t2)*(a4) + (b1*t1-b2*t2)*(b4) + (c1*t1-c2*t2)*(c4) )*/
								  ;
			 }

			 else if(Point_Check==3)//三点递归
			 {
				 Ft1=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][0]*t1+knn[0][2]*t2)
					 +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][0]*t1+knn[2][2]*t3);

				 Ft2=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][1]*t2+knn[0][2]*t1)
					 +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][0]*t2+knn[1][2]*t3);

				 Ft3=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][1]*t3+knn[1][2]*t2)
					 +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][1]*t3+knn[2][2]*t1);
			 }

			 t1b=t1-nn*Ft1;
			 t2b=t2-nn*Ft2;
			 t3b=t3-nn*Ft3;
			 t4b=t4-nn*Ft4;
			 if(Point_Check==4)
			 {
				 if(t1==t1b && t2==t2b && t3==t3b &&t4==t4b) break;
				 //if(ABS(t1-t1b)<0.000005 && ABS(t2-t2b)<0.000005 && ABS(t3-t3b)<0.000005 && ABS(t4-t4b)<0.000005) break;
			 }
			 else if(Point_Check==3)
			 {
				 if(t1==t1b && t2==t2b && t3==t3b ) break;
			 }
			 else if(Point_Check==10)
			 {
				 if(t1==t1b && t2==t2b && t3==t3b ) break;
			 }

			 t1=t1b;
			 t2=t2b;
			 t3=t3b;
			 t4=t4b;
		 }
			DiguNum=i;
		 }

		 //将递归结果返回全局变量保存
		 t1_Global=t1;
		 t2_Global=t2;
		 t3_Global=t3;
		 t4_Global=t4;

		  //---------------------------------------------------
		 //nn=0.00001581;
		 //t1-=0.001;
		 //t2-=0.001;
		 //t3-=0.001;
		 //t4-=0.001;
		 //for(int i=0;i<25535;i++)
		 //{

			// if(Point_Check==4)//四点递归
			// {
				 //Ft1=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][0]*t1+knn[0][2]*t2)
					//				  +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][0]*t1+knn[2][2]*t3)
					//				  +(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)*(2*knn[3][0]*t1+knn[3][2]*t4)
					//				  +10*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
					//				  *( (a1)*(a3*t3-a2*t2) + (b1)*(b3*t3-b2*t2) + (c1)*(c3*t3-c2*t2) )
					//				  /*+10*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
					//				   *( (a1)*(a4*t4-a2*t2) + (b1)*(b4*t4-b2*t2) + (c1)*(c4*t4-c2*t2) )*/
					//				  ;

				 // Ft2=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][1]*t2+knn[0][2]*t1)
					//				  +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][0]*t2+knn[1][2]*t3)
					//				  +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)*(2*knn[4][0]*t2+knn[4][2]*t4)
					//				  +10*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
					//				  *( (-a2)*(a3*t3-a2*t2)+ (a1*t1-a2*t2)*(-a2) + (-b2)*(b3*t3-b2*t2)+(b1*t1-b2*t2)*(-b2) + (-c2)*(c3*t3-c2*t2) +(c1*t1-c2*t2)*(-c2) )
					//				  /*+10*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
					//				  *( (-a2)*(a4*t4-a2*t2)+ (a1*t1-a2*t2)*(-a2) + (-b2)*(b4*t4-b2*t2)+(b1*t1-b2*t2)*(-b2) + (-c2)*(c4*t4-c2*t2) +(c1*t1-c2*t2)*(-c2) )*/
					//				  ;

				 // Ft3=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][1]*t3+knn[1][2]*t2)
					//				  +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][1]*t3+knn[2][2]*t1)
					//				  +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)*(2*knn[5][0]*t3+knn[5][2]*t4)
					//				  +10*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
					//				  *( (a1*t1-a2*t2)*(a3) + (b1*t1-b2*t2)*(b3) + (c1*t1-c2*t2)*(c3) )
					//				  ;

				 // Ft4=(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)*(2*knn[3][1]*t4+knn[3][2]*t1)
					//				  +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)*(2*knn[4][1]*t4+knn[4][2]*t2)
					//				  +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)*(2*knn[5][1]*t4+knn[5][2]*t3)
					//				  /*+10*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
					//				  *( (a1*t1-a2*t2)*(a4) + (b1*t1-b2*t2)*(b4) + (c1*t1-c2*t2)*(c4) )*/
					//				  ;
			// }

			// else if(Point_Check==3)//三点递归
			// {
			//	 Ft1=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][0]*t1+knn[0][2]*t2)
			//		 +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][0]*t1+knn[2][2]*t3);

			//	 Ft2=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][1]*t2+knn[0][2]*t1)
			//		 +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][0]*t2+knn[1][2]*t3);

			//	 Ft3=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][1]*t3+knn[1][2]*t2)
			//		 +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][1]*t3+knn[2][2]*t1);
			// }
			// t1b=t1-nn*Ft1;
			// t2b=t2-nn*Ft2;
			// t3b=t3-nn*Ft3;
			// t4b=t4-nn*Ft4;
			// if(Point_Check==4)
			// {
			//	 if(t1==t1b && t2==t2b && t3==t3b &&t4==t4b) break;
			// }
			// else if(Point_Check==3)
			// {
			//	 if(t1==t1b && t2==t2b && t3==t3b ) break;
			// }
			// t1=t1b;
			// t2=t2b;
			// t3=t3b;
			// t4=t4b;
		 //}
		 //---------------------------------------------------
		 //dst_min=180*180*180;
		 //for(int k1=-25;k1<+25;k1++)
		 //{
			// float kf1=t1+float(k1)*0.15f;

			// float x1=LineRays[0].x*kf1+x0;
			// float y1=LineRays[0].y*kf1+y0;
			// float z1=LineRays[0].z*kf1+z0;

			// for(int k2=-25;k2<25;k2++)
			// {
			//	 float kf2=t2+float(k2)*0.15f;

			//	 float x2=LineRays[4].x*kf2+x0;P
			//	 float y2=LineRays[4].y*kf2+y0;
			//	 float z2=LineRays[4].z*kf2+z0;

			//	 for(int k3=-25;k3<25;k3++)
			//	 {
			//		 float kf3=t3+float(k3)*0.15f;
			//		 float x3=LineRays[5].x*kf3+x0;
			//		 float y3=LineRays[5].y*kf3+y0;
			//		 float z3=LineRays[5].z*kf3+z0;


			//		 //-------
			//		 float line1_dst=GL_Distance(x1,y1,z1,x2,y2,z2);//理想值 sqrt(16)
			//		 float line2_dst=GL_Distance(x2,y2,z2,x3,y3,z3);//理想值 sqrt(16)

			//		 float line3_dst=GL_Distance(x1,y1,z1,x3,y3,z3);//理想值 32
			//		 float dst_sum=
			//			  ABS(line1_dst-16)
			//			 +ABS(line2_dst-16)
			//			 +ABS(line3_dst-32)
			//			 ;

			//		 if(dst_min>dst_sum)
			//		 {
			//			 dst_min=dst_sum;
			//			 t1=kf1;
			//			 t2=kf2;
			//			 t3=kf3;
			//		 }
			//	 }
			// }
		 //}
}

/***************************** 边长距离建模 最速迭代 *****************************/
// DiguNum : 记录递归算法小循环
// DiguCnt : 记录递归算法大循环 N*65535
// Point_Check ：模式选择：算法点数
void GL_Build_Steepest_M5Point(int &Point_Check,int &DiguCnt,int &DiguNum )
{
	Point_Check=5;//模式选择：算法点数
	//-------------------
	float t1=0,t2=0,t3=0,t4=0,t5=0;
	float Ft1,Ft2,Ft3,Ft4,Ft5;
	t1=t1_Global;t2=t2_Global;t3=t3_Global;t4=t4_Global,t5=t5_Global;
	float t1b=100,t2b=100,t3b=100,t4b=100,t5b=100;
	float nn=0.001071;// 递归步长 适中（不能太大也不能太小）。

	//射线向量
	float a1=LineRays[0].x; float b1=LineRays[0].y; float c1=LineRays[0].z;
	float a2=LineRays[4].x; float b2=LineRays[4].y; float c2=LineRays[4].z;
	float a3=LineRays[5].x; float b3=LineRays[5].y; float c3=LineRays[5].z;
	float a4=LineRays[6].x; float b4=LineRays[6].y; float c4=LineRays[6].z;

	float a5=LineRays[1].x; float b5=LineRays[1].y; float c5=LineRays[1].z;
	//边长平方
	float E12=16;// 1->2
	float E22=16;// 2->3
	float E32=32;// 1->3
	float E42=32;// 1->4
	float E52=16;// 2->4
	float E62=64;// 3->4
	
	float E72=25;// 1->5
	float E82=41;// 2->5
	float E92=17;// 3->5
	float EA2=97;// 4_>5
	//------构建模型系数
	float knn[10][3];
	GLB_Knn(knn,a1,b1,c1,a2,b2,c2,0);
	GLB_Knn(knn,a2,b2,c2,a3,b3,c3,1);
	GLB_Knn(knn,a1,b1,c1,a3,b3,c3,2);

	GLB_Knn(knn,a1,b1,c1,a4,b4,c4,3);
	GLB_Knn(knn,a2,b2,c2,a4,b4,c4,4);
	GLB_Knn(knn,a3,b3,c3,a4,b4,c4,5);

	GLB_Knn(knn,a1,b1,c1,a5,b5,c5,6);
	GLB_Knn(knn,a2,b2,c2,a5,b5,c5,7);
	GLB_Knn(knn,a3,b3,c3,a5,b5,c5,8);
	GLB_Knn(knn,a4,b4,c4,a5,b5,c5,9);


	// F(t1,t2,t3)=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)
	//            +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)
	//            +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)

	//            +(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)*(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)
	//            +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)*(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)
	//            +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)*(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)
	
	//            +(knn[6][0]*t1*t1+knn[6][1]*t5*t5+knn[6][2]*t1*t5-E72)*(knn[6][0]*t1*t1+knn[6][1]*t5*t5+knn[6][2]*t1*t5-E72)
	//            +(knn[7][0]*t2*t2+knn[7][1]*t5*t5+knn[7][2]*t2*t5-E82)*(knn[7][0]*t2*t2+knn[7][1]*t5*t5+knn[7][2]*t2*t5-E82)
	//            +(knn[8][0]*t3*t3+knn[8][1]*t5*t5+knn[8][2]*t3*t5-E92)*(knn[8][0]*t3*t3+knn[8][1]*t5*t5+knn[8][2]*t3*t5-E92)
	//            +(knn[9][0]*t4*t4+knn[9][1]*t5*t5+knn[9][2]*t4*t5-EA2)*(knn[9][0]*t4*t4+knn[9][1]*t5*t5+knn[9][2]*t4*t5-EA2)

	//-----------------
	// F(t1,t2,t3)/t1偏导数=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][0]*t1+knn[0][2]*t2)
	//                     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][0]*t1+knn[2][2]*t3)
	//                     +(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)*(2*knn[3][0]*t1+knn[3][2]*t4)
	//                     +(knn[6][0]*t1*t1+knn[6][1]*t5*t5+knn[6][2]*t1*t5-E72)*(2*knn[6][0]*t1+knn[6][2]*t5)

	// F(t1,t2,t3)/t2偏导数=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][1]*t2+knn[0][2]*t1)
	//                     +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][0]*t2+knn[1][2]*t3)
	//                     +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)*(2*knn[4][0]*t2+knn[4][2]*t4)
	//                     +(knn[7][0]*t2*t2+knn[7][1]*t5*t5+knn[7][2]*t2*t5-E82)*(2*knn[7][0]*t2+knn[7][2]*t5)

	// F(t1,t2,t3)/t3偏导数=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][1]*t3+knn[1][2]*t2)
	//                     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][1]*t3+knn[2][2]*t1)
	//                     +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)*(2*knn[5][0]*t3+knn[5][2]*t4)
	//                     +(knn[8][0]*t3*t3+knn[8][1]*t5*t5+knn[8][2]*t3*t5-E92)*(2*knn[8][0]*t3+knn[8][2]*t5)

	// F(t1,t2,t3)/t4偏导数=(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)*(2*knn[3][1]*t4+knn[3][2]*t1)
	//                     +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)*(2*knn[4][1]*t4+knn[4][2]*t2)
	//                     +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)*(2*knn[5][1]*t4+knn[5][2]*t3)
	//                     +(knn[9][0]*t4*t4+knn[9][1]*t5*t5+knn[9][2]*t4*t5-EA2)*(2*knn[9][0]*t4+knn[9][2]*t5)

	// F(t1,t2,t3)/t5偏导数=(knn[6][0]*t1*t1+knn[6][1]*t5*t5+knn[6][2]*t1*t5-E72)*(2*knn[6][1]*t5+knn[6][2]*t1)
	//                     +(knn[7][0]*t2*t2+knn[7][1]*t5*t5+knn[7][2]*t2*t5-E82)*(2*knn[7][1]*t5+knn[7][2]*t2)
	//                     +(knn[8][0]*t3*t3+knn[8][1]*t5*t5+knn[8][2]*t3*t5-E92)*(2*knn[8][1]*t5+knn[8][2]*t3)
	//                     +(knn[9][0]*t4*t4+knn[9][1]*t5*t5+knn[9][2]*t4*t5-EA2)*(2*knn[9][1]*t5+knn[9][2]*t4)

	//-------角度约束
	//GL_Vector Line1_V;// 0->1
	//Line1_V.x=a1*t1-a2*t2;
	//Line1_V.y=b1*t1-b2*t2;
	//Line1_V.z=c1*t1-c2*t2;

	//GL_Vector Line2_V;// 2->3
	//Line1_V.x=a3*t3-a2*t2;
	//Line1_V.y=b3*t3-b2*t2;
	//Line1_V.z=c3*t3-c2*t2;

	// 角度约束函数 0->1 2->3 夹角：F_Angle1=( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
	//                      *( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )


	// F_Angle/t1偏导数=2*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
	//                 *( (a1)*(a3*t3-a2*t2) + (b1)*(b3*t3-b2*t2) + (c1)*(c3*t3-c2*t2) )

	// F_Angle/t2偏导数=2*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
	//                 *( (-a2)*(a3*t3-a2*t2)+ (a1*t1-a2*t2)*(-a2) + (-b2)*(b3*t3-b2*t2)+(b1*t1-b2*t2)*(-b2) + (-c2)*(c3*t3-c2*t2) +(c1*t1-c2*t2)*(-c2) )

	// F_Angle/t3偏导数=2*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
	//                 *( (a1*t1-a2*t2)*(a3) + (b1*t1-b2*t2)*(b3) + (c1*t1-c2*t2)*(c3) )
	//---------------------------------------------------------------------------------------------------------------------
	// 角度约束函数 0->1 2->4 夹角：F_Angle2=( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
	//                      *( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )


	// F_Angle/t1偏导数=2*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
	//                 *( (a1)*(a4*t4-a2*t2) + (b1)*(b4*t4-b2*t2) + (c1)*(c4*t4-c2*t2) )

	// F_Angle/t2偏导数=2*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
	//                 *( (-a2)*(a4*t4-a2*t2)+ (a1*t1-a2*t2)*(-a2) + (-b2)*(b4*t4-b2*t2)+(b1*t1-b2*t2)*(-b2) + (-c2)*(c4*t4-c2*t2) +(c1*t1-c2*t2)*(-c2) )

	// F_Angle/t4偏导数=2*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
	//                 *( (a1*t1-a2*t2)*(a4) + (b1*t1-b2*t2)*(b4) + (c1*t1-c2*t2)*(c4) )


	//if(Step_Recursion<3)//初始前几次递归步长较大之后步长变小。
	//{
	// Step_Recursion++;
	// nn=0.002999;
	//}
	//-------------------

	//	{
	//		//fprintf(file,"(%d,%d,%d) (%f,%f,%f) (%d,%d,%d)\n",G_ax,G_ay,G_az,Rotx,Roty,Rotz,Compass_X,Compass_Y,Compass_Z);
	//		
	//	}

	
	//-------------------------------------------------------------
	 while(DiguCnt==0 ||(DiguCnt!=0 && DiguNum==65535))
		 {
			 /*if(DiguCnt>15)nn=0.000023;*/
			 if(DiguCnt>18)break;
			 DiguNum=0;
			 DiguCnt++;
			 int i=0;
		 for( i=0;i<65535;i++)
		 {
#if 0
			 if(i<25000)
			 {
				 nn=0.0008999;
			 }
			
			 else if(i>=25000 && i<52000)
			 {
				 nn=0.00068531;
			 }
			  else 
			 {
				  nn=0.00002;
			 }
#endif

			 
			 if(Point_Check==5)//四点递归
			 {
			  Ft1=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][0]*t1+knn[0][2]*t2)
			                      +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][0]*t1+knn[2][2]*t3)
			                      +(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)*(2*knn[3][0]*t1+knn[3][2]*t4)
								  +10*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
								  *( (a1)*(a3*t3-a2*t2) + (b1)*(b3*t3-b2*t2) + (c1)*(c3*t3-c2*t2) )
								  /*+10*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
								   *( (a1)*(a4*t4-a2*t2) + (b1)*(b4*t4-b2*t2) + (c1)*(c4*t4-c2*t2) )*/
								   +(knn[6][0]*t1*t1+knn[6][1]*t5*t5+knn[6][2]*t1*t5-E72)*(2*knn[6][0]*t1+knn[6][2]*t5)
								  ;

			  Ft2=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][1]*t2+knn[0][2]*t1)
			                      +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][0]*t2+knn[1][2]*t3)
			                      +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)*(2*knn[4][0]*t2+knn[4][2]*t4)
								  +10*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
								  *( (-a2)*(a3*t3-a2*t2)+ (a1*t1-a2*t2)*(-a2) + (-b2)*(b3*t3-b2*t2)+(b1*t1-b2*t2)*(-b2) + (-c2)*(c3*t3-c2*t2) +(c1*t1-c2*t2)*(-c2) )
								  /*+10*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
				                  *( (-a2)*(a4*t4-a2*t2)+ (a1*t1-a2*t2)*(-a2) + (-b2)*(b4*t4-b2*t2)+(b1*t1-b2*t2)*(-b2) + (-c2)*(c4*t4-c2*t2) +(c1*t1-c2*t2)*(-c2) )*/
								  +(knn[7][0]*t2*t2+knn[7][1]*t5*t5+knn[7][2]*t2*t5-E82)*(2*knn[7][0]*t2+knn[7][2]*t5)
								  ;

			  Ft3=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][1]*t3+knn[1][2]*t2)
			                      +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][1]*t3+knn[2][2]*t1)
			                      +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)*(2*knn[5][0]*t3+knn[5][2]*t4)
								  +10*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
								  *( (a1*t1-a2*t2)*(a3) + (b1*t1-b2*t2)*(b3) + (c1*t1-c2*t2)*(c3) )
								  +(knn[8][0]*t3*t3+knn[8][1]*t5*t5+knn[8][2]*t3*t5-E92)*(2*knn[8][0]*t3+knn[8][2]*t5)
								  ;

			  Ft4=(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)*(2*knn[3][1]*t4+knn[3][2]*t1)
			                      +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)*(2*knn[4][1]*t4+knn[4][2]*t2)
			                      +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)*(2*knn[5][1]*t4+knn[5][2]*t3)
								  /*+10*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
								  *( (a1*t1-a2*t2)*(a4) + (b1*t1-b2*t2)*(b4) + (c1*t1-c2*t2)*(c4) )*/
								  +(knn[9][0]*t4*t4+knn[9][1]*t5*t5+knn[9][2]*t4*t5-EA2)*(2*knn[9][0]*t4+knn[9][2]*t5)
								  ;

			  Ft5=(knn[6][0]*t1*t1+knn[6][1]*t5*t5+knn[6][2]*t1*t5-E72)*(2*knn[6][1]*t5+knn[6][2]*t1)
			                       +(knn[7][0]*t2*t2+knn[7][1]*t5*t5+knn[7][2]*t2*t5-E82)*(2*knn[7][1]*t5+knn[7][2]*t2)
			                       +(knn[8][0]*t3*t3+knn[8][1]*t5*t5+knn[8][2]*t3*t5-E92)*(2*knn[8][1]*t5+knn[8][2]*t3)
			                       +(knn[9][0]*t4*t4+knn[9][1]*t5*t5+knn[9][2]*t4*t5-EA2)*(2*knn[9][1]*t5+knn[9][2]*t4)
								   ;
			 }

			 t1b=t1-nn*Ft1;
			 t2b=t2-nn*Ft2;
			 t3b=t3-nn*Ft3;
			 t4b=t4-nn*Ft4;
			 t5b=t5-nn*Ft5;
			 if(Point_Check==5)
			 {
				 if(t1==t1b && t2==t2b && t3==t3b &&t4==t4b &&t5==t5b) break;
				 //if(ABS(t1-t1b)<0.000005 && ABS(t2-t2b)<0.000005 && ABS(t3-t3b)<0.000005 && ABS(t4-t4b)<0.000005 && ABS(t5-t5b)<0.000005) break;
			 }
			 

			 t1=t1b;
			 t2=t2b;
			 t3=t3b;
			 t4=t4b;
			 t5=t5b;
		 }
			DiguNum=i;
		 }

		 //将递归结果返回全局变量保存
		 t1_Global=t1;
		 t2_Global=t2;
		 t3_Global=t3;
		 t4_Global=t4;
		 t5_Global=t5;

		  //---------------------------------------------------
		 //nn=0.00001581;
		 //t1-=0.001;
		 //t2-=0.001;
		 //t3-=0.001;
		 //t4-=0.001;
		 //for(int i=0;i<25535;i++)
		 //{

			// if(Point_Check==4)//四点递归
			// {
				 //Ft1=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][0]*t1+knn[0][2]*t2)
					//				  +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][0]*t1+knn[2][2]*t3)
					//				  +(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)*(2*knn[3][0]*t1+knn[3][2]*t4)
					//				  +10*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
					//				  *( (a1)*(a3*t3-a2*t2) + (b1)*(b3*t3-b2*t2) + (c1)*(c3*t3-c2*t2) )
					//				  /*+10*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
					//				   *( (a1)*(a4*t4-a2*t2) + (b1)*(b4*t4-b2*t2) + (c1)*(c4*t4-c2*t2) )*/
					//				  ;

				 // Ft2=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][1]*t2+knn[0][2]*t1)
					//				  +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][0]*t2+knn[1][2]*t3)
					//				  +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)*(2*knn[4][0]*t2+knn[4][2]*t4)
					//				  +10*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
					//				  *( (-a2)*(a3*t3-a2*t2)+ (a1*t1-a2*t2)*(-a2) + (-b2)*(b3*t3-b2*t2)+(b1*t1-b2*t2)*(-b2) + (-c2)*(c3*t3-c2*t2) +(c1*t1-c2*t2)*(-c2) )
					//				  /*+10*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
					//				  *( (-a2)*(a4*t4-a2*t2)+ (a1*t1-a2*t2)*(-a2) + (-b2)*(b4*t4-b2*t2)+(b1*t1-b2*t2)*(-b2) + (-c2)*(c4*t4-c2*t2) +(c1*t1-c2*t2)*(-c2) )*/
					//				  ;

				 // Ft3=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][1]*t3+knn[1][2]*t2)
					//				  +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][1]*t3+knn[2][2]*t1)
					//				  +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)*(2*knn[5][0]*t3+knn[5][2]*t4)
					//				  +10*( (a1*t1-a2*t2)*(a3*t3-a2*t2) + (b1*t1-b2*t2)*(b3*t3-b2*t2) + (c1*t1-c2*t2)*(c3*t3-c2*t2) )
					//				  *( (a1*t1-a2*t2)*(a3) + (b1*t1-b2*t2)*(b3) + (c1*t1-c2*t2)*(c3) )
					//				  ;

				 // Ft4=(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4-E42)*(2*knn[3][1]*t4+knn[3][2]*t1)
					//				  +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4-E52)*(2*knn[4][1]*t4+knn[4][2]*t2)
					//				  +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4-E62)*(2*knn[5][1]*t4+knn[5][2]*t3)
					//				  /*+10*( (a1*t1-a2*t2)*(a4*t4-a2*t2) + (b1*t1-b2*t2)*(b4*t4-b2*t2) + (c1*t1-c2*t2)*(c4*t4-c2*t2) )
					//				  *( (a1*t1-a2*t2)*(a4) + (b1*t1-b2*t2)*(b4) + (c1*t1-c2*t2)*(c4) )*/
					//				  ;
			// }

			// else if(Point_Check==3)//三点递归
			// {
			//	 Ft1=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][0]*t1+knn[0][2]*t2)
			//		 +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][0]*t1+knn[2][2]*t3);

			//	 Ft2=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2-E12)*(2*knn[0][1]*t2+knn[0][2]*t1)
			//		 +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][0]*t2+knn[1][2]*t3);

			//	 Ft3=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3-E22)*(2*knn[1][1]*t3+knn[1][2]*t2)
			//		 +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3-E32)*(2*knn[2][1]*t3+knn[2][2]*t1);
			// }
			// t1b=t1-nn*Ft1;
			// t2b=t2-nn*Ft2;
			// t3b=t3-nn*Ft3;
			// t4b=t4-nn*Ft4;
			// if(Point_Check==4)
			// {
			//	 if(t1==t1b && t2==t2b && t3==t3b &&t4==t4b) break;
			// }
			// else if(Point_Check==3)
			// {
			//	 if(t1==t1b && t2==t2b && t3==t3b ) break;
			// }
			// t1=t1b;
			// t2=t2b;
			// t3=t3b;
			// t4=t4b;
		 //}
		 //---------------------------------------------------
		 //dst_min=180*180*180;
		 //for(int k1=-25;k1<+25;k1++)
		 //{
			// float kf1=t1+float(k1)*0.15f;

			// float x1=LineRays[0].x*kf1+x0;
			// float y1=LineRays[0].y*kf1+y0;
			// float z1=LineRays[0].z*kf1+z0;

			// for(int k2=-25;k2<25;k2++)
			// {
			//	 float kf2=t2+float(k2)*0.15f;

			//	 float x2=LineRays[4].x*kf2+x0;P
			//	 float y2=LineRays[4].y*kf2+y0;
			//	 float z2=LineRays[4].z*kf2+z0;

			//	 for(int k3=-25;k3<25;k3++)
			//	 {
			//		 float kf3=t3+float(k3)*0.15f;
			//		 float x3=LineRays[5].x*kf3+x0;
			//		 float y3=LineRays[5].y*kf3+y0;
			//		 float z3=LineRays[5].z*kf3+z0;


			//		 //-------
			//		 float line1_dst=GL_Distance(x1,y1,z1,x2,y2,z2);//理想值 sqrt(16)
			//		 float line2_dst=GL_Distance(x2,y2,z2,x3,y3,z3);//理想值 sqrt(16)

			//		 float line3_dst=GL_Distance(x1,y1,z1,x3,y3,z3);//理想值 32
			//		 float dst_sum=
			//			  ABS(line1_dst-16)
			//			 +ABS(line2_dst-16)
			//			 +ABS(line3_dst-32)
			//			 ;

			//		 if(dst_min>dst_sum)
			//		 {
			//			 dst_min=dst_sum;
			//			 t1=kf1;
			//			 t2=kf2;
			//			 t3=kf3;
			//		 }
			//	 }
			// }
		 //}
}
/***************************** 计算能量函数 *****************************/
void GL_Energy()
{
	//-------------------------------- 测试梯度下降算法
#if 0
	int Ces=1;
	if(Ces==0)
	{
		// y=x*x/2-2*x
		// y偏导= x-2
		float x=1;
		float Nn=0.9;
		float Ee=0;
		for(int i=0;i<6;i++)
		{
			float Fx=x-2;
			float xb=x-Nn*Fx;
			Ee=-Nn*Fx;
			x=xb;
		}

		printf("误差 = %f ,最优解 = %f \n",Ee,x);
	}
	if(Ces==1)
	{
		// f(x,y)=(x-7)*(x-7) + 2*(y-4)*(y-4)+3
		// f的x偏导数 = 2*(x-7)/2
		// f的y偏导数 = 4*(y-4)/2
		float x=1,y=1;
		float Nn=0.1;
		float Eex=0,Eey=0;
		for(int i=0;i<600;i++)
		{
			
			float Fx=2*(x-7);
			
			float Fy=4*(y-4);

			float xb=x-Nn*Fx;
			float yb=y-Nn*Fy;

			x=xb;
			y=yb;

			Eex=-Nn*Fx;
			Eey=-Nn*Fy;

			
		}

		//printf("误差xy = (%f, %f) ,最优解xy = (%f,%f) \n",Eex,Eey,x,y);
	}
#endif
	//--------------------------------
	float Edge=45*2;
	GL_Point Pt0;
	Pt0.x=-Edge;
	Pt0.y=81;
	Pt0.z=0;

	float x0=Pt0.x;
	float y0=Pt0.y;
	float z0=Pt0.z;

	//LineRays[];
	// (x-x0)/a=(y-y0)/b=(z-z0)/c=t
	// x=at+x0
	// y=bt+y0
	// z=ct+z0

	//-------------------------------------------------------------------------------------------------------
	if(!Best_Flag && EnableX[0]&&EnableY[0] && EnableX[5]&&EnableY[5] && EnableX[4]&&EnableY[4]// 扫描点 使能判断，满足使能条件 则 进行算法迭代。
	    && EnableX[6]&&EnableY[6] 
		&& EnableX[1]&&EnableY[1] 
	)
	{
		 int DiguNum=0;//记录递归算法小循环
		 int DiguCnt=0;//记录递归算法大循环 N*65535
		 int Point_Check;//模式选择：算法点数
		 int i=0;
		 
		 long start_time=clock();//Cpu测试算法时间 起始：
		 //-------------------------------------------------------------------------------------------递归算法 Start
		 //GL_Build_Steepest_M(Point_Check,DiguCnt,DiguNum );
		 GL_Build_Steepest_M5Point(Point_Check,DiguCnt,DiguNum );
		 Best_Flag=1;
		 //-------------------------------------------------------------------------------------------递归算法 Finsh
		
		 long end_time=clock();//Cpu 算法计时完成
		 float timex=float(end_time-start_time)/1000;
		 printf("/****************************************************************************/\n");
		 printf("                       ------<< %d 点估计 >>------ \n",Point_Check);
		 printf("                             <<< 第 %d 帧 >>>\n",Fps_World);
		 printf("递归次数：%d*65535 + %d \n",DiguCnt-1,DiguNum);
		 printf("CPU递归时间 ： %f \n",timex);
		 for(int i=0;i<10;i++)
		 {
			 printf("  %d) dis_minxy: (%f,%f)\n",i+1,DistsX_min[i],DistsY_min[i]);
		 }
		 for(int i=0;i<10;i++)
		 {
			 printf("  %d) 二维投影 : (%f,%f)\n",i+1,CV_PointS[i].x,CV_PointS[i].y);
		 }
		 
		 FILE *file=fopen(".//logcpu.txt","a+");
		 fprintf(file,"/***************************************/\n");
		 fprintf(file,"CPU递归时间 ： %f \n",timex);
		
		//printf("------ >>>  最小能量 ： %f\n",dst_min);
		//------
		float x1=LineRays[4].x*t2_Global+x0;
		float y1=LineRays[4].y*t2_Global+y0;
		float z1=LineRays[4].z*t2_Global+z0;
		float xp0=HeadPlay_Pt[4].x;
		float yp0=HeadPlay_Pt[4].y;
		float zp0=HeadPlay_Pt[4].z;
		//printf("/***************************************/\n");
		printf("单位：每厘米 为 0.4 \n");
		printf("真实值 : (%f,%f,%f)\n",xp0,yp0,zp0);
		printf("估计值 : (%f,%f,%f)\n",x1,y1,z1);

		printf("上一帧真实值 : (%f,%f,%f)\n",xp0_Old,yp0_Old,zp0_Old);
		printf("上一帧估计值 : (%f,%f,%f)\n",x1_Old,y1_Old,z1_Old);

		printf("真实速度= (%f,%f,%f)\n",(xp0-xp0_Old),(yp0-yp0_Old),(zp0-zp0_Old));
		printf("估计速度= (%f,%f,%f)\n",(x1-x1_Old),(y1-y1_Old),(z1-z1_Old));

		printf("误差值 ：(%f,%f,%f)\n",abs(xp0-x1),abs(yp0-y1),abs(zp0-z1));
		//------
		if(abs(xp0-x1)<2 && abs(yp0-y1)<2 && abs(zp0-z1)<2)
		{
			Cnt_Correct++;
		}
		else
		{
			Cnt_Err++;
		}

		printf("4.5*4.5空间 >>> 达标与不达标个数： ( %d , %d )\n",Cnt_Correct,Cnt_Err);

		printf("\n&&&---------------------------------\n");
		printf("3*3空间 单位：每厘米 为 0.6 \n");
		if(abs(xp0-x1)<2.5 && abs(yp0-y1)<2.5 && abs(zp0-z1)<2.5)
		{
			Cnt_Correct3++;
		}
		else
		{
			Cnt_Err3++;
		}
		printf("  3*3  空间 >>> 达标与不达标个数： ( %d , %d )\n",Cnt_Correct3,Cnt_Err3);

		if(abs(xp0-x1)<6.4 && abs(yp0-y1)<6.4 && abs(zp0-z1)<6.4)
		{
			Cnt_Correct12++;
		}
		else
		{
			Cnt_Err12++;
		}
		printf("4.5*4.5空间(16cm 界限) >>> 达标与不达标个数： ( %d , %d )\n",Cnt_Correct12,Cnt_Err12);
		//------------------------------------
		//多点显示
		float x_ture[5],y_ture[5],z_ture[5];//真实
		float x_etm[5],y_etm[5],z_etm[5];//估计

		x_etm[0]=LineRays[0].x*t1_Global+x0;
		y_etm[0]=LineRays[0].y*t1_Global+y0;
		z_etm[0]=LineRays[0].z*t1_Global+z0;

		x_etm[1]=LineRays[4].x*t2_Global+x0;
		y_etm[1]=LineRays[4].y*t2_Global+y0;
		z_etm[1]=LineRays[4].z*t2_Global+z0;

		x_etm[2]=LineRays[5].x*t3_Global+x0;
		y_etm[2]=LineRays[5].y*t3_Global+y0;
		z_etm[2]=LineRays[5].z*t3_Global+z0;

		x_etm[3]=LineRays[6].x*t4_Global+x0;
		y_etm[3]=LineRays[6].y*t4_Global+y0;
		z_etm[3]=LineRays[6].z*t4_Global+z0;

		x_etm[4]=LineRays[1].x*t5_Global+x0;
		y_etm[4]=LineRays[1].y*t5_Global+y0;
		z_etm[4]=LineRays[1].z*t5_Global+z0;
		//---------------------------------
		x_ture[0]=HeadPlay_Pt[0].x;
		y_ture[0]=HeadPlay_Pt[0].y;
		z_ture[0]=HeadPlay_Pt[0].z;

		x_ture[1]=HeadPlay_Pt[4].x;
		y_ture[1]=HeadPlay_Pt[4].y;
		z_ture[1]=HeadPlay_Pt[4].z;

		x_ture[2]=HeadPlay_Pt[5].x;
		y_ture[2]=HeadPlay_Pt[5].y;
		z_ture[2]=HeadPlay_Pt[5].z;

		x_ture[3]=HeadPlay_Pt[6].x;
		y_ture[3]=HeadPlay_Pt[6].y;
		z_ture[3]=HeadPlay_Pt[6].z;

		x_ture[4]=HeadPlay_Pt[1].x;
		y_ture[4]=HeadPlay_Pt[1].y;
		z_ture[4]=HeadPlay_Pt[1].z;
		//-----------------------------
		printf("-------------------------- 冗余多点信息 ：\n");

		FILE *file_Err=fopen(".//ErronPos.txt","a+");
		
		for(int i=0;i<Point_Check;i++)
		{
			printf(" %d) 真实值：(%f,%f,%f) \n",i+1,x_ture[i],y_ture[i],z_ture[i]);
			printf("     估计值：(%f,%f,%f) \n",    x_etm[i],y_etm[i],z_etm[i]);
			printf("     误差值：(%f,%f,%f) \n",ABS(x_ture[i]-x_etm[i]),ABS(y_ture[i]-y_etm[i]),ABS(z_ture[i]-z_etm[i]) );
			if( ABS(x_ture[i]-x_etm[i])<1.0f && ABS(y_ture[i]-y_etm[i])<1.0f && ABS(z_ture[i]-z_etm[i])<1.0f )
			{
				printf("------------------------------->> Success !!!\n");
			}
			else
			{
				printf("------------------------------->> Erron   !!!………………\n");
				Erron_CntBest++;

				//错误情况进行记录用于分析
				fprintf(file_Err,"                       ------<< %d 点估计 >>------ \n",Point_Check);
				fprintf(file_Err,"                             <<< 第 %d 帧 >>>\n",Fps_World);
				fprintf(file_Err,"递归次数：%d*65535 + %d \n",DiguCnt-1,DiguNum);
				fprintf(file_Err,"CPU递归时间 ： %f \n",timex);
				fprintf(file_Err,"%d) 错误情况: 点ID:%d\n",Erron_CntBest,i+1);
				fprintf(file_Err,"    真实值：(%f,%f,%f)\n",x_ture[i],y_ture[i],z_ture[i]);
				fprintf(file_Err,"    估计值：(%f,%f,%f)\n",x_etm[i],y_etm[i],z_etm[i]);
				fprintf(file_Err,"    误差值：(%f,%f,%f)\n",ABS(x_ture[i]-x_etm[i]),ABS(y_ture[i]-y_etm[i]),ABS(z_ture[i]-z_etm[i]));
				fprintf(file_Err,"    PRY姿态角：(%d,%d,%d)\n",Pitch_angle,Rool_angle,Yaw_angle);
			}
		}
		fclose(file_Err);

		printf("@::::::::::::::::::  (2.5cm以内)最优错误计数 ： %d\n",Erron_CntBest);
		//---------------------
		
		fprintf(file,"单位：每厘米 为 0.4 \n");
		fprintf(file,"真实值 : (%f,%f,%f)\n",xp0,yp0,zp0);
		fprintf(file,"估计值 : (%f,%f,%f)\n",x1,y1,z1);

		fprintf(file,"误差值 ：(%f,%f,%f)\n",abs(xp0-x1),abs(yp0-y1),abs(zp0-z1));

		
		xp0_Old=xp0;
		yp0_Old=yp0;
		zp0_Old=zp0;
		x1_Old=x1;
		y1_Old=y1;
		z1_Old=z1;


		fclose(file);

		 FILE *file2=fopen(".//LastData.txt","a+");
		 fprintf(file2,"(%f,%f,%f),(%f,%f,%f,%f),(%d,%d),(%d,%d),(%d,%d)\n",
					pos_x,pos_y,pos_z,q0,q1,q2,q3,
					Cnt_Correct,Cnt_Err,Cnt_Correct3,Cnt_Err3,Cnt_Correct12,Cnt_Err12
					);
		fclose(file2);
	}
	

}
/**********************************************************/
void GL_Draw_Track_Point()
{
	float Edge=45*2;
	GL_Point Pt0;
	Pt0.x=-Edge;
	Pt0.y=81;
	Pt0.z=0;

	float x0=Pt0.x;
	float y0=Pt0.y;
	float z0=Pt0.z;

	//if(Best_Flag)
	{
		//------------------------
		float x1=LineRays[0].x*t1_Global+x0;
		float y1=LineRays[0].y*t1_Global+y0;
		float z1=LineRays[0].z*t1_Global+z0;

		float x2=LineRays[4].x*t2_Global+x0;
		float y2=LineRays[4].y*t2_Global+y0;
		float z2=LineRays[4].z*t2_Global+z0;

		float x3=LineRays[5].x*t3_Global+x0;
		float y3=LineRays[5].y*t3_Global+y0;
		float z3=LineRays[5].z*t3_Global+z0;

		float x4=LineRays[6].x*t4_Global+x0;
		float y4=LineRays[6].y*t4_Global+y0;
		float z4=LineRays[6].z*t4_Global+z0;

		glPushMatrix();//储存当前视图矩阵
		glLineWidth(1); 
		glColor3f(0.8,0.0,0.8); 
		glTranslatef(x1,y1,z1);
		glutSolidSphere(3.6, 20, 20);
		glPopMatrix();//弹出上次保存的位置

		glPushMatrix();//储存当前视图矩阵
		glLineWidth(1); 
		glColor3f(0.8,0.0,0.8); 
		glTranslatef(x2,y2,z2);
		glutSolidSphere(0.9, 20, 20);
		glPopMatrix();//弹出上次保存的位置

		//glPushMatrix();//储存当前视图矩阵
		//glLineWidth(1); 
		//glColor3f(0.0,0.8,0.0); 
		//glTranslatef(x3,y3,z3);
		//glutSolidSphere(0.8, 20, 20);
		//glPopMatrix();//弹出上次保存的位置

		//if(Point_Check==4)
		//{
		//	glPushMatrix();//储存当前视图矩阵
		//	glLineWidth(1); 
		//	glColor3f(0.0,0.8,0.0); 
		//	glTranslatef(x4,y4,z4);
		//	glutSolidSphere(0.8, 20, 20);
		//	glPopMatrix();//弹出上次保存的位置
		//}


		glLineWidth(5); 
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.6f, 0.0f); 
		glVertex3f(x1,y1,z1);
		glVertex3f(x2,y2,z2);
		glEnd();


	}
}
/**********************************************************/
void display(void)
{  
	glClear (GL_COLOR_BUFFER_BIT);
	glClearColor( 0.1f, 0.2f, 0.2f, 1.0f );
	reshape (320*2, 240*2);
	//glPushMatrix();//储存当前视图矩阵
	//glRotatef(angle,0,1,0);
	//glPopMatrix();//储存当前视图矩阵
	//glEnable(GL_AUTO_NORMAL);
	/*************绘制网格线*************/
#if 0
	glBegin(GL_TRIANGLES);  
	glColor3f(0.92f, 0.08f, 0.02f );
	glVertex3f(0.0f,0.0f,0.0f);  
	glVertex3f(50.0f,0.0f,0.0f);  
	glVertex3f(25.0f,25.0f,0.0f);  

	glVertex3f(-25.0f,0.0f,0.0f);  
	glVertex3f(-75.0f,0.0f,0.0f);  
	glVertex3f(-50.0f,25.0f,0.0f);  
	//结束绘三角形  
	glEnd(); 
#endif

//--------------------------------
	//glPushMatrix();//储存当前视图矩阵
	//glTranslatef(45,0,0);         
	////glRotatef(Wwn,QtN.q1,QtN.q2,QtN.q3);
	//glColor3f(0.8f, 0.2f, 0.0f );
	//glScaled(2,15,2);
	//glutSolidCube(2);
	//printf("-------------------- step1.1\n");
	GL_Draw_Filed();
	//printf("-------------------- step1.2\n");
	GL_Scan(0.0091);

	//printf("-------------------- step1.3\n");
	GLB_IMU(gxr_Global,gyr_Global, gzr_Global,1 ) ;
	gxr_Global=0;
	gyr_Global=0;
	gzr_Global=0;
	//printf("-------------------- step1.4\n");
	GL_Head_Display();
	//printf("-------------------- step1.5\n");
	GL_GotHead_DisPlay_Pt();
	//printf("-------------------- step1.6\n");
	GL_LineRays();
	//printf("-------------------- step1.7\n");
	if(Angle_Y>87)
	GL_Energy();
	
	//printf("-------------------- step1.8\n");
	GL_Draw_Track_Point();
	//printf("-------------------- step1.9\n");
	cvWaitKey(1);
	/********************************************/
	glutSwapBuffers();
}
/**********************************************************/
void myIdle(void) 
{  
	angle_GL+=1;
	
}
/**********************************************************/
DWORD _stdcall ThreadProc(LPVOID lpParameter)//线程执行函数
{
	//LastData_Init();
	glutCreateWindow("三维模型"); 
	int num=10;//初始化 目标点的X扫描面 与 Y扫描面 标志
	for(int i=0;i<num;i++)
	{
		EnableX[i]=0;
		EnableY[i]=0;
		DistsX_min[i]=60000;
		DistsY_min[i]=60000;
	
		New_FpsX[i]=0;
		New_FpsY[i]=0;
	}
	init ();
	glutIdleFunc(&myIdle);
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(glutKeyboard);    // called when the application receives a input from the keyboard
    
	glutMainLoop();
	return 0;
}




#endif