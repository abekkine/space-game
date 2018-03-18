#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <functional>

#include "config.h"
#include "game_exception.h"
#include "singleton.h"

static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

class Display : public Singleton<Display> {
 public:
    explicit Display(token)
    : window_(0)
    , key_cb_([](int k) { (void)k; })
    , vp_left_(-1.0)
    , vp_right_(1.0)
    , vp_top_(1.0)
    , vp_bottom_(-1.0)
    , vp_center_x_(0.0)
    , vp_center_y_(0.0)
    , width_(0)
    , height_(0)
    {}
    ~Display() {}
    void Init() {
        if (!glfwInit()) {
            std::string message = std::string("Unable to initialize GLFW.");
            throw GameException(GameException::eGLFWError, message);
        }

        width_ = CONFIG.GetParam<int>({"display", "width"}, 1024);
        height_ = CONFIG.GetParam<int>({"display", "height"}, 576);
        bool fullscreen = CONFIG.GetParam<bool>({"display", "fullscreen"}, false);
        size_ = CONFIG.GetParam<double>({"world", "size"}, 100.0);
        SetupViewport();

        if (fullscreen) {
            window_ = glfwCreateWindow(width_, height_, "Testing", glfwGetPrimaryMonitor(), NULL);
        } else {
            window_ = glfwCreateWindow(width_, height_, "Testing", NULL, NULL);
        }

        if (!window_) {
            Quit();
            std::string message = std::string("Unable to create window.");
            throw GameException(GameException::eGLFWError, message);
        }

        glfwSetWindowPos(window_, 10, 10);

        glfwMakeContextCurrent(window_);

        glfwSetKeyCallback(window_, key_callback);
    }
    void SetupViewport() {
        vp_left_ = -0.5 * size_ + vp_center_x_;
        vp_right_ = 0.5 * size_ + vp_center_x_;
        vp_top_ = vp_center_y_ + 0.5 * height_ * size_ / static_cast<double>(width_);
        vp_bottom_ = vp_center_y_ - 0.5 * height_ * size_ / static_cast<double>(width_);
    }
    bool QuitRequested() {
        bool quit_request;

        quit_request = glfwWindowShouldClose(window_);

        return quit_request;
    }
    void CenterAt(double x, double y) {
        vp_center_x_ = x;
        vp_center_y_ = y;
        SetupViewport();
    }
    void UiMode() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, width_, height_, 0.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
    }
    void WorldMode() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(vp_left_, vp_right_, vp_bottom_, vp_top_, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
    }
    void PreRender() {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    void PostRender() {
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
    void RequestQuit() {
        glfwSetWindowShouldClose(window_, GLFW_TRUE);
    }
    void Quit() {
        glfwTerminate();
    }
    void RegisterKeyCallback(std::function<void(int)> cb) {
        key_cb_ = cb;
    }
    void KeyCallback(int key) {
        key_cb_(key);
    }

 private:
    GLFWwindow * window_;
    std::function<void(int)> key_cb_;
    double vp_left_;
    double vp_right_;
    double vp_top_;
    double vp_bottom_;
    double vp_center_x_;
    double vp_center_y_;
    int width_;
    int height_;
    double size_;
};

#define DISPLAY Display::Instance()

static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    (void)window;
    (void)scancode;
    (void)mods;

    if (GLFW_PRESS == action) {
        DISPLAY.KeyCallback(key);
    }
}

#endif  // DISPLAY_H_
