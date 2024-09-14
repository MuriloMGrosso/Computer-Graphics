/*
 * Este programa foi feito para o projeto 1 da disciplina 
 * de Computacao Grafica do curso Bacharelado em Ciencias
 * da Computacao (BCC) na FFCLRP USP.
 *
 * Autores:
 * Murilo M. Grosso
 * Octavio X. Furio
 */

#include <cmath>
#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include "input.h"
#include "segment.h"
				
#define FPS 60				// Frames por segundo
#define DELTA_TIME 1.0/FPS		// Tempo por frame, em segundos
#define CAM_MOVE_SPEED 100.0		// Velocidade de movimento da camera (pixels/s)
#define FISH_SPEED 100.0		// Velocidade do peixe (pixels/s)

float camWidth = 640.0;			// Largura da camera (pixels)
float camHeight = 360.0;		// Altura da camera (pixels)
float camAspect = camWidth/camHeight;	// Aspecto da camera
float camAngle = 45.0;			// Angulo da view da camera
					
float radius = 300.0;
float pos = 0.0;

float camX = 0.0;			// Posicao X da camera
float camY = 0.0;			// Posicao Y da camera
float camZ = radius;			// Posicao Z da camera

Segment fishFocus(NULL, 	50);	// O que o peixe segue
Segment fishHead(&fishFocus, 	20);	// Cabeca do peixe
Segment fishDorsal(&fishHead, 	20);	// Corpo do peixe
Segment fishTail(&fishDorsal, 	20);	// Cauda do peixe

void draw();				// Desenha objetos na cena
void updateView();			// Inicia e atualiza a view
void update(int value);			// Funcao executada a cada frame
void start(int argc, char **argv);	// Inicia parametros iniciais do Open GL
		
void windowSizeUpdate(int width, int height); 	// Atualiza o tamanho da janela

void mouseManager(int button, int state, int x, int y) 
{
	switch(button) {
		case 0: pos -= 0.1; camX = radius * sin(pos); camZ = radius * cos(pos); break; 	// Botao esq
		case 1: break;									// Botao meio
		case 2: pos += 0.1; camX = radius * sin(pos); camZ = radius * cos(pos); break; 	// Botao dir
		case 3: camAngle *= 0.95; 	break;	// Wheel up
		case 4: camAngle *= 1.05; 	break;	// Wheel down
	}
}

int main(int argc, char **argv) 
{	
	start(argc, argv);
	return 0;
}

void update(int value) {
	/*
	 * Movimenta o peixe.
	 * Nota: Multiplicar por DELTA_TIME garante que o 
	 * movimento seja o mesmo independentemente do FPS 
	 * escolhido.
	 */ 
	fishFocus.translate
	(
		input::getHorizontalAxis() * FISH_SPEED * DELTA_TIME,
		input::getVerticalAxis() * FISH_SPEED * DELTA_TIME,
		input::getDepthAxis() * FISH_SPEED * DELTA_TIME
	);

	fishHead.updatePosition(); fishDorsal.updatePosition(); fishTail.updatePosition(); // Atualizar posicoes

	// Atualiza a view e chama o proximo frame
	glutTimerFunc(DELTA_TIME * 1000, update, value + 1);
	updateView();
}

void draw() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, camWidth, camHeight);

	// Aquario
	glColor3f(0, 0, 1);
	glutWireCube(200.0);

	// Isca
	glColor3f(1, 1, 1);
	glPushMatrix();
		glTranslatef(fishFocus.getX(), fishFocus.getY(), fishFocus.getZ());
		glRotatef(fishHead.getRotationX(), 0, 0, 1);
		glRotatef(fishHead.getRotationY(), 0, 1, 0);
		glRotatef(fishHead.getRotationZ(), 1, 0, 0);
	glutWireCube(5.0);

	// Cabeca do peixe
	glColor3f(0.8, 0.0, 0.3);
	glPushMatrix();
		glTranslatef(
			fishHead.getX(), 
			fishHead.getY(), 
			fishHead.getZ());
		glRotatef(fishHead.getRotationX(), 1, 0, 0);
		glRotatef(fishHead.getRotationY(), 0, 1, 0);
		glRotatef(fishHead.getRotationZ(), 0, 0, 1);
		glutWireCube(20.0);
	glPopMatrix();

	// Corpo do peixe
	glColor3f(0.5, 0.0, 0.3);
	glPushMatrix();
		glTranslatef(
			fishDorsal.getX(), 
			fishDorsal.getY(), 
			fishDorsal.getZ());
		glRotatef(fishDorsal.getRotationX(), 1, 0, 0);
		glRotatef(fishDorsal.getRotationY(), 0, 1, 0);
		glRotatef(fishDorsal.getRotationZ(), 0, 0, 1);
		glutWireCube(20.0);
	glPopMatrix();

	// Cauda do peixe
	glColor3f(0.2, 0.0, 0.8);
	glPushMatrix();
		glTranslatef(
			fishTail.getX(), 
			fishTail.getY(), 
			fishTail.getZ());
		glRotatef(fishTail.getRotationX(), 1, 0, 0);
		glRotatef(fishTail.getRotationY(), 0, 1, 0);
		glRotatef(fishTail.getRotationZ(), 0, 0, 1);
		glutWireCube(20.0);
	glPopMatrix();

	glutSwapBuffers();
}

void start(int argc, char **argv) {
    glutInit(&argc, argv);    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowPosition(700, 100);
    glutInitWindowSize(camWidth, camHeight);
    glutCreateWindow("Peixe");

    glutDisplayFunc(draw);
	glutReshapeFunc(windowSizeUpdate);

	glutMouseFunc(mouseManager);
	glutKeyboardFunc(input::keyboardDown);
	glutKeyboardUpFunc(input::keyboardUp);
	glutSpecialFunc(input::specialKeysDown);
	glutSpecialUpFunc(input::specialKeysUp);

	glutTimerFunc(1, update, 0);

	glClearColor(0.0, 0.0, 0.2, 1.0);
	glEnable(GL_DEPTH_TEST);

	updateView();

	glutMainLoop();
}

void updateView() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camAngle, camAspect, 0.1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(	camX, 	camY, 	camZ,	// Cam-pos
			fishFocus.getX(), fishFocus.getY(), fishFocus.getZ(),	// Tar-pos
			0, 	1, 	0	// Normal
		);

	glutPostRedisplay();
}

void windowSizeUpdate(int width, int height)
{
	height = height == 0 ? 1 : height;

	camWidth = width;
	camHeight = height;
	camAspect = (float)width / (float)height;
}
