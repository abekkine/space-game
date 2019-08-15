#ifndef TEST_PATTERN_HPP_
#define TEST_PATTERN_HPP_

#include <GL/glut.h>

class TestPattern {
public:
    static void World() {
        glLineWidth(1.0);
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_LOOP);
        glVertex2d(-0.5, -0.5);
        glVertex2d( 0.5, -0.5);
        glVertex2d( 0.5,  0.5);
        glVertex2d(-0.5,  0.5);
        glEnd();
    }
    static void Ui(const int w, const int h) {
//        glColor3f(1.0, 0.0, 0.0);
//        glBegin(GL_QUADS);
//        glVertex2i(10, 10);
//        glVertex2i(20, 10);
//        glVertex2i(20, 20);
//        glVertex2i(10, 20);
//        glEnd();
//
//        glColor3f(0.0, 1.0, 0.0);
//        glBegin(GL_QUADS);
//        glVertex2i(w - 20, h - 20);
//        glVertex2i(w - 10, h - 20);
//        glVertex2i(w - 10, h - 10);
//        glVertex2i(w - 20, h - 10);
//        glEnd();

        glColor4f(1.0, 1.0, 1.0, 0.5);
        glLineWidth(4.0);
        glBegin(GL_LINES);
        glVertex2i(w/2 - 10, h/2);
        glVertex2i(w/2 + 10, h/2);
        glVertex2i(w/2, h/2 - 10);
        glVertex2i(w/2, h/2 + 10);
        glEnd();
    }
};

#endif // TEST_PATTERN_HPP_
