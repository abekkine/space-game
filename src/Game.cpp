#include "Game.h"
#include "Display.h"
#include "GamePlay.h"
#include "Menu.h"
#include "UniverseManager.h"
#include "ObjectManager.h"
#include "GameTimer.h"

Game::Game()
: render_function_(0)
, state_(GameDefinitions::gameState_InMenu)
, universe_mgr_(0)
, menu_(0)
, game_play_(0)
, render_states_{0}
, key_processors_{0}
{}
Game::~Game() {

    OBJMGR.Remove("universe");

    delete game_play_;
    delete menu_;
    delete universe_mgr_;
}
void Game::Init(int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    DISPLAY.Init();

    // Start universe thread.
    universe_mgr_ = new UniverseManager();
    universe_mgr_->Init();
    OBJMGR.Set("universe", universe_mgr_);

    menu_ = new Menu();
    menu_->Init();

    game_play_ = new GamePlay();
    game_play_->Init();

    {
        using std::placeholders::_1;
        using std::placeholders::_2;
        key_processors_[GameDefinitions::gameState_InMenu] = std::bind(&Menu::KeyInput, menu_, _1, _2);
        key_processors_[GameDefinitions::gameState_InGame] = std::bind(&GamePlay::KeyInput, game_play_, _1, _2);
        render_states_[GameDefinitions::gameState_InMenu] = std::bind(&Menu::Render, menu_);
        render_states_[GameDefinitions::gameState_InGame] = std::bind(&GamePlay::Render, game_play_);
        DISPLAY.RegisterKeyCallback(std::bind(&Game::ProcessKey, this, _1, _2));
    }
}

void Game::Run() {

    universe_mgr_->Run();

    while (!DISPLAY.QuitRequested()) {

        universe_mgr_->SetState(state_);

        DISPLAY.PreRender();

        render_states_[state_]();

        DISPLAY.PostRender();

        GameTimer::Sleep(10);
    }

    universe_mgr_->Exit();
    game_play_->Exit();
    menu_->Exit();

    DISPLAY.Quit();
}

void Game::ProcessKey(int key, bool action) {
    state_ = key_processors_[state_](key, action);
}
