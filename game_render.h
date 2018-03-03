#ifndef GAME_RENDER_H_
#define GAME_RENDER_H_

#include <GLFW/glfw3.h>

#include <iostream>

#include "texture.h"
#include "font.h"

class GameRender {
 public:
    explicit GameRender(bool test_mode)
    : test_mode_(test_mode)
    , test_texture_(0)
    , test_font_(0) {
    }

    ~GameRender() {
        delete test_texture_;
        delete test_font_;
    }

    void Init() {
        if (test_texture_ != 0) {
            delete test_texture_;
        }

        test_texture_ = new Texture("test.png");
        test_font_ = new Font("FreeMono.ttf");
    }
    void RenderWorld() {
        if (test_mode_) {
            TestPrimitives();
            TestTextures();
        } else {
            // TODO Render game content +render +game id:17 gh:20 ic:gh
            // - Implement methods required to render game content.
        }
    }
    void RenderUi() {
        if (test_mode_) {
            TestUiFonts();
        }
        else {

        }
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

    void TestUiFonts() {
        glColor3f(1.0, 1.0, 0.0);
        glRasterPos2i(10, 10);
        test_font_->FaceSize(40);
        test_font_->Render(std::string("This is custom font!"));
    }

 private:
    const bool test_mode_;
    Texture * test_texture_;
    Font * test_font_;
};

#endif  // GAME_RENDER_H_
