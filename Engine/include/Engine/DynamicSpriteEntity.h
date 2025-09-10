#pragma once

#include <unordered_set>
#include "Engine/DynamicEntity.h"
#include "Engine/ISpriteTextures.h"
#include "Engine/SpriteData.h"
#include "Engine/Clock.h"

namespace engine {

using AttackId = uint8_t;

/// @brief Data structure defining attack properties and behavior.
struct AttackData {
    AttackId id;                                  ///< Unique attack identifier.
    float damage;                                 ///< Damage dealt by this attack.
    std::chrono::steady_clock::duration duration; ///< How long attack lasts.
    utils::f2v offsetRight;                       ///< Hitbox offset when facing right.
    utils::f2v offsetLeft;                        ///< Hitbox offset when facing left.
    utils::f2v size;                              ///< Hitbox dimensions (width, height).
    AnimationId animationId;                      ///< Animation to play during attack.
    std::function<void()> onHit;                  ///< Callback executed on successful hit.
    std::optional<std::string> sourceTag;         ///< Optional tag for attack source.
};

/// @brief Represents dynamic entity rendered using spritesheet. Can perform attacks, has health.
class DynamicSpriteEntity : public DynamicEntity {
public:
    /// @brief Configuration struct used to initialize a DynamicSimpleEntity.
    struct Config : DynamicEntity::Config {
        SpriteData spriteData; ///< Initial sprite animation data.
    };

    DynamicSpriteEntity(Settings&, ISpriteTextures&, const Config&);

    /// @brief Updates entity's logic based on the elapsed time.
    void update(float deltaTime) override;

    void render(SDL_Renderer&, Camera&) const override;

    bool isAttacking() const { return _currentAttack.has_value(); }
    std::optional<AttackId> getAttackId() const { return _currentAttack; }
    std::optional<SDL_FRect> getWeaponHitbox() const { return _weaponHitbox; };
    const AttackData& getCurrentAttackDataView() const;
    
    /// @brief Registers a hit on another entity.
    /// @return True if hit successfully (during frame with first intersection), false otherwise.
    bool registerHit(Entity::Id target) { return _alreadyHitThisAttack.insert(target).second; }

protected:
    void addAtack(const AttackData& attack) { _attacks[attack.id] = attack; }
    void performAttack(AttackId);
    void handleAttack();
    SpriteData& getSpriteData() { return _spriteData; }

    void tryToChangeAnimation(AnimationId);

private:
    void updateHealthBar();

    /// @brief Renders the weapon's hitbox during attacks (green rectangle).
    void renderWeaponHitbox(SDL_Renderer& renderer, Camera& camera) const;

    void renderHealthBar(SDL_Renderer&, Camera&) const;

    /// @brief Updates weapon hitbox based on current attack and direction.
    void calculateWeaponHitbox();

    ISpriteTextures& _textures;                             ///< Sprite textures provider.

    SpriteData _spriteData;                                 ///< Sprite properties.
    std::unordered_map<AttackId, AttackData> _attacks{};    ///< Available attacks mapped by ID.
    std::optional<AttackId> _currentAttack{};               ///< Currently active attack ID.
    std::optional<SDL_FRect> _weaponHitbox{};               ///< Current weapon collision area.
    Clock::Type::time_point _lastAttackStartTime{};         ///< When current attack started.
    std::unordered_set<Entity::Id> _alreadyHitThisAttack{}; ///< Entities id hit current attack.
    utils::f2v _healthBarPos{};                             ///< Health bar screen position.
    utils::f2v _healthBarSize{};                            ///< Health bar dimensions.
};

}
