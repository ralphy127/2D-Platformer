#include "Game.h"

int main() {
    Game game;
    
    while(game.isRunning()) {
        game.handleEvents();
        game.update();
        game.render();
    }
}