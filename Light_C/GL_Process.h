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


using namespace cv;
using namespace std;

const int ESC = 27;
//
void reshape (int w, int h);


CvFont font;  
char buf_T[256];

int angle_GL=0;
bool flag_stop=0;

GLfloat eyex=0,eyey=160,eyez=350;
float Step_X1=0,Step_X2=0;
bool flag_StepX=0;
int Angle_Y=0;

bool Flag_X=0,Flag_Y=1;

//------------------------------------------------------------------ OpenGL
void init(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
}

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
	}
}

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
/************************* 叉积 *************************/
void GL_CAJI(float a1,float a2,float a3,float b1,float b2,float b3,float &n1,float &n2,float &n3 )
{
	//A×B=(a2b3-a3b2)*i+(a3b1-a1b3)*j+(a1b2-a2b1)*k.
	n1=(a2*b3-a3*b2);
	n2=(a3*b1-a1*b3);
	n3=(a1*b2-a2*b1);

	float norm=n1*n1+n2*n2+n3*n3;
	norm=sqrt(norm);
	n1/=norm;
	n2/=norm;
	n3/=norm;

}

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
	gluCylinder(cylinder_obj, 3, 3, 40, 20, 30);   
	glPopMatrix();

	glPushMatrix();//---- 1
	glLineWidth(1); 
	glTranslatef(Edge,40,0);

	glRotatef(angle_GL,0,1,0);

	glColor3f(0.0f, 1.0f, 0.0f); 

	glutWireSphere(6, 15, 15);
	glPopMatrix();

	//-------------------------------------
	glPushMatrix();
	glTranslatef(-Edge,0,0);
	glRotatef(-90,1,0,0);
	glColor3f(0.5f, 0.2f, 1.0f); 
	gluCylinder(cylinder_obj, 3, 3, 40, 20, 30);   
	glPopMatrix();

	glPushMatrix();//---- 2
	glLineWidth(1); 
	glTranslatef(-Edge,40,0);

	glRotatef(angle_GL,0,1,0);

	glColor3f(1.0f, 0.0f, 0.0f); 

	glutWireSphere(6, 15, 15);
	glPopMatrix();
	//-------------------------------
	glPushMatrix();
	glTranslatef(Edge,0,Edge2);
	glRotatef(-90,1,0,0);
	glColor3f(0.5f, 0.2f, 1.0f); 
	gluCylinder(cylinder_obj, 3, 3, 40, 20, 30);   
	glPopMatrix();

	glPushMatrix();
	glLineWidth(1); 
	glTranslatef(Edge,40,Edge2);//---- 3

	glRotatef(angle_GL,0,1,0);

	glColor3f(0.0f, 0.0f, 1.0f); 

	glutWireSphere(6, 15, 15);
	glPopMatrix();

	//-------------------------------------
	glPushMatrix();
	glTranslatef(-Edge,0,Edge2);
	glRotatef(-90,1,0,0);
	glColor3f(0.5f, 0.2f, 1.0f); 
	gluCylinder(cylinder_obj, 3, 3, 40, 20, 30);   
	glPopMatrix();

	glPushMatrix();//---- 4
	glLineWidth(1); 
	glTranslatef(-Edge,40,Edge2);

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
	glTranslatef(-Edge,52,0);
	glRotatef(angle_GL,0,1,0);
	glColor3f(0.0f, 1.0f, 1.0f); 
	glutSolidSphere(6, 15, 15);
	glPopMatrix();

	//------ Light B
	glPushMatrix();
	glLineWidth(1); 
	glTranslatef(Edge,52,Edge2);
	glRotatef(angle_GL,0,1,0);
	glColor3f(1.0f, 0.0f, 1.0f); 
	glutSolidSphere(6, 15, 15);
	glPopMatrix();
}
void GL_Scan()
{
	float Edge=45*2;
	float a1=2*Edge;
	float b1=0;
	float c1=2*Edge;

	float x1,y1,z1;
	//------ X Scan
	//(x-x0)/a1=(z-z0)/c1=t
	if(Flag_X==0)
	{
		if(flag_StepX==0)
		{
			Step_X1+=1.0f/90.0f;
		}

		if(Step_X1>1.0f)flag_StepX=1;
		x1=Step_X1*a1-Edge;
		y1=0;
		z1=Step_X1*c1-0;

		if(flag_StepX==1)
		{
			Step_X2+=1.0f/50.0f*52;
			y1=Step_X2;

			if(Step_X2>52)
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
		glVertex3f(-Edge,52,0);
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
		glBegin(GL_LINES);//绘制居中轨迹
		glColor3f(0.05f, 0.6f, 0.9f); 
		glVertex3f(-Edge,52,0);
		glVertex3f(xl,yl,zl);
		glVertex3f(-Edge,52,0);
		glVertex3f(xr,yr,zr);
		glVertex3f(xl,yl,zl);
		glVertex3f(xr,yr,zr);
		glEnd();
	}
	
	//------ Y Scan
	if(Flag_Y==0)
	{
		float Rr=Edge*2*1.414;
		float x2=cos(float(Angle_Y)*CV_PI/180)*Rr-Edge;
		float y2=0;
		float z2=sin(float(Angle_Y)*CV_PI/180)*Rr;

		Angle_Y++;
		if(Angle_Y>90)
		{
			Angle_Y=0;
			Flag_X=0;
			Flag_Y=1;
		}

		glPushMatrix();
		glTranslatef(x2,y2,z2);
		glRotatef(angle_GL,0,1,0);
		glColor3f(0.0f, 1.0f, 0.0f); 
		glutSolidSphere(6, 15, 15);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(x2,y2+52,z2);
		glRotatef(angle_GL,0,1,0);
		glColor3f(0.0f, 1.0f, 0.0f); 
		glutSolidSphere(6, 15, 15);
		glPopMatrix();

		glBegin(GL_LINES);//绘制居中轨迹
		glColor3f(0.01f, 1.0f, 1.0f); 
		glVertex3f(-Edge,0,0);
		glVertex3f(x2,y2,z2);
		glVertex3f(-Edge,52,0);
		glVertex3f(x2,y2+52,z2);

		glEnd();

		glBegin(GL_LINES);//绘制居中轨迹
		glColor3f(1.1f, 0.0f, 0.0f); 

		glVertex3f(x2,y2,z2);
		glVertex3f(x2,y2+52,z2);

		glEnd();

		
	}
	


}
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

	
	GL_Draw_Filed();
	GL_Scan();
	/********************************************/
	glutSwapBuffers();
}
void myIdle(void) 
{  
	angle_GL++;

}
/************************************************/
DWORD _stdcall ThreadProc(LPVOID lpParameter)//线程执行函数
{
	glutCreateWindow("三维模型"); 
	init ();
	glutIdleFunc(&myIdle);
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(glutKeyboard);    // called when the application receives a input from the keyboard
  
	glutMainLoop();
	return 0;
}




#endif