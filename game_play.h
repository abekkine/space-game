#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include <GLFW/glfw3.h>

#include "game_definitions.h"
#include "display.h"
#include "texture.h"
#include "game_data.h"
#include "background.h"

#include "data_bus.h"
#include "devices/generic_hud_device.h"
#include "devices/hotas_device.h"
#include "systems/ship_systems_manager.h"

class GamePlay {
private:
    GameData::Player player_;
    GameData::Planet planet_;
    Background background_;
    GenericHudDevice hud_;
    HOTASDevice hotas_;

public:
    GamePlay() {}
    ~GamePlay() {}
    void Init() {
        // Create ship systems
        SYSTEMSMGR.createEngineSystem();

        background_.Init();
        hud_.Init();
        hotas_.Init();
    }
    void Render() {
        GAMEDATA.GetPlayer(&player_);
        GAMEDATA.GetPlanet(&planet_);

        DISPLAY.WorldMode();

        RenderPlayer();

        glPushMatrix();

        glRotated(player_.angle, 0.0, 0.0, -1.0);
        glTranslated(-player_.x, -player_.y, 0.0);

        RenderBackground();
        RenderUniverse();

        glPopMatrix();

        RenderHUD();
    }
    GameDefinitions::GameStateEnum KeyInput(int key, bool action) {
        GameDefinitions::GameStateEnum state = GameDefinitions::gameState_InGame;

        switch (key) {
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                state = GameDefinitions::gameState_InMenu;
                break;
            case GLFW_KEY_W: // main thruster
                if (action == true) {
                    // enable thruster.
                    hotas_.SetThrottle(1.0);
                }
                else {
                    // disable thruster.
                    hotas_.SetThrottle(0.0);
                }
                break;
            case GLFW_KEY_A: // right thruster
                if (action == true) {
                    hotas_.SetSteering(-1.0);
                }
                else {
                    hotas_.SetSteering(0.0);
                }
                break;
            case GLFW_KEY_D: // left thruster
                if (action == true) {
                    hotas_.SetSteering(1.0);
                }
                else {
                    hotas_.SetSteering(0.0);
                }
                break;
            case GLFW_KEY_G:
                hotas_.ToggleLandingGear();
                break;
        }

        return state;
    }

private:
    void RenderPlayer() {

        glLoadIdentity();
        // Color
        glColor3fv(player_.c);

        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(0.0, 0.0);
        for (int i=0; i<player_.n; ++i)
        glVertex2d(player_.vertices[i][0], player_.vertices[i][1]);
        glVertex2d(player_.vertices[0][0], player_.vertices[0][1]);
        glEnd();
    }
    void RenderUniverse() {

        glTranslated(planet_.x, planet_.y, 0.0);
        glRotated(planet_.angle, 0.0, 0.0, 1.0);
        glColor3fv(planet_.c);

        const double R = planet_.radius;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(0.0, 0.0);
        for (double a=0.0; a < 2.0 * M_PI; a+=0.05) {
            glVertex2d(R * cos(a), R * sin(a));
        }
        glVertex2d(R, 0.0);
        glEnd();

        glPointSize(3.0);
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POINTS);
        glVertex2d(0.0, R*0.99);
        glEnd();
    }

    void RenderBackground() {
        background_.Render(player_);
    }

    void RenderHUD() {
        hud_.Render();
    }
};

#endif  // GAME_PLAY_H_
