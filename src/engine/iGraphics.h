//
//  Original Author: S. M. Shahriar Nirjon
//
//  Last Modified by: Mohammad Saifur Rahman
//  last modified: December 20, 2015
//
//  Version: 2.0.2012.2015
//


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include "paths.h" // before GL, so Windows headers come in without min/max macros
#include <GL/glut.h> // freeglut, on Windows and Linux

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int iScreenHeight, iScreenWidth;
int iMouseX, iMouseY;
int ifft = 0;
void (*iAnimFunction[10])(void)=
{	0};
int iAnimCount = 0;
int iAnimDelays[10];
int iAnimPause[10];

void iDraw();
void iKeyboard(unsigned char);
void iSpecialKeyboard(unsigned char);
void iMouseMove(int, int);
void iPassiveMouse(int, int);
void iMouse(int button, int state, int x, int y);

// Windows' SetTimer, which the game was first written for, never fired faster
// than about 16 ms; the game speed is tuned on that, so keep it on every system.
#define I_MIN_TIMER_MS 16

static void iTimerTick(int index)
{
	if (!iAnimPause[index])
		iAnimFunction[index]();
	glutTimerFunc(iAnimDelays[index], iTimerTick, index);
}

int iSetTimer(int msec, void (*f)(void))
{
	int i = iAnimCount;

	if (iAnimCount >= 10)
	{
		printf("Error: Maximum number of already timer used.\n");
		return -1;
	}

	iAnimFunction[i] = f;
	iAnimDelays[i] = msec < I_MIN_TIMER_MS ? I_MIN_TIMER_MS : msec;
	iAnimPause[i] = 0;
	glutTimerFunc(iAnimDelays[i], iTimerTick, i);
	iAnimCount++;

	return i;
}

void iPauseTimer(int index)
{
	if (index >= 0 && index < iAnimCount)
	{
		iAnimPause[index] = 1;
	}
}

void iResumeTimer(int index)
{
	if (index >= 0 && index < iAnimCount)
	{
		iAnimPause[index] = 0;
	}
}

// Loads assets/images/<name> (PNG or JPEG) into a texture. Returns 0, which draws
// nothing, when the file is missing.
unsigned int iLoadImage(const char *name)
{
	int width, height, bpp;
	unsigned int texture;

	std::string path = assetPath(std::string("images/") + name);
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &bpp, 4);
	if (!data)
	{
		fprintf(stderr, "Cannot load image %s\n", path.c_str());
		return 0;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width, height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data);

	stbi_image_free(data);

	return texture;
}

void iShowImage(int x, int y, int width, int height, unsigned int texture)
{

	//unsigned int texture;


	//int width, height, bpp;
	/*
	BYTE* data(0);
	data = stbi_load(filename, &width, &height, &bpp, 4);


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
			     GL_RGBA,
				 width, height,
				 0,
				 GL_RGBA,
				 GL_UNSIGNED_BYTE,
				 data);

	stbi_image_free(data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	*/

	//width = 100;
	//height = 100;
	glEnable(GL_TEXTURE_2D);



	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//glPushMatrix();
	//glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);

		glTexCoord2f(0, 0);
		glVertex2f(x, y);

		glTexCoord2f(1, 0);
		glVertex2f(x + width, y);

		glTexCoord2f(1, -1);
		glVertex2f(x + width, y+height);

		glTexCoord2f(0, -1);
		glVertex2f(x, y + height);

	glEnd();
	//glPopMatrix();

	glDisable(GL_TEXTURE_2D);

}



void iGetPixelColor(int cursorX, int cursorY, int rgb[])
{
	GLubyte pixel[3];
	glReadPixels(cursorX, cursorY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *) pixel);

	rgb[0] = pixel[0];
	rgb[1] = pixel[1];
	rgb[2] = pixel[2];

	//printf("%d %d %d\n",pixel[0],pixel[1],pixel[2]);
}

void iText(GLdouble x, GLdouble y, const char *str, void* font = GLUT_BITMAP_8_BY_13)
{
	glRasterPos3d(x, y, 0);
	int i;
	for (i = 0; str[i]; i++)
	{
		glutBitmapCharacter(font, str[i]); //,GLUT_BITMAP_8_BY_13, GLUT_BITMAP_TIMES_ROMAN_24
	}
}

void iPoint(double x, double y, int size = 0)
{
	int i, j;
	glBegin (GL_POINTS);
	glVertex2f(x, y);
	for (i = x - size; i < x + size; i++)
	{
		for (j = y - size; j < y + size; j++)
		{
			glVertex2f(i, j);
		}
	}
	glEnd();
}

void iLine(double x1, double y1, double x2, double y2)
{
	glBegin (GL_LINE_STRIP);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void iFilledPolygon(double x[], double y[], int n)
{
	int i;
	if (n < 3)
		return;
	glBegin (GL_POLYGON);
	for (i = 0; i < n; i++)
	{
		glVertex2f(x[i], y[i]);
	}
	glEnd();
}

void iPolygon(double x[], double y[], int n)
{
	int i;
	if (n < 3)
		return;
	glBegin (GL_LINE_STRIP);
	for (i = 0; i < n; i++)
	{
		glVertex2f(x[i], y[i]);
	}
	glVertex2f(x[0], y[0]);
	glEnd();
}

void iRectangle(double left, double bottom, double dx, double dy)
{
	double x1, y1, x2, y2;

	x1 = left;
	y1 = bottom;
	x2 = x1 + dx;
	y2 = y1 + dy;

	iLine(x1, y1, x2, y1);
	iLine(x2, y1, x2, y2);
	iLine(x2, y2, x1, y2);
	iLine(x1, y2, x1, y1);
}

void iFilledRectangle(double left, double bottom, double dx, double dy)
{
	double xx[4], yy[4];
	double x1, y1, x2, y2;

	x1 = left;
	y1 = bottom;
	x2 = x1 + dx;
	y2 = y1 + dy;

	xx[0] = x1;
	yy[0] = y1;
	xx[1] = x2;
	yy[1] = y1;
	xx[2] = x2;
	yy[2] = y2;
	xx[3] = x1;
	yy[3] = y2;

	iFilledPolygon(xx, yy, 4);
}

void iFilledCircle(double x, double y, double r, int slices = 30)
{
	double t, PI = acos(-1.0), dt, x1, y1, xp, yp;
	dt = 2 * PI / slices;
	xp = x + r;
	yp = y;
	glBegin (GL_POLYGON);
	for (t = 0; t <= 2 * PI; t += dt)
	{
		x1 = x + r * cos(t);
		y1 = y + r * sin(t);

		glVertex2f(xp, yp);
		xp = x1;
		yp = y1;
	}
	glEnd();
}

void iCircle(double x, double y, double r, int slices = 30)
{
	double t, PI = acos(-1.0), dt, x1, y1, xp, yp;
	dt = 2 * PI / slices;
	xp = x + r;
	yp = y;
	for (t = 0; t <= 2 * PI; t += dt)
	{
		x1 = x + r * cos(t);
		y1 = y + r * sin(t);
		iLine(xp, yp, x1, y1);
		xp = x1;
		yp = y1;
	}
}

void iEllipse(double x, double y, double a, double b, int slices = 40)
{
	double t, PI = acos(-1.0), dt, x1, y1, xp, yp;
	dt = 2 * PI / slices;
	xp = x + a;
	yp = y;
	for (t = 0; t <= 2 * PI; t += dt)
	{
		x1 = x + a * cos(t);
		y1 = y + b * sin(t);
		iLine(xp, yp, x1, y1);
		xp = x1;
		yp = y1;
	}
}

void iFilledEllipse(double x, double y, double a, double b, int slices = 40)
{
	double t, PI = acos(-1.0), dt, x1, y1, xp, yp;
	dt = 2 * PI / slices;
	xp = x + a;
	yp = y;
	glBegin (GL_POLYGON);
	for (t = 0; t <= 2 * PI; t += dt)
	{
		x1 = x + a * cos(t);
		y1 = y + b * sin(t);
		glVertex2f(xp, yp);
		xp = x1;
		yp = y1;
	}
	glEnd();
}

//
// Rotates the co-ordinate system
// Parameters:
//  (x, y) - The pivot point for rotation
//  degree - degree of rotation
//
// After calling iRotate(), evrey subsequent rendering will
// happen in rotated fashion. To stop rotation of subsequent rendering,
// call iUnRotate(). Typical call pattern would be:
//      iRotate();
//      Render your objects, that you want rendered as rotated
//      iUnRotate();
//
void iRotate(double x, double y, double degree)
{
	// push the current matrix stack
	glPushMatrix();

	//
	// The below steps take effect in reverse order
	//

	// step 3: undo the translation
	glTranslatef(x, y, 0.0);

	// step 2: rotate the co-ordinate system across z-axis
	glRotatef(degree, 0, 0, 1.0);

	// step 1: translate the origin to (x, y)
	glTranslatef(-x, -y, 0.0);
}

void iUnRotate()
{
	glPopMatrix();
}

void iSetColor(double r, double g, double b)
{
	double mmx;
	mmx = r;
	if (g > mmx)
		mmx = g;
	if (b > mmx)
		mmx = b;
	mmx = 255;
	if (mmx > 0)
	{
		r /= mmx;
		g /= mmx;
		b /= mmx;
	}
	glColor3f(r, g, b);
}

void iDelay(int sec)
{
	int t1, t2;
	t1 = time(0);
	while (1)
	{
		t2 = time(0);
		if (t2 - t1 >= sec)
			break;
	}
}

void iDelayMS(int msec)
{
	clock_t end;
	end = clock() + msec * CLOCKS_PER_SEC / 1000;
	while (end > clock())
		;
}

void iClear()
{
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode (GL_MODELVIEW);
	glClearColor(0, 0, 0, 0);
	glFlush();
}

void displayFF(void)
{

	iDraw();
	glutSwapBuffers();
}

// Redraw about 60 times a second, instead of as fast as the machine allows.
void redrawTick(int)
{
	glutPostRedisplay();
	glutTimerFunc(I_MIN_TIMER_MS, redrawTick, 0);
}

void keyboardHandler1FF(unsigned char key, int x, int y)
{
	iKeyboard(key);
	glutPostRedisplay();
}
void keyboardHandler2FF(int key, int x, int y)
{
	iSpecialKeyboard(key);
	glutPostRedisplay();
}

void mouseMoveHandlerFF(int mx, int my)
{
	iMouseX = mx;
	iMouseY = iScreenHeight - my;
	iMouseMove(iMouseX, iMouseY);

	glFlush();
}


void passiveMouseHandlerFF(int mx, int my)
{
	iMouseX = mx;
	iMouseY = iScreenHeight - my;
	iPassiveMouse(iMouseX, iMouseY);

	glFlush();
}
void mouseHandlerFF(int button, int state, int x, int y)
{
	iMouseX = x;
	iMouseY = iScreenHeight - y;

	iMouse(button, state, iMouseX, iMouseY);

	glFlush();
}

void iInitialize(int width = 500, int height = 500, const char *title = "iGraphics")
{
	iScreenHeight = height;
	iScreenWidth = width;

	int argc = 1;
	char program[] = "game";
	char *argv[] = {program, NULL};
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(10, 10);
	glutCreateWindow(title);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
	//glOrtho(-100.0 , 100.0 , -100.0 , 100.0 , -1.0 , 1.0) ;
	//SetTimer(0, 0, 10, timer_proc);



}

void iStart()
{
	iClear();

	glutDisplayFunc(displayFF);
	glutKeyboardFunc(keyboardHandler1FF); //normal
	glutSpecialFunc(keyboardHandler2FF); //special keys
	glutMouseFunc(mouseHandlerFF);
	glutMotionFunc(mouseMoveHandlerFF);
	glutPassiveMotionFunc(passiveMouseHandlerFF);
	glutTimerFunc(I_MIN_TIMER_MS, redrawTick, 0);

	//
	// Setup Alpha channel testing.
	// If alpha value is greater than 0, then those
	// pixels will be rendered. Otherwise, they would not be rendered
	//
	glAlphaFunc(GL_GREATER, 0.0f);
	glEnable(GL_ALPHA_TEST);
	glutMainLoop();
}
