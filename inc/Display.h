#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "Singleton.h"

#include <GLFW/glfw3.h>

#include <functional>

class Display : public Singleton<Display> {
public:
    explicit Display(token);
    ~Display();
    void Init();
    void GetSize(int & width, int & height);
    void AdjustViewportOffsets();
    void SetupViewport();
    bool QuitRequested();
    void UiMode();
    void WorldMode(double speed_factor);
    void PreRender();
    void PostRender();
    void RequestQuit();
    void Quit();
    void RegisterKeyCallback(std::function<void(int, bool)> cb);
    void KeyCallback(int key, bool action);

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

#endif  // DISPLAY_H_
