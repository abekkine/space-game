#ifndef UI_RENDER_H_
#define UI_RENDER_H_

#include <GLFW/glfw3.h>

#include <string>

#include "font.h"

// TODO Implement UiRender class +ui +game +menu +text +font id:16 gh:18 ic:gh
// - Handles renderables related to user interface.
class UiRender {
 public:
    explicit UiRender(bool test_mode)
    : test_mode_(test_mode)
    , test_font_(0) {
    }
    ~UiRender() {
        delete test_font_;
    }

    void Init() {
        if (test_font_ != 0) {
            delete test_font_;
            test_font_ = 0;
        }
        test_font_ = new Font("FreeMono.ttf");
    }

    void Render() {
        if (test_mode_) {
            TestUiFonts();
        } else {
            // TODO Implement main function of UI rendering +ui +menu id:23 gh:30 ic:gh
        }
    }

    void TestUiFonts() {
        glColor3f(1.0, 1.0, 0.0);
        glRasterPos2i(50, 50);
        test_font_->FaceSize(40);
        test_font_->Render(std::string("This is custom font!"));
    }

 private:
    bool test_mode_;
    Font * test_font_;
};

#endif  // UI_RENDER_H_
