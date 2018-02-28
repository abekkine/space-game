#include <iostream>

#include "game.h"

int main(int argc, char * argv[]) {

    Game* game = new Game();

    try {

        game->Init(argc, argv);
        game->Run();
    }
    catch (std::exception & e) {
        std::cerr << "Error : " << e.what() << '\n';
    }

    return 0;
}
