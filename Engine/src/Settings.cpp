#include "Engine/Settings.h"

namespace engine {

void Settings::unregisterObserver(ISettingsObserver& observer) {
    _observers.erase(
        std::remove(_observers.begin(), _observers.end(), &observer),
        _observers.end()
    );
}

void Settings::notifyObservers() {
    for (auto* observer : _observers) {
        if(!observer)
            throw std::logic_error("Trying to acccess observer which does not exist");

        observer->onSettingsChanged();
    }
}

}
