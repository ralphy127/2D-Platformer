#pragma once

#include "Engine/Drawable.h"
#include "Engine/IUpdatable.h"

namespace engine {

// to be implemented
class StaticEntity : public Drawable {
    struct Config : Drawable::Config {
        
    };

    StaticEntity(Settings&, const Config&);
};

}
