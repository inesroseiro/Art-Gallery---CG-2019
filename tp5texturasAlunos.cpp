
#include <stdlib.h>
#include <stdio.h> // printf
#include "RgbImage.h"
#include <math.h>
#include <time.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h> // openGL

//--------------------------------- Definir cores
#define AZUL 0.0, 0.0, 1.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0, 1.0
#define AMARELO 1.0, 1.0, 0.0, 1.0
#define VERDE 0.0, 1.0, 0.0, 1.0
#define LARANJA 1.0, 0.5, 0.1, 1.0
#define WHITE 1.0, 1.0, 1.0, 1.0
#define BLACK 0.0, 0.0, 0.0, 1.0
#define GRAY 0.9, 0.92, 0.29, 1.0
#define PI 3.14159
#define CASTANHO 0.545, 0.271, 0.075, 1.0
#define CINZENTO 0.329412, 0.329412, 0.329412

//------------------------------------------------------------ Sistema Coordenadas
GLfloat xC = 15.0, yC = 15.0, zC = 60.0;
GLint wScreen = 800, hScreen = 600;
GLfloat mesa = 3.0;
GLfloat quad = 6.0;
GLfloat mesaP[] = {4, 0, 10};
GLfloat quadP[] = {4, 4, 0.1};
GLint msec = 10; //.. definicao do timer (actualizacao)

//------------------------------------------------------------ Observador
GLint defineView = 0;
GLint defineProj = 1;
GLfloat raio = 30;
GLfloat angulo = 0.35 * PI;

//------------------------------------------------------------ Observador
GLfloat obsP[] = {raio * cos(angulo), 5.5, raio *sin(angulo)};
GLfloat incy = 0.5;
GLfloat inca = 0.03;
GLfloat angBule = 0;
GLfloat incBule = 1;

//------------------------------------------------------------ Texturas
GLint repete = 1;
GLint maxR = 20;
GLint numQuadro = 5;
GLuint texture[7];
GLuint tex;
RgbImage imag;

// Iluminacao
// Global (ambiente)
//posicao da luz
GLfloat posicao_luz[] = {0.0, 50.0, 50.0, 1.0};

GLint noite = 1;
GLfloat luzGlobalCor[4] = {1.0, 1.0, 1.0, 1.0};

GLfloat luz_ambiente[4] = {0, 1, 1, 1};
GLfloat luz_especular[4] = {0.1, 0.1, 0.1, 0.1};
GLfloat luz_difusa[4] = {0.1, 0.1, 0.1, 0.1};

// de 0 a 128
GLint especMaterial = 90;
GLfloat espec[4] = {0.5, 0.5, 0.5, 0.5};

//sistema de particulas

void define_iluminacao(void)
{

	// luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luz_ambiente);
	// concentracao do brilho
	// glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// quantidade de brilho do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, espec);
	glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
}

void criaDefineTexturas()
{
	// cinzento da parte de cima/baixo das latas
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("latinha.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());

	// textura lata da mesa
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("oi.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());

	// Textura Lata da prateleira
	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("lata.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());

	// chao
	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("azulejo.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());

	// aplica textura a parede do lado
	glGenTextures(1, &texture[4]);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
	imag.LoadBmpFile("ex.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());

	// aplica textura a parede frente
	glGenTextures(1, &texture[5]);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
	imag.LoadBmpFile("paredes.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());

	// aplica textura janela
	glGenTextures(1, &texture[6]);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
	imag.LoadBmpFile("paisagem.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());
}

void init(void)
{
	glClearColor(WHITE);
	glShadeModel(GL_SMOOTH);
	criaDefineTexturas();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);

}

void resizeWindow(GLsizei w, GLsizei h)
{
	wScreen = w;
	hScreen = h;
	glViewport(0, 0, wScreen, hScreen);
	glutReshapeWindow(wScreen, hScreen);
	glutPostRedisplay();
}

void DrawCircle(float raio, float lineW)
{
	float rad;
	float angle, radian, x, y, tx, ty, xcos, ysin;
	int i;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glLineWidth(lineW);
	glBegin(GL_TRIANGLE_FAN);
	for (i = 0; i < 360; i++)
	{
		rad = i * PI / 180.0;
		xcos = (float)cos(rad);
		ysin = (float)sin(rad);
		x = xcos * raio;
		y = ysin * raio * lineW;
		tx = xcos * 0.5 + 0.5;
		ty = ysin * 0.5 + 0.5;

		glTexCoord2f(tx, ty);
		glVertex2f(x, y);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void draw_primeira_fila()
{
	GLUquadricObj *y = gluNewQuadric();
	int i;
	for (i = 0; i < 4; i++)
	{
		//Parte de baixo da lata
		glColor3f(CINZENTO);
		glPushMatrix();
		glTranslatef(3 + 1.5f * i, 0.1, 2);
		glRotatef(-90, 1, 0, 0);
		DrawCircle(0.6, 1);
		glPopMatrix();
		//Parte de cima da lata
		glColor3f(CINZENTO);
		glPushMatrix();
		glTranslatef(3 + 1.5f * i, 2.1, 2);
		glRotatef(-90, 1, 0, 0);
		DrawCircle(0.6, 1);
		glPopMatrix();

		//Cilindro com textura (lata)
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glPushMatrix();
		glTranslatef(3 + 1.5f * i, 0.1, 2);
		glRotatef(-90, 1, 0, 0);
		gluQuadricDrawStyle(y, GLU_FILL);
		gluQuadricNormals(y, GLU_SMOOTH);
		gluQuadricTexture(y, GL_TRUE);
		gluCylinder(y, 0.6, 0.6, 2.0, 200, 200);
		glPopMatrix();
	}
}

void draw_primeira_fila2()
{
	GLUquadricObj *y = gluNewQuadric();
	int i;
	for (i = 0; i < 4; i++)
	{
		//Parte de baixo da lata
		glColor3f(CINZENTO);
		glPushMatrix();
		glTranslatef(9.5 + 1.5f * i, 0.1, 2);
		glRotatef(-90, 1, 0, 0);
		DrawCircle(0.6, 1);
		glPopMatrix();
		//Parte de cima da lata
		glColor3f(CINZENTO);
		glPushMatrix();
		glTranslatef(9.5 + 1.5f * i, 2.1, 2);
		glRotatef(-90, 1, 0, 0);
		DrawCircle(0.6, 1);
		glPopMatrix();

		//Cilindro com textura (lata)
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glPushMatrix();
		glTranslatef(9.5 + 1.5f * i, 0.1, 2);
		glRotatef(-90, 1, 0, 0);
		gluQuadricDrawStyle(y, GLU_FILL);
		gluQuadricNormals(y, GLU_SMOOTH);
		gluQuadricTexture(y, GL_TRUE);
		gluCylinder(y, 0.6, 0.6, 2.0, 200, 200);
		glPopMatrix();
	}
}

void draw_segunda_fila(float aumento)
{
	GLUquadricObj *y = gluNewQuadric();
	int i;
	for (i = 0; i < 3; i++)
	{
		//Parte de baixo da lata
		glColor3f(CINZENTO);
		glPushMatrix();
		glTranslatef(3.75+aumento + 1.5f * i, 2.1, 2);
		glRotatef(-90, 1, 0, 0);
		DrawCircle(0.6, 1);
		glPopMatrix();
		//Parte de cima da lata
		glColor3f(CINZENTO);
		glPushMatrix();
		glTranslatef(3.75+aumento + 1.5f * i, 4.1, 2);
		glRotatef(-90, 1, 0, 0);
		DrawCircle(0.6, 1);
		glPopMatrix();

		//Cilindro com textura (lata)
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glPushMatrix();
		glTranslatef(3.75+aumento + 1.5f * i, 2.1, 2);
		glRotatef(-90, 1, 0, 0);
		gluQuadricDrawStyle(y, GLU_FILL);
		gluQuadricNormals(y, GLU_SMOOTH);
		gluQuadricTexture(y, GL_TRUE);
		gluCylinder(y, 0.6, 0.6, 2.0, 200, 200);
		glPopMatrix();
	}
}

void draw_terceira_fila(float aumento)
{
	GLUquadricObj *y = gluNewQuadric();
	int i;
	for (i = 0; i < 2; i++)
	{
		//Parte de baixo da lata
		glColor3f(CINZENTO);
		glPushMatrix();
		glTranslatef(aumento + 4.5 + 1.5f * i, 4.1, 2);

		glRotatef(-90, 1, 0, 0);
		DrawCircle(0.6, 1);
		glPopMatrix();
		//Parte de cima da lata
		glColor3f(CINZENTO);
		glPushMatrix();
		glTranslatef(aumento + 4.5 + 1.5f * i, 6.1, 2);
		glRotatef(-90, 1, 0, 0);
		DrawCircle(0.6, 1);
		glPopMatrix();

		//Cilindro com textura (lata)
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glPushMatrix();
		glTranslatef(aumento + 4.5 + 1.5f * i, 4.1, 2);
		glRotatef(-90, 1, 0, 0);
		gluQuadricDrawStyle(y, GLU_FILL);
		gluQuadricNormals(y, GLU_SMOOTH);
		gluQuadricTexture(y, GL_TRUE);
		gluCylinder(y, 0.6, 0.6, 2.0, 200, 200);
		glPopMatrix();
	}
}

void desenha_lata(float aumento)
{
	GLUquadricObj *y = gluNewQuadric();

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glPushMatrix();
	glTranslatef(aumento + 5.25, 6.1, 2);
	glRotatef(-90, 1, 0, 0);
	gluQuadricDrawStyle(y, GLU_FILL);
	gluQuadricNormals(y, GLU_SMOOTH);
	gluQuadricTexture(y, GL_TRUE);
	gluCylinder(y, 0.6, 0.6, 2, 100, 100);

	glPopMatrix();

	glColor3f(CINZENTO);
	glPushMatrix();
	glTranslatef(aumento + 5.25, 6.1, 2);
	glRotatef(-90, 1, 0, 0);
	DrawCircle(0.6, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(aumento + 5.25, 8.1, 2);
	glRotatef(-90, 1, 0, 0);
	DrawCircle(0.6, 1);
	glPopMatrix();
}
void draw_toalha_mesa(){

	glDisable(GL_TEXTURE_2D);
	glColor4f(0.4f, 0.1f, 0.05f, 1.0f);
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(0, -6.9, 9);
	glVertex3f(xC - 4, -6.9, 9);
	glVertex3f(xC - 4, -6.9, 13);
	glVertex3f(0, -6.9, 13);
	
	glVertex3f(0, -6.9, 9);
	glVertex3f(0, -6.9, 13);
	glVertex3f(0, -7.9, 13);
	glVertex3f(0, -7.9, 9);
	
	glVertex3f(11, -6.9, 9);
	glVertex3f(11, -6.9, 13);
	glVertex3f(11, -7.9, 13);
	glVertex3f(11, -7.9, 9);


	glEnd();
	glPopMatrix();
}




void desenha_chaleira(){
	glPushMatrix();
	glColor4f(0.0f, 0.3f, 0.5f, 1.0f);
	glTranslatef(8.5,-5.8, 11);
	glutSolidTeapot(1.5);
	glPopMatrix();
}

void draw_janela()
{
	//desenhar paisagem

	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture[6]);

	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.8, 6.3, 8.3);

	glTexCoord2f(2.0f, 1.0f);
	glVertex3f(-4.8, 6.3, xC + 0.3);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.8, -3.3, xC + 0.3);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.8, -3.3, 8.3);

	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//desenhar madeira da janela

	glDisable(GL_TEXTURE_2D);
	glColor4f(0.5f, 0.35f, 0.05f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.9, 6.5, 7.9);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.9, 6.5, xC + 0.5);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.9, -3.5, xC + 0.5);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.9, -3.5, 7.9);

	//desenhar madeira da janela

	glDisable(GL_TEXTURE_2D);
	glColor4f(0.5f, 0.35f, 0.05f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glPushMatrix();
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.7, 6.5, 12.4);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.7, 6.5, xC - 3);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.7, -3.5, xC - 3);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.7, -3.5, 12.4);

	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void desenha_mesa()
{
	glDisable(GL_TEXTURE_2D);
	glColor4f(0.5f, 0.35f, 0.05f, 1.0f);
	glPushMatrix();
	glBegin(GL_QUADS);

	glVertex3i(0, -7, 7);
	glVertex3i(xC - 4, -7, 7);
	glVertex3i(xC - 4, -7, 15);
	glVertex3i(0, -7, xC);

	glEnd();
	glPopMatrix();
	// ------------------------------------
	glDisable(GL_TEXTURE_2D);
	glColor4f(0.5f, 0.35f, 0.05f, 1.0f);
	glPushMatrix();
	glBegin(GL_QUADS);

	//perna direita da mesa 
	glVertex3f(8, -7, 7);
	glVertex3f(8, -7, 15);
	glVertex3f(8, -9.8, 15);
	glVertex3f(8, -9.8,7);

	glVertex3f(9, -7, 7);
	glVertex3f(9, -7, 15);
	glVertex3f(9, -9.8, 15);
	glVertex3f(9, -9.8,7);

	// plano da frente
	glVertex3f(8, -7, 15);
	glVertex3f(8, -9.8, 15);
	glVertex3f(9, -7, 15);
	glVertex3f(9, -9.8, 15);

	// plano de tras
	glVertex3f(8, -7, 7);
	glVertex3f(8, -9.8, 7);
	glVertex3f(9, -7, 7);
	glVertex3f(9, -9.8, 7);

	//perna esquerda da mesa 
	glVertex3f(4, -7, 7);
	glVertex3f(4, -7, 15);
	glVertex3f(4, -9.8, 15);
	glVertex3f(4, -9.8,7);

	glVertex3f(5, -7, 7);
	glVertex3f(5, -7, 15);
	glVertex3f(5, -9.8, 15);
	glVertex3f(5, -9.8,7);

	// plano da frente
	glVertex3f(4, -7, 15);
	glVertex3f(4, -9.8, 15);
	glVertex3f(5, -7, 15);
	glVertex3f(5, -9.8, 15);

	// plano de tras
	glVertex3f(8, -7, 7);
	glVertex3f(8, -9.8, 7);
	glVertex3f(9, -7, 7);
	glVertex3f(9, -9.8, 7);

	glEnd();
	glPopMatrix();
}

void drawScene()
{

	GLUquadricObj *y = gluNewQuadric();

	//desenha chao
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 3.0f);
	glVertex3i(-5, -10, 0);
	glTexCoord2f(3.0f, 3.0f);
	glVertex3i(xC, -10, 0);
	glTexCoord2f(3.0f, 0.0f);
	glVertex3i(xC, -10, xC + 5);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3i(-5, -10, xC + 5);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//desenha parede de frente
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 3.0f);
	glVertex3i(-5, 10, 0);
	glTexCoord2f(3.0f, 3.0f);
	glVertex3i(xC, 10, 0);
	glTexCoord2f(3.0f, 0.0f);
	glVertex3i(xC, -10, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3i(-5, -10, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//desenha parede de lado
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 3.0f);
	glVertex3i(-5, 10, 0);
	glTexCoord2f(3.0f, 3.0f);
	glVertex3i(-5, 10, xC + 5);
	glTexCoord2f(3.0f, 0.0f);
	glVertex3i(-5, -10, xC + 5);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3i(-5, -10, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//prateleira
	glDisable(GL_TEXTURE_2D);
	glColor4f(0.5f, 0.35f, 0.05f, 1.0f);

	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 2.0f);
	glVertex3i(-5, 0, 0);
	glTexCoord2f(2.0f, 2.0f);
	glVertex3i(xC, 0, 0);
	glTexCoord2f(2.0f, 0.0f);
	glVertex3i(xC, 0, xC / 3);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3i(-5, 0, xC / 3);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


	draw_janela();
	desenha_mesa();
	draw_toalha_mesa();
	draw_primeira_fila();
	draw_primeira_fila2();
	draw_segunda_fila(0);
	draw_segunda_fila(6.5);
	draw_terceira_fila(0);
	draw_terceira_fila(6.5);

	desenha_lata(0);
	desenha_lata(6.5);

	desenha_chaleira();

	


	//desenha lata da mesa 

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glTranslatef(3,-6.9, 11);
	glRotatef(-90, 1, 0, 0);
	gluQuadricDrawStyle(y, GLU_FILL);
	gluQuadricNormals(y, GLU_SMOOTH);
	gluQuadricTexture(y, GL_TRUE);
	gluCylinder(y, 1, 1, 4, 100, 100);

	glPopMatrix();

	glColor3f(CINZENTO);
	glPushMatrix();
	glTranslatef(3.1, -2.9, 11);
	glRotatef(-90, 1, 0, 0);
	DrawCircle(1, 1);
	glPopMatrix();


}

void display(void)
{

	// [ Apagar ]
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// [ Janela Visualizacao ]
	glViewport(0, 0, wScreen, hScreen);

	// [ Projeccao]
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch (defineProj)
	{
	case 1:
		gluPerspective(99.0, wScreen / hScreen, 0.1, zC);
		break;
	default:
		glOrtho(-xC, xC, -yC, yC, -zC, zC);
		break;
	}

	// [ Modelo+View(camera/observador) ]
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);

	// [ Objectos ]
	define_iluminacao();

	drawScene();

	//Actualizacao
	glutSwapBuffers();
}

void Timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1);
}
// Eventos do teclado
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
	case 'Q':
		defineProj = (defineProj + 1) % 2;
	case 27:
		exit(0);
		break;

	case 'l':
		printf("l");
		if (glIsEnabled(GL_LIGHTING))
			glDisable(GL_LIGHTING);
		else
			glEnable(GL_LIGHTING);
		break;

	case 'e':
	case 'E':
		break;

	}
}

void teclasNotAscii(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		obsP[1] = obsP[1] + incy;
	if (key == GLUT_KEY_DOWN)
		obsP[1] = obsP[1] - incy;
	if (key == GLUT_KEY_LEFT)
		angulo = angulo + inca;
	if (key == GLUT_KEY_RIGHT)
		angulo = angulo - inca;

	if (obsP[1] > yC)
		obsP[1] = yC;
	if (obsP[1] < -yC)
		obsP[1] = -yC;
	obsP[0] = raio * cos(angulo);
	obsP[2] = raio * sin(angulo);

	glutPostRedisplay();
}
// ---------------------------------------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Can's Shop");

	init();

	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);
	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutReshapeFunc(resizeWindow);

	glutMainLoop();

	return 0;
}
