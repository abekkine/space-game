#include <iostream>

#include "game.h"

int main(int argc, char * argv[]) {
    Game* game = new Game();

    try {
        game->Init(argc, argv);
        game->Run();
    }
    catch (std::string eMsg) {
        std::cerr << eMsg << '\n';
    }
    catch (std::exception & e) {
        std::cerr << "Error : " << e.what() << '\n';
    }

    delete game;

    return 0;
}
