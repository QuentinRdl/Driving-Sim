#include "game.h"
#include "vehicle.h"
int main() {
    Game game;

    while(game.running()) {
        game.update();
        game.render();
    }

    return EXIT_SUCCESS;
}
