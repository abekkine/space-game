#include <stdarg.h>
#include <string.h>
#include <GL/glut.h>

#include <iostream>
#include <string>

#include "main.h"

#define SCREEN_SIZE 1024

namespace logic {

void update()  {
    // TODO : Fill Here
}

void end_program() {

    exit(0);
}

} // namespace logic

namespace input {

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
    case 27:
        logic::end_program();
        break;
    }
}

void mouse(int button, int state, int x, int y) {
    // TODO
}

void motion(int x, int y) {
    // TODO
}

} // namespace input

namespace render {

void world() {
    // TODO : Fill here

    glLoadIdentity();
    glTranslatef(3.0, 5.0, 0.0);
    glRotatef(37.0, 0.0, 0.0, 1.0);

    glLineWidth(4.0);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(-3.0, 1.0);
    glVertex2f(0.0, -4.0);
    glVertex2f(2.0, -2.0);
    glEnd();
}

void ui() {
    // TODO : Fill here

    glLoadIdentity();
    glTranslatef(10, 10, 0);

    glColor4f(0.0, 0.0, 1.0, 0.8);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(200, 0);
    glVertex2i(200, 80);
    glVertex2i(0, 80);
    glEnd();

    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);
    text::print(3, 2, "Testing");
    text::print(3, 3, "Text");
    text::print(3, 4, "Interface");
}

} // namespace render

namespace display {

int screen_size = SCREEN_SIZE;
double world_radius = 10.0;
double world_center_x = 0.0;
double world_center_y = 0.0;
std::string window_name = std::string();
bool blend_enable = false;
bool aa_enable = false;

void world_mode() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(
        world_center_x - world_radius,
        world_center_x + world_radius,
        world_center_y - world_radius,
        world_center_y + world_radius,
        -1.0,
        1.0);
    glMatrixMode(GL_MODELVIEW);
}

void ui_mode() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screen_size, screen_size, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    world_mode();
    render::world();

    ui_mode();
    render::ui();

    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int width, int height) {

    glViewport(0, 0, width, height);
}

void init(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(screen_size, screen_size);
    glutInitWindowPosition(10, 10);
    glutCreateWindow(window_name.c_str());

    if (blend_enable) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    if (aa_enable) {
        glEnable(GL_MULTISAMPLE);
    }

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glutDisplayFunc(draw);
    glutKeyboardFunc(input::keyboard);
    glutMouseFunc(input::mouse);
    glutMotionFunc(input::motion);
    glutPassiveMotionFunc(input::motion);
    glutReshapeFunc(reshape);
    glutIdleFunc(logic::update);
}

void run() {

    glutMainLoop();
}

} // namespace display

namespace text {

void *font = GLUT_BITMAP_9_BY_15;
const int kPrintBufferSize = 256;
char print_buffer[kPrintBufferSize];
void print(int col, int row, const char* format, ...) {
    memset(print_buffer, 0, kPrintBufferSize);
    va_list(args);
    va_start(args, format);
    vsnprintf(print_buffer, kPrintBufferSize, format, args);
    va_end(args);

    int x = (col-1)*9;
    int y = (row)*15;
    glRasterPos2i(x, y);
    for (unsigned int i=0; i<sizeof(print_buffer); ++i) {
        glutBitmapCharacter(font, print_buffer[i]);
    }
}

} // namespace text

int main(int argc, char **argv) {

    display::window_name = "Test Base";
    display::blend_enable = true;
    display::aa_enable = true;
    display::init(argc, argv);

    display::run();
}

