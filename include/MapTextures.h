#pragma once

#include <SDL2/SDL.h>
#include <vector>

class MapTextures {
public:
    MapTextures(const MapTextures&) = delete;
    ~MapTextures();

    MapTextures& operator=(const MapTextures&) = delete;

    static MapTextures& getInstance() { static MapTextures instance; return instance; }

    void init(SDL_Renderer* renderer);

    typedef enum TextureType { 
        GROUND,
        BUILDING
    } TextureType;

    SDL_Texture* getTexture(TextureType type, int k);
private:
    MapTextures() : _renderer(nullptr), _initialized(false) {}

    SDL_Renderer* _renderer;

    std::vector<SDL_Texture*> _groundTextures;
    std::vector<SDL_Texture*> _villageBuildingTextures;

    bool _initialized;

    void loadTextures();
};