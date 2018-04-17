#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include <math.h>
#include <GLFW/glfw3.h>

#include "game_definitions.h"
#include "display.h"
#include "texture.h"
#include "background.h"

#include "data_bus.h"
#include "devices/generic_hud_device.h"
#include "devices/hotas_device.h"
#include "systems/ship_systems_manager.h"

#include "planet.h"
#include "space_ship.h"

#include "object_manager.h"

class GamePlay {
private:
    double debug_scale_;
    int num_planets_;
    Planet* planet_;
    Background background_;
    GenericHudDevice hud_;
    HOTASDevice hotas_;

public:
    GamePlay() {
        debug_scale_ = 1.0;
    }
    ~GamePlay() {}
    void Init() {

        background_.Init();
        hud_.Init();
        hotas_.Init();

        ship_ = static_cast<SpaceShip*>(OBJMGR.Get("ship"));
        num_planets_ = *(static_cast<int*>(OBJMGR.Get("nplanets")));
        planet_ = static_cast<Planet *>(OBJMGR.Get("planets"));
    }

    SpaceShip* ship_;
    double ship_angle_;
    double ship_x_;
    double ship_y_;
    double ship_speed_;
    double speed_scale_;
    void RefreshPlayerParams() {
        ship_speed_ = ship_->GetSpeed();
        ship_angle_ = ship_->GetAngle();
        b2Vec2 p = ship_->GetPosition();
        ship_x_ = p.x;
        ship_y_ = p.y;
        speed_scale_ = 1.0 + (1.0 / (1.0 + exp(-ship_speed_+5.0)));
    }
    void Render() {

        RefreshPlayerParams();

        DISPLAY.WorldMode(debug_scale_ * speed_scale_);

        glPushMatrix();

        glRotated(ship_angle_, 0.0, 0.0, -1.0);
        glTranslated(-ship_x_, -ship_y_, 0.0);

        RenderBackground();
        RenderUniverse();

        glPopMatrix();

        RenderPlayer();

        DISPLAY.UiMode();
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
            case GLFW_KEY_V:
                if (debug_scale_ > 50.0) {
                    debug_scale_ = 1.0;
                }
                else {
                    debug_scale_ = 100.0;
                }
                break;
        }

        return state;
    }

private:
    void RenderPlayer() {

        ship_->Render();

    }
    void RenderUniverse() {

        for (int i=0; i<num_planets_; ++i) {
            glPushMatrix();
            planet_[i].Render();
            glPopMatrix();
        }
    }

    void RenderBackground() {
        background_.Render(ship_x_, ship_y_, ship_angle_);
    }

    void RenderHUD() {
        hud_.Render();
    }
};

#endif  // GAME_PLAY_H_
