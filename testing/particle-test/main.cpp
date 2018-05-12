#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>

#include "BigBang.h"
#include "Thruster.h"

BigBang* bang = 0;
Thruster* thruster = 0;

void render();
void idle();

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	render();

	glutSwapBuffers();
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {

	switch(key) {
        case '1':
            thruster->Period(0.05);
            thruster->BeamAngle(45.0);
            thruster->SpeedRange(10.0, 15.0);
            break;
        case '2':
            thruster->Period(0.02);
            thruster->BeamAngle(35.0);
            thruster->SpeedRange(10.0, 15.0);
            break;
        case '3':
            thruster->Period(0.001);
            thruster->BeamAngle(25.0);
            thruster->SpeedRange(10.0, 15.0);
            break;
		case 27:
			exit(0);
			break;
	}
}

void reshape(int width, int height) {

	glViewport(0, 0, width, height);
}

void init_gfx() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void init() {

    init_gfx();

    // Emit one particle each second.
    bang = new BigBang(1.0);
    bang->Init();

    // Emit one particle each 0.01 second, or with 100.0 Hz.
    thruster = new Thruster(0.02);
    thruster->Init();
}

void update_viewport() {
    const double size = 50.0;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-size, size, -size, size, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {

    int screen_size = 1600;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);

	glutInitWindowSize(screen_size, screen_size);

	glutInitWindowPosition(10, 10);

	glutCreateWindow("particles");

	init();

    update_viewport();

	glutMainLoop();

	return 0;
}


void render() {

    if (bang != 0) {
        bang->Render();
    }

    if (thruster != 0) {
        thruster->Render();
    }
}

void idle() {

    bang->Update(0.02);
    thruster->Update(0.02);

    usleep(20000);
}

