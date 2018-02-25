#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <GLFW/glfw3.h>

#include <string>

class Display {
public:
    Display()
    : window_(0)
    {}
    ~Display() {}
    void Init() {
        if (! glfwInit()) {
            throw std::string("Unable to initialize GLFW.");
        }

        window_ = glfwCreateWindow(200, 200, "Testing", NULL, NULL);
        if (!window_) {
            Quit();
            throw std::string("Unable to create window.");
        }

        glfwMakeContextCurrent(window_);
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
    void Quit() {
        glfwTerminate();
    }
private:
    GLFWwindow * window_;
};

#endif  // DISPLAY_H_
