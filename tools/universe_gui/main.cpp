#include "CommFactory.hpp"

#include <imgui.h>
#include <imgui_impl_glut.h>
#include <imgui_impl_opengl2.h>

#include <GL/glut.h>

#include <stdint.h>
#include <sys/time.h>

ImFont * font2;

#pragma pack(push, 1)
struct Params {
    int32_t octaveCount;
    int32_t frequency;
    int32_t stepSize;
    int32_t xOffset;
    int32_t yOffset;
    int32_t minValue;
    int32_t zOffset;

    Params() {
        octaveCount = 1;
        frequency = 1;
        stepSize = 1;
        xOffset = 1;
        yOffset = 1;
        minValue = 1;
        zOffset = 1;
    }
} params ;
#pragma pack(pop)

int32_t * p_OctaveCount = &params.octaveCount;
int32_t * p_Frequency = &params.frequency;
int32_t * p_StepSize = &params.stepSize;
int32_t * p_XOffset = &params.xOffset;
int32_t * p_YOffset = &params.yOffset;
int32_t * p_MinValue = &params.minValue;
int32_t * p_ZOffset = &params.zOffset;

#define SLIDER(name) ImGui::SliderInt(#name, p_##name, 0, 0xfffff);

CommInterface * sock;

void universe_gui() {
    bool close = true;

    ImGui::PushFont(font2);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(800, 800));
    ImGui::Begin("Universe GUI", &close, window_flags);

    SLIDER(OctaveCount);
    SLIDER(Frequency);
    SLIDER(StepSize);
    SLIDER(XOffset);
    SLIDER(YOffset);
    SLIDER(MinValue);
    SLIDER(ZOffset);
    
    ImGui::End();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding.x = 32.0;
    style.WindowPadding.y = 32.0;

    ImGui::PopFont();

    if (ImGui::IsKeyPressed(27)) {
        exit(0);
    }
}

timeval t0;
void display() {

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    universe_gui();

    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
    glutPostRedisplay();

    if (sock != 0)
    {
        timeval tnow;
        gettimeofday(&tnow, 0);
        int dSec = tnow.tv_sec - t0.tv_sec;
        int dUSec = tnow.tv_usec - t0.tv_usec;
        double elapsedMs = (dSec * 1000000.0 + dUSec) / 1000.0;
        // static int count = 0;
        // if (count == 0) {
        if (elapsedMs > 100.0) {
            t0 = tnow;
            sock->Write((uint8_t *)&params, sizeof(Params));
        }
        // }
        // ++count;
        // count %= 128;
    }
}

int main(int argc, char ** argv) {

    gettimeofday(&t0, 0);

    // sock = CommFactory::getUdpSocket(-1, 3201, "127.0.0.1");
    sock = CommFactory::getSharedMemory("unitest", 256);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Universe GUI");

    glutDisplayFunc(display);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFont* font1 = io.Fonts->AddFontDefault();
    font2 = io.Fonts->AddFontFromFileTTF("ubuntu.ttf", 32.0f);

    // ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

    glutMainLoop();

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
