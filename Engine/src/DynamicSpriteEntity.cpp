#include "Engine/DynamicSpriteEntity.h"

namespace engine {

DynamicSpriteEntity::DynamicSpriteEntity(Settings& settings, ISpriteTextures& textures, const Config& config)
    : DynamicEntity(settings, config),
      _textures(textures),
      _spriteData(std::move(config.spriteData)) {}

void DynamicSpriteEntity::update(float deltaTime) {
    _spriteData.updateFrame();

    DynamicEntity::update(deltaTime);
}

void DynamicSpriteEntity::render(SDL_Renderer& renderer, Camera& camera) const {
    const auto type = getType();

    auto& texture = _textures.getTexture(type, _spriteData.getAnimation(), _spriteData.getFrame());
    
    const auto flip = getDirection() > 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    
    const utils::f2v texturePos(getPos().x - (getTextureSize().x - getSize().x) / 2., getPos().y - (getTextureSize().y - getSize().y));
    
    const auto textureRect = camera.worldToViewport(texturePos, getTextureSize());

    if (SDL_RenderCopyEx(&renderer, &texture, nullptr, &textureRect, 0, nullptr, flip) < 0)
        throw std::runtime_error(std::string("SDL_RenderCopyEx error: ") + SDL_GetError() + " while rendering entity of type: " + std::to_string(type));

    if (getSettings().showHitboxes()) {
        auto entityRect = camera.worldToViewport(getPos(), getSize());
        SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(&renderer, &entityRect);
    }

    if (getSettings().showTextureHitboxes()) {
        SDL_SetRenderDrawColor(&renderer, 128, 128, 128, 255);
        SDL_RenderDrawRect(&renderer, &textureRect);
    }
}

}
