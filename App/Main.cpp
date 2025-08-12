#include "Mesh.h"
#include "MeshRenderer.h"
#include "glut.h"

// Globals
Mesh mesh;
MeshRenderer renderer;

int win_posx = 100, win_posy = 100;
int win_width = 720, win_height = 480;

float ex = 2.0f, ey = 2.0f, ez = 2.0f;
float cx = 0.0f, cy = 0.0f, cz = 0.0f;
float upx = 0.0f, upy = 1.0f, upz = 0.0f;
float fovy = 60.0f, z_near = 0.1f, z_far = 100.0f;

float cam_lx = 0.0f, cam_ly = 0.0f, cam_lz = 0.0f;
float cam_posx = 0.0f, cam_posy = 0.0f, cam_posz = 0.0f;
float cam_rotx = 0.0f, cam_roty = 0.0f, cam_rotz = 0.0f;

float move_step = 0.1f;
const double PI = 3.141592654;

void renderBitmapString(float x, float y, void* font, const char* string) {
	glRasterPos2f(x, y);
	while (*string) {
		glutBitmapCharacter(font, *string);
		string++;
	}
}

void displayLabel() {
	// Save current projection and modelview matrices
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, win_width, 0, win_height);  // Match window size

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);  // Ensure text appears on top

	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1.0f, 1.0f, 0.8f);

	void* font = GLUT_BITMAP_8_BY_13;
	float x = 10.0f;
	float y = win_height - 20.0f;  // Top-left
	int line_height = 18;

	renderBitmapString(x, y, font, "W/S: Move Camera Forward / Backward (Z axis)");
	renderBitmapString(x, y -= line_height, font, "A/D: Move Camera Left / Right (X axis)");
	renderBitmapString(x, y -= line_height, font, "Q/E: Move Camera Up / Down (Y axis)");
	renderBitmapString(x, y -= line_height, font, "Arrow Keys: Rotate Camera X / Y");
	renderBitmapString(x, y -= line_height, font, "Shift + Left/Right: Rotate Camera Z");
	renderBitmapString(x, y -= line_height, font, "ESC: Exit Program");

	glPopAttrib();

	glEnable(GL_DEPTH_TEST);  // Restore depth test

	// Restore matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void axes() {
	glEnable(GL_COLOR_MATERIAL);
	glPushAttrib(GL_CURRENT_BIT);

	// x axis
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f); // red
	glVertex3f(-10.0f, 0.0f, 0.0f);
	glVertex3f(10.0f, 0.0f, 0.0f);
	glEnd();

	// y axis
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f); // green
	glVertex3f(0.0f, -10.0f, 0.0f);
	glVertex3f(0.0f, 10.0f, 0.0f);
	glEnd();

	// z axis
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f); // blue
	glVertex3f(0.0f, 0.0f, -10.0f);
	glVertex3f(0.0f, 0.0f, 10.0f);
	glEnd();

	glPopAttrib();
	glDisable(GL_COLOR_MATERIAL);
}

void camera() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(ex + cam_lx, ey + cam_ly, ez + cam_lz, cx, cy, cz, upx, upy, upz);

	glTranslatef(cam_posx, cam_posy, cam_posz);

	glRotatef(cam_rotx, 1.0f, 0.0f, 0.0f);
	glRotatef(cam_roty, 0.0f, 1.0f, 0.0f);
	glRotatef(cam_rotz, 0.0f, 0.0f, 1.0f);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w': cam_posz -= move_step; break;
	case 's': cam_posz += move_step; break;
	case 'a': cam_posx -= move_step; break;
	case 'd': cam_posx += move_step; break;
	case 'q': cam_posy += move_step; break;
	case 'e': cam_posy -= move_step; break;
	case 27: exit(0); // ESC key to quit
	}
	glutPostRedisplay();
}

void keyboardSpecial(int key, int x, int y) {
	int multiplier = 10;
	int mod = glutGetModifiers();  // Get current modifier key (e.g., Shift, Ctrl)

	switch (key) {
	case GLUT_KEY_LEFT:
		if (mod == GLUT_ACTIVE_SHIFT)
			cam_rotz -= move_step * multiplier;  // Rotate around Z axis
		else
			cam_roty -= move_step * multiplier;  // Rotate around Y axis
		break;
	case GLUT_KEY_RIGHT:
		if (mod == GLUT_ACTIVE_SHIFT)
			cam_rotz += move_step * multiplier;
		else
			cam_roty += move_step * multiplier;
		break;
	case GLUT_KEY_UP:
		cam_rotx += move_step * multiplier;  // Rotate around X axis
		break;
	case GLUT_KEY_DOWN:
		cam_rotx -= move_step * multiplier;
		break;
	}
	glutPostRedisplay();
}

void keyLight() {
	glEnable(GL_LIGHT0);
	GLfloat pos[] = { 5.0f, 10.0f, 5.0f, 1.0f };  // Positioned top-front-right
	GLfloat diffuse[] = { 1.0f, 0.4f, 0.4f, 1.0f };  // Warm reddish
	GLfloat specular[] = { 1.0f, 0.4f, 0.4f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01f);
}

void fillLight() {
	glEnable(GL_LIGHT1);
	GLfloat pos[] = { -5.0f, 5.0f, 10.0f, 1.0f };  // Positioned front-left, lower intensity
	GLfloat diffuse[] = { 0.4f, 0.4f, 1.0f, 1.0f };  // Cool bluish
	GLfloat specular[] = { 0.4f, 0.4f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_POSITION, pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.07f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.02f);
}

void backLight() {
	glEnable(GL_LIGHT2);
	GLfloat pos[] = { 0.0f, 10.0f, -10.0f, 1.0f };  // Positioned behind the subject
	GLfloat diffuse[] = { 0.4f, 1.0f, 0.4f, 1.0f };  // Greenish
	GLfloat specular[] = { 0.4f, 1.0f, 0.4f, 1.0f };

	glLightfv(GL_LIGHT2, GL_POSITION, pos);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular);

	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.1f);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.03f);
}

void lights() {
	glEnable(GL_LIGHTING);
	keyLight();
	fillLight();
	backLight();

	// Enable color material for easy coloring
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_LIGHTING); // for safety, disable lighting

	camera();
	axes();
	displayLabel();

	lights();

	// Render mesh wireframe
	//renderer.renderWireframe(mesh);
	renderer.renderSolid(mesh);

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);

	if (h == 0) h = 1;
	GLfloat aspect_ratio = (GLfloat)w / (GLfloat)h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, aspect_ratio, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv) {
	// Setup mesh data
	//mesh = Mesh::createPlane(1.0f);
	mesh = Mesh::createCube(1.0f);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

	glutInitWindowPosition(win_posx, win_posy);
	glutInitWindowSize(win_width, win_height);
	glutCreateWindow("3D");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutSpecialFunc(keyboardSpecial);
	glutKeyboardFunc(keyboard);

	init();
	glutMainLoop();

	return 0;
}