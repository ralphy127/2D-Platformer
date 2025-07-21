#pragma once

namespace engine {

class ISettingsObserver {
public:
    virtual ~ISettingsObserver() = default;
    
    virtual void onSettingsChanged() = 0;
};

}
