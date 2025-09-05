#include "Engine/DynamicSimpleEntity.h"

#include "Engine/Colors.h"

namespace engine {

DynamicSimpleEntity::DynamicSimpleEntity(
    Settings& settings,
    ISimpleTextures& textures,
    const Config& config)
    : DynamicEntity(settings, config),
      _textures(textures) {}

void DynamicSimpleEntity::render(SDL_Renderer& renderer, Camera& camera) const {
    const auto type = getType();

    auto& texture = _textures.getTexture(type);

    const auto flip = getDirection() == Direction::Right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    const auto pos = getPos();

    const auto size = getSize();

    const auto textureSize = getTextureSize();

    const utils::f2v texturePos(
        pos.x - (textureSize.x - size.x) / 2.,
        pos.y - (textureSize.y - size.y));
    
    const auto textureRect = camera.worldToViewport(texturePos, textureSize);

    if (SDL_RenderCopyEx(&renderer, &texture, nullptr, &textureRect, 0., nullptr, flip) < 0)
        throw std::runtime_error(std::string("SDL_RenderCopyEx error: ") + SDL_GetError()
                                 + " while rendering entity of type: " + std::to_string(type));

    if (getSettings().showHitboxes())
        renderEntityHitbox(renderer, camera);

    if (getSettings().showTextureHitboxes())
        renderTextureHitbox(renderer, camera, textureRect);
}

}
