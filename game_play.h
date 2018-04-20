#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include <math.h>
#include <GLFW/glfw3.h>

#include "game_definitions.h"
#include "display.h"
#include "texture.h"
#include "background.h"

#include "planet.h"
#include "space_ship.h"
#include "effects_manager.h"

#include "object_manager.h"

class GamePlay {
private:
    double debug_scale_;
    int num_planets_;
    Planet* planet_;
    EffectsManager* effects_;
    Background* background_;

public:
    GamePlay() {
        debug_scale_ = 1.0;
    }
    ~GamePlay() {}
    void Init() {

        background_ = new Background();
        background_->Init();

        ship_ = static_cast<SpaceShip*>(OBJMGR.Get("ship"));
        num_planets_ = *(static_cast<int*>(OBJMGR.Get("nplanets")));
        planet_ = static_cast<Planet *>(OBJMGR.Get("planets"));
        effects_ = static_cast<EffectsManager *>(OBJMGR.Get("effects"));
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
        RenderEffects();

        glPopMatrix();

        RenderShip();
    }
    GameDefinitions::GameStateEnum KeyInput(int key, bool action) {
        GameDefinitions::GameStateEnum state = GameDefinitions::gameState_InGame;

        switch (key) {
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                state = GameDefinitions::gameState_InMenu;
                break;
            case GLFW_KEY_V:
                if (debug_scale_ > 50.0) {
                    debug_scale_ = 1.0;
                }
                else {
                    debug_scale_ = 100.0;
                }
                break;

            default:
                ship_->HotasInput(key, action);
                break;

        }

        return state;
    }

private:
    void RenderShip() {

        ship_->Render();

    }
    void RenderUniverse() {

        // [TODO] : Render method from universe class may be used instead.
        for (int i=0; i<num_planets_; ++i) {
            glPushMatrix();
            planet_[i].Render();
            glPopMatrix();
        }
    }
    void RenderEffects() {

        effects_->Render();
    }

    void RenderBackground() {
        background_->Render(ship_x_, ship_y_, ship_angle_);
    }
};

#endif  // GAME_PLAY_H_
