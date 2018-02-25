#include <iostream>

#include "game.h"

int main(int argc, char * argv[]) {

    Game* game = new Game();

    try {

        game->Init(argc, argv);
        game->Run();
    }
    catch (std::string & errorMessage) {
        std::cerr << "Game Error : " << errorMessage << '\n';
    }
    catch (std::exception & e) {
        std::cerr << "System Error : " << e.what() << '\n';
    }

    return 0;
}
