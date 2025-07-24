#pragma once

namespace engine {

/// @brief Interface for classes that observe changes in settings.
class ISettingsObserver {
public:
    virtual ~ISettingsObserver() = default;

    /// @brief Called when settings have changed.
    virtual void onSettingsChanged() = 0;
};

}
