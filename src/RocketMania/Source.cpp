/*
* OGL02Animation.cpp: 3D Shapes with animation
*/
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <vector>
#include <time.h>
#include "Mp3.h"
#include <stdio.h>
#include <string.h>
#include <ctime>
using namespace std;

/* Global variables */
char title[] = "3D Shapes with animation";
int refreshMills = 15;        // refresh interval in milliseconds [NEW]
int cubeAmount = 10;

clock_t start;
double duration;
boolean printScore = true;

GLfloat cubeSize = 0.5f;
GLfloat cubeAngle = 0.0f;     // Rotational angle for cube [NEW]
GLfloat cubePosX = 0.0f;
GLfloat cubePosY = 25.0f;
GLfloat cubePosZ = -50.0f;
GLfloat cubeSpeedX = 0.0f;
GLfloat cubeSpeedY = 0.2f;
GLfloat cubeSpeedZ = 0.0f;
GLfloat cubeSpeedRot = -1.5f;

GLfloat pyramidSize = 1.0f;
GLfloat pyramidAngle = 0.0f;  // Rotational angle for pyramid [NEW]
GLfloat pyramidPosX = 0.0f;
GLfloat pyramidPosY = -18.0f;
GLfloat pyramidPosZ = -50.0f;
GLfloat pyramidSpeedX = 0.0f;
GLfloat pyramidSpeedY = 0.0f;
GLfloat pyramidSpeedZ = 0.0f;
GLfloat pyramidSpeedRot = 0.2f;

// WINDOW STUFF
float minX = -(20.0f + cubeSize * 3);
float maxX = -minX;
float minY = (20.0 + cubeSize * 3);
float maxY = -minY;

// FIND FURTHEST CUBE
// check if it's gone yet
int cubeMaxY;
float yValMax = 0.0f;
/* Initialize OpenGL Graphics */

void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

class Entity {
public:
	GLfloat size, posX, posY, posZ, angle, speedX, speedY, speedZ, speedRot;
	Entity(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	void move();
	virtual void draw() = 0;
	void cubeDie();
};

Entity::Entity(GLfloat _size, GLfloat _posX, GLfloat _posY, GLfloat _posZ, GLfloat _angle, GLfloat _speedX, GLfloat _speedY, GLfloat _speedZ, GLfloat _speedRot) {
	size = _size;
	posX = _posX;
	posY = _posY;
	posZ = _posZ;
	angle = _angle;
	speedX = _speedX;
	speedY = _speedY;
	speedZ = _speedZ;
	speedRot = _speedRot;
}

void Entity::move() {
	posX -= speedX;
	posY -= speedY;
	posZ -= speedZ;
	angle += speedRot;
}

class Cube : public Entity {
public:
	Cube();
	Cube(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	void draw();
};

Cube::Cube() : Entity(0.1f * (rand() % 10 + 4), 20.0f * (rand() % 10 * -rand() % 1 + 1), 15.0f * (rand() % 5 + 1), -50.0f, 1.0f * (rand() % 10 + 1), 0.0f, 0.1f * (rand() % 5 + 1), 0.0f, -0.1f * (rand() % 10 + 1)) {
}

Cube::Cube(GLfloat _size, GLfloat _posX, GLfloat _posY, GLfloat _posZ, GLfloat _angle, GLfloat _speedX, GLfloat _speedY, GLfloat _speedZ, GLfloat _speedRot) : Entity(_size, _posX, _posY, _posZ, _angle, _speedX, _speedY, _speedZ, _speedRot) {
	//
}

void Cube::draw() {
	glLoadIdentity();                 // Reset the model-view matrix
	glTranslatef(posX, posY, posZ);  // Move right and into the screen
	glRotatef(angle, 1.0f, 1.0f, 1.0f);  // Rotate about (1,1,1)-axis [NEW
	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
									  // Top face (y = 1.0f)
									  // Define vertices in counter-clockwise (CCW) order with normal pointing out
	glColor3f(0.0f, size, 0.0f);     // Green
	glVertex3f(size, size, -size);
	glVertex3f(-size, size, -size);
	glVertex3f(-size, size, size);
	glVertex3f(size, size, size);

	// Bottom face (y = -size)
	//glColor3f(size, 0.5f, 0.0f);     // Orange
	glVertex3f(size, -size, size);
	glVertex3f(-size, -size, size);
	glVertex3f(-size, -size, -size);
	glVertex3f(size, -size, -size);

	// Front face  (z = size)
	//glColor3f(size, 0.0f, 0.0f);     // Red
	glVertex3f(size, size, size);
	glVertex3f(-size, size, size);
	glVertex3f(-size, -size, size);
	glVertex3f(size, -size, size);

	// Back face (z = -size)
	//glColor3f(size, size, 0.0f);     // Yellow
	glVertex3f(size, -size, -size);
	glVertex3f(-size, -size, -size);
	glVertex3f(-size, size, -size);
	glVertex3f(size, size, -size);

	// Left face (x = -size)
	//glColor3f(0.0f, 0.0f, size);     // Blue
	glVertex3f(-size, size, size);
	glVertex3f(-size, size, -size);
	glVertex3f(-size, -size, -size);
	glVertex3f(-size, -size, size);

	// Right face (x = size)
	//glColor3f(size, 0.0f, size);     // Magenta
	glVertex3f(size, size, -size);
	glVertex3f(size, size, size);
	glVertex3f(size, -size, size);
	glVertex3f(size, -size, -size);
	glEnd();  // End of drawing color-cube

	move();
}

//Pyramid Class
class Pyramid : public Entity {
public:
	GLfloat power;
	boolean alive;
	Pyramid(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	void draw();
};

Pyramid::Pyramid(GLfloat _size, GLfloat _posX, GLfloat _posY, GLfloat _posZ, GLfloat _angle, GLfloat _speedX, GLfloat _speedY, GLfloat _speedZ, GLfloat _speedRot) : Entity(_size, _posX, _posY, _posZ, _angle, _speedX, _speedY, _speedZ, _speedRot) {
	power = 0.25f;
	alive = true;
}

void Pyramid::draw() {
	glLoadIdentity();                  // Reset the model-view matrix
	glTranslatef(posX, posY, posZ);  // Move left and into the screen
	glRotatef(angle, 0.0f, 1.0f, 0.0f);  // Rotate about the (1,1,0)-axis [NEW]

										 // PUSAT DITENGAH SEGITIGA
	glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
									 // Front
	glColor3f(size, 0.0f, 0.0f);     // Red
	glVertex3f(0.0f, size, 0.0f);
	glColor3f(0.0f, size, 0.0f);     // Green
	glVertex3f(-size, -size, size);
	glColor3f(0.0f, 0.0f, size);     // Blue
	glVertex3f(size, -size, size);

	// Right
	glColor3f(size, 0.0f, 0.0f);     // Red
	glVertex3f(0.0f, size, 0.0f);
	glColor3f(0.0f, 0.0f, size);     // Blue
	glVertex3f(size, -size, size);
	glColor3f(0.0f, size, 0.0f);     // Green
	glVertex3f(size, -size, -size);

	// Back
	glColor3f(size, 0.0f, 0.0f);     // Red
	glVertex3f(0.0f, size, 0.0f);
	glColor3f(0.0f, size, 0.0f);     // Green
	glVertex3f(size, -size, -size);
	glColor3f(0.0f, 0.0f, size);     // Blue
	glVertex3f(-size, -size, -size);

	// Left
	glColor3f(size, 0.0f, 0.0f);       // Red
	glVertex3f(0.0f, size, 0.0f);
	glColor3f(0.0f, 0.0f, size);       // Blue
	glVertex3f(-size, -size, -size);
	glColor3f(0.0f, size, 0.0f);       // Green
	glVertex3f(-size, -size, size);

	glEnd();   // Done drawing the pyramid

	move();
	speedX *= 0.95;
	speedY *= 0.95;
}

class Bullet : public Entity {
public:
	Bullet(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	void draw();
};

Bullet::Bullet(GLfloat _size, GLfloat _posX, GLfloat _posY, GLfloat _posZ, GLfloat _angle, GLfloat _speedX, GLfloat _speedY, GLfloat _speedZ, GLfloat _speedRot) : Entity(_size, _posX, _posY, _posZ, _angle, _speedX, _speedY, _speedZ, _speedRot) {

}

void Bullet::draw() {
	glLoadIdentity();                  // Reset the model-view matrix
	glTranslatef(posX, posY, posZ);  // Move left and into the screen

	move();

}

vector<Bullet> bullets;

Pyramid pyramid1(pyramidSize, pyramidPosX, pyramidPosY, pyramidPosZ, pyramidAngle, pyramidSpeedX, pyramidSpeedY, pyramidSpeedZ, pyramidSpeedRot);

Cube cube1(cubeSize, cubePosX, cubePosY, cubePosZ, cubeAngle, cubeSpeedX, cubeSpeedY, cubeSpeedZ, cubeSpeedRot);
vector<Cube> cubes;

void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective 
}

void initMusic() {
	// this is used to hold the user input text 
	char buffer[256];

	// a flag to see if the user wants to quit yet
	int done = 0;

	// initialise DirectShow
	Mp3Init();

	// load an mp3 file
	Mp3Load("tune.mp3");
	Mp3Play();
}

/* Handler for window-repaint event. Called back when the window first appears and
whenever the window needs to be re-painted. */
void display() {
	GLfloat x, y, z, x_min, x_max, y_min, y_max, z_min, z_max;

	// Draw3D
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	cube1.draw();
	if (pyramid1.alive) pyramid1.draw();
	for (int i = 0; i < cubes.size(); i++) {
		// draw cubes
		cubes[i].draw();

		// check if collide
		x = pyramid1.posX; y = pyramid1.posY; z = pyramid1.posZ;
		x_min = cubes[i].posX - cubes[i].size; x_max = cubes[i].posX + cubes[i].size;
		y_min = cubes[i].posY - cubes[i].size; y_max = cubes[i].posY + cubes[i].size;
		z_min = cubes[i].posZ - cubes[i].size; z_max = cubes[i].posZ + cubes[i].size;
		if (x_min <= x && x <= x_max && y_min <= y && y <= y_max && z_min <= z && z <= z_max) {
			// printf("COLLIDE!\n");
			pyramid1.alive = false;
		}
	}

	
	duration = (clock() - start) / (double)CLOCKS_PER_SEC;

	if (!pyramid1.alive) {
		duration = (clock() - start) / (double)CLOCKS_PER_SEC;
		if(printScore) printf("GAME OVER!\n\nYour score is: %2f\n", duration * 1000);
		printScore = false;
	}
	else {
		printf("score: %2f\n", duration * 1000);
		ClearScreen();
	}

	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

/* Called back when timer expired [NEW] */
void timer(int value) {
	glutPostRedisplay();      // Post re-paint request to activate display()
	glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}



void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT:
		if (pyramid1.posX < maxX) pyramid1.speedX = -pyramid1.power;
		else pyramid1.speedX = 0;
		break;
	case GLUT_KEY_LEFT:
		if (pyramid1.posX > minX) pyramid1.speedX = pyramid1.power;
		else pyramid1.speedX = 0;
		break;
	case GLUT_KEY_UP:
		if (pyramid1.posY > maxY) pyramid1.speedY = -pyramid1.power;
		else pyramid1.speedY = 0;
		break;
	case GLUT_KEY_DOWN:
		if (pyramid1.posY < minY) pyramid1.speedY = pyramid1.power;
		else pyramid1.speedY = 0;
		break;
	default:
		break;
	}
}

/* Handler for window re-size event. Called back when the window first appears and
whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
											   // Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
								  // Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	srand(time(NULL));
	// Init Cubes
	int j = 1;
	
	printf("START!\n");
	

	// CubeCreator
	cubeAmount = 10;
	for (int j = 1; j < 100; j++) {
		for (int i = 0; i < cubeAmount; i++) {
			float xVal = 2 * (rand() % 20 + 1.0) + minX;
			float yVal = 15.0f * (rand() % 5) + minY * j;
			float zVal = -50.0f;

			cubes.push_back(Cube(0.1f * (rand() % 10 + 4.0f), xVal, yVal, zVal, 1.0f * (rand() % 10 + 1.0f), 0.0f, 0.1f * (rand() % 5 + 1.0f), 0.0f, -0.1f * (rand() % 10 + 1.0f)));
			if (xVal > maxX || xVal < minX) printf("cube xVal exceeds! %f\n", xVal);
			if (yVal < minY) printf("cube xVal exceeds! %f\n", yVal);

			// GET FURTHEST CUBE
			if (yValMax < yVal) { yValMax = yVal; cubeMaxY = i; }
		}

	}
	start = clock();

	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(700, 700);   // Set the window's initial width & height
	glutInitWindowPosition(400, 50); // Position the window's initial top-left corner
	glutCreateWindow(title);          // Create window with the given title

	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	glutSpecialFunc(specialKeys);

	initGL();                       // Our own OpenGL initialization
	initMusic();
	glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}