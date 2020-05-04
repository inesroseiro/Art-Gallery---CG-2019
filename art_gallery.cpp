
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
#define WHITE 1.0, 1.0, 1.0, 0.5
#define BLACK 0.0, 0.0, 0.0, 1.0
#define GRAY 0.9, 0.92, 0.29, 1.0
#define PI 3.14159
#define CASTANHO 0.36, 0.25, 0.2, 0.6
#define CINZENTO 0.329412, 0.329412, 0.329412
#define CINZENTOCLARO 0.329412, 0.329412, 0.329412

// - Sistema de Particulas

#define frand() ((float)rand() / RAND_MAX)
#define MAX_PARTICULAS 1000
#define MAX_PARTICULAS2 4000

//---------------------------------------- Particle attributes
typedef struct
{
	float size;			// tamanho
	float life;			// vida
	float fade;			// fade
	float r, g, b;		// color
	GLfloat x, y, z;	// posicao
	GLfloat vx, vy, vz; // velocidade
	GLfloat ax, ay, az; // aceleracao
} Particle;

Particle particula1[MAX_PARTICULAS];
Particle particula2[MAX_PARTICULAS2];
Particle particula3[MAX_PARTICULAS];

//-------------------------------------------------------

const float BOX_SIZE = 7.0f;	   //The length of each side of the cube
const float BOX_HEIGHT = BOX_SIZE; //The height of the box off of the ground
const float FLOOR_SIZE = 20.0f;	//The length of each side of the floor

GLfloat turquoiA[] = {0.1, 0.18725, 0.1745};
GLfloat turquoiD[] = {0.396, 0.74151, 0.69102, 0.4};
GLfloat turquoiS[] = {0.297254, 0.30829, 0.306678};
GLint turquoiC = 0.1 * 128;

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
GLfloat raio = 40;
GLfloat angulo = 0.35 * PI;

//------------------------------------------------------------ Observador
GLfloat obsP[] = {raio * cos(angulo), 3, raio *sin(angulo)};
GLfloat incy = 0.5;
GLfloat inca = 0.03;
GLfloat angBule = 0;
GLfloat incBule = 1;

//------------------------------------------------------------ Texturas
GLint repete = 1;
GLint maxR = 20;
GLint numQuadro = 5;
GLuint texture[12];
GLuint tex;
GLuint texture2[1];
RgbImage imag;


// Iluminacao
// Global (ambiente)
//posicao da luz
/* GLfloat posicao_luz[] = {0.0, 50.0, 50.0, 1.0};

GLint noite = 1;
GLfloat luzGlobalCor[4] = {1.0, 1.0, 1.0, 1.0};

GLfloat luz_ambiente[4] = {0, 1, 1, 1};
GLfloat luz_especular[4] = {0.1, 0.1, 0.1, 0.1};
GLfloat luz_difusa[4] = {0.1, 0.1, 0.1, 0.1};

// de 0 a 128
GLint especMaterial = 90;
GLfloat espec[4] = {0.5, 0.5, 0.5, 0.5};
*/

//--------------------------------------------- LUZES
GLfloat luzPosition[] = {9.5, 5, -6.9, 1.0};   //luz pontual
GLfloat luzAmbiente[4]={0.8,0.8,0.8,1.0};					//intensidade da luz ambiente a 50%
GLfloat luzDifusa[4]={0.01,0.01,0.01,0.3};	   				// "cor"
GLfloat luzEspecular[4]={0.9,0.9, 0.9,1};       // "brilho"

// Capacidade de brilho do material
GLfloat especularidade[4]={0.5,0.5,0.5,0.5};
GLint especMaterial = 70;

GLint milisec = 1000;

//rotacão da chaleira
static float xrot;

// ..................................................................

void define_iluminacao(void)
{

	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// luz ambiente

    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
	// concentracao do brilho
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT1, GL_POSITION, luzPosition );

}

void criaDefineTexturas()
{

	// chao
	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("chao.bmp");
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

	// quadro 4
	glGenTextures(1, &texture[6]);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
	imag.LoadBmpFile("quadros.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());

	glGenTextures(1, &texture[7]);
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
	imag.LoadBmpFile("quadros2.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());

	
	// ---------------------- quadro -------------------------------
	glGenTextures(1, &texture[10]);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
	imag.LoadBmpFile("banksy.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());
}

void iniParticulas(Particle *particula)      //inicialização das partículas
{
 GLfloat v, theta, phi;
 int i;
 GLfloat px, py, pz;
 GLfloat ps;

	px = 4.8;
	py = 5.0;
	pz = 8.4;
	ps = 0.1;

 for(i=0; i<MAX_PARTICULAS; i++)  {

	//---------------------------------
	v     = 1*frand()+0.02;
    theta = 2.0*frand()*M_PI;   // [0..2pi]
	phi   = frand()*M_PI;		// [0.. pi]

  particula[i].size = ps ;		// tamanh de cada particula
  particula[i].x	  = px + 0.1*frand()*px;    // [-200 200]
  particula[i].y	  = py + 0.5*frand()*py;	// [-200 200]
  particula[i].z	  = pz + 0.1*frand()*pz;	// [-200 200]

	particula[i].vx = v * cos(theta) * sin(phi);	// esferico
	particula[i].vy = v * cos(phi);
  particula[i].vz = v * sin(theta) * sin(phi);
	particula[i].ax = 0.01f;
  particula[i].ay = -0.01f;
  particula[i].az = 0.015f;


	particula[i].life = 1.0f;
	particula[i].fade = 0.01f;	// Em 100=1/0.01 iteracoes desaparece
	}
}

void iniParticulas3(Particle *particula)      //inicialização das partículas
{
 GLfloat v, theta, phi;
 int i;
 GLfloat px, py, pz;
 GLfloat ps;

	px = 11;
	py = 5.0;
	pz = 9;
	ps = 0.1;



 for(i=0; i<MAX_PARTICULAS; i++)  {

	//---------------------------------
	v     = 1*frand()+0.02;
    theta = 2.0*frand()*M_PI;   // [0..2pi]
	phi   = frand()*M_PI;		// [0.. pi]

  particula[i].size = ps ;		// tamanh de cada particula
  particula[i].x	  = px + 0.01*frand()*px;    // [-200 200]
  particula[i].y	  = py + 0.5*frand()*py;	// [-200 200]
  particula[i].z	  = pz + 0.01*frand()*pz;	// [-200 200]

	particula[i].vx = v * cos(theta) * sin(phi);	// esferico
	particula[i].vy = v * cos(phi);
  particula[i].vz = v * sin(theta) * sin(phi);
	particula[i].ax = 0.01f;
  particula[i].ay = -0.01f;
  particula[i].az = 0.015f;

	particula[i].life = 1.0f;
	particula[i].fade = 0.04f;	// Em 100=1/0.01 iteracoes desaparece
	}
}

void init(void)
{
	glClearColor(WHITE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
    glEnable(GL_LIGHT0);
    // Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LIGHT0);
	criaDefineTexturas();

	iniParticulas(particula1);	
	iniParticulas3(particula3);


	xrot = 0.;
}

void showParticulas3(Particle *particula, int sistema) {
 int i;
 int numero;

 numero=(int) (frand()*10.0);

 for (i=0; i<MAX_PARTICULAS; i++)
	{

	glPushMatrix();
	glTranslatef(particula[i].x -particula[i].size,particula[i].y -particula[i].size,particula[i].z);
	glColor3f(0.15,0.05,0.01);
	glutSolidSphere(0.015,20,20);
	glPopMatrix();
	particula[i].x += particula[i].vx;
  particula[i].y += particula[i].vy;
  particula[i].z += particula[i].vz;
  particula[i].vx += particula[i].ax;
  particula[i].vy += particula[i].ay;
  particula[i].vz += particula[i].az;
	particula[i].life -= particula[i].fade;
	}

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


void desenha_chaleira()
{
	glPushMatrix();
	glColor4f(0.9f, 0.1f, 0.0f, 0.5f);
	glTranslatef(-1, -5, 13);
	glRotatef(-xrot * .35, 0., 1., 0.);
	glutSolidTeapot(1.5);
	glPopMatrix();
}

void desenha_quadro()
{

	//desenhar paisagem

	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPushMatrix();
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(5.5, 5, -6.9);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(13.5, 5, -6.9);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(13.5, -5, -6.9);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(5.5, -5, -6.9);


	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
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
	glVertex3f(-4.8, 6, 7.5);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.8, 6, 18);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.8, -3, 18);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.8, -3, 7.5);

	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//desenhar madeira da janela

	glDisable(GL_TEXTURE_2D);
	glColor3f(CINZENTO);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.9, 6.5, 7);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.9, 6.5, 18.2);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.9, -3.5, 18.2);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.9, -3.5, 7);
}



void desenha_mesa()
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.329412, 0.329412, 0.329412, 0.5);
	glPushMatrix();
	glBegin(GL_QUADS);

	glVertex3i(0 - 5, -7, 7);
	glVertex3i(xC - 4 - 5, -7, 7);
	glVertex3i(xC - 4 - 5, -7, 15);
	glVertex3i(0 - 5, -7, xC);

	glEnd();
	glPopMatrix();
	// ------------------------------------
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.329412, 0.329412, 0.329412, 1);
	glPushMatrix();
	glBegin(GL_QUADS);

	//perna mesa
	glColor4f(WHITE);

	glVertex3f(11 - 5, -7, 7);
	glVertex3f(11 - 5, -7, 15);
	glVertex3f(11 - 5, -9.8, 15);
	glVertex3f(11 - 5, -9.8, 7);

	glVertex3f(0 - 5, -7, 7);
	glVertex3f(0 - 5, -7, 15);
	glVertex3f(0 - 5, -9.8, 15);
	glVertex3f(0 - 5, -9.8, 7);

	glVertex3f(0 - 5, -7, 15);
	glVertex3f(11 - 5, -7, 15);
	glVertex3f(11 - 5, -9.8, 15);
	glVertex3f(0 - 5, -9.8, 15);

	glEnd();
	glPopMatrix();
}

// x inicial e 13 e 16, y -7 e 8.5
void desenha_lance2(int x, int y)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.329412, 0.329412, 0.329412, 0.5);
	glPushMatrix();
	glBegin(GL_QUADS);

	glVertex3i(x, y, 7);
	glVertex3i(3 + x, y, 7);
	glVertex3i(3 + x, y, 15);
	glVertex3i(x, y, 15);

	glEnd();
	glPopMatrix();
	// ------------------------------------
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(WHITE);
	glPushMatrix();
	glBegin(GL_QUADS);

	//perna mesa
	glColor4f(WHITE);

	glVertex3f(3 + x, y, 7);
	glVertex3f(3 + x, y, 15);
	glVertex3f(3 + x, y - 1.5, 15);
	glVertex3f(3 + x, y - 1.5, 7);

	glVertex3f(x, y, 7);
	glVertex3f(x, y, 15);
	glVertex3f(x, y - 1.5, 15);
	glVertex3f(x, y - 1.5, 7);

	glVertex3f(x, y, 15);
	glVertex3f(3 + x, y, 15);
	glVertex3f(3 + x, y - 1.5, 15);
	glVertex3f(x, y - 1.5, 15);

	glEnd();
	glPopMatrix();
}

void desenha_copo()
{
	GLUquadricObj *y = gluNewQuadric();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(0.55, 0.0, 0, 0.5);

	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(7, -6.9, 11);
	glRotatef(-90, 1, 0, 0);
	gluQuadricDrawStyle(y, GLU_FILL);
	gluQuadricNormals(y, GLU_SMOOTH);
	gluQuadricTexture(y, GL_TRUE);
	gluCylinder(y, 0.7, 1, 2, 100, 100);
	glPopMatrix();
}


void showParticulas(Particle *particula, int sistema)
{
	int i;
	int numero;

	numero = (int)(frand() * 10.0);

	for (i = 0; i < MAX_PARTICULAS; i++)
	{

		glPushMatrix();
		glTranslatef(particula[i].x - particula[i].size, particula[i].y - particula[i].size, particula[i].z);
		glColor3f(0.15, 0.05, 0.01);
		glutSolidSphere(0.015, 20, 20);
		glPopMatrix();
		particula[i].x += particula[i].vx;
		particula[i].y += particula[i].vy;
		particula[i].z += particula[i].vz;
		particula[i].vx += particula[i].ax;
		particula[i].vy += particula[i].ay;
		particula[i].vz += particula[i].az;
		particula[i].life -= particula[i].fade;
	}
}

void drawCorrimao(){
  GLUquadricObj*  y = gluNewQuadric ( );

  glPushMatrix();
  glTranslatef(10,12,16.5);
  glRotatef(90,1,0,0);

  gluQuadricDrawStyle(y, GLU_FILL);
  gluQuadricNormals(y, GLU_SMOOTH);
  gluQuadricTexture(y, GL_TRUE);
  glColor4f(CASTANHO);
  gluCylinder(y,0.3,0.3, 12, 10, 10);
  glPopMatrix();
  gluDeleteQuadric ( y );


}

void drawScene()
{

	GLUquadricObj *y = gluNewQuadric();

	//desenha chao
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 2.0f);
	glVertex3i(-5, -10, 0);

	glTexCoord2f(2.0f, 2.0f);
	glVertex3i(xC + 10, -10, 0);

	glTexCoord2f(2.0f, 0.0f);
	glVertex3i(xC + 10, -10, xC + 15);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3i(-5, -10, xC + 15);

	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glScalef(2, 1, 3);

	showParticulas(particula1, 1);
	showParticulas(particula3, 3);
	glPopMatrix();

	//desenha parede de frente
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3i(-5, 10, 0);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3i(25, 10, 0);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3i(25, -10, 0);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3i(-5, -10, 0);

	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//desenha parede de lado
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPushMatrix();
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3i(-5, 10, 0);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3i(-5, 10, xC + 15);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3i(-5, -10, xC + 15);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3i(-5, -10, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//desenha parede do lado da porta
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPushMatrix();
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3i(15 + 10, 1, 0);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3i(15 + 10, 1, xC + 15);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3i(15 + 10, -10, xC + 15);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3i(15 + 10, -10, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//desenha chao do cimo das escadas
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 2.0f);
	glVertex3i(xC + 10, 1, 0);

	glTexCoord2f(2.0f, 2.0f);
	glVertex3i(xC + 20, 1, 0);

	glTexCoord2f(2.0f, 0.0f);
	glVertex3i(xC + 20, 1, xC + 15);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3i(xC + 10, 1, xC + 15);

	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(0 - 5, 0, 0);
	desenha_copo();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -15, 0);
	glScalef(1, -1, 1);

	glPopMatrix();
	glDisable(GL_STENCIL_TEST);

	glPushMatrix();
	glTranslatef(- 5, -15, 0);

	glScalef(1, -1, 1);
	desenha_copo();

	glPopMatrix();
	glDisable(GL_STENCIL_TEST);

	//Blending
	glEnable(GL_BLEND);

	glPushMatrix();
	desenha_mesa();
	glDisable(GL_BLEND);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(1, 0, 7);
	desenha_quadro();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(1, 0, 7);
	glRotatef(-xrot * .35, 0., 1., 0.);
	desenha_lance2(13, -7);
	desenha_lance2(15, -5);
	desenha_lance2(17, -3);
	desenha_lance2(19, -1);



	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, 0, 7);

	
	desenha_lance2(21, 1);

	//drawCorrimao();

	glPopMatrix();




	
	glPushMatrix();
	desenha_chaleira();

	glPopMatrix();

	glRotatef(0.1, 0.1, 0.1, 0.0);


	glPopMatrix();
}

void display(void)
{
	// [ Apagar ]
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

void idle(void)
{

	glutPostRedisplay();
}

void Timer(int value)
{
	if(xrot < 67){
		xrot += 0.2f;
	}
	else{
		xrot = 0;
	}
	
	//printf(" -> %f\n", xrot);
	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1000);
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
		//printf("l");
		if (glIsEnabled(GL_LIGHTING))
			glDisable(GL_LIGHTING);
		else
			glEnable(GL_LIGHTING);
		break;
	case 50:
		iniParticulas(particula1);
		glutPostRedisplay();
		break;
	case 49:
		//printf("p");

		iniParticulas3(particula3);
		glutPostRedisplay();
		break;

	case 51:
		iniParticulas(particula1);
		iniParticulas3(particula3);
		glutPostRedisplay();
		break;
	}
}

void teclasNotAscii(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)

		obsP[0] = obsP[0] - 0.2;
	obsP[2] = obsP[2] - 0.2;

	if (key == GLUT_KEY_RIGHT)

		obsP[0] = obsP[0] + 0.2;
	obsP[2] = obsP[2] + 0.2;

	if (key == GLUT_KEY_UP)
		obsP[2] = obsP[2] - 0.2;

	if (key == GLUT_KEY_DOWN)
		obsP[2] = obsP[2] + 0.2;

	glutPostRedisplay();
}
// ---------------------------------------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Van Gogh's Museum");

	init();
	glutIdleFunc(idle);

	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);
	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutReshapeFunc(resizeWindow);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-30.0, 30.0, -30.0, 30.0, -30.0, 30.0);
	glutMainLoop();

	return 0;
}
