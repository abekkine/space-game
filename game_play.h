#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include <GLFW/glfw3.h>

#include "game_definitions.h"
#include "display.h"
#include "texture.h"
#include "game_data.h"

class GamePlay {
 public:
     GamePlay() {}
     ~GamePlay() {}
     void Init() {}
     void Render() {
         GameData::Player plr;
         GAMEDATA.GetPlayer(&plr);
         DISPLAY.CenterAt(plr.x, plr.y);

         DISPLAY.WorldMode();

         glPushMatrix();

         RenderUniverse();
         RenderPlayer();

         glPopMatrix();
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
        GameData::Player plr;
        GAMEDATA.GetPlayer(&plr);

        glLoadIdentity();
        // Position
        glTranslated(plr.x, plr.y, 0.0);
        // Rotation
        glRotated(plr.a, 0.0, 0.0, 1.0);
        // Color
        glColor3fv(plr.c);

        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(0.0, 0.0);
        for (int i=0; i<plr.n; ++i)
        glVertex2d(plr.vertices[i][0], plr.vertices[i][1]);
        glVertex2d(plr.vertices[0][0], plr.vertices[0][1]);
        glEnd();
    }
    void RenderUniverse() {
        GameData::Planet p;
        GAMEDATA.GetPlanet(&p);

        glLoadIdentity();
        glTranslated(p.x, p.y, 0.0);
        glRotated(p.a, 0.0, 0.0, 1.0);
        glColor3fv(p.c);

        const double R = p.r;
        glBegin(GL_TRIANGLE_FAN);
        for (double a=0.0; a < 2.0 * M_PI; a+=0.05) {
            glVertex2d(R * cos(a), R * sin(a));
        }
        glEnd();
        glPointSize(3.0);
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POINTS);
        glVertex2d(0.0, R*0.99);
        glEnd();
    }
};

#endif  // GAME_PLAY_H_
