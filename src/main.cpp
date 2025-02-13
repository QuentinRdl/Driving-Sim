#include <iostream>
#include "game.h"
#include "GraphicsUtils/segmenttype.h"

int main() {
    Game game;

    while(game.running()) {
        game.update();
        game.render();
    }

    return EXIT_SUCCESS;
}
