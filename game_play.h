#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include <GLFW/glfw3.h>

#include "game_definitions.h"
#include "display.h"
#include "texture.h"
#include "game_data.h"
#include "background.h"

class GamePlay {
private:
    GameData::Player player_;
    GameData::Planet planet_;
    Background background_;

public:
    GamePlay() {}
    ~GamePlay() {}
    void Init() {
        background_.Init();
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

        RenderVectors();
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
                    GAMEDATA.SetThrust(20.0, 0.0, 0.0);
                }
                else {
                    // disable thruster.
                    GAMEDATA.SetThrust(0.0, 0.0, 0.0);
                }
                break;
            case GLFW_KEY_A: // right thruster
                if (action == true) {
                    GAMEDATA.SetThrust(0.0, 0.0, 1.0);
                }
                else {
                    GAMEDATA.SetThrust(0.0, 0.0, 0.0);
                }
                break;
            case GLFW_KEY_D: // left thruster
                if (action == true) {
                    GAMEDATA.SetThrust(0.0, 1.0, 0.0);
                }
                else {
                    GAMEDATA.SetThrust(0.0, 0.0, 0.0);
                }
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

    void RenderVectors() {
        // Gravity
        double gx, gy;
        GAMEDATA.GetGravityDebug(gx, gy);
        // Thrust
        double tx, ty;
        GAMEDATA.GetThrust(tx, ty);
        // Velocity
        double vx, vy;
        GAMEDATA.GetVelocityDebug(vx, vy);

        glPushMatrix();

        glLoadIdentity();
        glRotated(player_.angle, 0.0, 0.0, -1.0);

        glLineWidth(2.0);
        // Thrust  : green
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES);
            glVertex2d(0.0, 0.0);
            glVertex2d(tx, ty);
        glEnd();
        // Gravity : yellow
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_LINES);
            glVertex2d(0.0, 0.0);
            glVertex2d(gx, gy);
        glEnd();
        // Velocity: blue
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
            glVertex2d(0.0, 0.0);
            glVertex2d(vx, vy);
        glEnd();

        glPopMatrix();
    }
};

#endif  // GAME_PLAY_H_
