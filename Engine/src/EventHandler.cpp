#include "Engine/EventHandler.h"

namespace engine {

void EventHandler::pollEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                _quitRequested = true; 
                break;
            case SDL_KEYDOWN:
                _keyStates[event.key.keysym.sym] = true;
                break;
            case SDL_KEYUP:
                _keyStates[event.key.keysym.sym] = false;
                break;
        }
    }
}

bool EventHandler::isKeyPressed(SDL_Keycode key) const {
    const auto it = _keyStates.find(key);
    return it != _keyStates.end() && it->second;
}

void EventHandler::resetKeyStates() {
    for (auto& [key, state] : _keyStates)
        state = false;
}

}
