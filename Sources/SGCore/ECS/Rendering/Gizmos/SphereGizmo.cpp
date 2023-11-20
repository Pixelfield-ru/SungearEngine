#include "SphereGizmo.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/Defines.h"

SGCore::SphereGizmo::SphereGizmo()
{
    m_meshData = Ref<IMeshData>(CoreMain::getRenderer().createMeshData());

    /*m_mesh->m_material = std::shared_ptr<Memory::Assets::IMaterial>(
            Core::Main::CoreMain::getRenderer().createBlankStandardMaterial(
                    Graphics::StandardShaderType::SG_COMPLEX_PRIMITIVES_SHADER
            )
    );*/
}

void SGCore::SphereGizmo::setVertexPosition(const size_t& vertexIdx, const float& x, const float& y,
                                               const float& z) noexcept
{

}
