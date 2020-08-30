#include "GamePlay.h"
#include "Background.h"
#include "SpaceShip.h"
#include "UniverseManager.h"
#include "ObjectManager.h"
#include "Display.h"
#include "EffectsManager.h"

#include <Box2D/Box2D.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <math.h>

GamePlay::GamePlay() {
    debug_scale_ = 1.0;
    quit_ = false;
}
GamePlay::~GamePlay() {
    delete background_;
}
void GamePlay::Exit() {
    quit_ = true;
}
void GamePlay::Init() {

    background_ = new Background();
    background_->Init();

    ship_ = std::static_pointer_cast<SpaceShip>(OBJMGR.Get("ship"));
    assert(ship_ != 0 && "ship not defined!");

    universe_mgr_ = std::static_pointer_cast<UniverseManager>(OBJMGR.Get("universe_manager"));
    assert(universe_mgr_ != 0 && "universe not defined!");
}

void GamePlay::RefreshPlayerParams() {
    ship_speed_ = ship_->GetSpeed();
    ship_angle_ = ship_->GetAngle();
    b2Vec2 p = ship_->GetPosition();
    ship_x_ = p.x;
    ship_y_ = p.y;
    speed_scale_ = 1.0 + (1.0 / (1.0 + exp(-ship_speed_+5.0)));
}
void GamePlay::Render() {

    if (quit_) return;

    RefreshPlayerParams();

    // TODO : (#151) Move ship near to lower side of screen at high velocities
    DISPLAY.WorldMode(debug_scale_ * speed_scale_);

    glPushMatrix();

    glRotated(ship_angle_, 0.0, 0.0, -1.0);
    glTranslated(-ship_x_, -ship_y_, 0.0);

    if (debug_scale_ < 2.0) {
        background_->Render(ship_x_, ship_y_, ship_angle_);
    }
    universe_mgr_->Render();

    glPopMatrix();

    // TODO : fix this.
    DISPLAY.WorldMode(debug_scale_ * speed_scale_);
    ship_->Render();
}
GameDefinitions::GameStateEnum GamePlay::KeyInput(int key, bool action) {
    GameDefinitions::GameStateEnum state = GameDefinitions::gameState_InGame;

    switch (key) {
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE:
            state = GameDefinitions::gameState_InMenu;
            break;
        case GLFW_KEY_V:
            if (action) {
                if (debug_scale_ < 2000.0) {
                    debug_scale_ *= 2.0;
                }
                else {
                    debug_scale_ = 1.0;
                }
            }
            break;

        case GLFW_KEY_M:
            if (action) {
                universe_mgr_->ToggleMap();
            }
            break;

        default:
            ship_->HotasInput(key, action);
            break;

    }

    return state;
}
