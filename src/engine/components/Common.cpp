#include "components/Common.hpp"

#include "components/MeshComponent.hpp"

#include "components/BoxCollision.hpp"
#include "components/CameraComponent.hpp"
#include "components/ColorComponent.hpp"
#include "components/InputComponent.hpp"
#include "components/MovementComponent.hpp"
#include "components/TextureComponent.hpp"
#include "components/TranslationComponent.hpp"

void registerComponents(EntityManager& manager)
{
    manager.registerComponent<TranslationComponent>();
    manager.registerComponent<MeshComponent>();
    manager.registerComponent<CameraComponent>();
    manager.registerComponent<InputComponent>();
    manager.registerComponent<TextureComponent>();
    manager.registerComponent<MovementComponent>();
    manager.registerComponent<BoxCollision>();
    manager.registerComponent<ColorComponent>();
}