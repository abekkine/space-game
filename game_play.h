#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include <GLFW/glfw3.h>

#include "game_definitions.h"
#include "display.h"
#include "texture.h"
#include "game_data.h"
#include "background.h"

// [TODO] HUD /w DataBus
#include "data_bus.h"
// [END]

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

// [TODO] HUD /w DataBus
        vx = vy = tx = ty = gx = gy = 0.0;
        using std::placeholders::_1;
        DATABUS.Subscribe(db_PlayerGravity,
            std::bind(&GamePlay::debugGravity, this, _1));
        DATABUS.Subscribe(db_PlayerThrust,
            std::bind(&GamePlay::debugThrust, this, _1));
        DATABUS.Subscribe(db_PlayerVelocity,
            std::bind(&GamePlay::debugVelocity, this, _1));
    }
    double gx, gy;
    void debugGravity(BusDataInterface * data) {
        BD_Vector *v = static_cast<BD_Vector *>(data);
        if (v != 0) {
            gx = v->x;
            gy = v->y;
        }
    }
    double tx, ty;
    void debugThrust(BusDataInterface * data) {
        BD_Vector *v = static_cast<BD_Vector *>(data);
        if (v != 0) {
            tx = v->x;
            ty = v->y;
        }
    }
    double vx, vy;
    void debugVelocity(BusDataInterface * data) {
        BD_Vector *v = static_cast<BD_Vector *>(data);
        if (v != 0) {
            vx = v->x;
            vy = v->y;
        }
    }
// [END]
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

    void RenderHUD() {
        // [TODO]
    }

    void RenderVectors() {
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
