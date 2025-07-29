#pragma once

#include "Engine/DynamicEntity.h"
#include "Engine/ISpriteTextures.h"
#include "Engine/SpriteData.h"

namespace engine {

using AttackId = int;

struct AttackData {
    AttackId id;
    float damage;
    std::chrono::steady_clock::duration duration;
    utils::f2v offsetRight;
    utils::f2v offsetLeft;
    utils::f2v size;
    size_t animationId;
    std::function<void()> onHit;
    std::optional<std::string> sourceTag;
};

/// @brief Represents a dynamic entity that uses sprite-based animation for rendering.
class DynamicSpriteEntity : public DynamicEntity {
public:
    using ClockType = std::chrono::steady_clock;

    /// @brief Configuration structure for DynamicSpriteEntity.
    /// Extends DynamicEntity::Config with sprite data.
    struct Config : DynamicEntity::Config {
        SpriteData spriteData; ///< Initial sprite animation data.
    };

    /// @brief Constructs a DynamicSpriteEntity with sprite textures and animation data.
    /// @param settings Reference to engine settings.
    /// @param textures Sprite texture manager.
    /// @param config Configuration containing position, size, health, speed and sprite info.
    DynamicSpriteEntity(Settings&, ISpriteTextures&, const Config&);

    /// @brief Updates the entity state, including sprite animation.
    /// @param deltaTime Time elapsed since the last frame (in seconds).
    void update(float deltaTime) override;

    /// @brief Renders the entity using the provided renderer and camera.
    /// @param renderer SDL renderer to use for drawing.
    /// @param camera Camera for world-to-screen transformations.
    void render(SDL_Renderer&, Camera&) const override;

protected:
    /// @brief Provides access to the internal sprite animation data.
    /// @return Reference to the entity's SpriteData.
    SpriteData& getSpriteData() { return _spriteData; }

    void addAtack(const AttackData& attack) { _attacks[attack.id] = attack; }

    void performAttack(AttackId);

    void handleAttack();

    bool isAttacking() const { return _currentAttack.has_value(); }

private:
    /// @brief Renders the weapon's hitbox during attacks (green rectangle).
    void renderWeaponHitbox(SDL_Renderer& renderer, Camera& camera) const;


private:
    ISpriteTextures& _textures; ///< Reference to sprite texture provider.
    
    SpriteData _spriteData;     ///< Current animation state and sprite properties.

    std::unordered_map<AttackId, AttackData> _attacks;

    std::optional<AttackId> _currentAttack;

    ClockType::time_point _lastAttackStartTime;
};

}
