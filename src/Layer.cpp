#include "Layer.h"
#include "TextureManager.h"
#include "MapTextures.h"

#include <iostream>
#include <sstream>
#include <fstream>

Layer::Layer(SDL_Renderer* renderer, Type type, unsigned int level, int parallaxVel) : _type(type), _level(level), _parallaxVel(parallaxVel) {
    std::string fileName;

    switch(_type) {
        case BACKGROUND:
            fileName = "background";
            break;
        case FAR:
            fileName  = "far";
            break;
        case NEAR:
            fileName = "near";
            break;
        default:
            std::cerr << "Error: wrong type of non-grid layer." << std::endl;
            return;
    }

    std::stringstream ss;
    ss << "../assets/levels/level_" << (_level < 10 ? "0" : "") << (_level < 100 ? "0" : "") << _level << "/textures/" << fileName << ".png";

    std::cout << ss.str() << std::endl;
    _texture = TextureManager::loadTexture(renderer, ss.str());
    if(!_texture) {
        std::cerr << "Error: failed to initialize layer texture." << std::endl;
    }
}

Layer::Layer(Type type, unsigned int level, unsigned int tileSize, i2v& windowSize) : _type(type), _level(level), _tileSize(tileSize), _windowSize(windowSize), _texture(nullptr) {
    std::string fileName;

    switch(_type) {
        case DECORATION:
            fileName = "decoration";
            break;
        case INTERACTIVE:
            fileName = "interactive";
            break;
        case MAP:
            fileName = "map";
            break;
        default:
            std::cerr << "Error: wrong type of grid layer." << std::endl;
            return;
    }

    std::stringstream ss;
    ss << "../assets/levels/level_" << (_level < 10 ? "0" : "") << (_level < 100 ? "0" : "") << _level << "/" << fileName << ".txt";

    std::ifstream file(ss.str());

    if(!file.is_open()) {
        std::cerr << "Error opening grid file: " << ss.str() << ',';

        if(file.good()) {
            std::cerr << " file exists but could not be opened." << std::endl;
        } else {
            std::cerr << " file does not exist." << std::endl;
        }

        return;
    }

    std::string line;
    while(getline(file, line)) {
        std::vector<int> row;
        for(int i = 0; i < line.size(); i += 3) {
            std::string tileStr = line.substr(i, 3);
            int tile = stoi(tileStr);
            row.push_back(tile);
        }

        _grid.push_back(row);
    }

    file.close();
}

Layer::~Layer() {
    if(_texture) {
        SDL_DestroyTexture(_texture);
    }
}

void Layer::render(SDL_Renderer* renderer, Camera& camera) {
    switch(_type) {
        case BACKGROUND:
        case FAR:
        case NEAR:
            renderNonGrid(renderer, camera);
            break;
        case DECORATION:
        case INTERACTIVE:
        case MAP:
            renderGrid(renderer, camera);
            break;
        default:
            std::cerr << "Error: wrong type of layer to render." << std::endl;
            break;
    }
}

void Layer::renderNonGrid(SDL_Renderer* renderer, Camera& camera) {
    if(!_texture) {
        return;
    }

    SDL_Rect rect = { 0, 0, 3000, 5000 };

    // TODO: to remove
    if(_type == BACKGROUND) _texture = TextureManager::loadTexture(renderer, "../assets/levels/level_001/textures/background.png");
    if(_type == FAR) _texture = TextureManager::loadTexture(renderer, "../assets/levels/level_001/textures/far.png");

    if(SDL_RenderCopy(renderer, _texture, nullptr, nullptr) < 0) {
        std::cerr << "SDL_RenderCopy Error: " << SDL_GetError() << '.' << std::endl;
    }
}

void Layer::renderGrid(SDL_Renderer* renderer, Camera& camera) {
    int x = 0; 
    int y = 0;

    if(_grid.size() == 0) {
        return;
    }

    for(const std::vector<int>& row : _grid) {
        for(const int& tile : row) {
            MapTextures::TextureType tileType;
            int k = 0;
            if(tile > 0 && tile < 14) {
                k = tile;
                tileType = MapTextures::GROUND;
            } else if(tile > 100 && tile < 162) {
                k = tile;
                tileType = MapTextures::BUILDING;
            }
            else {
                x += _tileSize;
                continue;
            }

            SDL_Texture* tileTexture = MapTextures::getInstance().getTexture(tileType, k);
            if(!tileTexture) {
                std::cerr << "Error: cannot read tile texture for tile type " << tileType << " and index " << k << '.' <<std::endl;
                return;
            }

            f2v pos(x, y);
            f2v size(_tileSize, _tileSize);
            SDL_Rect tileRect = camera.apply(pos, size);
            if (tileRect.x + tileRect.w < 0 || tileRect.x > _windowSize.x ||
                tileRect.y + tileRect.h < 0 || tileRect.y > _windowSize.y) {
                x += _tileSize;
                continue;
            }
            
            SDL_RenderCopy(renderer, tileTexture, nullptr, &tileRect);

            x += _tileSize;
        }

        x = 0;
        y += _tileSize;
    }
}