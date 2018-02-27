#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <functional>

#include "singleton.h"

static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

class Display : public Singleton<Display> {
public:
    explicit Display(token)
    : window_(0)
    , key_cb_([](int k) { (void)k; })
    {}
    ~Display() {}
    void Init() {
        if (! glfwInit()) {
            throw std::string("Unable to initialize GLFW.");
        }

        window_ = glfwCreateWindow(1024, 576, "Testing", NULL, NULL);
        if (!window_) {
            Quit();
            throw std::string("Unable to create window.");
        }

        glfwSetWindowPos(window_, 10, 10);

        glfwMakeContextCurrent(window_);

        glfwSetKeyCallback(window_, key_callback);
    }
    bool QuitRequested() {
        bool quit_request;

        quit_request = glfwWindowShouldClose(window_);

        return quit_request;
    }
    void PreRender() {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    void PostRender() {
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
    void RequestQuit() {
        std::cout << "Display::RequestQuit()\n";
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
};

#define DISPLAY Display::Instance()

static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    (void)window;
    (void)scancode;
    (void)action;
    (void)mods;
    DISPLAY.KeyCallback(key);
}

#endif  // DISPLAY_H_
