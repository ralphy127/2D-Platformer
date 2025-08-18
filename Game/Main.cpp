#include <Utils/Logging.h>
#include "Game/Game.h"

int main() 
try {
    SDL_LogSetOutputFunction(utils::consoleSDLLogger, nullptr);
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

    game::Game game;
    game.run();

    return EXIT_SUCCESS;
}
catch (const std::exception& e) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s", e.what());

    return EXIT_FAILURE;
}