#include "Engine/DynamicSimpleEntity.h"

namespace engine {

DynamicSimpleEntity::DynamicSimpleEntity(Settings& settings, ISimpleTextures& textures, const Config& config)
    : DynamicEntity(settings, config),
      _textures(textures),
      _type(config.type) {}

void DynamicSimpleEntity::render(SDL_Renderer& renderer, Camera& camera) const {
    auto& texture = _textures.getTexture(_type);

    const auto flip = getDirection() > 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    const auto pos = getPos();
    const auto size = getSize();
    const auto textureSize = getTextureSize();

    const utils::f2v texturePos(pos.x - (textureSize.x - size.x) / 2., pos.y - (textureSize.y - size.y));

    const auto textureRect = camera.worldToViewport(texturePos, textureSize);

    if (SDL_RenderCopyEx(&renderer, &texture, nullptr, &textureRect, 0., nullptr, flip) < 0)
        throw std::runtime_error(std::string("SDL_RenderCopyEx error: ") + SDL_GetError() + " while rendering drawable of type: " + toString(_type));

    if (getSettings().showHitboxes()) {
        const auto entityRect = camera.worldToViewport(pos, size);
        SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(&renderer, &entityRect);
    }

    if (getSettings().showTextureHitboxes()) {
        const auto textureRect = camera.worldToViewport(texturePos, textureSize);
        SDL_SetRenderDrawColor(&renderer, 128, 128, 128, 255);
        SDL_RenderDrawRect(&renderer, &textureRect);
    }
}

}
