#include "systems/TestSystem.hpp"
#include "components/MeshComponent.hpp"

void TestSystem::update(EntityManager& manager)
{
    ComponentsForEachFn<TranslationComponent, MeshComponent> const forEachTranslation{
        [&manager](Entity entity, TranslationComponent& input, MeshComponent& mesh) {
            static float rotation = 0.0F;
            rotation += 0.1F;
            rotation = fmod(rotation, 360.0F);

            input.rotation.x = rotation;

            return true;
        }};

    manager.forEachComponents<TranslationComponent, MeshComponent>(forEachTranslation);
}
