#include "game.h"
#include "vehicle.h"
int main() {
    /*
    Vehicle v;
    v.plotTest();
    v.plotTestIterative();
    */

    Game game;

    while(game.running()) {
        game.update();
        game.render();
    }


    return EXIT_SUCCESS;
}
