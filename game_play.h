#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include <math.h>
#include <GLFW/glfw3.h>

#include "game_definitions.h"
#include "display.h"
#include "texture.h"
#include "background.h"

#include "universe.h"
#include "space_ship.h"
#include "effects_manager.h"
#include "object_manager.h"

class GamePlay {
private:
    double debug_scale_;
    EffectsManager* effects_;
    Background* background_;
    bool quit_;

public:
    GamePlay() {
        debug_scale_ = 1.0;
        quit_ = false;
    }
    ~GamePlay() {
        delete background_;
    }
    void Exit() {
        quit_ = true;
    }
    void Init() {

        background_ = new Background();
        background_->Init();

        ship_ = static_cast<SpaceShip*>(OBJMGR.Get("ship"));
        assert(ship_ != 0 && "ship not defined!");

        universe_ = static_cast<Universe*>(OBJMGR.Get("universe"));
        assert(universe_ != 0 && "universe not defined!");

        effects_ = static_cast<EffectsManager *>(OBJMGR.Get("effects"));
        assert(effects_ != 0 && "effects not defined!");
    }

    Universe* universe_;
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

        if (quit_) return;

        RefreshPlayerParams();

        DISPLAY.WorldMode(debug_scale_ * speed_scale_);

        glPushMatrix();

        glRotated(ship_angle_, 0.0, 0.0, -1.0);
        glTranslated(-ship_x_, -ship_y_, 0.0);

        background_->Render(ship_x_, ship_y_, ship_angle_);
        universe_->Render();
        effects_->Render();

        glPopMatrix();

        ship_->Render();
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
};

#endif  // GAME_PLAY_H_
