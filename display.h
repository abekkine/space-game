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
    , key_cb_([](int k, bool a) { (void)k; (void)a; })
    , vp_left_(-1.0)
    , vp_right_(1.0)
    , vp_top_(1.0)
    , vp_bottom_(-1.0)
    , vp_center_x_(0.0)
    , vp_center_y_(0.0)
    , vp_left_offset_(0.0)
    , vp_right_offset_(0.0)
    , vp_top_offset_(0.0)
    , vp_bottom_offset_(0.0)
    , width_(0)
    , height_(0)
    {}
    ~Display() {}
    void Init() {
        if (!glfwInit()) {
            std::string message = std::string("Unable to initialize GLFW.");
            throw GameException(GameException::eGLFWError, message);
        }
        glfwWindowHint(GLFW_SAMPLES, 8);

        width_ = CONFIG.GetParam<int>({"display", "width"}, 1024);
        height_ = CONFIG.GetParam<int>({"display", "height"}, 576);
        // BUG : There are issues with full screen mode; desktop resolution does not go back to normal.
        bool fullscreen = CONFIG.GetParam<bool>({"display", "fullscreen"}, false);
        size_ = CONFIG.GetParam<double>({"world", "size"}, 100.0);
        AdjustViewportOffsets();
        SetupViewport();

        if (fullscreen) {
            window_ = glfwCreateWindow(width_, height_, "Space Game", glfwGetPrimaryMonitor(), NULL);
        } else {
            window_ = glfwCreateWindow(width_, height_, "Space Game", NULL, NULL);
        }

        if (!window_) {
            Quit();
            std::string message = std::string("Unable to create window.");
            throw GameException(GameException::eGLFWError, message);
        }

        int window_x, window_y;
        window_x = CONFIG.GetParam<int>({"window", "x"}, 10);
        window_y = CONFIG.GetParam<int>({"window", "y"}, 10);
        glfwSetWindowPos(window_, window_x, window_y);

        glfwMakeContextCurrent(window_);

        glfwSetKeyCallback(window_, key_callback);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        bool enable_aa;
        enable_aa = CONFIG.GetParam<bool>({"display", "antialias"}, false);
        if (enable_aa) {
            glEnable(GL_MULTISAMPLE);
        } else {
            glDisable(GL_MULTISAMPLE);
        }
    }
    void GetSize(int & width, int & height) {
        width = width_;
        height = height_;
    }
    void AdjustViewportOffsets() {
        if (width_ > height_) {
            vp_left_offset_ = -0.5 * size_;
            vp_right_offset_ = 0.5 * size_;
            vp_top_offset_ = 0.5 * size_ * height_ / width_;
            vp_bottom_offset_ = -0.5 * size_ * height_ / width_; 
        } else {
            vp_left_offset_ = -0.5 * size_ * width_ / height_;
            vp_right_offset_ = 0.5 * size_ * width_ / height_;
            vp_top_offset_ = 0.5 * size_;
            vp_bottom_offset_ = -0.5 * size_;
        }
    }
    void SetupViewport() {
        vp_left_ = vp_center_x_ + vp_left_offset_;
        vp_right_ = vp_center_x_ + vp_right_offset_;
        vp_top_ = vp_center_y_ + vp_top_offset_;
        vp_bottom_ = vp_center_y_ + vp_bottom_offset_;
    }
    bool QuitRequested() {
        bool quit_request;

        quit_request = glfwWindowShouldClose(window_);

        return quit_request;
    }
    void UiMode() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, width_, height_, 0.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
    }
    void WorldMode(double speed_factor) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(
            speed_factor * vp_left_,
            speed_factor * vp_right_,
            speed_factor * vp_bottom_,
            speed_factor * vp_top_,
            -1.0, 1.0);
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
    void RegisterKeyCallback(std::function<void(int, bool)> cb) {
        key_cb_ = cb;
    }
    void KeyCallback(int key, bool action) {
        key_cb_(key, action);
    }

 private:
    GLFWwindow * window_;
    std::function<void(int, bool)> key_cb_;
    double vp_left_;
    double vp_right_;
    double vp_top_;
    double vp_bottom_;
    double vp_center_x_;
    double vp_center_y_;
    double vp_left_offset_;
    double vp_right_offset_;
    double vp_top_offset_;
    double vp_bottom_offset_;
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
        DISPLAY.KeyCallback(key, true);
    }
    else if (GLFW_RELEASE == action) {
        DISPLAY.KeyCallback(key, false);
    }
}

#endif  // DISPLAY_H_
