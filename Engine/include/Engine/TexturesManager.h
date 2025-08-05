#pragma once

#include <vector>
#include <string>

#include <Utils/SDLUtils.h>
#include <Utils/i2v.h>

namespace engine {

/// @brief Provides utility functions for loading SDL textures from files.
class TexturesManager {
public:
    TexturesManager() = delete;

    /// @brief Load a single texture from a file
    /// @param renderer SDL renderer used to create the texture
    /// @param fileName Path to the image file
    /// @return Unique pointer to the loaded texture
    static utils::SDLUtils::TexturePtr loadFromFile(SDL_Renderer&, const std::string& fileName);

    /// @brief Load a clipped part of a texture from a file
    /// @param renderer SDL renderer used to create the texture
    /// @param fileName Path to the image file
    /// @param pos Position of the clipping rectangle (x, y)
    /// @param size Size of the clipping rectangle (width, height)
    /// @return Unique pointer to the clipped texture
    static utils::SDLUtils::TexturePtr loadClippedFromFile(SDL_Renderer&,
        const std::string& fileName,
        const utils::i2v& pos,
        const utils::i2v& size);

    /// @brief Load a sequence of textures matching a filename pattern
    /// @param renderer SDL renderer used to create textures
    /// @param pattern Base pattern for filenames, e.g. "sprite_"
    /// @param count Number of textures to load
    /// @param extension File extension, default is ".png"
    /// @return Vector of unique pointers to loaded textures
    static std::vector<utils::SDLUtils::TexturePtr> loadSequenceFromPattern(
        SDL_Renderer&,
        const std::string& pattern,
        const size_t count,
        std::string extension = ".png");
};

}
