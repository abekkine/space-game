#ifndef GAME_RENDER_H_
#define GAME_RENDER_H_

#include <math.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "texture.h"

class GameRender {
 public:
    explicit GameRender(bool test_mode)
    : test_mode_(test_mode)
    , test_texture_(0) {
    }

    ~GameRender() {
        delete test_texture_;
    }

    void Init() {
        if (test_texture_ != 0) {
            delete test_texture_;
        }

        test_texture_ = new Texture("test.png");
    }
    void Render() {
        glPushMatrix();
        RenderUniverse();
        RenderPlayer();
        // if (test_mode_) {
        //     TestPrimitives();
        //     TestTextures();
        // } else {
            // TODO Render game content +render +game id:17 gh:20 ic:gh
            // - Implement methods required to render game content.
        // }
        glPopMatrix();
    }
    void RenderPlayer() {
        glLoadIdentity();
        glTranslated(0.0, 0.0, 0.0);
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_TRIANGLES);
            glVertex2d(-0.6, -0.3);
            glVertex2d(0.6, -0.3);
            glVertex2d(0.0,  1.0);
        glEnd();
    }
    void RenderUniverse() {
        glLoadIdentity();
        glTranslated(0.0, -100.0, 0.0);
        glColor3f(1.0, 0.0, 0.0);

        const double R = 90.0;
        glBegin(GL_TRIANGLE_FAN);
            for (double a=0.0; a < 2.0 * M_PI; a+=0.05) {
                glVertex2d(R * cos(a), R * sin(a));
            }
        glEnd();

    }

 private:
    void TestPrimitives() {
        glLoadIdentity();
        glTranslated(-5.0, -5.0, 0.0);
        glBegin(GL_QUADS);
            glColor3f(1.0, 0.0, 0.0);
            glVertex2d(-1.0, -1.0);
            glColor3f(0.0, 1.0, 0.0);
            glVertex2d(1.0, -1.0);
            glColor3f(0.0, 0.0, 1.0);
            glVertex2d(1.0,  1.0);
            glColor3f(1.0, 0.0, 1.0);
            glVertex2d(-1.0,  1.0);
        glEnd();
    }

    void TestTextures() {
        glLoadIdentity();
        glTranslated(-5.0, 5.0, 0.0);
        glScaled(5.0, 5.0, 1.0);
        test_texture_->Render();
    }

 private:
    const bool test_mode_;
    Texture * test_texture_;
};

#endif  // GAME_RENDER_H_
